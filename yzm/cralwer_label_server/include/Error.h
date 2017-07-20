/**
  * FileName: Error.h
  * Author: jackcai
  * Version :1.0
  * Date: 2008-01-16
  * Description: 错误信息，错误码列表
  * ChangeList:
  */


#ifndef      __ERROR_H
#define      __ERROR_H

// 错误码
enum ErrorNum
{
	RES_OK           = 0,                            // 返回正确

	/**
	 * 文件配置错误
	 */
	ERR_LOAD_CFG	 = 32211001,				     // 读取配置文件失败
	ERR_NULL_XML_NODE	= 32211002, 				 // 指定的XML节点不存在
	ERR_BAD_XML_VALUE	= 32211003, 				 // 指定的XML节点不存在
	ERR_BAD_DATA_TYPE  = 32211004,					 // 错误的数据类型
	ERR_BAD_STATEMENT_TYPE	= 32211005, 	         // 错误的数据类型
	ERR_BAD_FIELD_DEF  = 32211006,					 // 数据字段定义错误
	ERR_BAD_VAR_DEF 	= 32211067,				     // 变量定义错误

	/**
	 * 模板错误
	 */
	ERR_FUN_UNKNOWN    = 32211101,                  // 没有对应的函数
	ERR_FUN_ECODEFUN  =  32211102,                  // 加密错误
	ERR_FUN_NOSPACE   =  32211103,                  // 空间不够

	ERR_TEMP_TYPE    =  32211104,                   // 类型未知
	ERR_TEMP_PARAM    =  32211105,                  // 模板参数配置错误

	ERR_OP_UNKNOWN    =  32211199,                  // 未知操作
	ERR_CONVERT_CODE    =  32211200,				// 转码错误

	/**
	 * Curl错误
	 */
	ERR_CURL_INIT = 32211201,                       // curl 初始化错误
	ERR_CURL_SETOPT = 32211202,                     // curl setopt错误
	ERR_CURL_PERFORM   = 32211203,                     // curl

	/**
	 * 数据库操作错误
	 */
	ERR_DB_INITIAL = 32212001,			 // 分配数据库连接失败
	ERR_DB_CONNECT = 32212002,		   // 连接数据库失败
	ERR_DB_LOST = 32212003, 			  // 和数据库断开连接
	ERR_DB_NULL_RESULT = 32212004, // 获取到空数据库结果集
	ERR_DB_AFFECT_ROW = 32212005, // 获取到空数据库结果集
	ERR_DB_AUTO_CMMIT= 32212006,  // 设置自动提交模式失败
	ERR_DB_BEGIN = 32212007,			// 开始事务失败
	ERR_DB_COMMIT = 32212008,		  // 提交事务失败
	ERR_DB_ROLLBACK = 32212009, 	 // 回滚事务失败
	ERR_DB_NONE_ROW = 32212010,   // 没有取到任何结果
	ERR_DB_MULTI_ROW = 32212011,   // 取到结果多余一行
	ERR_DB_FETCH_ROW = 32212012,   // 取结果行错误
	ERR_DB_UNKNOW_FAULT = 32212013,   // 意外错误
	ERR_DB_ROW_LENGTH   = 32212014,    // 结果行数过长
	ERR_DB_SQL_NOVALUE     = 32212015,    // 拼接SQL语句错误,没有值

	/**
	 * 应用错误
	 */
	ERR_NO_BIND   = 32221001,     // 用户没有绑定
	ERR_APP_BINDED  = 32221002,    // 用户已经绑定
	ERR_APP_PARAM   = 32221201,      // 参数错误

	ERR_APP_DUPLISTID = 32221202,	// 订单已存在
	ERR_APP_BILLNO	  = 32221203,   // 取订单号失败
	ERR_APP_NOTBIND	  = 32221204,   // 用户未绑定
	ERR_APP_NOLIST    = 32221205,   // 订单不存在
	ERR_APP_TRADESTATE = 32221206,  // 交易状态不匹配
	ERR_APP_LISTINVALID = 32221207, // 交易单过期
	ERR_APP_NOCOMPANY  =  32221208, // 对应的公司不存在
	ERR_APP_NOTENOUGHCREDIT = 32221209, //用户积分不足
	ERR_APP_NOTENOUGHPRIZE  = 32221210, //所兑换奖品缺货
	ERR_APP_ERRORCREDITTYPE = 32221211, //非法的积分类型

