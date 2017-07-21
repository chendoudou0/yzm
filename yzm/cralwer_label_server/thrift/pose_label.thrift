namespace cpp server.pose_label
struct ReturnVals {
    1: i32 code = 0,
    2: string msg,
}
  
struct PicInfo{
    1:binary oriBin,       //图片二进制流
    2:string md5,          //图片MD5
    3:string key,          //爬虫搜索关键字，关键字之间用逗号分隔，中文以UTF-8编码
    4:string tag,          //爬虫搜索标签，关标签之间用逗号分隔，中文以UTF-8编码
    5:string filename,     //图片原始文件名
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
    * 0代表成功， -1代表失败
    */
    ReturnVals PicUpload(1:PicInfo pic,2:double percent,3:i32 sumTotal),
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
    * 0代表成功， -1代表失败
    */
    ReturnVals PicUpload(1:PicInfo pic),
     /*
    * 爬虫开始接口
    * @params
    * keyword : 搜索关键字(UTF-8编码)
    * website : 搜索网站 Baidu(UTF-8编码)
    * tag :对应的标签(UTF-8编码)
    * @code
    * 0代表成功， -1代表失败
    */  
    ReturnVals start(1:string keyword,2:string website,3:string tag),
    /*
    * 爬虫服务状态查询接口
    * @params  无
    * @code
    * 0代表成功， -1代表失败
    */  
   CrawlerStatusRet  InquireCrawlerStatus(),
}
//////////////////////////////////////////////////////////////////////// 
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
}

/////////////////////////////////////////////////////

struct QueryedPicInfo{
    1:i32    pic_id = 0,       
    2:string  pic_url,
    3:binary  screenshot_bin,     
}

struct QueryUnlabeledRet{
    1:i32 code = 0,       
    2:string msg,
    3:list<QueryedPicInfo>  picVec,       
}

struct QueryLabeledRet{
    1:i32 code = 0,       
    2:string msg,
    3:list<QueryedPicInfo>  picVec,       
}

struct DownloadRet {
    1: i32    code = 0,
    2: string msg,
    3: binary bin,                  //图片二进制流
    4: string HumanPose2DInfo,      //2D骨骼信息，以JSON格式存储
    5: string HumanPose3DInfo,      //3D骨骼信息，以JSON格式存储
}

service LabelService{
    /*
    * 未标注图片查询接口，查询一次就返回10张
    * @params
    * user:用户名, index:查询起始点
    * @code
    * 0查询成功， -1 查询失败, 1 没有未标注的图片
    */
   QueryUnlabeledRet QueryUnlabeledPic(1:string user, 2:i32 index),
    /*
    * 查询自己标注过的图片
    * @params
    * user:用户名, index:查询起始点
    * @code
    * 0查询成功， -1 查询失败, 1 没有标注过的图片
    */
    QueryLabeledRet QueryLabeledPic(1:string user, 2:i32 index),
    /*
    * 图片传输接口
    * @params
    * pic_url : 图片URL
    * @code
    * 0代表成功， -1代表失败
    */
    DownloadRet DownloadPic(1:string pic_url),
    /*
	 * 标注结果入库
         * @params
         *   1.poseInfo : 标注信息，以JSON格式存储
	 *   2.pic_id       : 被标注图片ID
        * @code
        * 0入库成功， -1 入库失败
	*/
	ReturnVals InsertToDb(1:string poseInfo, 2:i32 pic_id, 3:string user),
    
}

