#!/bin/sh

COUNT=$(cat /proc/cpuinfo | grep 'model name' | sed -e 's/.*: //' | wc -l)


	echo "\nCompiling - Script(Unix/Linux) for I2P-Messenger v.04"
	echo  "\n\nNeeded software:"
	echo  "\t QT4"
	echo  "\t QT4-devs"
	echo  "\t NAS (Network Audio System ) for Sound"
	echo  "\n"
	echo  "\t Found:\t ${COUNT} cpu's"
	echo  "\t Use:\t ${COUNT} thread's for compile"
	echo  "\n\n"


if [ -e "/usr/bin/qmake-qt4" ]; then
	make_qt=$("qmake-qt4")
	echo "\t Found qmake-qt4,- use it"

elif [  -e "/usr/bin/qmake" ]; then
	make_qt=$("qmake")
	echo "\t Found qmake,- use it"
      
else
	echo "\tFound no qmake or qmake-qt4"
	exit
fi

if [ $# = 0 ]; 
then
	echo  "Select a option:"
	echo  "\t--R vor Release"
	echo  "\t--D vor Debug"
	exit
fi


make clean
rm I2P-Messenger

if [ "$1" = "--D" ]; 
then
	${make_qt} I2P-Messenger.pro
	make -j $COUNT

elif [ "$1" = "--R" ]; 
then
	${make_qt} I2P-Messenger_release.pro
	make -j $COUNT
	mv I2P-Messenger_release I2P-Messenger
	strip I2P-Messenger
else
	echo  "Select a option:"
	echo  "\t--R vor Release"
	echo  "\t--D vor Debug"
	exit
fi



echo  "\n\n\nFinished"