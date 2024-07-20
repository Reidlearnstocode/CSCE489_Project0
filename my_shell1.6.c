#include <stdio.h>
#include <stdlib.h> // needed for exit and atoi
#include <unistd.h> //needed for get, sleep, execl
#include <sys/types.h> //needed for get pid
#include <sys/wait.h> //needed for wait
#include <string.h> //needed for str compare and tokenization
//#include <fcntl.h> - locks not used due to complexity

//Pointer function here - seems to work without this, but VS Code sometimes identifies problems when leaving it out. 

void execute_command(char*);

int main() {
    printf("Hello, and welcome to Shell 2024.\n Before you get started, you may wish to read the readme file for a simple list of commands, do's, and do-not's!\n Enjoy!\n");

    //Function that prints the process ID https://man7.org/linux/man-pages/man2/getpid.2.html
    //This process serves as the parent process, as opposed to the children that come later with the fork command.
    printf("Shell Process ID: %d\n", getpid());

    //Declares char command will be an array of size 256. (Should take 8 bits/1 byte of memory)
    char command[256];

    //Initiates an infinite loop to keep the program running indefinitely (While (1) is true, will loop continuously https://en.wikipedia.org/wiki/While_loop; https://en.wikipedia.org/wiki/Infinite_loop)
    while (1) {

        //The structure of this loop is such that there are finite options available to the program/user.
        //Simply put, if the program does not detect a specified command, it must print "Invalid command" as it is the last possible condition in the infinite while loop
        //These options are clearly defined in each section as "argv" - a C function to convert an input string to an array, and provide a value that can be recognized as an input, thereby allowing for "if" conditions.
        //This print statements serves as a prompt for the user - "$" similar to the ubuntu operating system terminal prompt 
        printf("$ ");

        //Reads a line from the input and stores it into the string "command"
        fgets(command, sizeof(command), stdin);

        //Set length to string length of command.

        // Remove newline character at the end of command
        //This code fragment is necesssary because of the fgets() function - must reduce size of the string to parse the command correctly - learned something new today!
        size_t len = strlen(command);
        if (len > 0 && command[len-1] == '\n') {
            command[len-1] = '\0';
        }

        //As one of the primary commands available to the user (Halt) - it should come early in the loop - as in this case, immediately after the command is read by the program.
        //The use of the "break" of the loop halts the infinite loop, and terminates the program.
        if (strcmp(command, "halt") == 0) {
            break;
        }

        //Fork a child process so that execl does not overrite the program itself.
        pid_t pid = fork();
        

        //Structuring the pid if condition in this fashion allows differentiation between parent process and child process.
        if (pid == 0) {
            execute_command(command);

        //If pid > 0; will be treated as a child process.
        } else if (pid > 0) {
            printf("Child process ID: %d\n", pid);


            //To allow for the "&" criteria allowing for another command to be entered - take the length of the array, and if the last value is "&"
            //Continue.  If not, then wait.
            //https://www.geeksforgeeks.org/wait-system-call-c/
            //This is a texbook example (assuming it actually works) of blocking a process.

            //Initially I thought I had this right, but after adding the printf statement and testing, I believe I am somewhat off target and the process may actually wait more often than intended.
            
            if (command[len -1] == '&') {
                continue;
            } else {wait(NULL);
            //printf("Waiting.\n");
            }
        } else {
            printf("Failed to fork.\n");
        }
    }
    return 0;
}
//Template for command parsing, and converting strings to integers using the atoi and argv was sourced online, geeksforgeeks, stack overflow, tutorialspoint, and various coding assistant platforms. (My initial plan to parse commands using scanf, then use pointers to a string array quickly proved inadequate for this task and I had to search elsewhere.)

