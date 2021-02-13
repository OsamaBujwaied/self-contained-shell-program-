#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h> 

#define BUFFER_LEN 1024

int main(){
    char line[BUFFER_LEN];  //get command line
    char* argv[100];        //user command
    char* path= "/bin/";    //set path at bin
    char progpath[20];      //full file path
    int argc;               //arg count

    while(1){                                   // infinite loop 

        printf("\nMy shell>> ");                //print shell prompt

        if(!fgets(line, BUFFER_LEN, stdin)){    //get command and put it in line
            break;                              //if user hits CTRL+D break
        }

        if(strcmp(line, "exit\n")==0){          //check if command is exit
            break;                              // end the infinite
        }

        char *pos;                              
        if ((pos=strchr(line, '\n')) != NULL)   //delete the "\n" from line
            *pos = '\0';

        char *token;                            //split command into strings
        token = strtok(line," ");
        int i=0;
        while(token!=NULL){
            argv[i]=token;      
            token = strtok(NULL," ");
            i++;
        }
        argv[i]=NULL;                           //set last execvp value to NULL 

        /*argc=i;  
        printf("\nYour cammand contain: \n");   //print command/args
        for(i=0; i<argc; i++)
            printf("> %s\n", argv[i]);*/      

        strcpy(progpath, path);                 //copy /bin/ to file path
        strcat(progpath, argv[0]);              //add program to path

        for(i=0; i<strlen(progpath); i++){      //delete newline
            if(progpath[i]=='\n'){      
                progpath[i]='\0';
            }
        }

        int pid= fork();                        //fork child

        if(pid==0){                             //Child 
            
            if (strcmp(argv[0], "mycat") == 0) {
                char buffer[1];
                char path[255] = "/root/sandbox/Project/";
                strcat(path, argv[1]);

                int fd = open(argv[1], O_RDONLY, S_IREAD);

                if (fd != -1) {
                    int readBytes = 0;
                    while (readBytes = read(fd, buffer, 1)) {
                        if (readBytes > 0) write(1, buffer, readBytes);
                    }
                }
                close(fd);
            } 
            
            else if (strcmp(argv[0], "myls") == 0) {
                DIR * d;
                struct dirent * dir;
                d = opendir(".");
                if (d) {
                    while ((dir = readdir(d)) != NULL) {
                        printf("%s\n", dir -> d_name);
                    }
                    closedir(d);
                }
            } 
            
            else if (strcmp(argv[0], "myclear") == 0)
                system("clear");

            else if (strcmp(argv[0], "mycp") == 0) {
                char buffer[1];
                int fd = open(argv[2], O_RDONLY, S_IREAD);
                int fd2 = open(argv[1], O_RDWR | O_CREAT | O_EXCL, S_IREAD | S_IWRITE);

                if (fd != -1 && fd2 != -1) {
                    int readBytes = 0;
                    while (readBytes = read(fd, buffer, 1)) {
                        if (readBytes > 0) write(fd2, buffer, readBytes);
                    }
                    close(fd);
                    close(fd2);
                } 
                else
                    printf("Error opening the files\n");
            } 
            
            else if (strcmp(argv[0], "mymv") == 0) {
                char path[255] = "/root/sandbox/";
                strcat(path, argv[2]);
                strcat(path, "/");
                strcat(path, argv[1]);

                char buffer[1];
                int fd = open(argv[1], O_RDONLY, S_IREAD);
                int fd2 = open(path, O_RDWR | O_CREAT | O_EXCL, S_IREAD | S_IWRITE);
                printf("fd:%d, fd2: %d \n", fd, fd2);

                if (fd != -1 && fd2 != -1) {
                    int readBytes = 0;
                    while (readBytes = read(fd, buffer, 1)) {
                        if (readBytes > 0) write(fd2, buffer, readBytes);
                    }
                    close(fd);
                    close(fd2);
                    remove(argv[1]);
                }
            } 
            
            else if (strcmp(argv[0], "myrm") == 0) {
                printf("%s\n", argv[1]);
                remove(argv[1]);
            } 
            
            else if (strcmp(argv[0], "mycd") == 0) {
                printf("%s\n", argv[1]);
                if (strcmp(argv[1], "..") == 0)
                    chdir("..");
                else
                    chdir(argv[1]);
            } 
            
            else if (strcmp(argv[0], "mymkdir") == 0) {
                mkdir(argv[1], 0777);
            } 
            
            else if (strcmp(argv[0], "mypwd") == 0) {
                char dir[255];
                printf("%s\n", getcwd(dir, 255));
            } 
            
            else if (strcmp(argv[0], "myhead") == 0) {
                char buffer[1];
                printf("%s\n", argv[1]);
                int fd = open(argv[1], O_RDONLY, S_IREAD);
                int count = 0;

                if (fd != -1) {
                    int readBytes = 0;
                    while (readBytes = read(fd, buffer, 1) && count < 10) {
                        if (readBytes > 0)
                            write(0, buffer, readBytes);
                        if (buffer[0] == '\n')
                            count++;
                    }
                    close(fd);
                } 
                else
                    printf("Error opening the files\n");
            } 
            
            else if (strcmp(argv[0], "mytail") == 0) {
                printf("%s\n", argv[1]);
                int l = 255, r = 255;
                char lines[r][l];
                FILE * f = NULL;
                f = fopen(argv[1], "r");

                int i;
                for (i = 0; fgets(lines[i], l, f); i++)
                    lines[i][strlen(lines[i]) - 1] = '\0';

                int lineNum = i, last = i - 10;

                for (last = i - 10; last < lineNum; last++) {
                    printf("%s\n", lines[last]);
                }
            } 
            
            else if (strcmp(argv[0], "0") == 0);

            else{
                execvp(progpath,argv);
                fprintf(stderr, "Unrecognized command\n");
            }

            break;
        }

        else{                    //Parent
            wait(NULL);
            printf("///// Process End /////\n");
        }
    }
} 
