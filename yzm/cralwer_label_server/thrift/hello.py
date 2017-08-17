
#!/usr/bin/env python
 
import sys
sys.path.append('./gen-py')
 
from pose_label import CrawlerClientService
from pose_label.ttypes import *
from thrift import Thrift 
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import  TBinaryProtocol

def UpdatePicInfo(client, id, tag, pose_type):
        try:
           ret = client.UpdatePicInfo(id, tag, pose_type)
           print "UpdatePicInfo, ret.code " 
           print ret.code
           return ret.code == 0

        except Thrift.TException, ex:
          print "%s" % (ex.message)
          return False

try:

  transport = TSocket.TSocket('192.168.1.211', 10077)

  transport = TTransport.TFramedTransport(transport)

  protocol = TBinaryProtocol.TBinaryProtocol(transport)

  client = CrawlerClientService.Client(protocol)
  transport.open()
 
 
  id = 64
  tag = "dddddd"
  pose_type = "ssssss"
  ret = UpdatePicInfo(client, id, tag, pose_type)
  print ret


  transport.close()

except Thrift.TException, ex:
  print "%s" % (ex.message) 


本周的工作总结
1.完成爬虫客户端的服务开发
2.完成了标注服务端开发
3.修改了部分测试组提供的BUG
4.将服务迁移到实际环境


下周的工作目标计划
1.添加标注工具打分功能
2.优化sql语句，提高模糊查询速率
3.完成安卓导购机升级服务拆分成两路需求
3.修改服务端BUG    