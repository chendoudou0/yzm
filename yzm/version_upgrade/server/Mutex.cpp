#include "Mutex.h"

int cMutex::Init()
{
    if(pthread_mutex_init(&m_mutex,NULL) != 0 )  
    {  
        exit(1);  
    }  
	return 0;
}

cMutex::cMutex()
{

}
cMutex::~cMutex()
{
	 pthread_mutex_destroy( &m_mutex );
}

int cMutex::Lock()
{
	int val;  
    val = pthread_mutex_lock(&m_mutex);/*lock the mutex*/  
    if(val != 0)  
    {  
		return -1;
    }  
	return 0;
}

int cMutex::unLock()
{
	pthread_mutex_unlock(&m_mutex);/*unlock the mutex*/ 
	return 0;
}

