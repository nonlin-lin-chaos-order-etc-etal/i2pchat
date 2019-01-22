CONFIG += qt

greaterThan(QT_MAJOR_VERSION, 4): CONFIG += c++11

QT += network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia core

TEMPLATE = app

#DEFINES += DEBUG_LOGGING_ON=1

QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS   += -std=c++11
QMAKE_CXXFLAGS += -Wall

DEPENDPATH += . \
	src/gui/ \
	src/backend/ \
	src/controller/

INCLUDEPATH += . \
        src/backend \
        src/backend/controller \
        src/backend/enums \
        src/backend/error \
        src/backend/factories \
        src/backend/model \
        src/backend/model/roster \
        src/backend/model/roster/entries \
        src/backend/model/impl \
        src/backend/model/impl/roster \
        src/backend/model/impl/roster/entries \
        src/backend/persistence \
        src/backend/protocol \
        src/backend/sam \
        src/backend/util \
        src/controller \
        src/gui \
        src/sound

TRANSLATIONS = lang/ru.ts

SOURCES += \
        src/gui/form_Main.cpp \
        src/gui/form_chatwidget.cpp \
        src/gui/form_fileSend.cpp \
        src/gui/form_fileRecive.cpp \
        src/gui/form_rename.cpp \
        src/gui/form_createNewSwarmType1.cpp \
        src/gui/form_about.cpp \
        src/gui/form_TopicSubscribe.cpp \
        src/gui/abstractrosterentryview.cpp \
        src/gui/form_DebugMessages.cpp \
        src/gui/form_UserSearch.cpp \
        src/gui/form_newUser.cpp \
        src/gui/form_settingsgui.cpp \
        src/backend/error/ParseError.cpp \
        src/backend/model/impl/SwarmType1LocalImageImpl.cpp \
        src/backend/model/impl/SwarmType1WhiteListImpl.cpp \
        src/backend/model/impl/UserImpl.cpp \
        src/backend/model/impl/AppContextImpl.cpp \
        src/backend/model/impl/ChatParticipationImpl.cpp \
        src/backend/model/impl/roster/entries/AbstractRosterEntryImpl.cpp \
        src/backend/model/impl/roster/entries/AbstractGroupRosterEntryImpl.cpp \
        src/backend/model/impl/roster/entries/OneToOneRosterEntryImpl.cpp \
        src/backend/model/impl/roster/entries/SwarmType1RosterEntryImpl.cpp \
        src/backend/model/impl/roster/RosterImpl.cpp \
        src/backend/persistence/UnsentChatMessagesStorage.cpp \
        src/backend/protocol/Connector.cpp \
        src/backend/protocol/FileTransferReceive.cpp \
        src/backend/protocol/FileTransferSend.cpp \
        src/backend/util/ParseErrorsUtil.cpp \
        src/backend/util/ChatMessageChangerUtil.cpp \
        src/backend/util/TextEmotionChangerUtil.cpp \
        src/backend/controller/UserManager.cpp \
        src/backend/controller/UserBlockManager.cpp \
        src/backend/controller/FileTransferManager.cpp \
        src/backend/controller/DebugMessageManager.cpp \
        src/backend/controller/SwarmsController.cpp \
        src/backend/sam/PacketManager.cpp \
        src/backend/sam/I2PStream.cpp \
        src/backend/sam/HttpOverStreamObject.cpp \
        src/backend/sam/I2PSamMessageAnalyser.cpp \
        src/backend/sam/SessionController.cpp \
        src/backend/sam/ConnectionManager.cpp \
        src/backend/factories/RosterEntryFactory.cpp \
        src/backend/factories/SwarmType1FactoryMethods.cpp \
        src/controller/RosterController.cpp \
        src/controller/Main.cpp \
        src/sound/SoundManager.cpp \
        src/backend/model/RemoteUserLocalImage.cpp \
        src/backend/model/AbstractUserLocalImage.cpp \
        src/backend/model/LocalUserLocalImage.cpp \
        src/backend/model/SwarmParticipantLocalImage.cpp \
        src/backend/model/OneToOneChatLocalImage.cpp

HEADERS += \
        src/gui/form_chatwidget.h \
        src/gui/icons/i2pmessenger_win.rc.h \
        src/gui/gui_icons.h \
        src/gui/form_settingsgui.h \
        src/gui/form_newUser.h \
        src/gui/form_fileRecive.h \
        src/gui/form_about.h \
        src/gui/form_HelpDialog.h \
        src/gui/form_rename.h \
        src/gui/form_TopicSubscribe.h \
        src/gui/abstractrosterentryview.h \
        src/gui/form_Main.h \
        src/gui/form_DebugMessages.h \
        src/gui/form_fileSend.h \
        src/gui/form_createNewSwarmType1.h \
        src/gui/form_UserSearch.h \
        src/backend/error/ParseError.h \
        src/backend/model/impl/SwarmType1LocalImageImpl.h \
        src/backend/model/impl/ChatParticipationImpl.h \
        src/backend/model/impl/UserImpl.h \
        src/backend/model/impl/AppContextImpl.h \
        src/backend/model/impl/roster/RosterImpl.h \
        src/backend/model/impl/roster/entries/AbstractGroupRosterEntryImpl.h \
        src/backend/model/impl/roster/entries/OneToOneRosterEntryImpl.h \
        src/backend/model/impl/roster/entries/SwarmType1RosterEntryImpl.h \
        src/backend/model/impl/roster/entries/AbstractRosterEntryImpl.h \
        src/backend/model/impl/SwarmType1WhiteListImpl.h \
        src/backend/persistence/UnsentChatMessagesStorage.h \
        src/backend/protocol/FileTransferSend.h \
        src/backend/protocol/FileTransferReceive.h \
        src/backend/protocol/Connector.h \
        src/backend/util/ChatMessageChangerUtil.h \
        src/backend/util/TextEmotionChangerUtil.h \
        src/backend/util/ParseErrorsUtil.h \
        src/backend/controller/SwarmsController.h \
        src/backend/controller/UserManager.h \
        src/backend/controller/UserBlockManager.h \
        src/backend/controller/FileTransferManager.h \
        src/backend/controller/DebugMessageManager.h \
        src/backend/model/SwarmType1LocalImage.h \
        src/backend/model/SwarmType1WhiteList.h \
        src/backend/model/ChatParticipation.h \
        src/backend/model/AppContext.h \
        src/backend/model/roster/Roster.h \
        src/backend/model/roster/entries/OneToOneRosterEntry.h \
        src/backend/model/roster/entries/AbstractGroupRosterEntry.h \
        src/backend/model/roster/entries/AbstractRosterEntry.h \
        src/backend/model/roster/entries/SwarmType1RosterEntry.h \
        src/backend/sam/ConnectionManager.h \
        src/backend/sam/I2PStream.h \
        src/backend/sam/I2PSamMessageAnalyser.h \
        src/backend/sam/SessionController.h \
        src/backend/sam/PacketManager.h \
        src/backend/sam/HttpOverStreamObject.h \
        src/backend/factories/SwarmType1FactoryMethods.h \
        src/backend/factories/RosterEntryFactory.h \
        src/controller/RosterController.h \
        src/controller/Main.h \
        src/sound/SoundManager.h \
        src/backend/model/AbstractUserLocalImage.h \
        src/backend/model/RemoteUserLocalImage.h \
        src/backend/model/LocalUserLocalImage.h \
        src/backend/model/SwarmParticipantLocalImage.h \
        src/backend/enums/RemoteUserOnlineStatus.h \
        src/backend/model/OneToOneChatLocalImage.h

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
#	src/OLD_CODE/Core_cpp.txt
