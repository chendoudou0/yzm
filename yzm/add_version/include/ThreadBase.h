#ifndef THREAD_BASE_H
#define THREAD_BASE_H
/*
* 文件名称：ThreadBase.h
* 文件标识：
* 摘　　要：线程基类
*/

// 线程的使用模式
// （1）父线程创建子线程，创建ThreadBase对象（通常是子类）
// （2）调用start()函数启动子线程，子线程开始运行（循环调用run()函数）
// （3）（可选）子线程运行完成，通知父线程（使用其他机制）
// （4）父线程因为子线程运行完成，或者其他原因需要结束子线程运行，
//      调用stop()函数停止子线程。内部会先设置标志，子线程run()中检测到标识退出（或者早已经退出），父线程等待子线程结束并销毁子线程
// （5）父线程释放ThreadBase对象

class ThreadBase
{
public: 
	ThreadBase(int nStackSize = 0);
	virtual ~ThreadBase(void);

	virtual bool start(void); 
	virtual bool stop(void);  // 停止线程，并等待线程结束，所以必须在其他线程中调用，不能在本线程中调用
	                          // 另外即使子线程run()函数已经主动退出，外部也要调用此函数

public:
	//G_hthread_t getThreadHandle(void) const { return _handle; }

	// 判断线程是否还在运行，但外部一般不应该进行这个判断，往往是设计上的问题，线程启动和停止时可能有race condition
	bool isRunning(void) const { return m_state == TB_STATE_RUNNING; }

protected:
	// 可以用于子类run()函数中，判断线程是否需要提前退出
	bool isTerminated(void) const { return m_state != TB_STATE_RUNNING; }

protected:

	// 线程执行函数,返回值<0就退出线程
	// 此函数会被循环调用，因此无任务时应适当sleep，以免耗太多CPU，
	// 此函数不应该执行太长时间，以免影响stop命令的处理，或者函数内部主动调用isTerminated()检查是否停止
	virtual int run(void) = 0;

private:
    ThreadBase(const ThreadBase &);
    ThreadBase & operator=(const ThreadBase &);

    static void * threadFunc(void * arg);

protected:
	int _stackSize;

	volatile enum { TB_STATE_EMPTY, TB_STATE_RUNNING, TB_STATE_STOPING, } m_state;

	//G_hthread_t _handle;
	//G_thread_t _threadId;
};

#endif
