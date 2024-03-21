# Setup
## GitHub Codespace
With GitHub Codespaces you use a machine in the cloud. This allows you to carry out the exercises without having Linux and other tools installed.
It is recommended to fork this repository into your own account to be able to save changes.

[hhttps://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks/fork-a-repo](How to fork a repo)
[https://docs.github.com/en/codespaces/developing-in-a-codespace/creating-a-codespace-for-a-repository?tool=webui](How to create a codespace)

## Local
Alternatively, the exercises can be carried out locally, but I will not offer any assistance if there are problems with the package installation.
However if you using ubuntu you probably need the following additional packages:
* `strace`
* `build-essential`
* `gcc`


# man pages
Most linux distributions are delivered with so-called man pages. The man pages are something like a manual for the functions of the installed programs, libraries and the kernel.
Familiarize yourself with the use of the man pages in the following.  Try to answer the following questions:
* how can i determine the cpu architecture of the running machine. tip: `man uname`
* what is the purpose of `-t` in ls -alht? tip: `man ls`
* what is the difference between `man 1 time` and `man 2 time` and `man 7 time`. tip: `man man`



# Exploring the Hardware
Next we want to find out a bit about the hardware on which the current session is running.  
Under Unix, the principle is to map everything as a file, this applies to hardware devices as well as to process and kernel information. A lot of interesting information is therefore offered in the `/proc` file system. 


htop,
/proc - everything is a file
 - cpuinfo
 - attached pci devices
uname

# strace


# Compilerexplorer
How does the cpu processes the example
https://godbolt.org/z/3E68K5aTh