#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define IMAGE_FILE "/mnt/c/Users/wac2sgp/Documents/hft-os-exercises/0x_fuse/fat.img"
#define N_FD 3

struct {
    int img_fd;
    uint16_t n_bytes_per_sector;
    uint8_t n_sectors_per_cluster;
    uint16_t n_reserved_sectors;
    uint8_t n_fat_copies;
    uint16_t n_fat_sectors;
    uint16_t n_root_entries;
    uint16_t n_root_sectors;
    uint16_t first_data_sector;
    uint8_t* file_allocation_table;
    struct {
        bool is_open;
        uint16_t current_cluster;
        uint16_t current_offset_in_cluster;
        uint16_t n_bytes_remaining;
    } open_file[N_FD];
} fat_fs;

static void seek_img(uint32_t pos) {
    lseek(fat_fs.img_fd, pos, SEEK_SET);
}

static void read_img_exact(int n_bytes, uint8_t* buf) {
    while(n_bytes > 0) {
        int n_bytes_read_last = read(fat_fs.img_fd, buf, n_bytes);
        if(n_bytes_read_last < 1) {
            printf("Unexpected EOF %d\r\n", errno);
            abort();
        }
        buf += n_bytes_read_last;
        n_bytes -= n_bytes_read_last;
    }
}

static uint32_t read_uint16(uint8_t* buf) {
    return (((uint32_t)buf[1]) << 8) + ((uint16_t)buf[0]);
}

static uint16_t read_uint32(uint8_t* buf) {
    return ((uint16_t)buf[3]) << 24 | ((uint16_t)buf[2]) << 16 | ((uint16_t)buf[1]) << 8 | ((uint16_t)buf[0]);
}

static void read_date(uint8_t* buf, struct tm* date) {
    date->tm_mday = buf[0] & 0x1F;
    date->tm_mon = (((buf[0] & 0xE0) >> 5) | ((buf[1] & 0x01) << 3)) - 1;
    date->tm_year = 80 + ((buf[1] & 0xFE) >> 1);
}

static void read_time(uint8_t* buf, struct tm* time) {
    time->tm_sec = (buf[0] & 0x1F) << 1;
    time->tm_min = ((buf[0] & 0xE0) >> 5) | ((buf[1] & 0x07) << 3); 
    time->tm_hour = (buf[1] & 0xF8) >> 3;
}

static void read_header() {
    uint8_t header[512];
    read_img_exact(sizeof(header), header);
    fat_fs.n_bytes_per_sector = read_uint16(&header[11]);
    fat_fs.n_sectors_per_cluster = header[13];
    fat_fs.n_reserved_sectors = read_uint16(&header[14]);
    fat_fs.n_fat_copies = header[16];
    fat_fs.n_root_entries = read_uint16(&header[17]);
    fat_fs.n_fat_sectors = read_uint16(&header[22]);
    fat_fs.n_root_sectors = ((fat_fs.n_root_entries * 32) + (fat_fs.n_bytes_per_sector - 1)) / fat_fs.n_bytes_per_sector;
    fat_fs.first_data_sector = fat_fs.n_reserved_sectors + fat_fs.n_fat_sectors * fat_fs.n_fat_copies + fat_fs.n_root_sectors;
}

static uint16_t read_fat_entry(uint16_t offset) {
    uint32_t first_byte_offset = (offset * 3) / 2;
    if(offset % 2) {
        return ((fat_fs.file_allocation_table[first_byte_offset] & 0xF0) >> 4) | (fat_fs.file_allocation_table[first_byte_offset + 1] << 4);
    } else {
        return fat_fs.file_allocation_table[first_byte_offset] | ((fat_fs.file_allocation_table[first_byte_offset + 1] & 0x0F) << 8);
    }
}

