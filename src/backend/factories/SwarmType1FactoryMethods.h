/*
 * SwarmType1FactoryMethods.h
 *
 *  Created on: Jan 10, 2017
 *      Author: user
 */

#ifndef SRC_BACKEND_SWARMTYPE1FACTORYMETHODS_H_
#define SRC_BACKEND_SWARMTYPE1FACTORYMETHODS_H_

#include "AppContext.h"
#include "QString"
#include "QFile"
#include "QDir"
#include "SwarmType1LocalImage.h"

class SwarmType1FactoryMethods {
public:
	//SwarmType1FactoryMethods();
	//virtual ~SwarmType1FactoryMethods();

    static SwarmType1LocalImage* createSwarmType1(SwarmType1LocalSwarmNickType localSwarmNick, AppContext& chatCore,
			bool& successBool, QString& errorMessage, QString& swarmIdFolderFullPathStr,
            SwarmType1LocalIdType & failedId, SwarmType1WhiteList* whiteList,
            SwarmType1LocalIdType idToLoad, bool useIdToLoad);

private:
	SwarmType1FactoryMethods(){}
	static SwarmType1LocalIdType detectAndOccupyANewLocalSwarmId(AppContext& chatCore,
            bool& successBool, QString& errorMessage, QString& swarmIdFolderFullPathStr,
            SwarmType1LocalIdType idToLoad, bool useIdToLoad);
};

#endif /* SRC_BACKEND_SWARMTYPE1FACTORYMETHODS_H_ */
