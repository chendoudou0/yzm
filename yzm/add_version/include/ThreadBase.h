#ifndef THREAD_BASE_H
#define THREAD_BASE_H
/*
* �ļ����ƣ�ThreadBase.h
* �ļ���ʶ��
* ժ����Ҫ���̻߳���
*/

// �̵߳�ʹ��ģʽ
// ��1�����̴߳������̣߳�����ThreadBase����ͨ�������ࣩ
// ��2������start()�����������̣߳����߳̿�ʼ���У�ѭ������run()������
// ��3������ѡ�����߳�������ɣ�֪ͨ���̣߳�ʹ���������ƣ�
// ��4�����߳���Ϊ���߳�������ɣ���������ԭ����Ҫ�������߳����У�
//      ����stop()����ֹͣ���̡߳��ڲ��������ñ�־�����߳�run()�м�⵽��ʶ�˳����������Ѿ��˳��������̵߳ȴ����߳̽������������߳�
// ��5�����߳��ͷ�ThreadBase����

class ThreadBase
{
public: 
	ThreadBase(int nStackSize = 0);
	virtual ~ThreadBase(void);

	virtual bool start(void); 
	virtual bool stop(void);  // ֹͣ�̣߳����ȴ��߳̽��������Ա����������߳��е��ã������ڱ��߳��е���
	                          // ���⼴ʹ���߳�run()�����Ѿ������˳����ⲿҲҪ���ô˺���

public:
	//G_hthread_t getThreadHandle(void) const { return _handle; }

	// �ж��߳��Ƿ������У����ⲿһ�㲻Ӧ�ý�������жϣ�����������ϵ����⣬�߳�������ֹͣʱ������race condition
	bool isRunning(void) const { return m_state == TB_STATE_RUNNING; }

protected:
	// ������������run()�����У��ж��߳��Ƿ���Ҫ��ǰ�˳�
	bool isTerminated(void) const { return m_state != TB_STATE_RUNNING; }

protected:

	// �߳�ִ�к���,����ֵ<0���˳��߳�
	// �˺����ᱻѭ�����ã����������ʱӦ�ʵ�sleep�������̫��CPU��
	// �˺�����Ӧ��ִ��̫��ʱ�䣬����Ӱ��stop����Ĵ������ߺ����ڲ���������isTerminated()����Ƿ�ֹͣ
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
