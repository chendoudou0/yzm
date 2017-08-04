/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef pose_label_TYPES_H
#define pose_label_TYPES_H

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

class CrawlerStatus;

class CrawlerStatusRet;

class CrawlingResult;

class HistoryQueryCondition;

class CrawlerHistory;

class CrawlerHistoryRet;

class QueryConditions;

class QueryedPicInfo;

class CrawleredPicRet;

class QueryLabeledRet;

class QueryUnlabeledRet;

class QueryByIdRet;

class DownloadRet;

class LabeledPoseDataRet;

class QueryCondition;

class LoginRet;

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
  _PicInfo__isset() : oriBin(false), filename(false), md5(false), key(false), tag(false), ori_url(false), website(false) {}
  bool oriBin :1;
  bool filename :1;
  bool md5 :1;
  bool key :1;
  bool tag :1;
  bool ori_url :1;
  bool website :1;
} _PicInfo__isset;

class PicInfo : public virtual ::apache::thrift::TBase {
 public:

  PicInfo(const PicInfo&);
  PicInfo& operator=(const PicInfo&);
  PicInfo() : oriBin(), filename(), md5(), key(), tag(), ori_url(), website() {
  }

  virtual ~PicInfo() throw();
  std::string oriBin;
  std::string filename;
  std::string md5;
  std::string key;
  std::string tag;
  std::string ori_url;
  std::string website;

  _PicInfo__isset __isset;

  void __set_oriBin(const std::string& val);

  void __set_filename(const std::string& val);

  void __set_md5(const std::string& val);

  void __set_key(const std::string& val);

  void __set_tag(const std::string& val);

  void __set_ori_url(const std::string& val);

  void __set_website(const std::string& val);

