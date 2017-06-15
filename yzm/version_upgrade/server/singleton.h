/*
* 文件名称：Singleton.h
* 文件标识：
* 摘　　要：单件模版类
*/

#ifndef SINGLETON_H
#define SINGLETON_H

//////////////////////////////////////////////////////////////////////////
// class Singleton 
//////////////////////////////////////////////////////////////////////////
#include<mutex>

template <typename T>
class Singleton
{
public:
	Singleton() = default;
	virtual ~Singleton() = default;
	Singleton(const Singleton<T> &) = delete;
	Singleton<T>& operator= (const Singleton<T> &) = delete;
	static T* instance()
	{
		std::lock_guard<std::mutex>  l(s_mutex);
		if (nullptr == s_instance)
		{
			if (0 == s_instance)
			{
				s_instance = new T;
			}
		}

		return s_instance;
	}

	static void unInstance()
	{
		std::lock_guard<std::mutex>  l(s_mutex);
		if (nullptr != s_instance)
		{
			if (0 != s_instance)
			{
				delete s_instance;
				s_instance = 0;
			}
		}
	}

	static T* s_instance;
	static std::mutex s_mutex;
};

template <class T>
T* Singleton<T>::s_instance = nullptr;

template <class T>
std::mutex Singleton<T>::s_mutex;

#endif