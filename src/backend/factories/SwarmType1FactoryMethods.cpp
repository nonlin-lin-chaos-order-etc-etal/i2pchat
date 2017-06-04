/*
 * SwarmType1FactoryMethods.cpp
 *
 *  Created on: Jan 10, 2017
 *      Author: user
 */

#include "SwarmType1FactoryMethods.h"

#include <QString>

/*
SwarmType1FactoryMethods::SwarmType1FactoryMethods() {
	// TODO Auto-generated constructor stub

}

SwarmType1FactoryMethods::~SwarmType1FactoryMethods() {
	// TODO Auto-generated destructor stub
}

*/

SwarmType1LocalIdType SwarmType1FactoryMethods::detectAndOccupyANewLocalSwarmId(AppContext& chatCore,
        bool& successBool, QString& errorMessage, QString & swarmIdFolderFullPathStr,
        SwarmType1LocalIdType idToLoad, bool useIdToLoad) {
	QString configPathStr = chatCore.getConfigPath();
	QDir configPathDir(configPathStr);
	QString	swarmsFolderName="swarms";
	configPathDir.mkpath(swarmsFolderName);
	QString	swarmsFolderPathStr = configPathStr+"/"+swarmsFolderName+"/";
	QDir swarmsFolderDir(swarmsFolderPathStr);
	//open folder "swarms"
	//find first unused swarm id and create a idlock file occupying it
	//file system concurrent

    SwarmType1LocalIdType id=useIdToLoad?idToLoad:0;
    while(1) {
        QString idStr = QString::number(id);
        QString fullPath = swarmsFolderPathStr+idStr;
        QFile idFile(fullPath);
        if(!useIdToLoad){
            if(idFile.exists()){
                ++id;
                continue;
            }
            //idlock create
            bool success = swarmsFolderDir.mkdir(idStr); //If the directory already exists when this function is called, it will return false.
            if(!success){
                if(idFile.exists()){
                    ++id;
                    continue;
                }
                successBool=false;
                errorMessage=QObject::tr("Error: swarmsDir.mkdir(idStr) returned failure and the id folder does not exist");
                return id;
            }
        }

        successBool=true;
        swarmIdFolderFullPathStr=fullPath;
        return id;
    }
}

SwarmType1LocalImage* SwarmType1FactoryMethods::createSwarmType1(SwarmType1LocalSwarmNickType localSwarmNick, AppContext& chatCore,
        bool& successBool, QString& errorMessage, QString& swarmIdFolderFullPathStr,
        SwarmType1LocalIdType & failedId, SwarmType1WhiteList* whiteList, SwarmType1LocalIdType idToLoad, bool useIdToLoad){
    SwarmType1LocalIdType id = SwarmType1FactoryMethods::detectAndOccupyANewLocalSwarmId(chatCore,
            successBool, errorMessage, swarmIdFolderFullPathStr, idToLoad, useIdToLoad);
    if(!successBool){failedId=id;return nullptr;}
    SwarmType1LocalImage * swarm = new SwarmType1LocalImage(id, localSwarmNick, swarmIdFolderFullPathStr,whiteList);
    return swarm;
}
