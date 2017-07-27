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

struct ANDROID_VERSION_INFO{
	std::string name;
	std::string domain;
	std::string bucket;
	std::string path;
	std::string md5;
	std::string size;
	std::string description;
};

struct LIVESHOW_ADD_INFO{
	std::string name;
	std::string domain;
	std::string bucket;
	std::string description;
	std::vector<std::pair<std::string, std::string> >   fileVec_;
};

class CDBOperator
{
public:
    CDBOperator();
    virtual ~CDBOperator();
	
public:
	bool initMYSQL();  
	bool QueryExistingVersions(int type, SqlMapVector &objOutMapVector);
	bool QueryAndroidExistingVersions(SqlMapVector &objOutMapVector);
	bool QueryNewaddedVersions(SqlMapVector &objOutMapVector, std::string type);  
	bool QueryAndroidNewaddedVersions(SqlMapVector &objOutMapVector);
	bool QueryMd5s(int type, std::string version, KeyValueMap& outMap);  
	bool insertMD5toDB(SqlMapVector& inMapVec, std::string type);
	bool QueryBucketAndDomain(int type ,std::string& inVersion, std::string& outBucket, std::string& outDomain); 
	bool QueryAndroidBucketAndDomain(std::string& inVersion, std::string& outBucket, std::string& outDomain);
	bool UpdateVersionStatus(std::string& inVersionName, std::string type);
	bool UpdateAndroidVersionStatus(std::string& inVersionName, std::string& inMd5, int& inSize);
	int  AddAndroidVersion(ANDROID_VERSION_INFO& avInfo);
	int  AddLiveshowVersion(LIVESHOW_ADD_INFO&  lsInfo);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	int  QueryPicByMd5(const std::string& md5);
	int  QueryPicByURL(const std::string& url, SqlResultSet &objOutMap);
	int  AddPicture(const ::server::pose_label::PicInfo&   pic, string url);
	bool UpdatePicPoseDate(const std::string& url, string& str2dPose, string& str3dPose);
	bool AddLabeledDate(int picId,const std::string user,const std::string poseDate);
	bool QueryUnlabeledPicture(SqlMapVector &objOutMapVector, const  std::string user, int index, int& page);
	bool QueryLabeledPicture(SqlMapVector &objOutMapVector, const  std::string user, int index, int& page);
	bool QueryLabeledPicByOthers(SqlMapVector &objOutMapVector, const  std::string user, int index, int& page);
	int  InvalidatePicture(int  id, const std::string user, const bool type);
	bool QueryLabeledPoseData(SqlResultSet& oMap, int pic_id, const std::string user);
	
private:
	std::shared_ptr<CMySQL>  _ptrMysql;
 
};

#endif 

