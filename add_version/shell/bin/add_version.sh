#!/bin/sh

#echo 524288000 > /proc/sys/kernel/shmmax
#echo 1024 > /proc/sys/kernel/msgmni
	
cd /home/yzm/version/shell/bin
chmod 755 ./AddVersion*
ulimit -c unlimited
killall -9 ./AddVersion
sleep 5
./AddVersion

