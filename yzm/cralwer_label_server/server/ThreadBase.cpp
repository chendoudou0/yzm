#include "ThreadBase.h"
#include <pthread.h>


void* ThreadBase::threadFunc(void *arg)
{
	ThreadBase *thread = (ThreadBase *)arg;
	if (thread == 0)                   
	{
		return 0; // error
	}

	while (!thread->isTerminated()) 
	{
		if (thread->run() < 0) 
		{
			//PLOG_INFO( "thread=%d exit because of return value!", (int)thread->getThreadHandle());
			thread->m_state = TB_STATE_STOPING; 
			break;
		}                                                                 
	}         

	return 0;
}

ThreadBase::ThreadBase(int nStackSize /* = 0 */)
:_stackSize(nStackSize)
{

	m_state = TB_STATE_EMPTY;
}

ThreadBase::~ThreadBase() 
{
	stop();	
}

bool ThreadBase::start() 
{
	if( m_state != TB_STATE_EMPTY )
		return true;                 

	m_state = TB_STATE_RUNNING;
	pthread_t tid;
	pthread_attr_t thr_attr;
	pthread_attr_t *pAttr = NULL;
    if(pAttr == NULL)
    {
        pAttr = &thr_attr;
        pthread_attr_init(pAttr);
        pthread_attr_setstacksize(pAttr, 1024 * 1024);  // 1 M的堆栈
        pthread_attr_setdetachstate(pAttr,  PTHREAD_CREATE_DETACHED);
    }
  
    int ret = pthread_create(&tid, pAttr, threadFunc, this);
    pthread_attr_destroy(pAttr);
    if ( ret < 0 )                                                      
	{
		m_state = TB_STATE_EMPTY;                                
		return false;                                                    
	}
		
	return true;
}

bool ThreadBase::stop()
{
	if( m_state != TB_STATE_EMPTY )
	{
		m_state = TB_STATE_STOPING;

		/*
		// 外部必须保证不在线程内部调用
		if( G_OS::thr_cmp_self( _threadId ) ) {
			PLOG_ERR( "thread close in self, should not do this!!!! thread = %d", (int)getThreadHandle() );
			return false;
		}
			
		G_thr_func_return ret = 0;
		G_Thread::join(_handle, &ret);
		_handle = 0;
		_threadId = 0;
		m_state = TB_STATE_EMPTY;

		*/
	}	

	return true;
}