static bool parse_dir_entry(uint8_t* entry_bytes, struct stat *stbuf, char* filename, uint16_t* cluster_number) {
    if(entry_bytes[0] == 0x00 || entry_bytes[0] == 0xE5) return false;

    int first_space_index;
    #define FILE_NAME_MAX_LENGTH 8
    for(first_space_index = 0; first_space_index < FILE_NAME_MAX_LENGTH && entry_bytes[first_space_index] != ' '; first_space_index++) 
        filename[first_space_index] = tolower(entry_bytes[first_space_index]);

    int second_space_index;
    #define FILE_EXTENSION_MAX_LENGTH 3
    for(second_space_index = 0; second_space_index < FILE_EXTENSION_MAX_LENGTH && entry_bytes[FILE_NAME_MAX_LENGTH + second_space_index] != ' '; second_space_index++)
        filename[first_space_index + second_space_index + 1] = tolower(entry_bytes[FILE_NAME_MAX_LENGTH + second_space_index]);

    if(second_space_index != 0) {
        filename[first_space_index] = '.';
        filename[first_space_index + second_space_index + 1] = '\0';
    } else {
        filename[first_space_index] = '\0';
    }

    #define ATTR_VOLUME_ID 0x08
    #define ATTR_DIRECTORY 0x10
    #define ATTR_LONG_NAME 0x0F
    uint8_t dir_attr = entry_bytes[11];
    if(dir_attr & ATTR_VOLUME_ID || dir_attr & ATTR_LONG_NAME) return false;

    if(!(dir_attr & ATTR_DIRECTORY)) {
        stbuf->st_mode = S_IFREG | 0444;  
        stbuf->st_size = read_uint32(&entry_bytes[28]); 
    } else {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_size = 0;
    }
    struct tm create_time;
    memset(&create_time, 0, sizeof(struct tm));
    read_time(&entry_bytes[14], &create_time);
    read_date(&entry_bytes[16], &create_time);

    struct tm access_time;
    memset(&access_time, 0, sizeof(struct tm));
    read_date(&entry_bytes[18], &access_time);

    struct tm modified_time;
    memset(&modified_time, 0, sizeof(struct tm));
    read_time(&entry_bytes[22], &modified_time);
    read_date(&entry_bytes[24], &modified_time);

    stbuf->st_atime = mktime(&access_time);
    stbuf->st_ctime = mktime(&create_time);
    stbuf->st_mtime = mktime(&modified_time);

    *cluster_number = read_uint16(&entry_bytes[26]);
    return true;
}

static bool find_rootfile_dir_entry(const char* filename, struct stat *stbuf, uint16_t* cluster_number) {
    seek_img(fat_fs.n_bytes_per_sector * (fat_fs.n_reserved_sectors + fat_fs.n_fat_sectors * fat_fs.n_fat_copies));
    memset(stbuf, 0, sizeof(struct stat));
    for(int i = 0; i < fat_fs.n_root_entries; i++) {
        uint8_t file_entry[32];
        read_img_exact(32, file_entry);
        char current_filename [16];
        if(parse_dir_entry(file_entry, stbuf, current_filename, cluster_number) && strcmp(filename, current_filename) == 0) {
            return true;
        }
    }
    return false;
}

static void *fat_init(struct fuse_conn_info *conn, struct fuse_config *cfg) {
    printf("fat_init\r\n");
    fat_fs.img_fd = open(IMAGE_FILE, O_RDONLY);
    if(fat_fs.img_fd < 0) {
        printf("Could not open file %d\r\n", errno);
        abort();
    }
    
    seek_img(0);
    read_header();
    uint32_t fat_length = fat_fs.n_fat_sectors * fat_fs.n_bytes_per_sector;
    fat_fs.file_allocation_table = malloc(fat_length);
    seek_img(fat_fs.n_bytes_per_sector * fat_fs.n_reserved_sectors);
    read_img_exact(fat_length, fat_fs.file_allocation_table);

    cfg->kernel_cache = 1;
    return NULL;
}

static void fat_destroy(void* private_data) {
    printf("fat_destroy\r\n");
    free(fat_fs.file_allocation_table);
    close(fat_fs.img_fd);
}

static int fat_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
    printf("fat_getattr %s\r\n", path);
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    } else {
        uint16_t first_cluster;
        if(find_rootfile_dir_entry(path + 1, stbuf, &first_cluster)) {
            return 0;
        }
    }
    return -ENOENT;
}
 
