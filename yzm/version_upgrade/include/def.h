#ifndef  _DEF_H
#define  _DEF_H

#include <sstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

#define  LOG_PATH				"../log/version_update"
#define  CONF_PATH              "../conf/conf.xml"    
#define  REDIS_EXPIRE_TIME      "600" 
#define  LIVE_SHOW_KEY_FLAG      "live_show" 

typedef		map<string , pair<vector<string>, int> >	         URI_IPS_MAP;
typedef		map<string, pair<vector<string>, int> >::iterator	 URI_IPS_MAP_IT;

#define  DEFAULT_STRING_NULL  ""

#define  DB_HOST_PATH      		 "/root/DBHost"
#define  DB_USER_PATH            "/root/DBUser"
#define  DB_PASSWD_PATH      	 "/root/DBPass"
#define  DB_PORT_PATH   		 "/root/DBPort"
#define  DB_OVERTIME_PATH   	 "/root/DBOverTime"

#define  LIVE_SHOW_QUERY_URI     "/live_show/query"
#define  LIVE_SHOW_UPDATE_URI	 "/live_show/update"
#define  ANDROID_QUERY_URI		 "/android/query"

enum  ServerType
{
	FITTING_ONLINE_VERSION_QUERY   	=   2000,
	FITTING_ONLINE_VERSION_UPDATE   =   2001,
	VERSION_QUERY_ANDROID  			=   2002,
	LIVE_SHOW_VERSION_QUERY   	    =   2003,
	LIVE_SHOW_VERSION_UPDATE        =   2004,
};


typedef struct _DB_PARAM
{
	string _DBHost;
  	string _DBUser;
  	string _DBPass;
  	string _DBPort;
  	int _DBOver_time;
}DB_PARAM;

typedef struct _REDIS_PARAM
{
	string _ip;
  	int _port;
  	int _time_out;
}REDIS_PARAM;



#endif

