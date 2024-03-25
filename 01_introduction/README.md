# Exercise 01: Introduction
## Setup
### GitHub Codespace
With GitHub Codespaces you use a machine in the cloud. This allows you to carry out the exercises without having Linux and other tools installed.
It is recommended to fork this repository into your own account to be able to save changes.

[hhttps://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks/fork-a-repo](How to fork a repo)
[https://docs.github.com/en/codespaces/developing-in-a-codespace/creating-a-codespace-for-a-repository?tool=webui](How to create a codespace)

### Local
Alternatively, the exercises can be carried out locally, but I will not offer any assistance if there are problems with the package installation.
However if you using ubuntu you probably need the following additional packages:
* `strace`
* `build-essential`
* `gcc`
* `pkgconf`
* `libfuse3-dev`


## man pages
Most linux distributions are delivered with so-called man pages. The man pages are something like a manual for the functions of the installed programs, libraries and the kernel.
Familiarize yourself with the use of the man pages in the following.  Try to answer the following questions:
* how can i determine the cpu architecture of the running machine. Hint: `man uname`
* what is the purpose of `-t` in ls -alht? Hint: `man ls`
* what is the difference between `man 1 time` and `man 2 time` and `man 7 time`. Hint: `man man`



## Exploring the Hardware
Next we want to find out a bit about the hardware on which the current session is running.  
Under Unix, the principle is to map everything as a file, this applies to hardware devices as well as to process and kernel information. A lot of interesting information is therefore offered in the `/dev` and `/proc` file system. 
 
Take a look under /dev and /proc:
* What devices do you know? What does /dev/null do? What is it good for?
* What is `/proc` all about? Hint: `man proc`
* Look at `/proc/meminfo` and the output of `free -h`. What is displayed? What is common, what is the difference?
* Look at `/proc/stats`. Which of the values can you explain?
* Look at `/proc/cpuinfo`. What is the name of the process?

Also take a look at the output of `lspci`


## Application - Operating System Interaction
Compile the two programs `adder` and `www` using `make`.
The program `adder` accepts two integer arguments which are then added together. e.g. `./adder 2 3`.
The program `web` retrieves the first website in the world and displays it together with the http header on the terminal

Start `adder` and `www` using `strace`. 
* What do you see?
* What happens during the program start?
* How is a network connection established and data exchanged?


# Compilerexplorer
How does the cpu processes the example
https://godbolt.org/z/3E68K5aTh


## Latencies

Latency numbers every programmer should know (according to Peter Norvig):
```
L1 cache reference ......................... 0.5 ns
Branch mispredict ............................ 5 ns
L2 cache reference ........................... 7 ns
Mutex lock/unlock ........................... 25 ns
Main memory reference ...................... 100 ns             
Compress 1K bytes with Zippy ............. 3,000 ns  =   3 µs
Send 2K bytes over 1 Gbps network ....... 20,000 ns  =  20 µs
SSD random read ........................ 150,000 ns  = 150 µs
Read 1 MB sequentially from memory ..... 250,000 ns  = 250 µs
Round trip within same datacenter ...... 500,000 ns  = 0.5 ms
Read 1 MB sequentially from SSD* ..... 1,000,000 ns  =   1 ms
Disk seek ........................... 10,000,000 ns  =  10 ms
Read 1 MB sequentially from disk .... 20,000,000 ns  =  20 ms
Send packet CA->Netherlands->CA .... 150,000,000 ns  = 150 ms
```

 
These durations are usually unimaginably small but processors execute billions of instructions per second and then they add up.
Multiply (some of) the numbers by 1 billion and give the durations in seconds, hours, days, weeks - can you find a comparison from everyday life?
See also [here](https://gist.github.com/hellerbarde/2843375#file-latency_humanized-markdown).