static int fat_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi,
                         enum fuse_readdir_flags flags) { 
    printf("fat_readdir %s\r\n", path);
    filler(buf, ".", NULL, 0, 0);
    filler(buf, "..", NULL, 0, 0);
    seek_img(fat_fs.n_bytes_per_sector * (fat_fs.n_reserved_sectors + fat_fs.n_fat_sectors * fat_fs.n_fat_copies));
    for(int i = 0; i < fat_fs.n_root_entries; i++) {
        uint8_t file_entry[32];
        read_img_exact(32, file_entry);
        struct stat stbuf;
        memset(&stbuf, 0, sizeof(struct stat));
        char filename [16];
        uint16_t first_cluster;
        if(parse_dir_entry(file_entry, &stbuf, filename, &first_cluster)) {
            filler(buf, filename, &stbuf, 0, FUSE_FILL_DIR_PLUS);
        }
    }
    return 0;
}

static int fat_open(const char *path, struct fuse_file_info *fi) {
    printf("fat_open %s\r\n", path);
    uint16_t first_cluster;
    int fd;
    for(fd = 0; fd < N_FD && fat_fs.open_file[fd].is_open; fd++);
    if(fd == N_FD) return ENOMEM;

    struct stat stbuf;
    if(!find_rootfile_dir_entry(path + 1, &stbuf, &first_cluster)) {
        return -ENOENT;
    }
    fat_fs.open_file[fd].current_cluster = first_cluster;
    printf("c: %d\r\n", first_cluster);
    fat_fs.open_file[fd].current_offset_in_cluster = 0;
    fat_fs.open_file[fd].n_bytes_remaining = stbuf.st_size;
    
    fi->fh = fd;
    fi->nonseekable = true;

    return 0;
}

static int fat_release(const char *path, struct fuse_file_info *fi) {
    printf("fat_release %s\r\n", path);
    int fd = fi->fh;
    fat_fs.open_file[fd].is_open = false;
    return 0;
}
 
static int fat_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi) {
    printf("fat_read %s\r\n", path);
    int fd = fi->fh;
    int n_bytes_read = 0;
    int bytes_per_cluster = fat_fs.n_sectors_per_cluster * fat_fs.n_bytes_per_sector;
    if(size > fat_fs.open_file[fd].n_bytes_remaining) size = fat_fs.open_file[fd].n_bytes_remaining;
    while (size > 0) {
        int bytes_to_copy = size;
        if (bytes_to_copy + fat_fs.open_file[fd].current_offset_in_cluster > fat_fs.n_sectors_per_cluster * fat_fs.n_bytes_per_sector)
            bytes_to_copy = fat_fs.n_sectors_per_cluster * fat_fs.n_bytes_per_sector - fat_fs.open_file[fd].current_offset_in_cluster;

        seek_img(fat_fs.first_data_sector * fat_fs.n_bytes_per_sector + bytes_per_cluster * (fat_fs.open_file[fd].current_cluster - 2) + fat_fs.open_file[fd].current_offset_in_cluster);
        read_img_exact(bytes_to_copy, buf);

        buf += bytes_to_copy;
        n_bytes_read += bytes_to_copy;
        size -= bytes_to_copy;
        fat_fs.open_file[fd].n_bytes_remaining -= bytes_to_copy;
        fat_fs.open_file[fd].current_offset_in_cluster += bytes_to_copy;
        if(fat_fs.open_file[fd].current_offset_in_cluster == bytes_per_cluster) {
            fat_fs.open_file[fd].current_offset_in_cluster = 0;
            uint16_t next_cluster = read_fat_entry(fat_fs.open_file[fd].current_cluster);
            if(next_cluster == 0x0FFF)
                break;
            fat_fs.open_file[fd].current_cluster = next_cluster;    
        }
    }
    return n_bytes_read;
}

static const struct fuse_operations fat_ops = {
        .init           = fat_init,
        .destroy        = fat_destroy,
        .getattr        = fat_getattr,
        .readdir        = fat_readdir,
        .open           = fat_open,
        .release        = fat_release,
        .read           = fat_read,
};

int main(int argc, char *argv[]) {
    int ret;
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

    ret = fuse_main(args.argc, args.argv, &fat_ops, NULL);
    fuse_opt_free_args(&args);
    return ret;
} 