/*
 * SwarmType1.h
 *
 *  Created on: Jan 10, 2017
 *      Author: user
 */

#ifndef SRC_BACKEND_SWARMTYPE1_H_
#define SRC_BACKEND_SWARMTYPE1_H_

#include <QString>
#include "SwarmType1WhiteList.h"

class CCore;

typedef unsigned int SwarmType1LocalIdType;

typedef QString SwarmType1LocalSwarmNickType;

class SwarmType1 {
public:
    SwarmType1(SwarmType1LocalIdType localId, SwarmType1LocalSwarmNickType localSwarmNick, QString& swarmIdFolderFullPathStr,SwarmType1WhiteList* whitelist_);
	virtual ~SwarmType1();

    SwarmType1WhiteList& getWhiteList() { return *whitelist; }

	SwarmType1LocalIdType getLocalId() { return localId; }

	SwarmType1LocalSwarmNickType& getLocalSwarmNick() { return localSwarmNick; }
	void setLocalSwarmNick(SwarmType1LocalSwarmNickType newLocalSwarmNick) { localSwarmNick=newLocalSwarmNick; }

	QString& getSwarmFolderFullPathStr() { return swarmIdFolderFullPathStr; }

    static SwarmType1* loadSwarm(QString localSwarmLabel, QString localId, SwarmType1WhiteList* whiteList, CCore& chatCore);

private:
    SwarmType1WhiteList* whitelist;
	SwarmType1LocalIdType localId;
	SwarmType1LocalSwarmNickType localSwarmNick;
	QString swarmIdFolderFullPathStr;
};

#endif /* SRC_BACKEND_SWARMTYPE1_H_ */
