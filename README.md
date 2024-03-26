# HFT SS24 - Operating Systems - Exercises
This repository contains the exercises for the operating systems part of the lecture Networks and Operating Systems of the summer semester 2024.

The tasks of the exercises can be roughly divided into three categories:
1. tasks with the shell / console - these explain how to use Linux and discover details of operating systems
2. programming tasks in C - here we look at programming concepts that are close to the operating system, such as threads and the development of a file system driver
3. programming tasks in Java - to get to know higher level parallelism constructs and to develop algorithms that do not require communication close to the operating system

## Setup
### GitHub Codespace
With GitHub Codespaces you use a machine in the cloud. This allows you to carry out the exercises without having Linux and other tools installed and enabled to scale upt to 32 cores for the multi-threading exercises. 

I recommend to fork this repository into your own account to be able to commit changes. 

[How to fork a repo](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks/fork-a-repo) - 
[How to create a codespace](https://docs.github.com/en/codespaces/developing-in-a-codespace/creating-a-codespace-for-a-repository?tool=webui)


### Local
Alternatively, the exercises can be carried out locally, but I will not offer any assistance if there are problems with the package installation.
However if you using ubuntu you probably need the following additional packages:
* `strace`
* `build-essential`
* `gcc`
* `pkg-config`
* `libfuse3-dev`
* `fdisk`
