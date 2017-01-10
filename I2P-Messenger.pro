CONFIG += qt

greaterThan(QT_MAJOR_VERSION, 4): CONFIG += c++11

QT += network xml 

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia core

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS   += -std=c++11
QMAKE_CXXFLAGS += -Wall

DEPENDPATH += . \
	src/gui/ \
        src/backend/ \
        src/controller/

INCLUDEPATH += . \
	      src/gui \
              src/backend/ \
              src/controller/

SOURCES += \
        src/backend/Main.cpp \
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
	src/gui/form_TopicSubscribe.cpp \
	src/backend/SwarmType1.cpp \
	src/backend/SwarmType1FactoryMethods.cpp \
	src/backend/SwarmType1WhiteList.cpp \
	src/gui/form_createNewSwarmType1.cpp \
	src/backend/rostermodel.cpp \
	src/backend/abstractrosterentry.cpp \
	src/backend/actorrosterentry.cpp \
	src/backend/abstractgrouprosterentry.cpp \
	src/backend/swarmtype1rosterentry.cpp \
	src/backend/rosterentryfactory.cpp

HEADERS += src/backend/Core.h \
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
	src/backend/HttpOverStreamObject.h \
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
	src/gui/form_TopicSubscribe.h \
	src/backend/SwarmType1.h \
	src/backend/SwarmType1FactoryMethods.h \
	src/backend/SwarmType1WhiteList.h \
	src/gui/icons/i2pmessenger_win.rc \
	src/gui/icons/i2pmessenger_win.rc.h \
	src/gui/form_createNewSwarmType1.h \
	src/backend/rostermodel.h \
	src/backend/abstractrosterentry.h \
	src/backend/actorrosterentry.h \
	src/backend/abstractgrouprosterentry.h \
	src/backend/swarmtype1rosterentry.h \
	src/backend/rosterentryfactory.h

FORMS += src/gui/form_Main.ui \
	src/gui/form_chatwidget.ui \
	src/gui/form_DebugMessages.ui \
	src/gui/form_fileRecive.ui \
	src/gui/form_fileSend.ui \
	src/gui/form_newUser.ui \
	src/gui/form_rename.ui \
	src/gui/form_settingsgui.ui \
	src/gui/form_userSearch.ui \
	src/gui/form_about.ui \  
	src/gui/form_topicSubscribe.ui \
	src/gui/form_createSwarmType1.ui

RESOURCES += src/gui/resourcen.qrc

#win32 {}
#unix {}

UI_DIR = src/gui
OBJECTS_DIR = temp/obj
RCC_DIR = temp/qrc
MOC_DIR = temp/moc
RC_FILE = src/gui/icons/i2pmessenger_win.rc

android {
	DEFINES += ANDROID=1
}

include(gitversion.pri)

#DISTFILES += \
#	src/gui/icons/add_group.png \
#	src/gui/icons/add_group48.png \
#	src/gui/icons/agt_forum48.png \
#	src/gui/icons/irc_protocol.png \
#	src/gui/icons/rebuild.png \
#	src/gui/icons/userred24.png \
#	src/gui/icons/personal.ico \
#	src/gui/icons/agt_forum48.png
