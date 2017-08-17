namespace cpp server.pose_label
struct ReturnVals {
    1: i32 code = 0,
    2: string msg,
}
  
 struct PicInfo{
    1:binary oriBin,       //图片二进制流
    2:string filename,     //文件名称
    3:string md5,          //图片MD5
    4:string key,          //爬虫搜索关键字，关键字之间用逗号分隔，中文以UTF-8编码
    5:string tag,          //爬虫搜索标签，关标签之间用逗号分隔，中文以UTF-8编码
    6:string ori_url,     //图片原始地址
    7:string website,     //图片来源网站
 }
struct CrawlerStatus{            
    1:i32    totalPicSum,         //爬取图片总数
    2:i32    dbPicSum,            //入库图片数目
 }
 
struct CrawlerStatusRet{
    1: i32 code = 0,
    2: string msg,
    3:CrawlerStatus     crawlerStatus,     //爬虫服务状态
 }
 
struct CrawlingResult{
    1: string    key,            //爬取关键字
    2: string    tag,           //爬取标签
    3: i32      sumTotal,       //总共爬取的数量
    4: string   tBegin,        //爬取结束时间       格式：2017-07-31 15:35:15
    5: string   tEnd,          //爬取开始时间      格式：2017-07-31 15:35:15
 }
 
 struct HistoryQueryCondition{
    1: string key,
    2: string tag,
 }
 
 struct CrawlerHistory{
    1: string  key,             //搜索关键字
    2: string  tag,             //搜索标签
    3: i32     total_sum,       //爬取总数量
    4: i32     db_sum,          //入库数量
    5: string  time_create,     //历史记录入库时间
 
 }
  struct CrawlerHistoryRet{
    1: i32 code = 0,
    2: string msg,
    3: list<CrawlerHistory>    hisVec,
 }
 struct QueryConditions{
    1: string tag,                  //标签
    2: string key,                  //标签
    3: string pose_type,            //姿态类型
    4: string website,              //来源网站
    5: string tBegin,               //入库开始时间
    6: string tEnd,                 //入库结束时间
}
 
 
 struct QueryedPicInfo{
    1:i32     pic_id = 0,     
    2:string  pic_url,
    3:string  tag,
    4:string  pose_type,
    5:string  create_time,
    6:i32     labeledCount,
    7:string  lastLabeledUser,
    8:binary  screenshot_bin, 
    9:string  key,               //关键字
    10:string website,           //来源网站
}
 
struct CrawleredPicRet{
    1:i32 code = 0,     
    2:string msg,
    3:i32    pageNum,
    4:list<QueryedPicInfo>  picVec,     
}
 
 
service DataService{
    /*
    * 图片传输请求接口
    * @params
    * md5 : 图片MD5
    * @code
    * 0允许上传， -1图片已经存在
    */
    ReturnVals UploadRequest(1:string md5),
    /*
    * 图片传输接口
    * @params
    * PicInfo : 图片信息
    * percent ： 爬虫爬取进度(0~1)，结束时发1，开始时发0，PicInfo为空
    * @code
    * 0代表成功， -1代表失败, 1 预处理不通过
    */
    ReturnVals PicUpload(1:PicInfo pic,2:double percent,3:i32 sumTotal),
    /*
    * 爬取结果上报
    * @params
    * CrawlingStatus : 结果信息
    * @code
    * 0代表成功， -1代表失败
    */
    ReturnVals UploadCrawlingResult(1:CrawlingResult cs),
}
 
