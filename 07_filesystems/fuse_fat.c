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
    uint8_t* file_allocation_table;
    // todo
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

static uint16_t read_fat_entry(uint16_t offset) {
    uint32_t first_byte_offset = (offset * 3) / 2;
    if(offset % 2) {
        return ((fat_fs.file_allocation_table[first_byte_offset] & 0xF0) >> 4) | (fat_fs.file_allocation_table[first_byte_offset + 1] << 4);
    } else {
        return fat_fs.file_allocation_table[first_byte_offset] | ((fat_fs.file_allocation_table[first_byte_offset + 1] & 0x0F) << 8);
    }
}

static bool parse_dir_entry(uint8_t* entry_bytes, struct stat *stbuf, char* filename, uint16_t* cluster_number) {
    // todo
    return false;
}

static void *fat_init(struct fuse_conn_info *conn, struct fuse_config *cfg) {
    printf("fat_init\r\n");
    fat_fs.img_fd = open(IMAGE_FILE, O_RDONLY);
    if(fat_fs.img_fd < 0) {
        printf("Could not open file %d\r\n", errno);
        abort();
    }
    // todo   

    cfg->kernel_cache = 1;
    return NULL;
}

static void fat_destroy(void* private_data) {
    printf("fat_destroy\r\n");
    close(fat_fs.img_fd);
}

static int fat_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
    printf("fat_getattr %s\r\n", path);
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    } else {
       // todo
    }
    return -ENOENT;
}
 
static int fat_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi,
                         enum fuse_readdir_flags flags) { 
    printf("fat_readdir %s\r\n", path);
    filler(buf, ".", NULL, 0, 0);
    filler(buf, "..", NULL, 0, 0);
    // todo
    return 0;
}

static int fat_open(const char *path, struct fuse_file_info *fi) {
    printf("fat_open %s\r\n", path);
    //todo

    return -ENOENT;
}

static int fat_release(const char *path, struct fuse_file_info *fi) {
    printf("fat_release %s\r\n", path);
    int fd = fi->fh;
    //todo

    return 0;
}
 
static int fat_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi) {
    printf("fat_read %s\r\n", path);
    int fd = fi->fh;
    int n_bytes_read = 0;

    //todo

    return n_bytes_read;
}

static const struct fuse_operations fat_ops = {
        /* called when the application startes and fs should be mounted. */
        .init           = fat_init,
        /* called when the fs should be unmounted */
        .destroy        = fat_destroy,
        /* called when the kernel or a application wants to get the metadata 
           of a file / a directory. e.g. type, permissions, creation date, etc. */
        .getattr        = fat_getattr,
        /* called when a application wants to list the files or directory contained in a given directory  */
        .readdir        = fat_readdir,
        /* called when a application wants to open a file (for reading) */
        .open           = fat_open,
        /* called when a application is finished with reading a file so the file handle could be freed */
        .release        = fat_release,
        /* called when a application wants to read from a file */
        .read           = fat_read,
};

int main(int argc, char *argv[]) {
    int ret;
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

    ret = fuse_main(args.argc, args.argv, &fat_ops, NULL);
    fuse_opt_free_args(&args);
    return ret;
} 