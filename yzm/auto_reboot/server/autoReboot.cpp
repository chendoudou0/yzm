#include <stdio.h>  
#include <unistd.h>  
#include <errno.h>  
#include <string.h>  
#include <sys/types.h>  
#include <sys/wait.h>  
#include <stdlib.h>  
   
int  
main(int argc, char **argv)  
{    
    int ret, i, status;  
    char *child_argv[100] = {0};  
    pid_t pid;  
    if (argc < 2) {  
        printf("Usage:%s <exe_path> <args...>", argv[0]);  
        return -1;  
    }  
    for (i = 1; i < argc; ++i) {  
        child_argv[i-1] = (char *)malloc(strlen(argv[i])+1);  
        strncpy(child_argv[i-1], argv[i], strlen(argv[i]));  
        child_argv[i-1][strlen(argv[i])] = 0;  
    }  
    while(1){  
   
        pid = fork();   
        if (pid == -1) {  
            printf("fork() error.errno:%d error:%s", errno, strerror(errno));  
            break;    
        }  
        if (pid == 0) {  
            ret = execv(child_argv[0], (char **)child_argv);  
            if (ret < 0) {  
                printf("execv ret:%d errno:%d error:%s", ret, errno, strerror(errno)); 
		        sleep(1); 
                continue;  
            }  
            exit(0);  
        }  
   
        if (pid > 0) {  
            pid = wait(&status);  
            printf("wait return");  
        }  
   
    }  
   
    return 0;  
}  
