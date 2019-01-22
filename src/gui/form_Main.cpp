/***************************************************************************
 *   Copyright (C) 2008 by I2P-Messenger   				   *
 *   Messenger-Dev@I2P-Messenger   					   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QSystemTrayIcon>

#include "form_Main.h"

#include "OneToOneRosterEntry.h"
#include "SwarmType1RosterEntry.h"
#include "RosterController.h"
#include "UserManager.h"

#include "AppContextImpl.h"


form_MainWindow::form_MainWindow(QString configDir, QWidget* parent)
    : QMainWindow(parent){
    setupUi(this);

    QApplication::setQuitOnLastWindowClosed(false);
    appCtx = new AppContextImpl(configDir)->shared_from_this();
    connect (appCtx,SIGNAL(signUserStatusChanged()),this,
             SLOT(eventUserChanged()));

    connect(appCtx,SIGNAL(signOnlineStatusChanged()),this,
            SLOT(OnlineStateChanged()));

    connect(listWidget,SIGNAL(itemDoubleClicked( QListWidgetItem* )),this,
            SLOT(openUserListeClicked()));

    connect(listWidget, SIGNAL( customContextMenuRequested(QPoint)), this,
            SLOT( connecttreeWidgetCostumPopupMenu(QPoint)));

    connect(comboBox,SIGNAL(currentIndexChanged( int)),this,
            SLOT(onlineComboBoxChanged()));


    connect(appCtx,SIGNAL(signOwnAvatarImageChanged()),this,
            SLOT(eventAvatarImageChanged()));

    connect(appCtx,SIGNAL(signNicknameChanged()),this,
            SLOT(eventNicknameChanged()));

    mUserSearchWindow=NULL;
    mTopicSubscribeWindow=NULL;
    mDialogCreatNewSwarm=NULL;
    mAboutWindow=NULL;
    mDebugWindow=NULL;

    Mute=false;
    applicationIsClosing=false;


    initStyle();
    initTryIconMenu();
    initTryIcon();
    initToolBars();


    appCtx->setOnlineStatus(USEROFFLINE);
    eventUserChanged();
    eventNicknameChanged();
    eventAvatarImageChanged();

}


form_MainWindow::~form_MainWindow(){
    applicationIsClosing=true;

    delete appCtx;
    delete trayIcon;
    this->close();
}

void form_MainWindow::onlineComboBoxChanged()
{
    QComboBox* comboBox= this->comboBox;
    QString text=comboBox->currentText();

    if(text.contains(tr("Online"),Qt::CaseInsensitive)==true){
        if(appCtx->getOnlineStatus()!=AbstractUserLocalImage::USERONLINE)
            appCtx->setOnlineStatus(AbstractUserLocalImage::USERONLINE);
    }
    else if(text.contains(tr("WantToChat"),Qt::CaseInsensitive)==true){
        if(appCtx->getOnlineStatus()!=AbstractUserLocalImage::USERWANTTOCHAT)
            appCtx->setOnlineStatus(AbstractUserLocalImage::USERWANTTOCHAT);
    }
    else if(text.contains(tr("Away"),Qt::CaseInsensitive)==true){
        if(appCtx->getOnlineStatus()!=AbstractUserLocalImage::USERAWAY)
            appCtx->setOnlineStatus(AbstractUserLocalImage::USERAWAY);
    }
    else if(text.contains(tr("Don't disturb"),Qt::CaseInsensitive)==true){
        if(appCtx->getOnlineStatus()!=AbstractUserLocalImage::USERDONT_DISTURB)
            appCtx->setOnlineStatus(AbstractUserLocalImage::USERDONT_DISTURB);
    }
    else if(text.contains(tr("Invisible"),Qt::CaseInsensitive)==true){
        if(appCtx->getOnlineStatus()!=AbstractUserLocalImage::USERINVISIBLE)
            appCtx->setOnlineStatus(AbstractUserLocalImage::USERINVISIBLE);
    }
    else if(text.contains(tr("Offline"),Qt::CaseInsensitive)==true){
        if(appCtx->getFileTransferManager()->checkIfAFileTransferOrReciveisActive()==false){
            if(appCtx->getOnlineStatus()!=AbstractUserLocalImage::USEROFFLINE)
                appCtx->setOnlineStatus(AbstractUserLocalImage::USEROFFLINE);
        }
        else{
            QMessageBox* msgBox= new QMessageBox(NULL);
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setText(tr("I2P-Messenger"));
            msgBox->setInformativeText(tr("Sorry a File transfer or File receive is active,\nclosing aborted"));
            msgBox->setStandardButtons(QMessageBox::Ok);
            msgBox->setDefaultButton(QMessageBox::Ok);
            msgBox->setWindowModality(Qt::NonModal);
            msgBox->show();
            OnlineStateChanged();

        }
    }
    else if(text.contains(tr("TryToConnect"),Qt::CaseInsensitive)==true){
    }

}

void form_MainWindow::initToolBars()
{
    //toolBar->setIconSize(QSize(24, 24));
    QSettings settings(appCtx->getConfigPath()+"/application.ini",QSettings::IniFormat);
    QToolBar* toolBar=this->toolBar;

    toolBar->setContextMenuPolicy(Qt::CustomContextMenu);
    toolBar->addAction(QIcon(ICON_NEWUSER)		,tr("Add AbstractUserLocalImage")		,this,SLOT(openAdduserWindow() ) );
/*
    {
        settings.beginGroup("Usersearch");
        if((settings.value("Enabled",true).toBool()) ==true){
            if(appCtx->getUserInfos().Nickname.isEmpty()==false){
                toolBar->addAction(QIcon(ICON_USERSEARCH),tr("Users Search")	,this,SLOT(openUserSearchWindow()));
            }else{
                QMessageBox* msgBox= new QMessageBox(NULL);
                msgBox->setIcon(QMessageBox::Information);
                msgBox->setText(tr("I2P-Messenger"));
                msgBox->setInformativeText(tr("You have to enter a Nickname (at AbstractUserLocalImage Details) to use the AbstractUserLocalImage Search. AbstractUserLocalImage Search deactivated"));
                msgBox->setStandardButtons(QMessageBox::Ok);
                msgBox->setDefaultButton(QMessageBox::Ok);
                msgBox->setWindowModality(Qt::NonModal);
                msgBox->exec();
            }
        }
        settings.endGroup();
    }
*/
    toolBar->addAction(QIcon(ICON_USERSEARCH), tr("Create new swarm"), this, SLOT(openDialog_creatNewSwarm()));