	// 南航返回的错误
	ERR_CS_MESSAGE     = 32225000,    // 南航接口返回message
	ERR_CS_VALID_PASS  = 32225001,    // 南航密码验证失败(包括系统错误和密码错误的情况)
	ERR_CS_QUERY  = 32225002,    // 南航查询失败
	ERR_CS_REG  = 32225003,    // 南航注册失败
	ERR_CS_REGED  = 32225004,    // 用户已经注册
	ERR_CS_PREPAY = 32225005,    // 支付接口调用失败
	ERR_CS_ORDERPLANE = 32225006, // 预订航班，锁座位失败
	ERR_CS_CALLBACK = 32225007, // 支付回调失败
	ERR_CS_SOAPPACKERR =32225008, //SOAP接口返回报文格式非法
	ERR_CS_MAXSEATTIMES =32225009,//超过合法选座次数

	ERR_CS_CSFAIL = 32225101, // 通知南航成功，南航那边返回失败
	ERR_CS_PASSNAMEERROR = 32225102, // 乘客姓名包含非法字符
	ERR_CS_ERRORJAVAPACKET = 32225103, //JAVA返回错误，错误的请求报文格式。
	ERR_CS_MAXYUYUECNT = 32225104,   //达到用户最大预约次数
    ERR_CS_NONEPLAYTIMES = 32225105,  //已无抽奖机会
    ERR_CS_PLAYERROR     = 32225106,  //抽奖时出错，数据错误
    ERR_CS_ERRORPAYDATA  = 32225107,  //数据错误，此订单无法支付，请放弃此订单重新查询订购。
    ERR_CS_QUERYLIMIT    = 32225108,  //查询航班频率过高访问受限。
    ERR_CS_QUERYDELAY    = 32225109,  //您所查询的航班数据正在更新中，请稍后重试！

    /*
     * 活动的错误代码定义
     */
	ERR_ACTIVITY_DUPINVITEE = 32225110,//已经被其他人邀请，此次邀请无效
	ERR_ACTIVITY_ACTIONID = 32225111,//错误的ActionID
	ERR_ACTIVITY_ENDED = 32225112,//活动已经结束
	ERR_ACTIVITY_OPERTYPE = 32225113,//不支持的操作
	/*
	 * 应用中错误代码定义
	 */
    ERR_APP_PAPERNOTUSERINFO = 32321004, //南航答题活动，无用户资料
    ERR_APP_PAPERTICKETNOUSE = 32321005, //南航答题活动，票号已经使用过了
    ERR_APP_PAPERTICKETNOERR = 32321006, //南航答题活动，票号不可用
    ERR_APP_PAPERERRPASSENGER= 32321007, //南航答题活动，票号乘客非答题者本人
    ERR_APP_PAPERSUBJECTERR =  32321008, //南航答题活动，题目太少，无法使用
    ERR_APP_PAPERETOUTDATE   =  32321025, //南航答题活动，票号不在有效期
    ERR_APP_SNDCFQEITHER   =  32325026, //赠送财付券，用户已赠送过本批次财付券
    ERR_APP_SNDCFQDAYOUTCNT   =  32325027, //赠送财付券，超过当天赠送限额
    ERR_APP_PAPERETNOTB2C     =  32325028, //本次活动只对活动期间在南航门户网站
    ERR_APP_SNSTIMESDATE    = 32325029,//您提供的票号无效，只有在活动期间（2009年12月10日-2010年4月15日）购票的票号才可以参与活动")
    ERR_APP_AOZHOUOUTDATE   = 32325030,//您提供的票号无效，只有在活动期间（2010年1月6日-2010年3月31日）购票的票号才可以参与活动
	// 32225300以后为南航错误信息的转义


	ERR_RELAY_FAIL				= 32325031,	//relay 错误
	ERR_RELAY_NET				= 32325032,	 //relay 网络错误
	ERR_NOT_CFT_REGISTER		= 32325033,	 //未注册财付通

