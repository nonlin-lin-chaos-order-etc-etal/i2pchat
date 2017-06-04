/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>

#include "AppContext.h"
#include "Roster.h"
#include "UnsentChatMessagesStorage.h"
#include "LocalUserLocalImage.h"

#include <memory>

using namespace std;

class UserManager : public std::enable_shared_from_this<UserManager> {

      public:
                                UserManager(shared_ptr<AppContext> appCtx,QString UserFileWithPath,
                                             shared_ptr<UnsentChatMessagesStorage> UnsentChatMessageStorage);
                                virtual ~UserManager();

                                //forbid some operators
                                UserManager(const UserManager&)=delete;
                                UserManager& operator=(const UserManager&)=delete;

                    virtual shared_ptr<LocalUserLocalImage> getLocalUser()=0;

                    void 		saveUserList()const;
                    void 		loadUserList();
                    bool 		addNewUser (
                    QString Name,QString I2PDestination,qint32 I2PStream_ID=0,bool SaveUserList=true,
                    AbstractUserLocalImage**resultingNewUser = nullptr, bool isSwarmUser = false);
                    bool 		checkIfUserExitsByI2PDestination ( QString I2PDestination )const;
                    void 		changeUserPositionInUserList(int oldPos,int newPos);
                    bool 		renameUserByI2PDestination (const QString Destination, const QString newNickname);
                    bool 		deleteUserByI2PDestination (const QString I2PDestination );
                    void		avatarImageChanged();
		
		
                    shared_ptr<AbstractUserLocalImage> 		getUserByI2P_ID ( qint32 ID )const;
                    shared_ptr<AbstractUserLocalImage> 		getUserByI2P_Destination ( QString Destination )const;
                    QString		getUserInfosByI2P_Destination(QString Destination)const;
        const       QList<shared_ptr<AbstractUserLocalImage>> 	getUserList()const;
                    bool 		validateI2PDestination(const QString I2PDestination)const;

	
                    void 		signUserStatusChanged();

                    void		slotSaveUnsentMessageForDest(QString I2PDest);
  
		
      private:
        shared_ptr<AppContext>	      	mCore;
        const QString 	mUserFileWithPath;
        shared_ptr<UnsentChatMessagesStorage> mUnsentMessageStorage;
        QList<shared_ptr<AbstractUserLocalImage>> 	mUsers;
};
#endif // USERMANAGER_H
