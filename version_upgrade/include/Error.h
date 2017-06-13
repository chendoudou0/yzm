/**
  * FileName: Error.h
  * Author: jackcai
  * Version :1.0
  * Date: 2008-01-16
  * Description: ������Ϣ���������б�
  * ChangeList:
  */


#ifndef      __ERROR_H
#define      __ERROR_H

// ������
enum ErrorNum
{
	RES_OK           = 0,                            // ������ȷ

	/**
	 * �ļ����ô���
	 */
	ERR_LOAD_CFG	 = 32211001,				     // ��ȡ�����ļ�ʧ��
	ERR_NULL_XML_NODE	= 32211002, 				 // ָ����XML�ڵ㲻����
	ERR_BAD_XML_VALUE	= 32211003, 				 // ָ����XML�ڵ㲻����
	ERR_BAD_DATA_TYPE  = 32211004,					 // �������������
	ERR_BAD_STATEMENT_TYPE	= 32211005, 	         // �������������
	ERR_BAD_FIELD_DEF  = 32211006,					 // �����ֶζ������
	ERR_BAD_VAR_DEF 	= 32211067,				     // �����������

	/**
	 * ģ�����
	 */
	ERR_FUN_UNKNOWN    = 32211101,                  // û�ж�Ӧ�ĺ���
	ERR_FUN_ECODEFUN  =  32211102,                  // ���ܴ���
	ERR_FUN_NOSPACE   =  32211103,                  // �ռ䲻��

	ERR_TEMP_TYPE    =  32211104,                   // ����δ֪
	ERR_TEMP_PARAM    =  32211105,                  // ģ��������ô���

	ERR_OP_UNKNOWN    =  32211199,                  // δ֪����
	ERR_CONVERT_CODE    =  32211200,				// ת�����

	/**
	 * Curl����
	 */
	ERR_CURL_INIT = 32211201,                       // curl ��ʼ������
	ERR_CURL_SETOPT = 32211202,                     // curl setopt����
	ERR_CURL_PERFORM   = 32211203,                     // curl

	/**
	 * ���ݿ��������
	 */
	ERR_DB_INITIAL = 32212001,			 // �������ݿ�����ʧ��
	ERR_DB_CONNECT = 32212002,		   // �������ݿ�ʧ��
	ERR_DB_LOST = 32212003, 			  // �����ݿ�Ͽ�����
	ERR_DB_NULL_RESULT = 32212004, // ��ȡ�������ݿ�����
	ERR_DB_AFFECT_ROW = 32212005, // ��ȡ�������ݿ�����
	ERR_DB_AUTO_CMMIT= 32212006,  // �����Զ��ύģʽʧ��
	ERR_DB_BEGIN = 32212007,			// ��ʼ����ʧ��
	ERR_DB_COMMIT = 32212008,		  // �ύ����ʧ��
	ERR_DB_ROLLBACK = 32212009, 	 // �ع�����ʧ��
	ERR_DB_NONE_ROW = 32212010,   // û��ȡ���κν��
	ERR_DB_MULTI_ROW = 32212011,   // ȡ���������һ��
	ERR_DB_FETCH_ROW = 32212012,   // ȡ����д���
	ERR_DB_UNKNOW_FAULT = 32212013,   // �������
	ERR_DB_ROW_LENGTH   = 32212014,    // �����������
	ERR_DB_SQL_NOVALUE     = 32212015,    // ƴ��SQL������,û��ֵ

	/**
	 * Ӧ�ô���
	 */
	ERR_NO_BIND   = 32221001,     // �û�û�а�
	ERR_APP_BINDED  = 32221002,    // �û��Ѿ���
	ERR_APP_PARAM   = 32221201,      // ��������

	ERR_APP_DUPLISTID = 32221202,	// �����Ѵ���
	ERR_APP_BILLNO	  = 32221203,   // ȡ������ʧ��
	ERR_APP_NOTBIND	  = 32221204,   // �û�δ��
	ERR_APP_NOLIST    = 32221205,   // ����������
	ERR_APP_TRADESTATE = 32221206,  // ����״̬��ƥ��
	ERR_APP_LISTINVALID = 32221207, // ���׵�����
	ERR_APP_NOCOMPANY  =  32221208, // ��Ӧ�Ĺ�˾������
	ERR_APP_NOTENOUGHCREDIT = 32221209, //�û����ֲ���
	ERR_APP_NOTENOUGHPRIZE  = 32221210, //���һ���Ʒȱ��
	ERR_APP_ERRORCREDITTYPE = 32221211, //�Ƿ��Ļ�������

