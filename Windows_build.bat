@echo off
cls 
echo "Start this Batch - Script from Qt-Console"
echo "R = Release"
echo "D = Debug"

set auswahl=
set /p auswahl="Option: "

if "%auswahl%" == "R" GOTO RELEASE
if "%auswahl%" == "D" GOTO DEBUG

echo "Abort chose R or D ;)"
pause
exit 

:DEBUG
	qmake -win32 I2P-Messenger.pro
	mingw32-make debug
	exit
:RELEASE
	qmake I2P-Messenger.pro
	mingw32-make release
	exit
