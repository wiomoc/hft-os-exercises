#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char **argv) {
  // Open file for writing ... why is the parameter O_R(ea)DONLY used?
  int fd = open("hello.txt", O_RDONLY | O_CREAT, 0644);
  if (fd == -1) {
    printf("open error: %s\r\n", strerror(errno));
    return 2;
  }
  // What is the meaning of all the write parameters?
  int write_res = write(fd, "Hello World\n", 4);
  if (write_res == -1) {
    printf("write error: %s\r\n", strerror(errno));
  }
  close(fd);
}