	// �Ϻ����صĴ���
	ERR_CS_MESSAGE     = 32225000,    // �Ϻ��ӿڷ���message
	ERR_CS_VALID_PASS  = 32225001,    // �Ϻ�������֤ʧ��(����ϵͳ����������������)
	ERR_CS_QUERY  = 32225002,    // �Ϻ���ѯʧ��
	ERR_CS_REG  = 32225003,    // �Ϻ�ע��ʧ��
	ERR_CS_REGED  = 32225004,    // �û��Ѿ�ע��
	ERR_CS_PREPAY = 32225005,    // ֧���ӿڵ���ʧ��
	ERR_CS_ORDERPLANE = 32225006, // Ԥ�����࣬����λʧ��
	ERR_CS_CALLBACK = 32225007, // ֧���ص�ʧ��
	ERR_CS_SOAPPACKERR =32225008, //SOAP�ӿڷ��ر��ĸ�ʽ�Ƿ�
	ERR_CS_MAXSEATTIMES =32225009,//�����Ϸ�ѡ������

	ERR_CS_CSFAIL = 32225101, // ֪ͨ�Ϻ��ɹ����Ϻ��Ǳ߷���ʧ��
	ERR_CS_PASSNAMEERROR = 32225102, // �˿����������Ƿ��ַ�
	ERR_CS_ERRORJAVAPACKET = 32225103, //JAVA���ش��󣬴���������ĸ�ʽ��
	ERR_CS_MAXYUYUECNT = 32225104,   //�ﵽ�û����ԤԼ����
    ERR_CS_NONEPLAYTIMES = 32225105,  //���޳齱����
    ERR_CS_PLAYERROR     = 32225106,  //�齱ʱ�������ݴ���
    ERR_CS_ERRORPAYDATA  = 32225107,  //���ݴ��󣬴˶����޷�֧����������˶������²�ѯ������
    ERR_CS_QUERYLIMIT    = 32225108,  //��ѯ����Ƶ�ʹ��߷������ޡ�
    ERR_CS_QUERYDELAY    = 32225109,  //������ѯ�ĺ����������ڸ����У����Ժ����ԣ�

    /*
     * ��Ĵ�����붨��
     */
	ERR_ACTIVITY_DUPINVITEE = 32225110,//�Ѿ������������룬�˴�������Ч
	ERR_ACTIVITY_ACTIONID = 32225111,//�����ActionID
	ERR_ACTIVITY_ENDED = 32225112,//��Ѿ�����
	ERR_ACTIVITY_OPERTYPE = 32225113,//��֧�ֵĲ���
	/*
	 * Ӧ���д�����붨��
	 */
    ERR_APP_PAPERNOTUSERINFO = 32321004, //�Ϻ����������û�����
    ERR_APP_PAPERTICKETNOUSE = 32321005, //�Ϻ�������Ʊ���Ѿ�ʹ�ù���
    ERR_APP_PAPERTICKETNOERR = 32321006, //�Ϻ�������Ʊ�Ų�����
    ERR_APP_PAPERERRPASSENGER= 32321007, //�Ϻ�������Ʊ�ų˿ͷǴ����߱���
    ERR_APP_PAPERSUBJECTERR =  32321008, //�Ϻ���������Ŀ̫�٣��޷�ʹ��
    ERR_APP_PAPERETOUTDATE   =  32321025, //�Ϻ�������Ʊ�Ų�����Ч��
    ERR_APP_SNDCFQEITHER   =  32325026, //���ͲƸ�ȯ���û������͹������βƸ�ȯ
    ERR_APP_SNDCFQDAYOUTCNT   =  32325027, //���ͲƸ�ȯ���������������޶�
    ERR_APP_PAPERETNOTB2C     =  32325028, //���λֻ�Ի�ڼ����Ϻ��Ż���վ
    ERR_APP_SNSTIMESDATE    = 32325029,//���ṩ��Ʊ����Ч��ֻ���ڻ�ڼ䣨2009��12��10��-2010��4��15�գ���Ʊ��Ʊ�Ųſ��Բ���")
    ERR_APP_AOZHOUOUTDATE   = 32325030,//���ṩ��Ʊ����Ч��ֻ���ڻ�ڼ䣨2010��1��6��-2010��3��31�գ���Ʊ��Ʊ�Ųſ��Բ���
	// 32225300�Ժ�Ϊ�Ϻ�������Ϣ��ת��


	ERR_RELAY_FAIL				= 32325031,	//relay ����
	ERR_RELAY_NET				= 32325032,	 //relay �������
	ERR_NOT_CFT_REGISTER		= 32325033,	 //δע��Ƹ�ͨ

