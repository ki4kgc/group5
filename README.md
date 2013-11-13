Progress Report #1
-Track changes with strace
-Passing strings back and forth between the kernel and user-space
-Finding the paths where a files system is modified
-Understanding wait queues and how they’re used in Linux

Progress Report #2
-Most of userspace/user-level helper is complete
--Passing commands to the kernel to freeze the file system
--Get a notification from the kernel to restore the structure of the file system when it was frozen
--Writing to a log to track changes for “dirty files”
--Orchestrating blocking and waking up

Progress Report #3
-Userspace/user-level helper is complete
-Program that interacts with the kernel by telling it when it wants to restore the file system from the frozen state
