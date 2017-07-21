/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef crawler_TYPES_H
#define crawler_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>


namespace server { namespace pose_label {

class ReturnVals;

class PicInfo;

class QueryUnlabeledRet;

class DownloadRet;

typedef struct _ReturnVals__isset {
  _ReturnVals__isset() : code(true), msg(false) {}
  bool code :1;
  bool msg :1;
} _ReturnVals__isset;

class ReturnVals : public virtual ::apache::thrift::TBase {
 public:

  ReturnVals(const ReturnVals&);
  ReturnVals& operator=(const ReturnVals&);
  ReturnVals() : code(0), msg() {
  }

  virtual ~ReturnVals() throw();
  int32_t code;
  std::string msg;

  _ReturnVals__isset __isset;

  void __set_code(const int32_t val);

  void __set_msg(const std::string& val);

  bool operator == (const ReturnVals & rhs) const
  {
    if (!(code == rhs.code))
      return false;
    if (!(msg == rhs.msg))
      return false;
    return true;
  }
  bool operator != (const ReturnVals &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ReturnVals & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(ReturnVals &a, ReturnVals &b);

std::ostream& operator<<(std::ostream& out, const ReturnVals& obj);

typedef struct _PicInfo__isset {
  _PicInfo__isset() : oriBin(false), inflatBin(false), md5(false), key(false), tag(false) {}
  bool oriBin :1;
  bool inflatBin :1;
  bool md5 :1;
  bool key :1;
  bool tag :1;
} _PicInfo__isset;

class PicInfo : public virtual ::apache::thrift::TBase {
 public:

  PicInfo(const PicInfo&);
  PicInfo& operator=(const PicInfo&);
  PicInfo() : oriBin(), inflatBin(), md5(), key(), tag() {
  }

  virtual ~PicInfo() throw();
  std::string oriBin;
  std::string inflatBin;
  std::string md5;
  std::string key;
  std::string tag;

  _PicInfo__isset __isset;

  void __set_oriBin(const std::string& val);

  void __set_inflatBin(const std::string& val);

  void __set_md5(const std::string& val);

  void __set_key(const std::string& val);

  void __set_tag(const std::string& val);

  bool operator == (const PicInfo & rhs) const
  {
    if (!(oriBin == rhs.oriBin))
      return false;
    if (!(inflatBin == rhs.inflatBin))
      return false;
    if (!(md5 == rhs.md5))
      return false;
    if (!(key == rhs.key))
      return false;
    if (!(tag == rhs.tag))
      return false;
    return true;
  }
  bool operator != (const PicInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PicInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(PicInfo &a, PicInfo &b);

std::ostream& operator<<(std::ostream& out, const PicInfo& obj);

typedef struct _QueryUnlabeledRet__isset {
  _QueryUnlabeledRet__isset() : code(true), msg(false), picJson(false) {}
  bool code :1;
  bool msg :1;
  bool picJson :1;
} _QueryUnlabeledRet__isset;

class QueryUnlabeledRet : public virtual ::apache::thrift::TBase {
 public:

  QueryUnlabeledRet(const QueryUnlabeledRet&);
  QueryUnlabeledRet& operator=(const QueryUnlabeledRet&);
  QueryUnlabeledRet() : code(0), msg(), picJson() {
  }

  virtual ~QueryUnlabeledRet() throw();
  int32_t code;
  std::string msg;
  std::string picJson;

  _QueryUnlabeledRet__isset __isset;

  void __set_code(const int32_t val);

  void __set_msg(const std::string& val);

  void __set_picJson(const std::string& val);

  bool operator == (const QueryUnlabeledRet & rhs) const
  {
    if (!(code == rhs.code))
      return false;
    if (!(msg == rhs.msg))
      return false;
    if (!(picJson == rhs.picJson))
      return false;
    return true;
  }
  bool operator != (const QueryUnlabeledRet &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const QueryUnlabeledRet & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(QueryUnlabeledRet &a, QueryUnlabeledRet &b);

std::ostream& operator<<(std::ostream& out, const QueryUnlabeledRet& obj);

typedef struct _DownloadRet__isset {
  _DownloadRet__isset() : code(true), msg(false), bin(false), pic_id(false), HumanPose2DInfo(false), HumanPose3DInfo(false) {}
  bool code :1;
  bool msg :1;
  bool bin :1;
  bool pic_id :1;
  bool HumanPose2DInfo :1;
  bool HumanPose3DInfo :1;
} _DownloadRet__isset;

class DownloadRet : public virtual ::apache::thrift::TBase {
 public:

  DownloadRet(const DownloadRet&);
  DownloadRet& operator=(const DownloadRet&);
  DownloadRet() : code(0), msg(), bin(), pic_id(0), HumanPose2DInfo(), HumanPose3DInfo() {
  }

  virtual ~DownloadRet() throw();
  int32_t code;
  std::string msg;
  std::string bin;
  int32_t pic_id;
  std::string HumanPose2DInfo;
  std::string HumanPose3DInfo;

  _DownloadRet__isset __isset;

  void __set_code(const int32_t val);

  void __set_msg(const std::string& val);

  void __set_bin(const std::string& val);

  void __set_pic_id(const int32_t val);

  void __set_HumanPose2DInfo(const std::string& val);

  void __set_HumanPose3DInfo(const std::string& val);

  bool operator == (const DownloadRet & rhs) const
  {
    if (!(code == rhs.code))
      return false;
    if (!(msg == rhs.msg))
      return false;
    if (!(bin == rhs.bin))
      return false;
    if (!(pic_id == rhs.pic_id))
      return false;
    if (!(HumanPose2DInfo == rhs.HumanPose2DInfo))
      return false;
    if (!(HumanPose3DInfo == rhs.HumanPose3DInfo))
      return false;
    return true;
  }
  bool operator != (const DownloadRet &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DownloadRet & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(DownloadRet &a, DownloadRet &b);

std::ostream& operator<<(std::ostream& out, const DownloadRet& obj);

}} // namespace

#endif