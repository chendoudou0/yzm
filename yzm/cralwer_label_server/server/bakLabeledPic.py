#!/usr/bin/env python
#coding=utf-8

import sys
import os
import time

while True:
    os.system('rsync -vzrtopg --progress -e ssh --delete DSU_chenzx@192.168.1.227:/var/DataStorage/L1/PP/success_pic/*  /home/yzm/chenzx/rsync_bak/') 
    time.sleep(60*60*6) 


