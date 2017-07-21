/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef LabelService_H
#define LabelService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "pose_label_types.h"

namespace server { namespace pose_label {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class LabelServiceIf {
 public:
  virtual ~LabelServiceIf() {}
  virtual void QueryUnlabeledPic(QueryUnlabeledRet& _return, const std::string& user, const int32_t index) = 0;
  virtual void QueryLabeledPic(QueryLabeledRet& _return, const std::string& user, const int32_t index) = 0;
  virtual void DownloadPic(DownloadRet& _return, const std::string& pic_url) = 0;
  virtual void InsertToDb(ReturnVals& _return, const std::string& poseInfo, const int32_t pic_id, const std::string& user) = 0;
};

class LabelServiceIfFactory {
 public:
  typedef LabelServiceIf Handler;

  virtual ~LabelServiceIfFactory() {}

  virtual LabelServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(LabelServiceIf* /* handler */) = 0;
};

class LabelServiceIfSingletonFactory : virtual public LabelServiceIfFactory {
 public:
  LabelServiceIfSingletonFactory(const boost::shared_ptr<LabelServiceIf>& iface) : iface_(iface) {}
  virtual ~LabelServiceIfSingletonFactory() {}

  virtual LabelServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(LabelServiceIf* /* handler */) {}

 protected:
  boost::shared_ptr<LabelServiceIf> iface_;
};

class LabelServiceNull : virtual public LabelServiceIf {
 public:
  virtual ~LabelServiceNull() {}
  void QueryUnlabeledPic(QueryUnlabeledRet& /* _return */, const std::string& /* user */, const int32_t /* index */) {
    return;
  }
  void QueryLabeledPic(QueryLabeledRet& /* _return */, const std::string& /* user */, const int32_t /* index */) {
    return;
  }
  void DownloadPic(DownloadRet& /* _return */, const std::string& /* pic_url */) {
    return;
  }
  void InsertToDb(ReturnVals& /* _return */, const std::string& /* poseInfo */, const int32_t /* pic_id */, const std::string& /* user */) {
    return;
  }
};

typedef struct _LabelService_QueryUnlabeledPic_args__isset {
  _LabelService_QueryUnlabeledPic_args__isset() : user(false), index(false) {}
  bool user :1;
  bool index :1;
} _LabelService_QueryUnlabeledPic_args__isset;

class LabelService_QueryUnlabeledPic_args {
 public:

  LabelService_QueryUnlabeledPic_args(const LabelService_QueryUnlabeledPic_args&);
  LabelService_QueryUnlabeledPic_args& operator=(const LabelService_QueryUnlabeledPic_args&);
  LabelService_QueryUnlabeledPic_args() : user(), index(0) {
  }

  virtual ~LabelService_QueryUnlabeledPic_args() throw();
  std::string user;
  int32_t index;

  _LabelService_QueryUnlabeledPic_args__isset __isset;

  void __set_user(const std::string& val);

  void __set_index(const int32_t val);

