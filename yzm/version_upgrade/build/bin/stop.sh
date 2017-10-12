#!/bin/bash

PROG="upgrade_server"

pids=`pidof $PROG`

echo $pids
if [ "$pids" = "" ]
then
	echo "$PROG is not running"
	exit 0
fi

for pid in $pids
do
	kill -9 $pid
done

sleep 2

pids=`pidof $PROG`
if [ $pids ];then
	echo "stop $PROG failed, pids = $pids"
	exit 1
fi

echo "stop $PROG successfully."

