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
	int  AddPicture(const ::server::pose_label::PicInfo&   pic, string url);
	bool UpdatePicPoseDate(const std::string& url, string& str2dPose, string& str3dPose);
	bool AddLabeledDate(int picId,const std::string user,const std::string poseDate);
	bool QueryUnlabeledPicture(SqlMapVector &objOutMapVector, const  std::string user, int index, int& page, const QueryCondition& qc);
	bool QueryLabeledPicture(SqlMapVector &objOutMapVector, const  std::string user, int index, int& page, const QueryCondition& qc);
	bool QueryLabeledPicByOthers(SqlMapVector &objOutMapVector, const  std::string user, int index, int& page, const QueryCondition& qc);
	bool QueryPicById(SqlMapVector &objOutMapVector, int pic_id, const std::string user);
	int  InvalidatePicture(int  id, const std::string user, const bool type);
	bool QueryLabeledPoseData(SqlResultSet& oMap, int pic_id, const std::string user);
	
private:   
	std::shared_ptr<CMySQL>  _ptrMysql;
 
};

#endif 

