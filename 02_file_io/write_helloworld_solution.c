#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char **argv) {
  int fd = open("hello.txt", O_WRONLY | O_CREAT, 0644);
  if (fd == -1) {
    printf("open error: %s\r\n", strerror(errno));
    return 2;
  }
  const char* text = "Hello World\n"
  int write_res = write(fd, text, strlen(text));
  if (write_res == -1) {
    printf("write error: %s\r\n", strerror(errno));
  }
  close(fd);
}