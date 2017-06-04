#include "SwarmsController.h"

SwarmsController::SwarmsController(QObject *parent) : QObject(parent)
{

}

void SwarmsController::transferMessageToSwarmWhiteListUser(SwarmType1LocalImage& swarm, QString& message,WhiteListUser whiteListUser) {
    using namespace PROTOCOL_TAGS;
    QString Nickname;

    if(mConnectionStatus==ONLINE &&
        mCurrentOnlineState != USEROFFLINE &&
        mCurrentOnlineState != USERINVISIBLE
    ){
        bool dataWasTruncatedTo0xffffMinus4Bool;
        if (getProtocolVersion_D() < (double)0.7) {//FIXME get rid of such code
            message=QString("At swarm ")+swarm.getLocalSwarmNick()+": "+message;
            QByteArray ByteMessage=message.toUtf8();
            mProtocol.send(CHATMESSAGE,mI2PStream_ID,ByteMessage,dataWasTruncatedTo0xffffMinus4Bool);
        }else{
            message=QString("SWARM_LOCAL_ID_FROM\t")+QString::number(swarm.getLocalId())+"\n"+message;
            QByteArray ByteMessage=message.toUtf8();
            mProtocol.send(CHAT_MESSAGE_SWARM,mI2PStream_ID,ByteMessage,dataWasTruncatedTo0xffffMinus4Bool);
        }
        if(dataWasTruncatedTo0xffffMinus4Bool){
            QMessageBox::warning(0, QObject::tr("Warning: Data was truncated"),QObject::tr("Data was truncated to 0xffff-4 bytes."));//FIXME
        }

        if(mCore.getUserInfos().Nickname.isEmpty()==true){
          Nickname=tr("AbstractUserLocalImage");
        }else{
          Nickname=mCore.getUserInfos().Nickname;
        }

        this->mAllMessages.push_back(Nickname+" ("+QTime::currentTime().toString("hh:mm:ss")  +"): "+Message+"<br>");
        this->mNewMessages.push_back(Nickname+" ("+QTime::currentTime().toString("hh:mm:ss")  +"): "+Message+"<br>");

        mHaveNewUnreadMessages=true;
        emit signNewMessageRecived();
    }
    else{
        //FIXME mUnsentedMessages.push_back(Message+"<br>");
        //slotIncomingMessageFromSystem(tr("Sending the message when the user comes online.<br>If you close the client, the message will be lost."));
    }
}