#if 0
    {
        settings.beginGroup("Topics");
        if((settings.value("Enabled",true).toBool())){
            toolBar->addAction(QIcon(ICON_USERSEARCH), tr("Subscribe to Topic"), this, SLOT(openTopicSubscribeWindow()));
        }
        settings.endGroup();
        settings.sync();
    }
#endif

    toolBar->addAction(QIcon(ICON_SETTINGS)		,tr("Settings")		,this,SLOT(openConfigWindow() ) );
    toolBar->addAction(QIcon(ICON_DEBUGMESSAGES)	,tr("DebugMessages")	,this,SLOT(openDebugMessagesWindow() ) );
    toolBar->addAction(QIcon(ICON_MYDESTINATION)	,tr("Your Destination")	,this,SLOT(namingMe()));
    toolBar->addAction(QIcon(ICON_CLOSE)		,tr("Close")		,this,SLOT(closeApplication()));
    toolBar->addAction(QIcon(ICON_ABOUT)		,tr("About")		,this,SLOT(openAboutDialog()));

}


void form_MainWindow::openConfigWindow(){

    form_settingsgui* dialog= new form_settingsgui(*appCtx);
    connect(this,SIGNAL(closeAllWindows()),dialog,
            SLOT(close()));

    dialog->show();

}
void form_MainWindow::openAdduserWindow(){
    form_newUserWindow* dialog= new form_newUserWindow(*appCtx);

    connect(this,SIGNAL(closeAllWindows()),dialog,
            SLOT(close()));

    dialog->show();
}

void form_MainWindow::openDebugMessagesWindow(){
    if(mDebugWindow==NULL){
        mDebugWindow= new form_DebugMessages(*appCtx);

        connect(this,SIGNAL(closeAllWindows()),mDebugWindow,
                SLOT(close()));

        connect(mDebugWindow,SIGNAL(closingDebugWindow()),this,
                SLOT(eventDebugWindowClosed()));

        mDebugWindow->show();
    }else{
        mDebugWindow->getFocus();
    }
}

void form_MainWindow::namingMe(){
    QClipboard *clipboard = QApplication::clipboard();
    QString Destination=appCtx->getMyDestination();
    if(Destination!=""){
        clipboard->setText(Destination);
        QMessageBox::information(this, "",
                                 tr("Your Destination has been copied to the clipboard"),QMessageBox::Close);
    }
    else
        QMessageBox::information(this, "",
                                 tr("Your Client must be Online for that"),QMessageBox::Close);
}
void form_MainWindow::closeApplication(){


    if(appCtx->getFileTransferManager()->checkIfAFileTransferOrReciveisActive()==false){

        QMessageBox* msgBox= new QMessageBox(this);
        msgBox->setIcon(QMessageBox::Question);
        msgBox->setText(tr("I2P-Messenger"));
        msgBox->setInformativeText(tr("Are you sure ?"));
        msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox->setDefaultButton(QMessageBox::Yes);
        msgBox->setWindowModality(Qt::WindowModal);
        int ret = msgBox->exec();
        if(ret== QMessageBox::No){
            return;
        }


        applicationIsClosing=true;
        emit closeAllWindows();

        delete appCtx;
        appCtx = 0;
        delete trayIcon;
        trayIcon = 0;

        this->close();
    }
    else{

        QMessageBox* msgBox= new QMessageBox(NULL);
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setText(tr("I2P-Messenger"));
        msgBox->setInformativeText(tr("Sorry a File transfer or File receive is active,\nClosing aborted"));
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->setDefaultButton(QMessageBox::Ok);
        msgBox->setWindowModality(Qt::NonModal);
        msgBox->exec();

    }

}

void form_MainWindow::eventUserChanged(){
    mLastRosterEntryMapIdWithUnreadMessages = "";
    bool showUnreadMessageAtTray = false;
    //QList<AbstractUserLocalImage*> users = appCtx->getUserManager()->getUserList();
    Roster & rosterModel = appCtx->getRosterModel();
    RosterController & rosterController = appCtx->getRosterController();
    QList<CFileTransferRecive*> FileRecives = appCtx->getFileTransferManager()->getFileTransferReciveList();
    QList<CFileTransferSend*> FileSends = appCtx->getFileTransferManager()->getFileTransferSendsList();

    listWidget->clear();

    {
        QList<AbstractRosterEntry*>* rosterEntries = rosterModel.cloneRosterEntriesSet();

        for(int i=0;i<rosterEntries->size();i++){
            //USERS AND SWARMS
            AbstractRosterEntry* rosterEntry = (*rosterEntries)[i];

            QListWidgetItem* newItem= new QListWidgetItem(listWidget);
            QListWidgetItem* ChildWidthUserData= new QListWidgetItem(listWidget);
            QListWidgetItem* ChildWidthTyp= new QListWidgetItem(listWidget);

            rosterController.shapeRosterEntryGUI(
                        rosterEntry,newItem,showUnreadMessageAtTray,mLastRosterEntryMapIdWithUnreadMessages,ChildWidthUserData,ChildWidthTyp);
        }

        delete rosterEntries;
    }

    for(int i=0;i<FileRecives.size();i++){
        //Filerecives
        QListWidgetItem* newItem= new QListWidgetItem(listWidget);
        QListWidgetItem* ChildWidthStreamIDAsText= new QListWidgetItem(listWidget);
        QListWidgetItem* ChildWidthTyp= new QListWidgetItem(listWidget);

        newItem->setIcon(QIcon(ICON_FILETRANSFER_RECIVE));
        newItem->setText(FileRecives.at(i)->getFileName());
        newItem->setTextAlignment(Qt::AlignLeft);
        newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QString t;
        t.setNum(FileRecives.at(i)->getStreamID(),10);

        ChildWidthStreamIDAsText->setText(t);
        ChildWidthStreamIDAsText->setHidden(true);//DEBUG
        ChildWidthTyp->setText("R");
        ChildWidthTyp->setHidden(true);//DEBUG
    }

    for(int i=0;i<FileSends.size();i++){
        //Filesends
        QListWidgetItem* newItem= new QListWidgetItem(listWidget);
        QListWidgetItem* ChildWidthStreamIDAsText= new QListWidgetItem(listWidget);
        QListWidgetItem* ChildWidthTyp= new QListWidgetItem(listWidget);

        newItem->setIcon(QIcon(ICON_FILETRANSFER_SEND));
        newItem->setText(FileSends.at(i)->getFileName());
        newItem->setTextAlignment(Qt::AlignLeft);
        newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QString t;
        t.setNum(FileSends.at(i)->getStreamID(),10);

        ChildWidthStreamIDAsText->setText(t);
        ChildWidthStreamIDAsText->setHidden(true);//DEBUG
        ChildWidthTyp->setText("S");
        ChildWidthTyp->setHidden(true);//DEBUG
    }



    if(showUnreadMessageAtTray==false){
        OnlineStateChanged();
    }
    else{
        trayIcon->setIcon(QIcon(ICON_NEWUNREADMESSAGETRAY));
    }
}



