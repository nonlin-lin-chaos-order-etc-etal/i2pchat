/*
 * SwarmType1WhiteList.h
 *
 *  Created on: Jan 10, 2017
 *      Author: user
 */

#ifndef SRC_BACKEND_SWARMTYPE1WHITELIST_H_
#define SRC_BACKEND_SWARMTYPE1WHITELIST_H_

#include <QMap>
#include <QString>

class QTextStream;
class CUser;
class CUserManager;
class CUnsentChatMessageStorage;

class SwarmType1WhiteList {
public:
	SwarmType1WhiteList();
	virtual ~SwarmType1WhiteList();

    void saveToQTextStreamForRosterConf(QTextStream& out);
    static SwarmType1WhiteList* loadFromRosterConf(
            QTextStream&in, int WhiteListLength, CUserManager& userManager, CUnsentChatMessageStorage& mUnsentMessageStorage);

    void addUser(CUser&user);
    void removeUser(CUser&user);

    //dealloc by `delete result`
    QList<CUser*>* getUsersSetClone();

private:
    QMap<QString,CUser*> i2pdestqstr2cuser;
};

#endif /* SRC_BACKEND_SWARMTYPE1WHITELIST_H_ */
