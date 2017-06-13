#ifndef  _DEF_H
#define  _DEF_H

#include <sstream>
#include <string>

using namespace std;

#define  LOG_PATH				"../log/version_update"
#define  CONF_PATH              "../conf/conf.xml"    
#define  REDIS_EXPIRE_TIME      "600" 
#define  LIVE_SHOW_KEY_FLAG     "live_show"
#define  ADD_LIVE_SHOW_VERSION         "live_show" 
#define  ADD_FITTING_ONLINE     "fitting_online"


#define  DEFAULT_STRING_NULL  ""

#define  DB_HOST_PATH      		 "/root/DBHost"
#define  DB_USER_PATH            "/root/DBUser"
#define  DB_PASSWD_PATH      	 "/root/DBPass"
#define  DB_PORT_PATH   		 "/root/DBPort"
#define  DB_OVERTIME_PATH   	 "/root/DBOverTime"


enum  ServerType
{
	FITTING_ONLINE_VERSION_QUERY   	=   2000,
	FITTING_ONLINE_VERSION_UPDATE   =   2001,
	VERSION_QUERY_ANDROID  			=   2002,
	LIVE_SHOW_VERSION_QUERY   	    =   2003,
	LIVE_SHOW_VERSION_UPDATE        =   2004,
};



#endif

