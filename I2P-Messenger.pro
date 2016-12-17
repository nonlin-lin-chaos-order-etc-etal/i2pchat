

CONFIG += qt \
	  debug	

QT += network xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

QMAKE_CXXFLAGS+= -std=c++11 -Wall
QMAKE_LFLAGS  += -std=c++11

DEPENDPATH += ". \
	      src/gui/ \
	      src/backend/"

INCLUDEPATH += . \
	      src/gui \
	      src/backend/

SOURCES +=  src/backend/Main.cpp \
	    src/backend/Core.cpp \
            src/backend/User.cpp \
            src/backend/I2PSamMessageAnalyser.cpp \
            src/backend/DebugMessageManager.cpp \
            src/backend/Protocol.cpp \
  	    src/backend/PacketManager.cpp \
	    src/backend/FileTransferSend.cpp \
	    src/backend/FileTransferRecive.cpp \
	    src/backend/SoundManager.cpp \
	    src/backend/ConnectionManager.cpp \
	    src/backend/SessionController.cpp \
  	    src/backend/I2PStream.cpp \
	    src/backend/ChatmessageChanger.cpp \
	    src/backend/UserBlockManager.cpp \
	    src/backend/SeedlessManager.cpp \
	    src/backend/SeedlessAnnouncer.cpp \
	    src/backend/SeedlessSearcher.cpp \
	    src/backend/HttpOverStreamObject.cpp \
	    src/backend/UserManager.cpp \
	    src/backend/FileTransferManager.cpp \
	    src/backend/TextEmotionChanger.cpp \
	    src/backend/UnsentChatMessageStorage.cpp \
	    src/gui/form_Main.cpp \
	    src/gui/form_chatwidget.cpp \
	    src/gui/form_DebugMessages.cpp \
	    src/gui/form_fileRecive.cpp \
	    src/gui/form_fileSend.cpp \
	    src/gui/form_newUser.cpp \
	    src/gui/form_rename.cpp \
	    src/gui/form_settingsgui.cpp \
	    src/gui/form_UserSearch.cpp \
	    src/gui/form_about.cpp \


HEADERS +=  src/backend/Core.h \
            src/backend/User.h \
            src/backend/I2PSamMessageAnalyser.h \
            src/backend/DebugMessageManager.h \
            src/backend/Protocol.h \
	    src/backend/PacketManager.h \
	    src/backend/FileTransferSend.h \
	    src/backend/FileTransferRecive.h \
	    src/backend/SoundManager.h \
	    src/backend/ConnectionManager.h \
	    src/backend/SessionController.h \
            src/backend/I2PStream.h \
  	    src/backend/ChatmessageChanger.h \
	    src/backend/UserBlockManager.h \
	    src/backend/SeedlessManager.h \
	    src/backend/SeedlessSearcher.h \
	    src/backend/HttpOverStreamObject.h \
	    src/backend/SeedlessAnnouncer.h \
	    src/backend/UserManager.h \
	    src/backend/FileTransferManager.h \
	    src/backend/TextEmotionChanger.h \
	    src/backend/UnsentChatMessageStorage.h \
	    src/gui/form_Main.h \
	    src/gui/form_chatwidget.h \
	    src/gui/form_DebugMessages.h \
	    src/gui/form_fileRecive.h \
	    src/gui/form_fileSend.h \
	    src/gui/form_newUser.h \
	    src/gui/form_rename.h \
	    src/gui/form_settingsgui.h \
	    src/gui/form_UserSearch.h \
	    src/gui/form_about.h \
	    src/gui/gui_icons.h \
	  



FORMS +=    src/gui/form_Main.ui \
	    src/gui/form_chatwidget.ui \
	    src/gui/form_DebugMessages.ui \
	    src/gui/form_fileRecive.ui \
	    src/gui/form_fileSend.ui \
	    src/gui/form_newUser.ui \
	    src/gui/form_rename.ui \
	    src/gui/form_settingsgui.ui \
	    src/gui/form_userSearch.ui \
	    src/gui/form_about.ui 
	    

RESOURCES += src/gui/resourcen.qrc


#win32 {}
#unix {}


UI_DIR = src/gui
OBJECTS_DIR = temp/obj
RCC_DIR = temp/qrc
MOC_DIR = temp/moc
RC_FILE = src/gui/icons/i2pmessenger_win.rc
