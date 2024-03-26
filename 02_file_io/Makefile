all: www write_helloworld write_helloworld_syscall

write_helloworld: write_helloworld.c
	gcc -o write_helloworld write_helloworld.c

write_helloworld_syscall: write_helloworld_syscall.c
	gcc -o write_helloworld_syscall write_helloworld_syscall.c

www: www.c
	gcc -o www www.c