	// 错误代码定义
	ERR_NOT_CFTUSER 			= 32226010,	//还不是财付通用户
	ERR_NOT_EXISTED 			= 32226011,	//序列号不存在
	ERR_MEET_MAX_TRY_TIME 	= 32226012,	//当天尝试错误序列号次数达到最大
	ERR_LOW_BALANCE 		   	= 32226013,	//库存不足
	ERR_COUPON_NOT_IN_ACTIVE_TIME	= 32226014,	//该序列号未处于有效期内
	ERR_EXPIRE				    	= 32226015,	//该序列号已经失效（过期）
	ERR_BEEN_USED 			= 32226016,	//该序列号已经被使用
	ERR_PLATFORM 				= 32226017,	//使用平台不匹配
	ERR_CALL_ACTIONTENPAY 	= 32226018,	//在调用action.tenpay.com下的cgi时，发生错误
	ERR_COUPON_INVALID		= 32226019,	//券已失效，不可用
	ERR_SYS 					= 32226020,	//系统错误
	ERR_BATCH_NOT_IN_ACTIVE_TIME	= 32226021,	//该批次券未处于有效期内
	ERR_NOT_MATCH_BINDUSER	= 32226022,	//与券绑定的用户不一致
	ERR_LOW_PAYNUM			= 32226023,	//未达最低支付金额要求
	ERR_PER_USER_CNT_LIMIT			= 32226024,	//领用次数受限
	ERR_NOT_TRUE_NAME_VERIFY			= 32226025,	//非实名认证用户
	ERR_BATCHID_PER_USER_MAX_LIMIT	= 32226026,	//一个批次限制单个用户领券次数(申请券时作的限制)
	ERR_GIFT_COUPON = 32226027,                // 转赠失败
	ERR_BIND_COUPON = 32226028,               // 绑定失败

	//抵扣券错误码定义占用322270段
	ERR_NO_VOUCHER 			= 32227000,	//用户帐户中没有可用抵扣券
	ERR_NO_ACTION 				= 32227001,	//活动不存在
	ERR_VOUCHERNO 			= 32227002,	//抵扣扣号码错误或不存在
	ERR_BIND_BEFORE 			= 32227003,	//该券已经被其他用户绑定	
	ERR_V_LOW_BALANCE 		= 32227004,	//抵扣券余额不足
	ERR_LIST_NOT_EXIST		= 32227005,	//解冻或确认消费时，专区订单号不存在
	ERR_PARAMS				= 32227006,	//参数错误
	ERR_NAME_CONFLIT 			= 32227007,	//活动名冲突
	ERR_SYSTEM 				= 32227008,	//系统错误
	ERR_NO_PRE_CONSUME		= 32227009,	//订单无预消费记录，取消预消费请求时返回
	ERR_MONEY_CANCEL_PRE_CONSUME			= 32227010,	//订单抵扣金额有误，取消预消费请求时返回	
	ERR_PLATFORM_NOT_MATCH			= 322270011,		//使用平台不符合
	ERR_BIND_OTHER_BEFORE 			= 32227012,	//用户已经绑定过相同活动的券，不能再绑定
	ERR_PER_USER_LIMIT_CNT 			= 32227013,	//单个用户领用次数超出限制
	ERR_RANDOM_RATE_MISS 			= 32227014,	//未成功领用，机率未命中
	ERR_PER_DAY_LIMIT_CNT 			= 32227015,	//当天领用次数超出限制
	

	//9元机票抢购错误码定义占用322271段
	ERR_NO_CONFIG 				= 32227100,	//未能找到相关配置信息
	ERR_9YUAN_LOW_BALANCE 	= 32227101,	//名额不足
	ERR_NO_RECORDS 			= 32227102,	//订单无消费记录，取消消费请求时返回
	ERR_NOT_ACTIVE 			= 32227103,	//活动未生效
	ERR_REPEAT_PROHIBIT 		= 32227104,	//禁止多次抢购
	
	ERR_PARAMS_G				= 32229997,	//参数错误
	ERR_SYSTEM_G 				= 32229998,	//系统错误
	ERR_OTHER_UNKNOW = 32229999   // 其他未知错误
};

#endif

