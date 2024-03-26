all: fuse_fat

fuse_fat: fuse_fat.c
	gcc fuse_fat.c -o fuse_fat `pkg-config fuse3 --cflags --libs`