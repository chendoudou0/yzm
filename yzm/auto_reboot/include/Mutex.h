#ifndef _CMUTEX_H_
#define _CMUTEX_H_


#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket 
#include <sys/socket.h>    // for socket 
#include <stdio.h>        // for printf 
#include <stdlib.h>        // for exit 
#include <string.h>        // for bzero 
#include <time.h>          //for time_t and time 
#include <arpa/inet.h> 
#include <errno.h>
#include <fcntl.h>

#include<pthread.h> 

#include<netinet/in.h>

#include "stdio.h"
#include <string>
#include <iostream>


class cMutex
{     
public:
        cMutex();
        ~cMutex();
        
        int Init();
        int Lock();
        int unLock();
                
private:
        pthread_mutex_t m_mutex;

};

#endif