	// ������붨��
	ERR_NOT_CFTUSER 			= 32226010,	//�����ǲƸ�ͨ�û�
	ERR_NOT_EXISTED 			= 32226011,	//���кŲ�����
	ERR_MEET_MAX_TRY_TIME 	= 32226012,	//���쳢�Դ������кŴ����ﵽ���
	ERR_LOW_BALANCE 		   	= 32226013,	//��治��
	ERR_COUPON_NOT_IN_ACTIVE_TIME	= 32226014,	//�����к�δ������Ч����
	ERR_EXPIRE				    	= 32226015,	//�����к��Ѿ�ʧЧ�����ڣ�
	ERR_BEEN_USED 			= 32226016,	//�����к��Ѿ���ʹ��
	ERR_PLATFORM 				= 32226017,	//ʹ��ƽ̨��ƥ��
	ERR_CALL_ACTIONTENPAY 	= 32226018,	//�ڵ���action.tenpay.com�µ�cgiʱ����������
	ERR_COUPON_INVALID		= 32226019,	//ȯ��ʧЧ��������
	ERR_SYS 					= 32226020,	//ϵͳ����
	ERR_BATCH_NOT_IN_ACTIVE_TIME	= 32226021,	//������ȯδ������Ч����
	ERR_NOT_MATCH_BINDUSER	= 32226022,	//��ȯ�󶨵��û���һ��
	ERR_LOW_PAYNUM			= 32226023,	//δ�����֧�����Ҫ��
	ERR_PER_USER_CNT_LIMIT			= 32226024,	//���ô�������
	ERR_NOT_TRUE_NAME_VERIFY			= 32226025,	//��ʵ����֤�û�
	ERR_BATCHID_PER_USER_MAX_LIMIT	= 32226026,	//һ���������Ƶ����û���ȯ����(����ȯʱ��������)
	ERR_GIFT_COUPON = 32226027,                // ת��ʧ��
	ERR_BIND_COUPON = 32226028,               // ��ʧ��

	//�ֿ�ȯ�����붨��ռ��322270��
	ERR_NO_VOUCHER 			= 32227000,	//�û��ʻ���û�п��õֿ�ȯ
	ERR_NO_ACTION 				= 32227001,	//�������
	ERR_VOUCHERNO 			= 32227002,	//�ֿۿۺ������򲻴���
	ERR_BIND_BEFORE 			= 32227003,	//��ȯ�Ѿ��������û���	
	ERR_V_LOW_BALANCE 		= 32227004,	//�ֿ�ȯ����
	ERR_LIST_NOT_EXIST		= 32227005,	//�ⶳ��ȷ������ʱ��ר�������Ų�����
	ERR_PARAMS				= 32227006,	//��������
	ERR_NAME_CONFLIT 			= 32227007,	//�����ͻ
	ERR_SYSTEM 				= 32227008,	//ϵͳ����
	ERR_NO_PRE_CONSUME		= 32227009,	//������Ԥ���Ѽ�¼��ȡ��Ԥ��������ʱ����
	ERR_MONEY_CANCEL_PRE_CONSUME			= 32227010,	//�����ֿ۽������ȡ��Ԥ��������ʱ����	
	ERR_PLATFORM_NOT_MATCH			= 322270011,		//ʹ��ƽ̨������
	ERR_BIND_OTHER_BEFORE 			= 32227012,	//�û��Ѿ��󶨹���ͬ���ȯ�������ٰ�
	ERR_PER_USER_LIMIT_CNT 			= 32227013,	//�����û����ô�����������
	ERR_RANDOM_RATE_MISS 			= 32227014,	//δ�ɹ����ã�����δ����
	ERR_PER_DAY_LIMIT_CNT 			= 32227015,	//�������ô�����������
	

	//9Ԫ��Ʊ���������붨��ռ��322271��
	ERR_NO_CONFIG 				= 32227100,	//δ���ҵ����������Ϣ
	ERR_9YUAN_LOW_BALANCE 	= 32227101,	//�����
	ERR_NO_RECORDS 			= 32227102,	//���������Ѽ�¼��ȡ����������ʱ����
	ERR_NOT_ACTIVE 			= 32227103,	//�δ��Ч
	ERR_REPEAT_PROHIBIT 		= 32227104,	//��ֹ�������
	
	ERR_PARAMS_G				= 32229997,	//��������
	ERR_SYSTEM_G 				= 32229998,	//ϵͳ����
	ERR_OTHER_UNKNOW = 32229999   // ����δ֪����
};

#endif

