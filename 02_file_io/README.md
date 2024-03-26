# Exercise 02: Application - Operating System Interaction, Sys Calls

## Writing a File
Build the programm `write_helloworld` using `make`. (You can ignore the errors for now, these are implied from the next task.)
The program should create a file test.txt and write 'hello world'. How ever there are two bugs in `write_helloworld.c` that prevent the programm from functioning correctly.
The bugs are pointed out by comments in the source code - The parameters of the open and write functions are not set correctly. Fix those bugs and verify the result with `cat hello.txt`

You can view the documentation of the functions on the command line with the commands:
```bash
man 2 open
man 2 write
```

**Bonus:** Replace the call to `open` with the constant `1` e.g. `int fd = 1;`. Where is 'hello world' written to?


## Observing Programm Behaviour with strace
Compile the program `adder` from the previous exercise and the program `www` from the current exercise using `make`.
The program `adder` from the previous exercise accepts two integer arguments which are then added together. e.g. `./adder 2 3`.
The program `web` retrieves a website and displays the raw response in the terminal.
We now going to explore which methods of the Linux Kernel (so-called Syscalls) are invoked by the program during execution. The programm `strace` is 

Start `adder` using `strace`.
* What happens during the program start?
* How is the standard library (libc) loaded? 
Note: The concept of virtual memory is explained later in the lecture. You could assume for now the Syscall `mmap` is used for loading a file to a specific point in memory and setting sufficent permissions for access.
* How is the result writen to terminal?
Now start `www` using `strace` e.g. `strace ./adder 42 99`
* How is the DNS lookup performed?
* How is a network connection established and data exchanged?

## Sending a SysCall manually
> Disclaimer: This tasks assumes you are using an x64 system. The following won't compile nor run on a 32bit nor an ARM (Raspberry Pi, Apple M1, etc.) system

The previous examples is using the functions `open` and `write` from the standard library (libc) that internally invokes the corresponding syscalls.
Now for the sake of demonstration, we want to invoke the syscall by ourself manualy by using inline assembly.
* Inspect the function `my_write` in the file `write_helloworld_syscall`. You see the signature matches the open function from the standard library. And the body contains a few lines of inline assembly with placeholders.
* Find out what the syscall what the syscall number of the `write` syscall is and which arguments belongs in which registers? You can use [this table](https://x64.syscall.sh/) for reference.
* Set the syscall number and the register names in the inline assembly. Refer to the [GCC Docs](https://gcc.gnu.org/onlinedocs/gcc/extensions-to-the-c-language-family/how-to-use-inline-assembly-language-in-c-code.html)for syntactical issues.
* Verify correct behaviour using `strace`. The output should contain a line like the following: `write(3, "Hello World\n", 12) = 12`