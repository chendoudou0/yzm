#ifndef  _DEF_H
#define  _DEF_H


#define  LOG_PATH				"../log/version_update"
#define  CONF_PATH              "../conf/app.conf"    
#define  REDIS_EXPIRE_TIME      "600" 
#define  LIVE_SHOW_KEY_FLAG      "live_show" 

#define  DEFAULT_STRING_NULL  ""

#define  LIVE_SHOW_QUERY_URI     	"/live_show/query"
#define  LIVE_SHOW_UPDATE_URI	 	"/live_show/update"
#define  ANDROID_QUERY_URI		 	"/android/query"
#define  ANDROID_VERSION_ADD_URI	"/android/add_version"
#define  LIVESHOW_VERSION_ADD_URI	"/live_show/add_version"

enum  ServerType
{
	FITTING_ONLINE_VERSION_QUERY   	=   2000,
	FITTING_ONLINE_VERSION_UPDATE   =   2001,
	VERSION_QUERY_ANDROID  			=   2002,
	LIVE_SHOW_VERSION_QUERY   	    =   2003,
	LIVE_SHOW_VERSION_UPDATE        =   2004,
	ANDROID_VERSION_ADD             =   2005,
	LIVESHOW_VERSION_ADD            =   2006,
};


#endif

