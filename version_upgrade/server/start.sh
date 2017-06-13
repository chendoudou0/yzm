#!/bin/bash

PROG="test"

pids=`pidof $PROG`
if [ $pids ];then
	echo "$PROG is running, pids = $pids"
	exit 0
fi

curdir=`dirname $(readlink -f $0)`
cd $curdir
echo $curdir

nohup ./$PROG   &

sleep 3

pids=`pidof $PROG`
if [ $pids ];then
	echo "$PROG is starting, pids = $pids"
	exit 0
else
	echo "start $PROG failed."
	exit 1
fi
