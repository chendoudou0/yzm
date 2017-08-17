#ifndef CDBOperator_H_
#define CDBOperator_H_

#include "Common.h"
#include "sqlapi.h"
#include "def.h"
#include "pose_label_types.h"


#include<memory>
#include <string>
#include<vector>
#include<utility>

using namespace ::server::pose_label;


class CDBOperator
{
public:
    CDBOperator();
    virtual ~CDBOperator();
	
public:
	bool initMYSQL();  
	/////////////////////////////////////////////////////////////////////////////////////////////////
	int  QueryPicByMd5(const std::string& md5);
	int  QueryPicByURL(const std::string& url, SqlResultSet &objOutMap);
	int  AddPicture(const ::server::pose_label::PicInfo&   pic, string& url, string& str2dPose, string& str3dPose, string& pose_type, string& body_pos);
	bool UpdatePicPoseDate(const std::string& url, string& str2dPose, string& str3dPose, string& pose_type, string& body_pos);
	bool AddLabeledDate(int picId,const std::string token,const std::string poseDate);
	bool QueryUnlabeledPicture(SqlMapVector &objOutMapVector, const  std::string token, int index, int& page, const QueryCondition& qc);
	bool QueryLabeledPicture(SqlMapVector &objOutMapVector, const  std::string token, int index, int& page, const QueryCondition& qc);
	bool QueryLabeledPicByOthers(SqlMapVector &objOutMapVector, const  std::string token, int index, int& page, const QueryCondition& qc);
	bool QueryPicById(SqlMapVector &objOutMapVector, int pic_id, const std::string token);
	int  QueryPicByUser(SqlMapVector &objOutMapVector, const std::string token, const std::string user, int index, int& page, const QueryCondition& qc);
	int  InvalidatePicture(int  id, const std::string token, const bool type);
	bool QueryLabeledPoseData(SqlResultSet& oMap, int pic_id,   const std::string token);
	bool QueryLastLabeledPoseData(SqlResultSet& oMap, int pic_id,   const std::string user);
	bool GetUserByToken(const std::string& token, std::string& user, int& roleId);
	int  Register(const std::string& user, const std::string& passWd);
	int  Login(std::string& token, int& role_id, const std::string& user, const std::string& passWd);
	bool QueryCrawlingHistory(SqlMapVector &objOutMapVector,  const HistoryQueryCondition& qc);
	bool QueryCrawleredPic(SqlMapVector &objOutMapVector,  const QueryConditions& qc, int index, int& page);
	bool AddCrawlingResult(const CrawlingResult&  cs, int dbSum);
	bool UpdatePicInfoById(int id, const std::string tag, const std::string pose_type);
	bool ScorePoseData(int pic_id , const std::string&  user_name , const std::string&  token , double  score);
	bool QueryPicPoseData(SqlMapVector &objOutMapVector, const int32_t  pic_id , const std::string&  token);
	bool QueryPoseDataScore(ScoreQueryRet&  sqRet , const int32_t  pic_id , const std::string&  label_user , const std::string&  token );
private:   
	std::shared_ptr<CMySQL>  _ptrMysql;   
 
};

#endif 

