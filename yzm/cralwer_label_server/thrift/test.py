
#!/usr/bin/env python
#coding=utf-8
import sys
import os
import time
sys.path.append('./gen-py')
 
from pose_label import CrawlerClientService
from pose_label import LabelService
from pose_label.ttypes import *
from thrift import Thrift 
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import  TBinaryProtocol

def UpdatePicInfos(client, id, tag, pose_type):
        try:
           ret = client.UpdatePicInfo(id, tag, pose_type)
           print "UpdatePicInfo() end,  ret.code " 
           print ret.code
           return ret.code == 0

        except Thrift.TException, ex:
          print "%s" % (ex.message)   
          return False                

"""
try:

  transport = TSocket.TSocket('192.168.1.211', 10077)   

  transport = TTransport.TFramedTransport(transport)

  protocol = TBinaryProtocol.TBinaryProtocol(transport)

  client = CrawlerClientService.Client(protocol)
  transport.open()

  id = 64          
  tag = "chenzixun"
  pose_type = "chenzixun"
  ret = UpdatePicInfo(client, id, tag, pose_type)
                       
  transport.close()

except Thrift.TException, ex:
  print "%s" % (ex.message)  

"""


try:
  transport = TSocket.TSocket('192.168.1.211', 10099)   

  transport = TTransport.TFramedTransport(transport)

  protocol  = TBinaryProtocol.TBinaryProtocol(transport)

  client = LabelService.Client(protocol)
  transport.open()                      
  """
  ret = client.QueryPicPoseData(65, "../pic_preproccess/8b56b799a5e7fbb099504498ce383151.jpg", "123456")
  print "QueryPicPoseData ret.code  " , ret.code 
  print "vec[0].user_name : %s " % ret.vecPoseData[0].user_name
  """
  ret = client.ScorePoseData(65, "chenzx", "123456", 4)   
  
  
  ret = client.QueryPoseDataScore(65, "shiyl", "123456")   

  transport.close()  

                         
  
except Thrift.TException, ex :   
  print "%s" % (ex.message)