  bool operator == (const LabelService_QueryUnlabeledPic_args & rhs) const
  {
    if (!(user == rhs.user))
      return false;
    if (!(index == rhs.index))
      return false;
    return true;
  }
  bool operator != (const LabelService_QueryUnlabeledPic_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LabelService_QueryUnlabeledPic_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class LabelService_QueryUnlabeledPic_pargs {
 public:


  virtual ~LabelService_QueryUnlabeledPic_pargs() throw();
  const std::string* user;
  const int32_t* index;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LabelService_QueryUnlabeledPic_result__isset {
  _LabelService_QueryUnlabeledPic_result__isset() : success(false) {}
  bool success :1;
} _LabelService_QueryUnlabeledPic_result__isset;

class LabelService_QueryUnlabeledPic_result {
 public:

  LabelService_QueryUnlabeledPic_result(const LabelService_QueryUnlabeledPic_result&);
  LabelService_QueryUnlabeledPic_result& operator=(const LabelService_QueryUnlabeledPic_result&);
  LabelService_QueryUnlabeledPic_result() {
  }

  virtual ~LabelService_QueryUnlabeledPic_result() throw();
  QueryUnlabeledRet success;

  _LabelService_QueryUnlabeledPic_result__isset __isset;

  void __set_success(const QueryUnlabeledRet& val);

  bool operator == (const LabelService_QueryUnlabeledPic_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const LabelService_QueryUnlabeledPic_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LabelService_QueryUnlabeledPic_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LabelService_QueryUnlabeledPic_presult__isset {
  _LabelService_QueryUnlabeledPic_presult__isset() : success(false) {}
  bool success :1;
} _LabelService_QueryUnlabeledPic_presult__isset;

class LabelService_QueryUnlabeledPic_presult {
 public:


  virtual ~LabelService_QueryUnlabeledPic_presult() throw();
  QueryUnlabeledRet* success;

  _LabelService_QueryUnlabeledPic_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _LabelService_QueryLabeledPic_args__isset {
  _LabelService_QueryLabeledPic_args__isset() : user(false), index(false) {}
  bool user :1;
  bool index :1;
} _LabelService_QueryLabeledPic_args__isset;

class LabelService_QueryLabeledPic_args {
 public:

  LabelService_QueryLabeledPic_args(const LabelService_QueryLabeledPic_args&);
  LabelService_QueryLabeledPic_args& operator=(const LabelService_QueryLabeledPic_args&);
  LabelService_QueryLabeledPic_args() : user(), index(0) {
  }

  virtual ~LabelService_QueryLabeledPic_args() throw();
  std::string user;
  int32_t index;

  _LabelService_QueryLabeledPic_args__isset __isset;

  void __set_user(const std::string& val);

  void __set_index(const int32_t val);

  bool operator == (const LabelService_QueryLabeledPic_args & rhs) const
  {
    if (!(user == rhs.user))
      return false;
    if (!(index == rhs.index))
      return false;
    return true;
  }
  bool operator != (const LabelService_QueryLabeledPic_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LabelService_QueryLabeledPic_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class LabelService_QueryLabeledPic_pargs {
 public:


  virtual ~LabelService_QueryLabeledPic_pargs() throw();
  const std::string* user;
  const int32_t* index;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LabelService_QueryLabeledPic_result__isset {
  _LabelService_QueryLabeledPic_result__isset() : success(false) {}
  bool success :1;
} _LabelService_QueryLabeledPic_result__isset;

class LabelService_QueryLabeledPic_result {
 public:

  LabelService_QueryLabeledPic_result(const LabelService_QueryLabeledPic_result&);
  LabelService_QueryLabeledPic_result& operator=(const LabelService_QueryLabeledPic_result&);
  LabelService_QueryLabeledPic_result() {
  }

  virtual ~LabelService_QueryLabeledPic_result() throw();
  QueryLabeledRet success;

  _LabelService_QueryLabeledPic_result__isset __isset;

  void __set_success(const QueryLabeledRet& val);

  bool operator == (const LabelService_QueryLabeledPic_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const LabelService_QueryLabeledPic_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LabelService_QueryLabeledPic_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LabelService_QueryLabeledPic_presult__isset {
  _LabelService_QueryLabeledPic_presult__isset() : success(false) {}
  bool success :1;
} _LabelService_QueryLabeledPic_presult__isset;

class LabelService_QueryLabeledPic_presult {
 public:


  virtual ~LabelService_QueryLabeledPic_presult() throw();
  QueryLabeledRet* success;

  _LabelService_QueryLabeledPic_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _LabelService_DownloadPic_args__isset {
  _LabelService_DownloadPic_args__isset() : pic_url(false) {}
  bool pic_url :1;
} _LabelService_DownloadPic_args__isset;

class LabelService_DownloadPic_args {
 public:

  LabelService_DownloadPic_args(const LabelService_DownloadPic_args&);
  LabelService_DownloadPic_args& operator=(const LabelService_DownloadPic_args&);
  LabelService_DownloadPic_args() : pic_url() {
  }

  virtual ~LabelService_DownloadPic_args() throw();
  std::string pic_url;

  _LabelService_DownloadPic_args__isset __isset;

  void __set_pic_url(const std::string& val);

  bool operator == (const LabelService_DownloadPic_args & rhs) const
  {
    if (!(pic_url == rhs.pic_url))
      return false;
    return true;
  }
  bool operator != (const LabelService_DownloadPic_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LabelService_DownloadPic_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class LabelService_DownloadPic_pargs {
 public:


  virtual ~LabelService_DownloadPic_pargs() throw();
  const std::string* pic_url;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LabelService_DownloadPic_result__isset {
  _LabelService_DownloadPic_result__isset() : success(false) {}
  bool success :1;
} _LabelService_DownloadPic_result__isset;

class LabelService_DownloadPic_result {
 public:

  LabelService_DownloadPic_result(const LabelService_DownloadPic_result&);
  LabelService_DownloadPic_result& operator=(const LabelService_DownloadPic_result&);
  LabelService_DownloadPic_result() {
  }

  virtual ~LabelService_DownloadPic_result() throw();
  DownloadRet success;

  _LabelService_DownloadPic_result__isset __isset;

  void __set_success(const DownloadRet& val);

  bool operator == (const LabelService_DownloadPic_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const LabelService_DownloadPic_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LabelService_DownloadPic_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LabelService_DownloadPic_presult__isset {
  _LabelService_DownloadPic_presult__isset() : success(false) {}
  bool success :1;
} _LabelService_DownloadPic_presult__isset;

class LabelService_DownloadPic_presult {
 public:


  virtual ~LabelService_DownloadPic_presult() throw();
  DownloadRet* success;

  _LabelService_DownloadPic_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _LabelService_InsertToDb_args__isset {
  _LabelService_InsertToDb_args__isset() : poseInfo(false), pic_id(false), user(false) {}
  bool poseInfo :1;
  bool pic_id :1;
  bool user :1;
} _LabelService_InsertToDb_args__isset;

class LabelService_InsertToDb_args {
 public:

  LabelService_InsertToDb_args(const LabelService_InsertToDb_args&);
  LabelService_InsertToDb_args& operator=(const LabelService_InsertToDb_args&);
  LabelService_InsertToDb_args() : poseInfo(), pic_id(0), user() {
  }

  virtual ~LabelService_InsertToDb_args() throw();
  std::string poseInfo;
  int32_t pic_id;
  std::string user;

  _LabelService_InsertToDb_args__isset __isset;

  void __set_poseInfo(const std::string& val);

  void __set_pic_id(const int32_t val);

  void __set_user(const std::string& val);

  bool operator == (const LabelService_InsertToDb_args & rhs) const
  {
    if (!(poseInfo == rhs.poseInfo))
      return false;
    if (!(pic_id == rhs.pic_id))
      return false;
    if (!(user == rhs.user))
      return false;
    return true;
  }
  bool operator != (const LabelService_InsertToDb_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LabelService_InsertToDb_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class LabelService_InsertToDb_pargs {
 public:


  virtual ~LabelService_InsertToDb_pargs() throw();
  const std::string* poseInfo;
  const int32_t* pic_id;
  const std::string* user;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LabelService_InsertToDb_result__isset {
  _LabelService_InsertToDb_result__isset() : success(false) {}
  bool success :1;
} _LabelService_InsertToDb_result__isset;

class LabelService_InsertToDb_result {
 public:

  LabelService_InsertToDb_result(const LabelService_InsertToDb_result&);
  LabelService_InsertToDb_result& operator=(const LabelService_InsertToDb_result&);
  LabelService_InsertToDb_result() {
  }

  virtual ~LabelService_InsertToDb_result() throw();
  ReturnVals success;

  _LabelService_InsertToDb_result__isset __isset;

  void __set_success(const ReturnVals& val);

  bool operator == (const LabelService_InsertToDb_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const LabelService_InsertToDb_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LabelService_InsertToDb_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LabelService_InsertToDb_presult__isset {
  _LabelService_InsertToDb_presult__isset() : success(false) {}
  bool success :1;
} _LabelService_InsertToDb_presult__isset;

class LabelService_InsertToDb_presult {
 public:


  virtual ~LabelService_InsertToDb_presult() throw();
  ReturnVals* success;

  _LabelService_InsertToDb_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class LabelServiceClient : virtual public LabelServiceIf {
 public:
  LabelServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  LabelServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void QueryUnlabeledPic(QueryUnlabeledRet& _return, const std::string& user, const int32_t index);
  void send_QueryUnlabeledPic(const std::string& user, const int32_t index);
  void recv_QueryUnlabeledPic(QueryUnlabeledRet& _return);
  void QueryLabeledPic(QueryLabeledRet& _return, const std::string& user, const int32_t index);
  void send_QueryLabeledPic(const std::string& user, const int32_t index);
  void recv_QueryLabeledPic(QueryLabeledRet& _return);
  void DownloadPic(DownloadRet& _return, const std::string& pic_url);
  void send_DownloadPic(const std::string& pic_url);
  void recv_DownloadPic(DownloadRet& _return);
  void InsertToDb(ReturnVals& _return, const std::string& poseInfo, const int32_t pic_id, const std::string& user);
  void send_InsertToDb(const std::string& poseInfo, const int32_t pic_id, const std::string& user);
  void recv_InsertToDb(ReturnVals& _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class LabelServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<LabelServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (LabelServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_QueryUnlabeledPic(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_QueryLabeledPic(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_DownloadPic(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_InsertToDb(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  LabelServiceProcessor(boost::shared_ptr<LabelServiceIf> iface) :
    iface_(iface) {
    processMap_["QueryUnlabeledPic"] = &LabelServiceProcessor::process_QueryUnlabeledPic;
    processMap_["QueryLabeledPic"] = &LabelServiceProcessor::process_QueryLabeledPic;
    processMap_["DownloadPic"] = &LabelServiceProcessor::process_DownloadPic;
    processMap_["InsertToDb"] = &LabelServiceProcessor::process_InsertToDb;
  }

  virtual ~LabelServiceProcessor() {}
};

class LabelServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  LabelServiceProcessorFactory(const ::boost::shared_ptr< LabelServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< LabelServiceIfFactory > handlerFactory_;
};

class LabelServiceMultiface : virtual public LabelServiceIf {
 public:
  LabelServiceMultiface(std::vector<boost::shared_ptr<LabelServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~LabelServiceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<LabelServiceIf> > ifaces_;
  LabelServiceMultiface() {}
  void add(boost::shared_ptr<LabelServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void QueryUnlabeledPic(QueryUnlabeledRet& _return, const std::string& user, const int32_t index) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->QueryUnlabeledPic(_return, user, index);
    }
    ifaces_[i]->QueryUnlabeledPic(_return, user, index);
    return;
  }

  void QueryLabeledPic(QueryLabeledRet& _return, const std::string& user, const int32_t index) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->QueryLabeledPic(_return, user, index);
    }
    ifaces_[i]->QueryLabeledPic(_return, user, index);
    return;
  }

  void DownloadPic(DownloadRet& _return, const std::string& pic_url) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->DownloadPic(_return, pic_url);
    }
    ifaces_[i]->DownloadPic(_return, pic_url);
    return;
  }

  void InsertToDb(ReturnVals& _return, const std::string& poseInfo, const int32_t pic_id, const std::string& user) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->InsertToDb(_return, poseInfo, pic_id, user);
    }
    ifaces_[i]->InsertToDb(_return, poseInfo, pic_id, user);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class LabelServiceConcurrentClient : virtual public LabelServiceIf {
 public:
  LabelServiceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  LabelServiceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void QueryUnlabeledPic(QueryUnlabeledRet& _return, const std::string& user, const int32_t index);
  int32_t send_QueryUnlabeledPic(const std::string& user, const int32_t index);
  void recv_QueryUnlabeledPic(QueryUnlabeledRet& _return, const int32_t seqid);
  void QueryLabeledPic(QueryLabeledRet& _return, const std::string& user, const int32_t index);
  int32_t send_QueryLabeledPic(const std::string& user, const int32_t index);
  void recv_QueryLabeledPic(QueryLabeledRet& _return, const int32_t seqid);
  void DownloadPic(DownloadRet& _return, const std::string& pic_url);
  int32_t send_DownloadPic(const std::string& pic_url);
  void recv_DownloadPic(DownloadRet& _return, const int32_t seqid);
  void InsertToDb(ReturnVals& _return, const std::string& poseInfo, const int32_t pic_id, const std::string& user);
  int32_t send_InsertToDb(const std::string& poseInfo, const int32_t pic_id, const std::string& user);
  void recv_InsertToDb(ReturnVals& _return, const int32_t seqid);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif

}} // namespace

#endif
