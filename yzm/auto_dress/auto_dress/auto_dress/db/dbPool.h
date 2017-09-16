#ifndef DB__POOL_H
#define DB__POOL_H

#include <deque>
#include <functional>
#include <memory>
#include <mutex>

template <typename T>
    using delFun = std::function<void(T*)>;
template <typename T>
    using dbOperPtr = std::unique_ptr<T, delFun<T>>; 

template <typename T>  
class CDbPool {
 public:
  CDbPool()
  {
  }
  ~CDbPool() { db_que_.clear(); }
  void   SetPoolSize(int size){
    pool_size_ = size;
  }

void InitPool() {
    std::lock_guard<std::mutex> l(mtx_);
    for (int i = 0; i < pool_size_; ++i) {
      db_que_.emplace_back( new T() );
    } 
  }

  dbOperPtr<T> GetDbOper() {
    std::lock_guard<std::mutex> l(mtx_);
    if (db_que_.empty()) {
      return nullptr;     
    }

    auto db_oper = std::move(db_que_.front());
    db_que_.pop_front();

    using std::placeholders::_1;  
    dbOperPtr<T> ptr(db_oper.release(), std::bind(&CDbPool::Release, this, _1));
    return ptr;    
  }
 
 private:
  void Release(T* db_oper) {
    if (db_oper) {   
      std::lock_guard<std::mutex> l(mtx_);
      db_que_.emplace_back(db_oper);   
    }
  }

  using DBOperaPtr = std::unique_ptr<T>;
  int pool_size_{4};
  std::mutex mtx_;
  std::deque<DBOperaPtr>  db_que_;    
};

#endif 




