/*
 * SwarmType1FactoryMethods.h
 *
 *  Created on: Jan 10, 2017
 *      Author: user
 */

#ifndef SRC_BACKEND_SWARMTYPE1FACTORYMETHODS_H_
#define SRC_BACKEND_SWARMTYPE1FACTORYMETHODS_H_

#include <SwarmType1.h>
#include "Core.h"
#include "QString"
#include "QFile"
#include "QDir"

class SwarmType1FactoryMethods {
public:
	//SwarmType1FactoryMethods();
	//virtual ~SwarmType1FactoryMethods();

    static SwarmType1* createSwarmType1(SwarmType1LocalSwarmNickType localSwarmNick, CCore& chatCore,
			bool& successBool, QString& errorMessage, QString& swarmIdFolderFullPathStr,
            SwarmType1LocalIdType & failedId, SwarmType1WhiteList* whiteList,
            SwarmType1LocalIdType idToLoad, bool useIdToLoad);

private:
	SwarmType1FactoryMethods(){}
	static SwarmType1LocalIdType detectAndOccupyANewLocalSwarmId(CCore& chatCore,
            bool& successBool, QString& errorMessage, QString& swarmIdFolderFullPathStr,
            SwarmType1LocalIdType idToLoad, bool useIdToLoad);
};

#endif /* SRC_BACKEND_SWARMTYPE1FACTORYMETHODS_H_ */
