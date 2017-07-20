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
}
service CrawlerService{
    /*
    * 图片传输请求接口
    * @params
    * md5 : 图片MD5
    * @rrorCode
    * 0允许上传， -1：查询失败， 1：图片已经存在
    */
    ReturnVals UploadRequest(1:string md5),
    /*
    * 图片传输接口
    * @params
    * PicInfo : 图片信息
    * @rrorCode
    * 0代表成功， -1代表失败
    */
    ReturnVals PicUpload(1:PicInfo pic)   
}

/////////////////////////////////////////////////////

struct QueryUnlabeledRet{
    1:i32 code = 0,       
    2:string msg,
    3:string picJson,       //图片信息数组，以JSON格式存储
}

/*
 picJson示例：
{
  "pic_array":[
    {
      "pic_id": 1111,                   //图片ID
      "pic_url": "/home/yzm/test.jpg",  //图片URL
      "screenshot_bin": "0000000"      //缩略图二进制流
    },
    {
	  "pic_id": 1122,
      "pic_url": "/home/yzm/test2.jpg",
      "screenshot_bin": "0000000" 
    }]
}
*/

struct DownloadRet {
    1: i32    code = 0,
    2: string msg,
    3: binary bin,                  //图片二进制流
    4: string HumanPose2DInfo,      //2D骨骼信息，以JSON格式存储
    5: string HumanPose3DInfo,      //3D骨骼信息，以JSON格式存储
}

service LabelService{
    /*
    * 未标注图片查询接口
    * @params
    * 无
    * @code
    * 0查询成功， -1 查询失败
    */
    QueryUnlabeledRet QueryUnlabeledPic(),
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
	ReturnVals InsertToDb(1:string poseInfo, 2:i32 pic_id,3:string userName),
    
}