void execute_command(char* command) {
    char *argv[64];
    int i = 0;

    //https://www.geeksforgeeks.org/strtok-strtok_r-functions-c-examples/
    //Here we "tokenize" a string (the command input from the user) and
    //Basic structure for implementation follows Jacob Sorber's youtube video: https://www.youtube.com/watch?v=6Dk8s0F2gow

    argv[i] = strtok(command, " ");
    while (argv[i] != NULL) {
            argv[++i] = strtok(NULL, " ");
    }

    //String compare function "strcmp()" compares strings character to character, and if equal, returns 0.  
    //In this secion, if the (string) command entered by the user is equal to "Create", then proceed with the command.
    //All other commands are structured in a similar fashion - take the user input, and compare the strings, if the string matches, proceed.
    
    if (strcmp(argv[0], "create") == 0) {
    

    //Read for the file
        FILE *file = fopen(argv[1], "r");

        //If the file exists, close it, and print an error.
        if (file) {
            fclose(file);
            printf("Error creating file, file already exists.\n");

            //if no file, then create the file with write condition.
        } else {
            FILE *file = fopen(argv[1], "w");
            if (file) {
                fclose(file);

                //If for some reason none of those conditions are met, print an error (Note: End-user testing never produced this error.)
            } else {
                printf("Error creating file: %s\n", argv[1]);

            }
        }
exit(0);}


else if (strcmp(argv[0], "update") == 0) {
    FILE *file = fopen(argv[1], "a");
    if (file) {

        //atoi convers strings argv[x] to integers 
        int count = atoi(argv[2]);
        for (int j = 0; j < count; j++){

            //USER NOTE: For the "update" command, ensure the following format of the command is observed.
            //If you wanted to update file5 three (3) times with the word "update", the command would look like the following:
            //update file5 3 update

            //Limitations include only being able to append one word to the file, but the user can add as many of that word (within reason) as they would like!

            //fprintf is used to print to a specified stream/outpu,t which in this case is the file.
            fprintf(file, "%s\n", argv[3]);
            
            //force the file write to disk
            fflush(file);
            sleep(5); 
        }
        fclose(file);
        printf("Update completed by process ID: %d\n", getpid());

    }else {
       printf("Error opening file: %s\n", argv[1]);
    }
exit(0);}

    else if (strcmp(argv[0], "list") == 0) {
        FILE *file = fopen(argv[1], "r");


        if (file) {
            fclose(file);
            char cmd[256];

            //print formatted string using "cat" - %s is the placeholder for argv[1] which should be the filename entered by the user
            //similar solution "snprintf(buf, sizeof buf, "%s%s%s", str_a, str_b, str_c);" 
            //from https://stackoverflow.com/questions/12065286/snprintf-for-string-concatenation
            
            snprintf(cmd, sizeof(cmd), "cat %s", argv[1]);

            //terminal commands to display file contents
            //One of two "execl()" functions in the program
            
            execl("/bin/sh", "sh", "-c", cmd, (char *) 0);

            }else {
                printf("File does not exist.");

    }
exit(0);}

    else if (strcmp(argv[0], "dir") == 0) {

        //This is the execl line mentioned in the project assignment question.
        //terminal commands to display directory contents using linux "ls" command
        //The second "execl()" function in the program
        
        execl("/bin/ls", "ls", NULL);
        exit(0);
    }

    //Useful feedback for the user:
    else {printf("Invalid Command. Please try again.\n");
    printf("Valid commands are 'create', 'dir', 'list', and 'halt' without quotes.\n See readme for additional information on command format.\n");
    exit(0);
    }

}

//Addtional reading that helped me formulate structure for this project:
//https://aueems.cce.af.mil/courses/13109 - functions in the assignment were the starting point to researching applicable higher level C functions I had not been exposed to previously.
//https://brennan.io/2015/01/16/write-a-shell-in-c/
//https://www.geeksforgeeks.org/making-linux-shell-c/
//https://medium.com/@winfrednginakilonzo/guide-to-code-a-simple-shell-in-c-bd4a3a4c41cd
//https://medium.com/@santiagobedoa/coding-a-shell-using-c-1ea939f10e7e