service CrawlerClientService{
     /*
    * 图片传输请求接口
    * @params
    * md5 : 图片MD5
    * @code
    * 0允许上传， -1图片已经存在
    */
    ReturnVals UploadRequest(1:string md5),
    /*
    * 图片传输接口
    * @params
    * PicInfo : 图片信息
    * @code
    * 0代表成功， -1代表失败，1 预处理不通过
    */
    ReturnVals PicUpload(1:PicInfo pic),
     /*
    * 爬虫开始接口
    * @params
    * keyword : 搜索关键字(UTF-8编码)
    * website : 搜索网站 Baidu(UTF-8编码)
    * tag :对应的标签(UTF-8编码)
    * @code
    * 0代表成功， -1代表失败，
    */  
    ReturnVals start(1:string keyword,2:string website,3:string tag),
    /*
    * 爬虫服务状态查询接口
    * @params  无
    * @code
    * 0代表成功， -1代表失败
    */  
   CrawlerStatusRet  InquireCrawlerStatus(),
     /*   爬虫停止
    * @params  无
    * @return
    * ReturnVals.code: 0代表成功， -1代表失败
    * ReturnVals.msg : 描述信息
    */ 
    ReturnVals stop(),
  /*   查询爬虫搜索历史
    * @params  无
    * key : 搜索关键字(UTF-8编码)
    * tag : 搜索标签(UTF-8编码)
    * ReturnVals.code: 0代表成功， -1代表失败， 1 按条件未搜索到爬取历史
    * ReturnVals.msg : 描述信息
    */
    CrawlerHistoryRet QueryCrawlingHistory(1:HistoryQueryCondition  qc),
     /* 查询所有入库数据
    * @params  无
    * qc : 查询条件
    * index : 记录开始索引
    * ReturnVals.code: 0代表成功， -1代表失败， 1 按条件未搜索到图片
    * ReturnVals.msg : 描述信息
    */
     CrawleredPicRet QueryCrawleredPic(1:QueryConditions qc, 2:i32  index),
     /* 更新图片信息
    * @params 
    * pic_id : 图片ID号
    * tag : 标签                
    * pose_type : 姿态分类
    * @return
    * code: 0代表成功， -1代表失败，
    * msg : 描述信息
    */
     ReturnVals UpdatePicInfo(1:i32 pic_id, 2:string tag, 3:string  pose_type),
 
 
}

/////////////////////////////////////////////////////
struct QueryLabeledRet{
    1:i32 code = 0,      
    2:string msg,
    3:i32    pageNum,
    4:list<QueryedPicInfo>  picVec,      
} 
struct QueryUnlabeledRet{
    1:i32 code = 0,      
    2:string msg,
    3:i32    pageNum,
    4:list<QueryedPicInfo>  picVec,      
}
 
struct QueryByIdRet{
    1:i32     code = 0,      
    2:string  msg,
    3:i32     pageNum,
    4:QueryedPicInfo  pic,      
}
 
struct DownloadRet {
    1: i32 code = 0,
    2: string msg,
    3: binary bin,                  //图片二进制流
    4: i32    pic_id,               //图片ID
    5: string HumanPose2DInfo,      //2D骨骼信息，以JSON格式存储
    6: string HumanPose3DInfo,      //3D骨骼信息，以JSON格式存储
}
 
struct LabeledPoseDataRet {
    1: i32 code = 0,
    2: string msg,
    3: binary bin,                  //600*800图片二进制流
    4: string poseData,             //姿态数据JSON包
    5: string pre2dPoseInfo,        //2D骨骼信息
}

struct QueryCondition{
    1: string tag,                  //标签
    2: string pose_type,            //姿态类型
    3: string tBegin,               //入库开始时间
    4: string tEnd,                 //入库结束时间
}

struct LoginRet {
    1: i32 code = 0,
    2: string msg,
    3: i32  role_id,     //角色ID，id为0是管理员，id为1是普通用户
    4: string  token,
} 

struct PoseData{
    1: string pose_data,       //标注信息
    2: string user_name,       //标注用户名
    3: string label_time,      //标注时间
}
 
struct PoseDatasRet {
    1: i32 code = 0,
    2: string msg,
    3: binary pic_bin,                      //图片二进制流
    4: list<PoseData>   vecPoseData,        //标记数据列表
}      
 
struct ScoreQueryRet {
    1: i32      code = 0,
    2: string   msg,
    3: double   selfScore,      //自己的评分,未评分值就是0
    4: double   averScore,      //平均分
    5: i32      scoreCount,     //评分人数
}

