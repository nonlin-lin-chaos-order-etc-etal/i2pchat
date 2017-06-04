#ifndef REMOTEUSERLOCALIMAGE_H
#define REMOTEUSERLOCALIMAGE_H

#include "RemoteUserOnlineStatus.h"

class RemoteUserLocalImage : public AbstractUserLocalImage
{
public:
    RemoteUserLocalImage();
    virtual ~RemoteUserLocalImage(){}
    virtual const QString getNickName()=0;
    virtual void setNickName(QString newNickName)=0;

    virtual RemoteUserOnlineStatus getOnlineStatus()=0;
    virtual void setOnlineStatus(RemoteUserOnlineStatus newStatus)=0;
};

#endif // REMOTEUSERLOCALIMAGE_H
