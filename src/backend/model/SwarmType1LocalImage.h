/*
 * SwarmType1LocalImage.h
 *
 *  Created on: Jan 10, 2017
 *      Author: user
 */

#ifndef SWARMTYPE1LOCALIMAGE_H
#define SWARMTYPE1LOCALIMAGE_H

#include <memory>

#include "SwarmType1WhiteList.h"

using namespace std;

typedef unsigned int SwarmType1LocalIdType;

typedef QString SwarmType1LocalSwarmNickType;

class SwarmType1LocalImage : public std::enable_shared_from_this<SwarmType1LocalImage> {
public:
    SwarmType1LocalImage(){};
	virtual ~SwarmType1LocalImage(){};

    virtual shared_ptr<SwarmType1WhiteList> getWhiteList()=0;
    virtual shared_ptr<SwarmType1WhiteList> getReverseWhiteList()=0;

    virtual SwarmType1LocalIdType getLocalId()=0;

    virtual SwarmType1LocalSwarmNickType getLocalSwarmNick()=0;
    virtual void setLocalSwarmNick(SwarmType1LocalSwarmNickType newLocalSwarmNick)=0;

    virtual QString& getSwarmFolderFullPathStr()=0;

    /*
    //FIXME unreliable, can lose business data
    QStringList pickNewIncomingMessagesAndClear(shared_ptr<AbstractGroupRosterEntry> group) {
        if(!group)return QStringList();
        auto swarmRE = group->asSwarmType1();
        if(!swarmRE)return QStringList();
        auto swarm = swarmRE->getSwarm();
        return swarm->pickNewIncomingMessagesAndClear(); //FIXME unreliable, can lose business data
    }
    */
    virtual QStringList pickNewIncomingMessagesAndClear()=0;
};

#endif /* SWARMTYPE1LOCALIMAGE_H */
