/*
 * SwarmType1LocalImage.cpp
 *
 *  Created on: Jan 10, 2017
 *      Author: user
 */

#include "SwarmType1LocalImage.h"

#include "SwarmType1FactoryMethods.h"

SwarmType1LocalImage::SwarmType1LocalImage(SwarmType1LocalIdType localId_, SwarmType1LocalSwarmNickType localSwarmNick_,
        QString& swarmIdFolderFullPathStr_,SwarmType1WhiteList* whitelist_) :
    whitelist(whitelist_),
	localId(localId_),
	localSwarmNick(localSwarmNick_),
	swarmIdFolderFullPathStr(swarmIdFolderFullPathStr_) {
}

SwarmType1LocalImage::~SwarmType1LocalImage() {
}

SwarmType1LocalImage* SwarmType1LocalImage::loadSwarm(QString localSwarmLabel, QString localId, SwarmType1WhiteList* whiteList, AppContext& chatCore) {
    bool successBool;
    QString errMsg;
    QString swarmIdFolderFullPathStr;
    SwarmType1LocalIdType failedId;
    return SwarmType1FactoryMethods::createSwarmType1(
                localSwarmLabel, chatCore, successBool, errMsg, swarmIdFolderFullPathStr, failedId, whiteList,
                localId.toInt(), true);
}
