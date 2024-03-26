#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int my_write(long int fd, const char* buf, long int len) {
  long int res;
  asm (
    // set the syscall nr
    "mov $??, %%??\n"
    // set the arguments
    "mov %[fd], %%??\n"
    "mov %[buf], %%??\n"
    "mov %[len], %%??\n"
    // trigger the syscall
    "SYSCALL\n"
    // get the result code
    "mov %%??, %[res]\n"
    : [res] "=r" (res)
    : [fd] "r" (fd), [buf] "r" (buf), [len]"r" (len)
    // registers used
    : "cc", "??", "??", "??", "??");
  return res;
}

int main(int argc, char **argv) {
  int fd = open("hello_syscall.txt", O_WRONLY | O_CREAT, 0644);
  if (fd == -1) {
    printf("open error: %s\r\n", strerror(errno));
    return 2;
  }
  const char* text = "Hello World\n"
  int write_res = my_write(fd, text, strlen(text));
  if (write_res == -1) {
    printf("write error: %s\r\n", strerror(errno));
  }
  close(fd);
}