service LabelService{
    /*
    * 未被任何人标注图片查询接口，查询一次就返回10张
    * @params
    * token:令牌, index:查询起始点
    * @code
    * 0查询成功， -1 查询失败, 1 没有未标注的图片
    */
    QueryUnlabeledRet QueryUnlabeledPic(1:string token, 2:i32 index, 3:QueryCondition qc),
    /*
    * 自己未标记，别人标记过的图片，查询一次就返回10张
    * @params
    * token:令牌, index:查询起始点
    * @code
    * 0查询成功， -1 查询失败, 1 没有标注过的图片
    */
    QueryLabeledRet QueryPicLabeledByOthers(1:string token, 2:i32 index, 3:QueryCondition qc),
    /*
    * 查询自己标注过的图片
    * @params
    * token:令牌, index:查询起始点
    * @code
    * 0查询成功， -1 查询失败, 1 没有标注过的图片
    */
    QueryLabeledRet QueryLabeledPic(1:string token, 2:i32 index, 3:QueryCondition qc),
     /*
    * 按图片ID查询
    * @params
    * token:令牌, pic_id:图片ID
    * @code
    * 0查询成功， -1 查询失败, 1 没有该图片
    */
    QueryByIdRet QueryPicById(1:string token, 2:i32 pic_id),
   /*
    * 查询某用户标记的图片
    * @params
    * token:令牌, user : 用户名（若用户名为空，则是查询所有用户标记的数据）
    * @code
    * 0查询成功， -1 查询失败, 1 没有图片, 2 不是管理员用户，无权限
    */
    QueryLabeledRet QueryPicByUserName(1:string token, 2:string user, 3:i32 index, 4:QueryCondition qc ),
    /*
    * 图片传输接口
    * @params
    * pic_url : 图片URL
    * @code
    * 0代表成功， -1代表失败,  1 没有标注过的图片
    */
    DownloadRet DownloadPic(1:string pic_url),
    /*
     * 标注结果入库
         * @params
         *   1.poseInfo : 标注关节点信息
         *   2.pic_id      : 被标注图片ID
         *   3.token:令牌
         *   @code
         *   0入库成功， -1 入库失败
    */
      
   ReturnVals InsertToDb(1:string poseInfo, 2:i32 pic_id, 3:string token),
   /*
     * 将图片标记为无效状态
         * @params
         *   1.pic_id : 被标注图片ID
         *   2.token : 令牌
         *   3.type      : TRUE 置为无效   FALSE 置为有效
        * @code
        * 0标记成功， -1 标记失败， 1 该用户无权标记
    */
   ReturnVals InvalidatePicture(1:i32 pic_id, 2:string token, 3:bool type),
   /*
     * 查找自己已经标注信息
         * @params
         *   1.pic_id : 被标注图片ID
         *    2.token:令牌
         * @code
         * 0标记成功， -1 标记失败
    */
       
   LabeledPoseDataRet QueryLabeledPoseData(1:i32 pic_id, 2:string pic_url, 3:string token),
    /*
     * 查找最后一个人的标注信息
         * @params
         *   1.pic_id : 被标注图片ID
         *    2.user: 最后标注者的用户名
         * @code
         * 0标记成功， -1 标记失败
    */
       
   LabeledPoseDataRet QueryLastLabeledPoseData(1:i32 pic_id, 2:string pic_url, 3:string user),
 
 /*
     * 普通用户注册
         * @params
         *   1.user : 用户名 
         *   2.passwd : 用户密码
        * @code
        * 注册成功， -1 注册失败, 1 用户已存在
    */
      
   ReturnVals Register(1:string user, 2:string passwd),
 /*
     * 用户登录
        * @params
        *   1.user : 用户名 
        *   2.passwd : 用户密码
        * @code
        * 0登录成功， -1 登录失败， 用户名或是密码错误
    */
      
   LoginRet Login(1:string user, 2:string passwd),
  /*
     * 查找图片的所有标记信息     
         * @params
         *   1.pic_id : 被标注图片ID
         *   2.token:   令牌
         * @return
         * code 0查询成功， -1 查询失败
    */
   PoseDatasRet QueryPicPoseData(1:i32 pic_id, 2:string pic_url ,3:string token),
    /*
     * 查找标记信息的评分     
         * @params
         *   1.pic_id : 被标注图片ID
         *   2.user_name : 标记用户名
         *   3.token     :   令牌
         * @return
         * code 0查询成功， -1 查询失败
    */
   ScoreQueryRet QueryPoseDataScore(1:i32 pic_id, 2:string label_user, 3:string token),
    /*
     * 对标记信息评分     
         * @params
         *   1.pic_id : 被标注图片ID
         *   2.user_name : 标记用户名
         *   3.token     :   令牌
         *   4:double    :   score
         * @return
         * code 0 评分成功， -1 评分失败
    */
   ReturnVals ScorePoseData(1:i32 pic_id, 2:string label_user, 3:string token, 4:double score),
   /*
         * 从新预处理图片   
         * @params
         *   1.pic_url :   图片URL
         *   2.token   :   令牌
         * @return
         * code 0 预处理成功， -1 预处理失败
    */
   ReturnVals RepreProcessPic(1:string pic_url, 2:string token),
 
}
////////////////////////////////////////////////////////////////////////////爬虫脚本服务
service CrawlerService{    
    /*
    * 爬虫开始接口
    * @params
    * keyword : 搜索关键字
    * website : 搜索网站 Baidu
    * tag :对应的标签                       
    * @code
    * 0代表成功， -1代表失败
    */  
    ReturnVals start(1:string keyword,2:string website,3:string tag),
     /*  爬虫停止
    * @params  无
    * @return
    * ReturnVals.code: 0代表成功， -1代表失败
    * ReturnVals.msg : 描述信息      
    */ 
    ReturnVals stop(),
}

