#!/bin/sh

COUNT=$(cat /proc/cpuinfo | grep 'model name' | sed -e 's/.*: //' | wc -l)


	echo "\n*** Build script (Unix/Linux) for I2P-Messenger"
	echo  "\n\nNeeded software:"
	echo  "\t QT5"
	echo  "\t QT5-devs + qtmultimedia5-dev"
	echo  "\t NAS (Network Audio System ) for sound"
	echo  "\n"
	echo  "\t Found:\t ${COUNT} CPUs"
	echo  "\t Using:\t ${COUNT} threads for compilation"
	echo  "\n\n"


if [ -e "/usr/bin/qmake-qt5" ]; then
	make_qt=$("qmake-qt5")
	echo "\t Found qmake-qt5; using it"

elif [  -e "/usr/bin/qmake" ]; then
	make_qt=$("qmake")
	echo "\t Found qmake; using it"
      
else
	echo "\tError: found neither qmake-qt5 nor qmake"
	exit
fi

if [ $# = 0 ]; 
then
	echo  "Specify an option:"
	echo  "\t--R for release build"
	echo  "\t--D for debug build"
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
	echo  "Specify an option:"
	echo  "\t--R for release build"
	echo  "\t--D for debug build"
	exit
fi

echo  "\n\n\nI2PChat build finished"