  bool operator == (const PicInfo & rhs) const
  {
    if (!(oriBin == rhs.oriBin))
      return false;
    if (!(filename == rhs.filename))
      return false;
    if (!(md5 == rhs.md5))
      return false;
    if (!(key == rhs.key))
      return false;
    if (!(tag == rhs.tag))
      return false;
    if (!(ori_url == rhs.ori_url))
      return false;
    if (!(website == rhs.website))
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

typedef struct _CrawlerStatus__isset {
  _CrawlerStatus__isset() : totalPicSum(false), dbPicSum(false) {}
  bool totalPicSum :1;
  bool dbPicSum :1;
} _CrawlerStatus__isset;

class CrawlerStatus : public virtual ::apache::thrift::TBase {
 public:

  CrawlerStatus(const CrawlerStatus&);
  CrawlerStatus& operator=(const CrawlerStatus&);
  CrawlerStatus() : totalPicSum(0), dbPicSum(0) {
  }

  virtual ~CrawlerStatus() throw();
  int32_t totalPicSum;
  int32_t dbPicSum;

  _CrawlerStatus__isset __isset;

  void __set_totalPicSum(const int32_t val);

  void __set_dbPicSum(const int32_t val);

  bool operator == (const CrawlerStatus & rhs) const
  {
    if (!(totalPicSum == rhs.totalPicSum))
      return false;
    if (!(dbPicSum == rhs.dbPicSum))
      return false;
    return true;
  }
  bool operator != (const CrawlerStatus &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CrawlerStatus & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(CrawlerStatus &a, CrawlerStatus &b);

std::ostream& operator<<(std::ostream& out, const CrawlerStatus& obj);

typedef struct _CrawlerStatusRet__isset {
  _CrawlerStatusRet__isset() : code(true), msg(false), crawlerStatus(false) {}
  bool code :1;
  bool msg :1;
  bool crawlerStatus :1;
} _CrawlerStatusRet__isset;

class CrawlerStatusRet : public virtual ::apache::thrift::TBase {
 public:

  CrawlerStatusRet(const CrawlerStatusRet&);
  CrawlerStatusRet& operator=(const CrawlerStatusRet&);
  CrawlerStatusRet() : code(0), msg() {
  }

  virtual ~CrawlerStatusRet() throw();
  int32_t code;
  std::string msg;
  CrawlerStatus crawlerStatus;

  _CrawlerStatusRet__isset __isset;

  void __set_code(const int32_t val);

  void __set_msg(const std::string& val);

  void __set_crawlerStatus(const CrawlerStatus& val);

  bool operator == (const CrawlerStatusRet & rhs) const
  {
    if (!(code == rhs.code))
      return false;
    if (!(msg == rhs.msg))
      return false;
    if (!(crawlerStatus == rhs.crawlerStatus))
      return false;
    return true;
  }
  bool operator != (const CrawlerStatusRet &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CrawlerStatusRet & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(CrawlerStatusRet &a, CrawlerStatusRet &b);

std::ostream& operator<<(std::ostream& out, const CrawlerStatusRet& obj);

typedef struct _CrawlingResult__isset {
  _CrawlingResult__isset() : key(false), tag(false), sumTotal(false), tBegin(false), tEnd(false) {}
  bool key :1;
  bool tag :1;
  bool sumTotal :1;
  bool tBegin :1;
  bool tEnd :1;
} _CrawlingResult__isset;

class CrawlingResult : public virtual ::apache::thrift::TBase {
 public:

  CrawlingResult(const CrawlingResult&);
  CrawlingResult& operator=(const CrawlingResult&);
  CrawlingResult() : key(), tag(), sumTotal(0), tBegin(), tEnd() {
  }

  virtual ~CrawlingResult() throw();
  std::string key;
  std::string tag;
  int32_t sumTotal;
  std::string tBegin;
  std::string tEnd;

  _CrawlingResult__isset __isset;

  void __set_key(const std::string& val);

  void __set_tag(const std::string& val);

  void __set_sumTotal(const int32_t val);

  void __set_tBegin(const std::string& val);

  void __set_tEnd(const std::string& val);

  bool operator == (const CrawlingResult & rhs) const
  {
    if (!(key == rhs.key))
      return false;
    if (!(tag == rhs.tag))
      return false;
    if (!(sumTotal == rhs.sumTotal))
      return false;
    if (!(tBegin == rhs.tBegin))
      return false;
    if (!(tEnd == rhs.tEnd))
      return false;
    return true;
  }
  bool operator != (const CrawlingResult &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CrawlingResult & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(CrawlingResult &a, CrawlingResult &b);

std::ostream& operator<<(std::ostream& out, const CrawlingResult& obj);

typedef struct _HistoryQueryCondition__isset {
  _HistoryQueryCondition__isset() : key(false), tag(false) {}
  bool key :1;
  bool tag :1;
} _HistoryQueryCondition__isset;

class HistoryQueryCondition : public virtual ::apache::thrift::TBase {
 public:

  HistoryQueryCondition(const HistoryQueryCondition&);
  HistoryQueryCondition& operator=(const HistoryQueryCondition&);
  HistoryQueryCondition() : key(), tag() {
  }

  virtual ~HistoryQueryCondition() throw();
  std::string key;
  std::string tag;

  _HistoryQueryCondition__isset __isset;

  void __set_key(const std::string& val);

  void __set_tag(const std::string& val);

  bool operator == (const HistoryQueryCondition & rhs) const
  {
    if (!(key == rhs.key))
      return false;
    if (!(tag == rhs.tag))
      return false;
    return true;
  }
  bool operator != (const HistoryQueryCondition &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const HistoryQueryCondition & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(HistoryQueryCondition &a, HistoryQueryCondition &b);

std::ostream& operator<<(std::ostream& out, const HistoryQueryCondition& obj);

typedef struct _CrawlerHistory__isset {
  _CrawlerHistory__isset() : key(false), tag(false), total_sum(false), db_sum(false), time_create(false) {}
  bool key :1;
  bool tag :1;
  bool total_sum :1;
  bool db_sum :1;
  bool time_create :1;
} _CrawlerHistory__isset;

class CrawlerHistory : public virtual ::apache::thrift::TBase {
 public:

  CrawlerHistory(const CrawlerHistory&);
  CrawlerHistory& operator=(const CrawlerHistory&);
  CrawlerHistory() : key(), tag(), total_sum(0), db_sum(0), time_create() {
  }

  virtual ~CrawlerHistory() throw();
  std::string key;
  std::string tag;
  int32_t total_sum;
  int32_t db_sum;
  std::string time_create;

  _CrawlerHistory__isset __isset;

  void __set_key(const std::string& val);

  void __set_tag(const std::string& val);

  void __set_total_sum(const int32_t val);

  void __set_db_sum(const int32_t val);

  void __set_time_create(const std::string& val);

  bool operator == (const CrawlerHistory & rhs) const
  {
    if (!(key == rhs.key))
      return false;
    if (!(tag == rhs.tag))
      return false;
    if (!(total_sum == rhs.total_sum))
      return false;
    if (!(db_sum == rhs.db_sum))
      return false;
    if (!(time_create == rhs.time_create))
      return false;
    return true;
  }
  bool operator != (const CrawlerHistory &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CrawlerHistory & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(CrawlerHistory &a, CrawlerHistory &b);

std::ostream& operator<<(std::ostream& out, const CrawlerHistory& obj);

typedef struct _CrawlerHistoryRet__isset {
  _CrawlerHistoryRet__isset() : code(true), msg(false), hisVec(false) {}
  bool code :1;
  bool msg :1;
  bool hisVec :1;
} _CrawlerHistoryRet__isset;

class CrawlerHistoryRet : public virtual ::apache::thrift::TBase {
 public:

  CrawlerHistoryRet(const CrawlerHistoryRet&);
  CrawlerHistoryRet& operator=(const CrawlerHistoryRet&);
  CrawlerHistoryRet() : code(0), msg() {
  }

  virtual ~CrawlerHistoryRet() throw();
  int32_t code;
  std::string msg;
  std::vector<CrawlerHistory>  hisVec;

  _CrawlerHistoryRet__isset __isset;

  void __set_code(const int32_t val);

  void __set_msg(const std::string& val);

  void __set_hisVec(const std::vector<CrawlerHistory> & val);

  bool operator == (const CrawlerHistoryRet & rhs) const
  {
    if (!(code == rhs.code))
      return false;
    if (!(msg == rhs.msg))
      return false;
    if (!(hisVec == rhs.hisVec))
      return false;
    return true;
  }
  bool operator != (const CrawlerHistoryRet &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CrawlerHistoryRet & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(CrawlerHistoryRet &a, CrawlerHistoryRet &b);

std::ostream& operator<<(std::ostream& out, const CrawlerHistoryRet& obj);

typedef struct _QueryConditions__isset {
  _QueryConditions__isset() : tag(false), key(false), pose_type(false), website(false), tBegin(false), tEnd(false) {}
  bool tag :1;
  bool key :1;
  bool pose_type :1;
  bool website :1;
  bool tBegin :1;
  bool tEnd :1;
} _QueryConditions__isset;

class QueryConditions : public virtual ::apache::thrift::TBase {
 public:

  QueryConditions(const QueryConditions&);
  QueryConditions& operator=(const QueryConditions&);
  QueryConditions() : tag(), key(), pose_type(), website(), tBegin(), tEnd() {
  }

  virtual ~QueryConditions() throw();
  std::string tag;
  std::string key;
  std::string pose_type;
  std::string website;
  std::string tBegin;
  std::string tEnd;

  _QueryConditions__isset __isset;

  void __set_tag(const std::string& val);

  void __set_key(const std::string& val);

  void __set_pose_type(const std::string& val);

  void __set_website(const std::string& val);

  void __set_tBegin(const std::string& val);

  void __set_tEnd(const std::string& val);

  bool operator == (const QueryConditions & rhs) const
  {
    if (!(tag == rhs.tag))
      return false;
    if (!(key == rhs.key))
      return false;
    if (!(pose_type == rhs.pose_type))
      return false;
    if (!(website == rhs.website))
      return false;
    if (!(tBegin == rhs.tBegin))
      return false;
    if (!(tEnd == rhs.tEnd))
      return false;
    return true;
  }
  bool operator != (const QueryConditions &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const QueryConditions & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(QueryConditions &a, QueryConditions &b);

std::ostream& operator<<(std::ostream& out, const QueryConditions& obj);

typedef struct _QueryedPicInfo__isset {
  _QueryedPicInfo__isset() : pic_id(true), pic_url(false), tag(false), pose_type(false), create_time(false), labeledCount(false), lastLabeledUser(false), screenshot_bin(false), key(false), website(false) {}
  bool pic_id :1;
  bool pic_url :1;
  bool tag :1;
  bool pose_type :1;
  bool create_time :1;
  bool labeledCount :1;
  bool lastLabeledUser :1;
  bool screenshot_bin :1;
  bool key :1;
  bool website :1;
} _QueryedPicInfo__isset;

class QueryedPicInfo : public virtual ::apache::thrift::TBase {
 public:

  QueryedPicInfo(const QueryedPicInfo&);
  QueryedPicInfo& operator=(const QueryedPicInfo&);
  QueryedPicInfo() : pic_id(0), pic_url(), tag(), pose_type(), create_time(), labeledCount(0), lastLabeledUser(), screenshot_bin(), key(), website() {
  }

  virtual ~QueryedPicInfo() throw();
  int32_t pic_id;
  std::string pic_url;
  std::string tag;
  std::string pose_type;
  std::string create_time;
  int32_t labeledCount;
  std::string lastLabeledUser;
  std::string screenshot_bin;
  std::string key;
  std::string website;

  _QueryedPicInfo__isset __isset;

  void __set_pic_id(const int32_t val);

  void __set_pic_url(const std::string& val);

  void __set_tag(const std::string& val);

  void __set_pose_type(const std::string& val);

  void __set_create_time(const std::string& val);

  void __set_labeledCount(const int32_t val);

  void __set_lastLabeledUser(const std::string& val);

  void __set_screenshot_bin(const std::string& val);

  void __set_key(const std::string& val);

  void __set_website(const std::string& val);

  bool operator == (const QueryedPicInfo & rhs) const
  {
    if (!(pic_id == rhs.pic_id))
      return false;
    if (!(pic_url == rhs.pic_url))
      return false;
    if (!(tag == rhs.tag))
      return false;
    if (!(pose_type == rhs.pose_type))
      return false;
    if (!(create_time == rhs.create_time))
      return false;
    if (!(labeledCount == rhs.labeledCount))
      return false;
    if (!(lastLabeledUser == rhs.lastLabeledUser))
      return false;
    if (!(screenshot_bin == rhs.screenshot_bin))
      return false;
    if (!(key == rhs.key))
      return false;
    if (!(website == rhs.website))
      return false;
    return true;
  }
  bool operator != (const QueryedPicInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const QueryedPicInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(QueryedPicInfo &a, QueryedPicInfo &b);

std::ostream& operator<<(std::ostream& out, const QueryedPicInfo& obj);

typedef struct _CrawleredPicRet__isset {
  _CrawleredPicRet__isset() : code(true), msg(false), pageNum(false), picVec(false) {}
  bool code :1;
  bool msg :1;
  bool pageNum :1;
  bool picVec :1;
} _CrawleredPicRet__isset;

class CrawleredPicRet : public virtual ::apache::thrift::TBase {
 public:

  CrawleredPicRet(const CrawleredPicRet&);
  CrawleredPicRet& operator=(const CrawleredPicRet&);
  CrawleredPicRet() : code(0), msg(), pageNum(0) {
  }

  virtual ~CrawleredPicRet() throw();
  int32_t code;
  std::string msg;
  int32_t pageNum;
  std::vector<QueryedPicInfo>  picVec;

  _CrawleredPicRet__isset __isset;

  void __set_code(const int32_t val);

  void __set_msg(const std::string& val);

  void __set_pageNum(const int32_t val);

  void __set_picVec(const std::vector<QueryedPicInfo> & val);

  bool operator == (const CrawleredPicRet & rhs) const
  {
    if (!(code == rhs.code))
      return false;
    if (!(msg == rhs.msg))
      return false;
    if (!(pageNum == rhs.pageNum))
      return false;
    if (!(picVec == rhs.picVec))
      return false;
    return true;
  }
  bool operator != (const CrawleredPicRet &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CrawleredPicRet & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(CrawleredPicRet &a, CrawleredPicRet &b);

std::ostream& operator<<(std::ostream& out, const CrawleredPicRet& obj);

typedef struct _QueryLabeledRet__isset {
  _QueryLabeledRet__isset() : code(true), msg(false), pageNum(false), picVec(false) {}
  bool code :1;
  bool msg :1;
  bool pageNum :1;
  bool picVec :1;
} _QueryLabeledRet__isset;

class QueryLabeledRet : public virtual ::apache::thrift::TBase {
 public:

  QueryLabeledRet(const QueryLabeledRet&);
  QueryLabeledRet& operator=(const QueryLabeledRet&);
  QueryLabeledRet() : code(0), msg(), pageNum(0) {
  }

  virtual ~QueryLabeledRet() throw();
  int32_t code;
  std::string msg;
  int32_t pageNum;
  std::vector<QueryedPicInfo>  picVec;

  _QueryLabeledRet__isset __isset;

  void __set_code(const int32_t val);

  void __set_msg(const std::string& val);

  void __set_pageNum(const int32_t val);

  void __set_picVec(const std::vector<QueryedPicInfo> & val);

  bool operator == (const QueryLabeledRet & rhs) const
  {
    if (!(code == rhs.code))
      return false;
    if (!(msg == rhs.msg))
      return false;
    if (!(pageNum == rhs.pageNum))
      return false;
    if (!(picVec == rhs.picVec))
      return false;
    return true;
  }
  bool operator != (const QueryLabeledRet &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const QueryLabeledRet & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(QueryLabeledRet &a, QueryLabeledRet &b);

std::ostream& operator<<(std::ostream& out, const QueryLabeledRet& obj);

typedef struct _QueryUnlabeledRet__isset {
  _QueryUnlabeledRet__isset() : code(true), msg(false), pageNum(false), picVec(false) {}
  bool code :1;
  bool msg :1;
  bool pageNum :1;
  bool picVec :1;
} _QueryUnlabeledRet__isset;

class QueryUnlabeledRet : public virtual ::apache::thrift::TBase {
 public:

  QueryUnlabeledRet(const QueryUnlabeledRet&);
  QueryUnlabeledRet& operator=(const QueryUnlabeledRet&);
  QueryUnlabeledRet() : code(0), msg(), pageNum(0) {
  }

  virtual ~QueryUnlabeledRet() throw();
  int32_t code;
  std::string msg;
  int32_t pageNum;
  std::vector<QueryedPicInfo>  picVec;

  _QueryUnlabeledRet__isset __isset;

  void __set_code(const int32_t val);

  void __set_msg(const std::string& val);

  void __set_pageNum(const int32_t val);

  void __set_picVec(const std::vector<QueryedPicInfo> & val);

  bool operator == (const QueryUnlabeledRet & rhs) const
  {
    if (!(code == rhs.code))
      return false;
    if (!(msg == rhs.msg))
      return false;
    if (!(pageNum == rhs.pageNum))
      return false;
    if (!(picVec == rhs.picVec))
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

typedef struct _QueryByIdRet__isset {
  _QueryByIdRet__isset() : code(true), msg(false), pageNum(false), pic(false) {}
  bool code :1;
  bool msg :1;
  bool pageNum :1;
  bool pic :1;
} _QueryByIdRet__isset;

class QueryByIdRet : public virtual ::apache::thrift::TBase {
 public:

  QueryByIdRet(const QueryByIdRet&);
  QueryByIdRet& operator=(const QueryByIdRet&);
  QueryByIdRet() : code(0), msg(), pageNum(0) {
  }

  virtual ~QueryByIdRet() throw();
  int32_t code;
  std::string msg;
  int32_t pageNum;
  QueryedPicInfo pic;

  _QueryByIdRet__isset __isset;

  void __set_code(const int32_t val);

  void __set_msg(const std::string& val);

  void __set_pageNum(const int32_t val);

  void __set_pic(const QueryedPicInfo& val);

  bool operator == (const QueryByIdRet & rhs) const
  {
    if (!(code == rhs.code))
      return false;
    if (!(msg == rhs.msg))
      return false;
    if (!(pageNum == rhs.pageNum))
      return false;
    if (!(pic == rhs.pic))
      return false;
    return true;
  }
  bool operator != (const QueryByIdRet &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const QueryByIdRet & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(QueryByIdRet &a, QueryByIdRet &b);

std::ostream& operator<<(std::ostream& out, const QueryByIdRet& obj);

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

typedef struct _LabeledPoseDataRet__isset {
  _LabeledPoseDataRet__isset() : code(true), msg(false), bin(false), poseData(false), pre2dPoseInfo(false) {}
  bool code :1;
  bool msg :1;
  bool bin :1;
  bool poseData :1;
  bool pre2dPoseInfo :1;
} _LabeledPoseDataRet__isset;

class LabeledPoseDataRet : public virtual ::apache::thrift::TBase {
 public:

  LabeledPoseDataRet(const LabeledPoseDataRet&);
  LabeledPoseDataRet& operator=(const LabeledPoseDataRet&);
  LabeledPoseDataRet() : code(0), msg(), bin(), poseData(), pre2dPoseInfo() {
  }

  virtual ~LabeledPoseDataRet() throw();
  int32_t code;
  std::string msg;
  std::string bin;
  std::string poseData;
  std::string pre2dPoseInfo;

  _LabeledPoseDataRet__isset __isset;

  void __set_code(const int32_t val);

  void __set_msg(const std::string& val);

  void __set_bin(const std::string& val);

  void __set_poseData(const std::string& val);

  void __set_pre2dPoseInfo(const std::string& val);

  bool operator == (const LabeledPoseDataRet & rhs) const
  {
    if (!(code == rhs.code))
      return false;
    if (!(msg == rhs.msg))
      return false;
    if (!(bin == rhs.bin))
      return false;
    if (!(poseData == rhs.poseData))
      return false;
    if (!(pre2dPoseInfo == rhs.pre2dPoseInfo))
      return false;
    return true;
  }
  bool operator != (const LabeledPoseDataRet &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LabeledPoseDataRet & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(LabeledPoseDataRet &a, LabeledPoseDataRet &b);

std::ostream& operator<<(std::ostream& out, const LabeledPoseDataRet& obj);

typedef struct _QueryCondition__isset {
  _QueryCondition__isset() : tag(false), pose_type(false), tBegin(false), tEnd(false) {}
  bool tag :1;
  bool pose_type :1;
  bool tBegin :1;
  bool tEnd :1;
} _QueryCondition__isset;

class QueryCondition : public virtual ::apache::thrift::TBase {
 public:

  QueryCondition(const QueryCondition&);
  QueryCondition& operator=(const QueryCondition&);
  QueryCondition() : tag(), pose_type(), tBegin(), tEnd() {
  }

  virtual ~QueryCondition() throw();
  std::string tag;
  std::string pose_type;
  std::string tBegin;
  std::string tEnd;

  _QueryCondition__isset __isset;

  void __set_tag(const std::string& val);

  void __set_pose_type(const std::string& val);

  void __set_tBegin(const std::string& val);

  void __set_tEnd(const std::string& val);

  bool operator == (const QueryCondition & rhs) const
  {
    if (!(tag == rhs.tag))
      return false;
    if (!(pose_type == rhs.pose_type))
      return false;
    if (!(tBegin == rhs.tBegin))
      return false;
    if (!(tEnd == rhs.tEnd))
      return false;
    return true;
  }
  bool operator != (const QueryCondition &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const QueryCondition & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(QueryCondition &a, QueryCondition &b);

std::ostream& operator<<(std::ostream& out, const QueryCondition& obj);

typedef struct _LoginRet__isset {
  _LoginRet__isset() : code(true), msg(false), role_id(false), token(false) {}
  bool code :1;
  bool msg :1;
  bool role_id :1;
  bool token :1;
} _LoginRet__isset;

class LoginRet : public virtual ::apache::thrift::TBase {
 public:

  LoginRet(const LoginRet&);
  LoginRet& operator=(const LoginRet&);
  LoginRet() : code(0), msg(), role_id(0), token() {
  }

  virtual ~LoginRet() throw();
  int32_t code;
  std::string msg;
  int32_t role_id;
  std::string token;

  _LoginRet__isset __isset;

  void __set_code(const int32_t val);

  void __set_msg(const std::string& val);

  void __set_role_id(const int32_t val);

  void __set_token(const std::string& val);

  bool operator == (const LoginRet & rhs) const
  {
    if (!(code == rhs.code))
      return false;
    if (!(msg == rhs.msg))
      return false;
    if (!(role_id == rhs.role_id))
      return false;
    if (!(token == rhs.token))
      return false;
    return true;
  }
  bool operator != (const LoginRet &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LoginRet & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(LoginRet &a, LoginRet &b);

std::ostream& operator<<(std::ostream& out, const LoginRet& obj);

}} // namespace

#endif
