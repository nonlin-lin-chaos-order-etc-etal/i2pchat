/*
 * SwarmType1.cpp
 *
 *  Created on: Jan 10, 2017
 *      Author: user
 */

#include "SwarmType1.h"
#include "SwarmType1FactoryMethods.h"

SwarmType1::SwarmType1(SwarmType1LocalIdType localId_, SwarmType1LocalSwarmNickType localSwarmNick_,
        QString& swarmIdFolderFullPathStr_,SwarmType1WhiteList* whitelist_) :
    whitelist(whitelist_),
	localId(localId_),
	localSwarmNick(localSwarmNick_),
	swarmIdFolderFullPathStr(swarmIdFolderFullPathStr_) {
}

SwarmType1::~SwarmType1() {
}

SwarmType1* SwarmType1::loadSwarm(QString localSwarmLabel, QString localId, SwarmType1WhiteList* whiteList, CCore& chatCore) {
    bool successBool;
    QString errMsg;
    QString swarmIdFolderFullPathStr;
    SwarmType1LocalIdType failedId;
    return SwarmType1FactoryMethods::createSwarmType1(
                localSwarmLabel, chatCore, successBool, errMsg, swarmIdFolderFullPathStr, failedId, whiteList,
                localId.toInt(), true);
}
