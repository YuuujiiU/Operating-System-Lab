#include <fcntl.h> 
#include<stdio.h>
#include <unistd.h>

/* 
Solution for questions:

1) What are the system call names for
   opening a file: open() or openat()
   closing a file: close() 
   reading a file: read()
   writing a file : write()

2) How many system calls are involved with opening a file, closing a file, reading a file and writing a file?
(count each individually)
    open: 4, close: 4, number of read/write system calls can be different for each student. 

3) What is the file number of the standard input? Should we expect it to ever change?
    0 is the file descriptor of the standard input, it is not expected to change.

4) What is the file number of the standard output? Should we expect it to ever change?
    1 is the file descriptor of the standard output, it is not expected to change.

5) What was the file number of your read file? Should we expect it to ever change?
    3, it is the lowest available number to be assigned to the file. Yes, it may change depending on sequence of when they were opened.

6) What was the file number of your write file? Should we expect it to ever change?
    4, it is the next available number to be assigned to the file. Yes, it may change depending on sequence of when they were opened.
*/


void main(int args,char *argv[]){

     char ifname[100];
     char ofname[100];
    // prompt for input file name 
     printf("Enter input file name: ");
     scanf("%s", ifname);
     
    //open the input file
    int ifd = open(ifname, O_RDONLY); 
     
     //the file does not exist 
    if (ifd ==-1){ 
        printf("Input file does not exist, aborting.\n"); 
        return;                 
    } 
    
      // prompt for output file name 
     printf("Enter output file name: ");
     scanf("%s", ofname); 
    
    //create the output file, O_EXCL option ensures the file does not already exist
    int ofd = open(ofname, O_WRONLY | O_CREAT | O_EXCL, 0777);
     
     //the output file already exists
    if (ofd ==-1) { 
        printf("Output file already exists, aborting.\n"); 
        return;                 
    } 
    
    //a buffer used to read and write to the file, one byte at a time.
    char buf;
    
    while(read(ifd, &buf, 1)){
	write(ofd, &buf, 1);
	}
    close(ifd);
    close(ofd);
    
    printf("Finished copying the file.\n"); 
    return; 

}

