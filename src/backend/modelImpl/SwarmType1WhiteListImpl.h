/*
 * SwarmType1WhiteList.h
 *
 *  Created on: Jan 10, 2017
 *      Author: user
 */

#ifndef SRC_BACKEND_SWARMTYPE1WHITELIST_IMPL_H_
#define SRC_BACKEND_SWARMTYPE1WHITELIST_IMPL_H_

#include <QMap>
#include <QString>

class QTextStream;
class AbstractUserLocalImage;
class UserManager;
class UnsentChatMessagesStorage;

class SwarmType1WhiteList {
public:
	SwarmType1WhiteList();
	virtual ~SwarmType1WhiteList();

    void saveToQTextStreamForRosterConf(QTextStream& out);
    static SwarmType1WhiteList* loadFromRosterConf(
            QTextStream&in, int WhiteListLength, UserManager& userManager, UnsentChatMessagesStorage& mUnsentMessageStorage);

    void addUser(AbstractUserLocalImage&user);
    void removeUser(AbstractUserLocalImage&user);

    //dealloc by `deallocUsersSetClone(QList<AbstractUserLocalImage*>*)`
    QList<AbstractUserLocalImage*>* getUsersSetClone();
    void deallocUsersSetClone(QList<AbstractUserLocalImage*>* clone){ delete clone; }

private:
    QMap<QString,AbstractUserLocalImage*> i2pdestqstr2cuser;
};

#endif /* SRC_BACKEND_SWARMTYPE1WHITELIST_IMPL_H_ */
