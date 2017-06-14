/*
* �ļ����ƣ�Singleton.h
* �ļ���ʶ��
* ժ����Ҫ������ģ����
*/

#ifndef SINGLETON_H
#define SINGLETON_H

#include <pthread.h>

//////////////////////////////////////////////////////////////////////////
// class Singleton 
//////////////////////////////////////////////////////////////////////////

template <class T> 
class Singleton  
{
public:
	static T* instance()
	{ 
		if ( 0 == s_instance )
		{
			pthread_mutex_lock(&s_mutex);
			if ( 0 == s_instance )
			{
				s_instance = new T;
			}
			pthread_mutex_unlock(&s_mutex);
		}

		return s_instance;
	}

	static void unInstance()
	{
		if ( 0 != s_instance )
		{
			pthread_mutex_lock(&s_mutex);
			if ( 0 != s_instance )
			{
				delete s_instance;
				s_instance = 0;
			}
			pthread_mutex_unlock(&s_mutex);
		}
	}

protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	Singleton(const Singleton<T> &); //��ʵ�� 
	Singleton<T>& operator= (const Singleton<T> &); //��ʵ��

	static T* s_instance;
	static pthread_mutex_t s_mutex;
};

template <class T>
T* Singleton<T>::s_instance = 0;

template <class T>
pthread_mutex_t Singleton<T>::s_mutex = PTHREAD_MUTEX_INITIALIZER;

////////////////////////////////////////////////////////////////

#endif
