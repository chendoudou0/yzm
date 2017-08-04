
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