void form_MainWindow::openUserListeClicked()
{	
    QListWidgetItem* t=listWidget->item(listWidget->currentRow()+2);

    if(t->text()=="A"){
        //open Chatwindow
        t= listWidget->item(listWidget->currentRow()+1);
        QVariant mapIdVariant = t->data(RosterController::RosterEntryModelRole);
        QString rosterEntryModelMapId = qvariant_cast<QString>(mapIdVariant);

        openChatWindow(rosterEntryModelMapId);
    }
    else if(t->text()=="R"){
        //openFileReciveWindow
        t= listWidget->item(listWidget->currentRow()+1);

        bool OK=false;
        qint32 StreamID =t->text().toInt ( &OK,10 );

        if(OK==false){
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText(tr("form_Main(openChat_or_FileRecive_Dialog))"));
            msgBox.setInformativeText(tr("can't parse value: %1" ).arg(t->text()));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
        openFileReciveWindow(StreamID);
    }
    else if(t->text()=="S"){
        //openFileSendWindow
        t= listWidget->item(listWidget->currentRow()+1);

        bool OK=false;
        qint32 StreamID =t->text().toInt ( &OK,10 );

        if(OK==false){
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText(tr("form_Main(openChat_or_FileRecive_Dialog)"));
            msgBox.setInformativeText(tr("can't parse value: %1").arg(t->text()));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
        openFileSendWindow(StreamID);
    }
}
void form_MainWindow::connecttreeWidgetCostumPopupMenu(QPoint point){
    if(listWidget->count()==0)return;

    QMenu contextMnu( this );
    QMenu contextMnuPos("Position",this);

    QMouseEvent *mevent = new QMouseEvent( QEvent::MouseButtonPress, point, Qt::RightButton,
                                           Qt::RightButton, Qt::NoModifier );


    QAction* UserChat = new QAction(QIcon(ICON_CHAT),tr("Chat"),this);
    connect( UserChat , SIGNAL(triggered()),this, SLOT( openUserListeClicked()));

    QAction* UserDelete = new QAction(QIcon(ICON_USER_DELETE),tr("Delete"),this);
    connect( UserDelete , SIGNAL(triggered()),this, SLOT( deleteUserClicked()));

    QAction* UserRename = new QAction(QIcon(ICON_USER_RENAME),tr("Rename"),this);
    connect(UserRename,SIGNAL(triggered()),this, SLOT(renameUserCLicked()));

    QAction* CopyDestination = new QAction(QIcon(ICON_COPYBASE64),tr("Copy Destination"),this);
    connect(CopyDestination,SIGNAL(triggered()),this, SLOT(copyDestination()));

    QAction* ShowUserInfos = new QAction(QIcon(ICON_ABOUT),tr("Userinfo"),this);
    connect(ShowUserInfos,SIGNAL(triggered()),this, SLOT(showUserInfos()));

    QAction* UserInvisible = new QAction(QIcon(ICON_USER_INVISIBLE),tr("Invisible"),this);
    UserInvisible->setCheckable(true);
    connect(UserInvisible,SIGNAL(triggered( bool )),this, SLOT(UserInvisible(bool)));

    QAction* UserToBlockList=new QAction(QIcon(ICON_BLOCK),tr("Block"),this);
    connect(UserToBlockList,SIGNAL(triggered()),this,SLOT(addUserToBlockList()));


    //for contextMnuPos
    QAction* UP = new QAction(tr("Up"),this);
    connect(UP,SIGNAL(triggered()),this, SLOT(UserPositionUP()));
    QAction* DOWN = new QAction(tr("Down"),this);
    connect(DOWN,SIGNAL(triggered()),this, SLOT(UserPositionDOWN()));

    contextMnu.clear();

    QListWidgetItem* t=listWidget->item(listWidget->currentRow()+2);

    if(t->text()=="A"){
        t= listWidget->item(listWidget->currentRow()+1);
        QVariant mapIdVariant = t->data(RosterController::RosterEntryModelRole);
        QString rosterEntryModelMapId = qvariant_cast<QString>(mapIdVariant);
        AbstractRosterEntry *rosterEntry = appCtx->getRosterModel()[rosterEntryModelMapId];
        if(!rosterEntry) {
            QMessageBox::critical(this,tr("Error: no rosterEntryModel found from variant"),tr("Error: no rosterEntryModel found from variant for ")+rosterEntryModelMapId);
            return;
        }
        {
            OneToOneRosterEntry * actorRosterEntry = rosterEntry->asOneToOne();
            if(actorRosterEntry){
                AbstractUserLocalImage* AbstractUserLocalImage = &(actorRosterEntry->getUser());
                QString Destination = AbstractUserLocalImage->getI2PDestination();

                contextMnu.addAction(UserChat);

                if(AbstractUserLocalImage->getConnectionStatus()==ONLINE)
                {
                    QAction* UserSendFile = new QAction(QIcon(ICON_FILETRANSFER_SEND),tr("SendFile"),this);
                    connect(UserSendFile,SIGNAL(triggered()),this, SLOT(SendFile()));
                    contextMnu.addAction(UserSendFile);
                }

                if(AbstractUserLocalImage->getIsInvisible()==true){
                    UserInvisible->setChecked(true);
                }
                else{
                    UserInvisible->setChecked(false);
                }

                contextMnu.addAction(UserToBlockList);
                contextMnu.addSeparator();
                contextMnu.addAction(UserRename);
                contextMnu.addAction(UserDelete);
                contextMnu.addAction(CopyDestination);
                contextMnu.addAction(UserInvisible);
                contextMnu.addAction(ShowUserInfos);

                contextMnuPos.addAction(UP);
                contextMnuPos.addAction(DOWN);

                contextMnu.addMenu(&contextMnuPos);
                contextMnu.exec( mevent->globalPos());
            }
        }
        {
            SwarmType1RosterEntry * swarmType1RosterEntry = rosterEntry->asSwarmType1();
            if(swarmType1RosterEntry){

                /*
                QAction* SwarmType1Delete = new QAction(QIcon(ICON_USER_DELETE),tr("Delete"),this);
                connect( SwarmType1Delete , SIGNAL(triggered()),this, SLOT( deleteSwarmType1Clicked()));

                QAction* SwarmType1Rename = new QAction(QIcon(ICON_USER_RENAME),tr("Rename"),this);
                connect(SwarmType1Rename,SIGNAL(triggered()),this, SLOT(renameSwarmType1Clicked()));

                QAction* SwarmType1ShowInfo = new QAction(QIcon(ICON_ABOUT),tr("Userinfo"),this);
                connect(SwarmType1ShowInfo,SIGNAL(triggered()),this, SLOT(showSwarmType1Info()));

                contextMnu.addAction(SwarmType1Rename);
                contextMnu.addAction(SwarmType1Delete);
                contextMnu.addAction(SwarmType1ShowInfo);

                contextMnu.addMenu(&contextMnuPos);
                contextMnu.exec( mevent->globalPos());
                */
                mevent->ignore();//FIXME
            }
        }
    }
}

void form_MainWindow::deleteUserClicked(){
    QListWidgetItem *t=listWidget->item(listWidget->currentRow()+1);
    QVariant mapIdVariant = t->data(RosterController::RosterEntryModelRole);
    QString rosterEntryModelMapId = qvariant_cast<QString>(mapIdVariant);
    AbstractRosterEntry *rosterEntry = appCtx->getRosterModel()[rosterEntryModelMapId];
    if(!rosterEntry) {
        QMessageBox::critical(this,tr("Error: no rosterEntryModel found from variant"),tr("Error: no rosterEntryModel found from variant for ")+rosterEntryModelMapId);
        return;
    }

    OneToOneRosterEntry * actorRosterEntry = rosterEntry->asOneToOne();
    if(!actorRosterEntry)return;
    AbstractUserLocalImage* AbstractUserLocalImage = &(actorRosterEntry->getUser());
    QString Destination = AbstractUserLocalImage->getI2PDestination();

    QMessageBox* msgBox= new QMessageBox(this);
    msgBox->setIcon(QMessageBox::Question);
    msgBox->setText(tr("I2P-Messenger"));
    msgBox->setInformativeText(tr("Do you really want to delete this contact?"));
    msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox->setDefaultButton(QMessageBox::Yes);
    msgBox->setWindowModality(Qt::WindowModal);
    int ret = msgBox->exec();
    if(ret== QMessageBox::Yes){
        appCtx->getUserManager()->deleteUserByI2PDestination(Destination);
    }



}

void form_MainWindow::renameUserCLicked(){
    QListWidgetItem *t1=listWidget->item(listWidget->currentRow());
    QString OldNickname=t1->text();

    QListWidgetItem *t=listWidget->item(listWidget->currentRow()+1);
    QVariant mapIdVariant = t->data(RosterController::RosterEntryModelRole);
    QString rosterEntryModelMapId = qvariant_cast<QString>(mapIdVariant);
    AbstractRosterEntry *rosterEntry = appCtx->getRosterModel()[rosterEntryModelMapId];
    if(!rosterEntry) {
        QMessageBox::critical(this,tr("Error: no rosterEntryModel found from variant"),tr("Error: no rosterEntryModel found from variant for ")+rosterEntryModelMapId);
        return;
    }

    OneToOneRosterEntry * actorRosterEntry = rosterEntry->asOneToOne();
    if(!actorRosterEntry)return;
    AbstractUserLocalImage* AbstractUserLocalImage = &(actorRosterEntry->getUser());
    QString Destination = AbstractUserLocalImage->getI2PDestination();

    form_RenameWindow* Dialog= new form_RenameWindow(*appCtx,OldNickname,Destination);
    Dialog->show();
}

void form_MainWindow::closeEvent(QCloseEvent *e)
{
    if ((trayIcon != 0) && (trayIcon->isVisible())) {
        static bool firstTime = true;
        if (firstTime)
        {

            //            QMessageBox::information(this, tr("I2PChat System tray"),
            //            tr("Application will continue running. Quit using context menu in the system tray"));
            firstTime = false;
        }
        hide();
        e->ignore();
    }
    
    if(applicationIsClosing==true) {
        e->accept();
        QApplication::exit(0);
    }
}

void form_MainWindow::updateMenu()
{
    toggleVisibilityAction->setText(isVisible() ? tr("Hide") : tr("Show"));
}

void form_MainWindow::toggleVisibility(QSystemTrayIcon::ActivationReason e)
{
    if(mLastRosterEntryMapIdWithUnreadMessages.isEmpty())return;

    static QPoint MainFormPosition=this->pos();

    if(e == QSystemTrayIcon::Trigger || e == QSystemTrayIcon::DoubleClick){
        if(isHidden()){
            this->move(MainFormPosition);
            show();


            if(isMinimized()){
                if(isMaximized()){
                    showMaximized();
                }else{
                    showNormal();
                }
            }
            raise();
            activateWindow();
        }else{
            MainFormPosition=this->pos();
            hide();
        }
    }
}

void form_MainWindow::toggleVisibilitycontextmenu()
{
    if (isVisible())
        hide();
    else
        show();
}

void form_MainWindow::OnlineStateChanged()
{
    disconnect(comboBox,SIGNAL(currentIndexChanged( int)),this,
               SLOT(onlineComboBoxChanged()));

    QComboBox* comboBox = this->comboBox;
    ONLINE_STATE onlinestatus= appCtx->getOnlineStatus();

    if(onlinestatus==AbstractUserLocalImage::USERTRYTOCONNECT){
        comboBox->clear();
        comboBox->addItem(QIcon(ICON_USER_TRYTOCONNECT)	, tr("TryToConnect"));		//index 0
        comboBox->addItem(QIcon(ICON_USER_OFFLINE)	, tr("Offline"));		//1
        comboBox->setCurrentIndex(0);
        trayIcon->setIcon(QIcon(ICON_USER_TRYTOCONNECT));
    }
    else{
        if(comboBox->count()<6){
            comboBox->clear();

            comboBox->addItem(QIcon(ICON_USER_ONLINE)	, tr("Online"));	//index 0
            comboBox->addItem(QIcon(ICON_USER_WANTTOCHAT)	, tr("WantToChat"));	//1
            comboBox->addItem(QIcon(ICON_USER_AWAY)		, tr("Away"));		//2
            comboBox->addItem(QIcon(ICON_USER_DONT_DUSTURB)	, tr("Don't disturb"));	//3
            comboBox->addItem(QIcon(ICON_USER_INVISIBLE)	, tr("Invisible"));	//4
            comboBox->addItem(QIcon(ICON_USER_OFFLINE)	, tr("Offline"));	//5
        }

        if(onlinestatus==AbstractUserLocalImage::USERONLINE){
            comboBox->setCurrentIndex(0);
            trayIcon->setIcon(QIcon(ICON_USER_ONLINE));
        }
        else if(onlinestatus==AbstractUserLocalImage::USERWANTTOCHAT){
            comboBox->setCurrentIndex(1);
            trayIcon->setIcon(QIcon(ICON_USER_WANTTOCHAT));
        }
        else if(onlinestatus==AbstractUserLocalImage::USERAWAY){
            comboBox->setCurrentIndex(2);
            trayIcon->setIcon(QIcon(ICON_USER_AWAY));
        }
        else if(onlinestatus==AbstractUserLocalImage::USERDONT_DISTURB){
            comboBox->setCurrentIndex(3);
            trayIcon->setIcon(QIcon(ICON_USER_DONT_DUSTURB));
        }
        else if(onlinestatus==AbstractUserLocalImage::USERINVISIBLE){
            comboBox->setCurrentIndex(4);
            trayIcon->setIcon(QIcon(ICON_USER_INVISIBLE));
        }
        else if(onlinestatus==AbstractUserLocalImage::USEROFFLINE){
            comboBox->setCurrentIndex(5);
            trayIcon->setIcon(QIcon(ICON_USER_OFFLINE));
        }
    }
    connect(comboBox,SIGNAL(currentIndexChanged( int)),this,
            SLOT(onlineComboBoxChanged()));
}

void form_MainWindow::openAboutDialog()
{
    if(mAboutWindow==NULL){
        mAboutWindow = new form_About(
                    appCtx->getClientName(),
                    appCtx->getClientVersion(),appCtx->getProtocolVersion(),
                    FileTransferProtocol::MINPROTOCOLVERSION,FileTransferProtocol::MAXPROTOCOLVERSION);
        connect(this,SIGNAL(closeAllWindows()),mAboutWindow,
                SLOT(close()));

        connect(mAboutWindow,SIGNAL(closingAboutWindow()),this,
                SLOT(eventAboutWindowClosed()));

        mAboutWindow->show();
    }else{
        mAboutWindow->getFocus();;
    }
}

void form_MainWindow::initStyle()
{
    //commented out the style stuff as styles on "ubuntu 14.04 mate" make the tray icon not appear
    /*
    QSettings * settings=new QSettings(appCtx->getConfigPath()+"/application.ini",QSettings::IniFormat);
    settings->beginGroup("General");
    //Load Style
    QString Style=(settings->value("current_Style","")).toString();
    if(Style.isEmpty()==true)
    {
        //find default Style for this System
        QRegExp regExp("Q(.*)Style");
        Style = QApplication::style()->metaObject()->className();

        if (Style == QLatin1String("QMacStyle"))
            Style = QLatin1String("Macintosh (Aqua)");
        else if (regExp.exactMatch(Style))
            Style = regExp.cap(1);

        //styleCombo->addItems(QStyleFactory::keys());
    }

    qApp->setStyle(Style);
    //Load Style end

    //Load Stylesheet
    QFile file(appCtx->getConfigPath() + "/qss/" +
               settings->value("current_Style_sheet","Default").toString() + ".qss");

    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
    //load Stylesheet end
    settings->endGroup();
	settings->sync();
    delete settings;
    */
}

void form_MainWindow::initTryIconMenu()
{
    // Tray icon Menu
    menu = new QMenu(this);
    QObject::connect(menu, SIGNAL(aboutToShow()), this, SLOT(updateMenu()));
    toggleVisibilityAction =
            menu->addAction(QIcon(ICON_CHAT), tr("Show/Hide"), this, SLOT(toggleVisibilitycontextmenu()));

    toggleMuteAction=
            menu->addAction(QIcon(ICON_SOUND_ON), tr("Sound on"),this,SLOT(muteSound()));
    menu->addSeparator();
    //menu->addAction(QIcon(ICON_MINIMIZE), tr("Minimize"), this, SLOT(showMinimized()));
    //menu->addAction(QIcon(ICON_MAXIMIZE), tr("Maximize"), this, SLOT(showMaximized()));
    menu->addSeparator();
    menu->addAction(QIcon(ICON_CLOSE), tr("&Quit"), this, SLOT(closeApplication()));



}

void form_MainWindow::initTryIcon()
{
    // Create the tray icon
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setToolTip(tr("I2P-Messenger"));
    trayIcon->setContextMenu(menu);
    trayIcon->setIcon(QIcon(ICON_CHAT));
    
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(toggleVisibility(QSystemTrayIcon::ActivationReason)));

    connect(trayIcon, SIGNAL(activated( QSystemTrayIcon::ActivationReason )),this,
            SLOT(eventTryIconDoubleClicked(enum QSystemTrayIcon::ActivationReason)));

    trayIcon->show();
}

void form_MainWindow::SendFile()
{
    QListWidgetItem *t=listWidget->item(listWidget->currentRow()+1);
    QVariant mapIdVariant = t->data(RosterController::RosterEntryModelRole);
    QString rosterEntryModelMapId = qvariant_cast<QString>(mapIdVariant);
    AbstractRosterEntry *rosterEntry = appCtx->getRosterModel()[rosterEntryModelMapId];
    if(!rosterEntry) {
        QMessageBox::critical(this,tr("Error: no rosterEntryModel found from variant"),tr("Error: no rosterEntryModel found from variant for ")+rosterEntryModelMapId);
        return;
    }

    OneToOneRosterEntry * actorRosterEntry = rosterEntry->asOneToOne();
    if(!actorRosterEntry)return;
    AbstractUserLocalImage* AbstractUserLocalImage = &(actorRosterEntry->getUser());
    QString Destination = AbstractUserLocalImage->getI2PDestination();
    QString FilePath=QFileDialog::getOpenFileName(this,tr("Select a file"), ".", tr("Any files (*)"));

    if(FilePath.endsWith("/")==true){
        //only a directory ,- dont send it
        return;//TODO send folders
    }

    if(Destination.length()==516){
        if(!FilePath.isEmpty())
            appCtx->getFileTransferManager()->addNewFileTransfer(FilePath,Destination);
    }
    else{
        qWarning()<<"\nform_MainWindow::SendFile()\n"
                 <<"Destination.legth!=516\n"
                <<"Destination: "<<Destination<<"\n"
               <<"\naction aported";
    }

}

void form_MainWindow::copyDestination()
{
    QListWidgetItem *t=listWidget->item(listWidget->currentRow()+1);
    QVariant mapIdVariant = t->data(RosterController::RosterEntryModelRole);
    QString rosterEntryModelMapId = qvariant_cast<QString>(mapIdVariant);
    AbstractRosterEntry *rosterEntry = appCtx->getRosterModel()[rosterEntryModelMapId];
    if(!rosterEntry) {
        QMessageBox::critical(this,tr("Error: no rosterEntryModel found from variant"),tr("Error: no rosterEntryModel found from variant for ")+rosterEntryModelMapId);
        return;
    }

    OneToOneRosterEntry * actorRosterEntry = rosterEntry->asOneToOne();
    if(!actorRosterEntry)return;
    AbstractUserLocalImage* AbstractUserLocalImage = &(actorRosterEntry->getUser());
    QString Destination = AbstractUserLocalImage->getI2PDestination();

    QClipboard *clipboard = QApplication::clipboard();

    clipboard->setText(Destination);
    QMessageBox::information(this, "",
                             tr("The Destination is in the clipboard"),QMessageBox::Close);

}

void form_MainWindow::muteSound()
{
    if(this->Mute==false)
    {
        toggleMuteAction->setIcon(QIcon(ICON_SOUND_OFF));
        toggleMuteAction->setText(tr("Sound off"));
        Mute=true;
    }
    else
    {
        toggleMuteAction->setIcon(QIcon(ICON_SOUND_ON));
        toggleMuteAction->setText(tr("Sound on"));
        Mute=false;

    }
    appCtx->getSoundManager()->doMute(Mute);
}

QString form_MainWindow::getCurrentUserDest() {
    QListWidgetItem *t=listWidget->item(listWidget->currentRow()+1);
    QVariant mapIdVariant = t->data(RosterController::RosterEntryModelRole);
    QString rosterEntryModelMapId = qvariant_cast<QString>(mapIdVariant);
    AbstractRosterEntry *rosterEntry = appCtx->getRosterModel()[rosterEntryModelMapId];
    if(!rosterEntry) {
        QMessageBox::critical(this,tr("Error: no rosterEntryModel found from variant"),tr("Error: no rosterEntryModel found from variant for ")+rosterEntryModelMapId);
        return "";
    }

    OneToOneRosterEntry * actorRosterEntry = rosterEntry->asOneToOne();
    if(!actorRosterEntry)return "";
    AbstractUserLocalImage* AbstractUserLocalImage = &(actorRosterEntry->getUser());
    return AbstractUserLocalImage->getI2PDestination();
}

void form_MainWindow::showUserInfos()
{
    QString Destination = getCurrentUserDest();
    if(Destination.isEmpty())return;
    QString UserInfos;
    QPixmap avatar;
    AbstractUserLocalImage*  user;

    user=appCtx->getUserManager()->getUserByI2P_Destination(Destination);
    UserInfos=appCtx->getUserManager()->getUserInfosByI2P_Destination(Destination);

    avatar.loadFromData(user->getRecivedUserInfos().AvatarImage);

    QMessageBox msgBox;
    if(avatar.isNull()==true){
        msgBox.setIcon(QMessageBox::Information);
    }else{
        msgBox.setIconPixmap(avatar);
    }
    msgBox.setText("");
    msgBox.setInformativeText(UserInfos);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setWindowModality(Qt::NonModal);
    msgBox.exec();
}

void form_MainWindow::UserPositionUP()
{
    QListWidget* listWidget=this->listWidget;
    if(listWidget->currentRow() >= 1)
        appCtx->getUserManager()->changeUserPositionInUserList(listWidget->currentRow()/3,listWidget->currentRow()/3 -1);
}

void form_MainWindow::UserPositionDOWN()
{
    QListWidget* listWidget=this->listWidget;
    if(listWidget->currentRow() < (listWidget->count()/3) -1)
        appCtx->getUserManager()->changeUserPositionInUserList(listWidget->currentRow()/3,listWidget->currentRow()/3 +1);

}

void form_MainWindow::UserInvisible(bool b)
{
    QString Destination = getCurrentUserDest();

    AbstractUserLocalImage* AbstractUserLocalImage;
    AbstractUserLocalImage=appCtx->getUserManager()->getUserByI2P_Destination(Destination);
    if(AbstractUserLocalImage!=NULL){
        AbstractUserLocalImage->setInvisible(b);
    }
}

void form_MainWindow::eventChatWindowClosed(QString rosterEntryMapId)
{
    if(mAllOpenChatWindows.contains(rosterEntryMapId)==true){
        delete (mAllOpenChatWindows.value(rosterEntryMapId));
        mAllOpenChatWindows.remove(rosterEntryMapId);
    }
    else{
        qCritical()<<"form_MainWindow::eventChatWindowClosed\n"
                  <<"Closing a unknown ChatWindow";
    }

}

void form_MainWindow::eventTryIconDoubleClicked(enum QSystemTrayIcon::ActivationReason /*Reason*/)
{
    if(!mLastRosterEntryMapIdWithUnreadMessages.isEmpty()){
        openChatWindow(mLastRosterEntryMapIdWithUnreadMessages);
    }
}

void form_MainWindow::openChatWindow(QString rosterEntryMapId)
{
    AbstractRosterEntry *rosterEntry = appCtx->getRosterModel()[rosterEntryMapId];
    if(!rosterEntry) {
        QMessageBox::critical(this,tr("Error: no rosterEntryModel found from variant"),tr("Error: no rosterEntryModel found from variant for ")+rosterEntryMapId);
        return;
    }
    {
        OneToOneRosterEntry * actorRosterEntry = rosterEntry->asOneToOne();
        if(actorRosterEntry){
            AbstractUserLocalImage* AbstractUserLocalImage = &(actorRosterEntry->getUser());
            QString Destination = AbstractUserLocalImage->getI2PDestination();
            if(AbstractUserLocalImage==NULL){
                qCritical()<<"form_MainWindow::openChatWindow"<<"try to open a Chatwindow, but the user doesn't exist";
                return;
            }

            QString mapId=actorRosterEntry->getMapIdQString();
            if(mAllOpenChatWindows.contains(mapId)==false){
                //create new chatWindow
                form_ChatWidget* tmp= new form_ChatWidget(*AbstractUserLocalImage,*actorRosterEntry,*appCtx);
                connect(this,SIGNAL(closeAllWindows()),tmp,
                        SLOT(close()));

                connect(tmp,SIGNAL(closingChatWindow(QString)),this,
                        SLOT(eventChatWindowClosed(QString)));

                connect(appCtx,SIGNAL(signOwnAvatarImageChanged()),tmp,
                        SLOT(slotLoadOwnAvatarImage()));

                mAllOpenChatWindows.insert(mapId,tmp);
                tmp->show();
            } else {
                //open the existing chatwindow
                mAllOpenChatWindows.value(mapId)->getFocus();
            }
        }
    }
    {
        SwarmType1RosterEntry * swarmType1RosterEntry = rosterEntry->asSwarmType1();
        if(swarmType1RosterEntry){
            QString mapId=swarmType1RosterEntry->getMapIdQString();
            if(mAllOpenChatWindows.contains(mapId)==false){
                //create new chatWindow
                form_ChatWidget* tmp= new form_ChatWidget(*swarmType1RosterEntry,*appCtx);
                connect(this,SIGNAL(closeAllWindows()),tmp,
                        SLOT(close()));

                connect(tmp,SIGNAL(closingChatWindow(QString)),this,
                        SLOT(eventChatWindowClosed(QString)));

                //connect(appCtx,SIGNAL(signOwnAvatarImageChanged()),tmp,
                //        SLOT(slotLoadOwnAvatarImage()));

                mAllOpenChatWindows.insert(mapId,tmp);
                tmp->show();
            } else {
                //open the existing chatwindow
                mAllOpenChatWindows.value(mapId)->getFocus();
            }
        }
    }
}

void form_MainWindow::eventFileReciveWindowClosed(qint32 StreamID)
{
    if(mAllFileReciveWindows.contains(StreamID)==true){
        delete (mAllFileReciveWindows.value(StreamID));
        mAllFileReciveWindows.remove(StreamID);
    }
    else{
        qCritical()<<"form_MainWindow::eventFileReciveWindowClose\n"
                  <<"Closing a unknown FileReciveWindow";
    }
}

void form_MainWindow::eventFileSendWindowClosed(qint32 StreamID)
{
    if(mAllFileSendWindows.contains(StreamID)==true){
        delete mAllFileSendWindows.value(StreamID);
        mAllFileSendWindows.remove(StreamID);
    }
    else{
        qCritical()<<"form_MainWindow::eventFileSendWindowClosed\n"
                  <<"Closing a unknown FileSendWindow";
    }
}

void form_MainWindow::openFileSendWindow(qint32 StreamID)
{
    CFileTransferSend* TransferSend=appCtx->getFileTransferManager()->getFileTransferSendsByID(StreamID);


    if(TransferSend==NULL){
        qCritical()<<"form_MainWindow::openFileSendWindow\n"<<"Can't find FileSend Object with ID: "<<StreamID<<"\nAction Ignored";
        return;
    }

    if(mAllFileSendWindows.contains(StreamID)==false){
        //open new FileSendWindow
        form_fileSend* Dialog= new form_fileSend(*TransferSend);
        connect(Dialog,SIGNAL(closingFileSendWindow(qint32)),this,
                SLOT(eventFileSendWindowClosed(qint32)));

        mAllFileSendWindows.insert(StreamID,Dialog);
        Dialog->show();

    }
    else{
        mAllFileSendWindows.value(StreamID)->getFocus();
    }

}

void form_MainWindow::openFileReciveWindow(qint32 StreamID)
{
    CFileTransferRecive* recive=appCtx->getFileTransferManager()->getFileTransferReciveByID(StreamID);
    if(recive==NULL){
        qCritical()<<"form_MainWindow::openFileReciveWindow\n"<<"Can't find FileRecive Object with ID: "<<StreamID<<"\nAction Ignored";
        return;
    }

    if(mAllFileReciveWindows.contains(StreamID)==false){
        //create new FileReciveWindow
        form_fileRecive* Dialog= new form_fileRecive(*recive);

        connect(Dialog,SIGNAL(closingFileReciveWindow(qint32)),this,
                SLOT(eventFileReciveWindowClosed(qint32)));

        mAllFileReciveWindows.insert(StreamID,Dialog);
        Dialog->show();
        Dialog->start();

    }
    else{
        //open the existing FileReciveWindow
        mAllFileReciveWindows.value(StreamID)->getFocus();
    }
}

void form_MainWindow::addUserToBlockList()
{	
    QListWidgetItem* t=listWidget->item(listWidget->currentRow()+2);

    if(t->text()=="A"){
        QListWidgetItem *t=listWidget->item(listWidget->currentRow()+1);
        QVariant mapIdVariant = t->data(RosterController::RosterEntryModelRole);
        QString rosterEntryModelMapId = qvariant_cast<QString>(mapIdVariant);
        AbstractRosterEntry *rosterEntry = appCtx->getRosterModel()[rosterEntryModelMapId];
        if(!rosterEntry) {
            QMessageBox::critical(this,tr("Error: no rosterEntryModel found from variant"),tr("Error: no rosterEntryModel found from variant for ")+rosterEntryModelMapId);
            return;
        }

        OneToOneRosterEntry * actorRosterEntry = rosterEntry->asOneToOne();
        if(!actorRosterEntry)return;
        AbstractUserLocalImage* AbstractUserLocalImage = &(actorRosterEntry->getUser());
        QString Destination = AbstractUserLocalImage->getI2PDestination();

        UserBlockManager* BlockManager= appCtx->getUserBlockManager();

        if(BlockManager){
            BlockManager->addNewBlockEntity(AbstractUserLocalImage->getName(),Destination);
        }
    }
}
/*
void form_MainWindow::openUserSearchWindow()
{
    ONLINE_STATE currentState=appCtx->getOnlineStatus();

    if(currentState==USEROFFLINE || currentState==USERTRYTOCONNECT){
        QMessageBox::information(this, "",
                                 tr("Your Client must be Online for that"),QMessageBox::Close);
        return;
    }

    if(mUserSearchWindow==NULL){

        mUserSearchWindow= new form_userSearch(*appCtx,*(appCtx->getSeedlessManager()));
        connect(this,SIGNAL(closeAllWindows()),mUserSearchWindow,
                SLOT(close()));

        connect(mUserSearchWindow,SIGNAL(signClosingUserSearchWindow()),this,
                SLOT(eventUserSearchWindowClosed()));
        mUserSearchWindow->show();
    }else {
        mUserSearchWindow->getFocus();
    }
}
*/
void form_MainWindow::eventUserSearchWindowClosed()
{
    delete mUserSearchWindow;
    mUserSearchWindow=NULL;
}

void form_MainWindow::eventTopicSubscribeWindowClosed()
{
    delete mTopicSubscribeWindow;
    mTopicSubscribeWindow=NULL;
}

void form_MainWindow::eventDialogCreateNewSwarmClosed()
{
    delete mDialogCreatNewSwarm;
    mDialogCreatNewSwarm=NULL;
}

void form_MainWindow::eventAboutWindowClosed()
{
    delete mAboutWindow;
    mAboutWindow=NULL;
}
void form_MainWindow::eventDebugWindowClosed()
{
    delete mDebugWindow;
    mDebugWindow=NULL;
}

void form_MainWindow::eventNicknameChanged()
{
    nicknamelabel->setText(appCtx->getUserInfos().Nickname);
}

void form_MainWindow::eventAvatarImageChanged()
{
    if(appCtx->getUserInfos().AvatarImage.isEmpty()==false){
        QPixmap avatar;
        avatar.loadFromData(appCtx->getUserInfos().AvatarImage);
        avatar=avatar.scaled(avatarlabel->width(),avatarlabel->height(),Qt::KeepAspectRatio);
        avatarlabel->setAlignment(Qt::AlignCenter);
        avatarlabel->setPixmap(avatar);
    }
}

void form_MainWindow::openTopicSubscribeWindow() {
    ONLINE_STATE currentState=appCtx->getOnlineStatus();

    if(currentState==USEROFFLINE || currentState==USERTRYTOCONNECT) {
        QMessageBox::information(this, "",
                                 tr("You must be connected for this"), QMessageBox::Close);
        return;
    }

    if(mTopicSubscribeWindow == NULL) {

        mTopicSubscribeWindow = new form_topicSubscribe(*appCtx);

        connect(this, SIGNAL(closeAllWindows()), mTopicSubscribeWindow,
                SLOT(close()));

        connect(mTopicSubscribeWindow, SIGNAL(signClosingTopicSubscribeWindow()), this,
                SLOT(eventTopicSubscribeWindowClosed()));
        mTopicSubscribeWindow->show();
    } else {
        mTopicSubscribeWindow->requestFocus();
    }

}

void form_MainWindow::openDialog_creatNewSwarm() {
    if(mDialogCreatNewSwarm == NULL) {

        mDialogCreatNewSwarm = new form_createNewSwarmType1(*appCtx);

        connect(this, SIGNAL(closeAllWindows()), mDialogCreatNewSwarm,
                SLOT(close()));

        connect(mDialogCreatNewSwarm, SIGNAL(signClosingWindow()), this,
                SLOT(eventDialogCreateNewSwarmClosed()));
        mDialogCreatNewSwarm->show();
    } else {
    	mDialogCreatNewSwarm->requestFocus();
    }

}
