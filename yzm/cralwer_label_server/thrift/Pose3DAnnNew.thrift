namespace cpp  ThriftPose3DAnn


struct Image{
   1:binary imgBin,    //图片的二进制流
   2:i32    width,     //图像宽度
   3:i32    height,    //图像高度	
   4:i32    channel    //图像通道数
}

struct Vector3DInt{
	1:i32 x,
	2:i32 y,
	3:i32 z
}

struct Vector3DFloat{
	1:double x,
	2:double y,
	3:double z
}

struct Orientation{
	1:Vector3DFloat xAxis,
	2:Vector3DFloat yAxis,
	3:Vector3DFloat zAxis
}

struct CameraInfo{
	1:list<double> camMatrix /*3x3 的内参矩阵*/
}

struct GuessInfo{
	1:list<Orientation> guessOrien, /*会包含两个待选的放向*/
	2:list<Vector3DFloat> guessJoints, /*2D骨骼信息*/
        3:CameraInfo camInfo, /*相机参数*/
        4:bool isSuccess  /*猜测是否成功*/
}

struct HumanPose3DInfo{       
	1:list<Vector3DFloat> joints, /*3D骨骼信息*/
	2:Orientation orien,        /*骨骼朝向*/    
	3:CameraInfo camInfo        /*相机参数*/        
} 

service Pose3DAnnPreprocess{

	/*
         * 检测图片是否有效
         * @params
         *   1.Image : 图片  
         * @return
         *   True 代表有效，False 代表无效
        */
	bool CheckIsValid(1:Image img),

	/*
	 * 初始图片猜测
         * @params
         *   1.Image : 图片
         * @return
         *   GuessInfo: 返回初始对于图片的猜测，包括身体朝向Orientation,2D骨骼点位置
	*/
	GuessInfo DoInitGuess(1:Image img),


	/*
	 * 自动3D骨骼生成
         * @params
         *   1.Image : 图片
         *   2.GuessInfo: 传入之前猜测的值
         * @return
         *   list<HumanPose3DInfo>: 返回两个待选的三维骨骼点
	*/
	list<HumanPose3DInfo> AutoCalPose3D(1:Image img,2:GuessInfo guessInfo),

}

service Pose3DAnnRuntime{

	/*
	 * 自动校准
         * @params
         *   1.HumanPose3DInfo : 3D 骨骼信息，包括朝向，相机，3D骨骼点
	 *   2.list<bool>      : 长度为15的布尔数组，标注哪些骨骼点是锁定的，哪些骨骼点是可以被自动校准的
         * @return
         *   HumanPose3DInfo :  返回校准后的3D 骨骼信息
	*/
	HumanPose3DInfo AutoAlignment(1:HumanPose3DInfo poseInfo, 2:list<bool> lockInfo),

        /*
	 * 检测结果是否有效
         * @params
         *   1.HumanPose3DInfo : 3D 骨骼信息，包括朝向，相机，3D骨骼点
                *   2.Image           : 图片
         * @return
         *   True 代表有效，False 代表无效
	*/
	bool CheckResultIsValid(1:HumanPose3DInfo poseInfo, 2:Image img),
}

/*
附录上骨骼点信息
        {0,  "Head"},
        {1,  "Neck"},
        {2,  "RShoulder"},
        {3,  "RElbow"},
        {4,  "RWrist"},
        {5,  "LShoulder"},
        {6,  "LElbow"},
        {7,  "LWrist"},
        {8,  "RHip"},
        {9,  "RKnee"},
        {10, "RAnkle"},
        {11, "LHip"},
        {12, "LKnee"},
        {13, "LAnkle"},
        {14, "Chest"}
*/
