Progress Report #1 - 11/17/2013
-Track changes with strace - Suny
-Passing strings back and forth between the kernel and user-space - Joey
-Finding the paths and functions where a file system is modified - Suny/Joey/Cecilia
--“sys_X” where X is the name of the system call
-Understanding wait queues and how they’re used in Linux - Suny/Joey/Cecilia
-Configure a controlled directory (/freeze/) to test tracking and logging of file changes - Cecilia

Progress Report #2 - 11/24/2013
-Most of userspace/user-level helper is complete
-Passing commands to the kernel to freeze the file system - Suny
-Get a notification from the kernel to restore the structure of the file system when it was frozen - Joey
-Writing to a log to track changes for “dirty files” - Cecilia
-Orchestrating blocking and waking up - Suny/Joey/Cecilia

Progress Report #3 - 12/01/2013
-Userspace/user-level helper is complete - Suny/Joey/Cecilia
-Program that interacts with the kernel by telling it when it wants to restore the file system from the frozen state - Cecilia

Demo - 12/10/2013
-All components will have been tested and confirmed working  - Suny/Joey/Cecilia
-Demonstration presentation prepared  - Suny/Joey/Cecilia


Approach:

The userspace program will allow the user to place the /freeze/ directory into a frozen or thawed state.  
While the directory is frozen, all system calls to modify files within the directory will be blocked while the userspace program runs.  
The userspace program will create a copy of the targeted file and log that the file has been copied.  
The userspace program will wake the system call back up and it will be free to modify the file.  
When the system is thawed, the userspace program will check the log to restore the backed up files, overwriting the files that were modified while in the frozen state.  
A separate program will be used to tell the kernel to restore the file system from the frozen state.
 
