# Inter-process-communication-using-shared-memory
Create a private shared memory in C/C++. The process then creates a child and waits for
the child to write the file’s contents to shared memory. The parent then reads the
shared memory and removes all duplicate integers from the data. The child reads it back
and writes the changed data back to the same file. (The file name is passed as command
line argument). Use open, read, and write system calls for filing.
Note: Private shared memory is the shared memory which is only accessible by the
process that created it and its children processes. Private shared memory is declared by
passing IPC_PRIVATE as key. Also, you do not need to explicitly attach the private shared
memory inchild process since the child inherits the parent’s address space.
