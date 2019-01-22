/***************************************************************************
 *   Copyright © 2008...2017 by I2P Messenger team			   		   	   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef AppContext_H
#define AppContext_H

#include <memory>

#include "DebugMessageManager.h"
#include "ConnectionManager.h"
#include "UserBlockManager.h"
#include "Connector.h"
#include "SoundManager.h"
class UserManager;
#include "FileTransferManager.h"
#include "Roster.h"
#include "RosterController.h"
#include "SwarmsController.h"

using namespace std;

class RosterController;

class AppContext : public std::enable_shared_from_this<AppContext> {
public:
    AppContext(){};
    virtual ~AppContext(){};

    //forbid some operators
    AppContext(const AppContext&)=delete;
    AppContext& operator=(const AppContext&)=delete;

    virtual QString 		getClientName()=0;
    virtual QString 		getClientVersion()=0;

    virtual shared_ptr<DebugMessageManager> 	getDebugMessageHandler()=0;
    virtual shared_ptr<ConnectionManager>	    getConnectionManager()=0;
    virtual shared_ptr<UserBlockManager>       	getUserBlockManager()=0;
    virtual shared_ptr<Connector>              	getProtocolConnector()=0;
    virtual shared_ptr<SoundManager>           	getSoundManager()=0;
    virtual shared_ptr<UserManager>            	getUserManager()=0;
    virtual shared_ptr<FileTransferManager>		getFileTransferManager()=0;
    
    //model
    virtual shared_ptr<Roster> 					getRoster()=0;
    virtual shared_ptr<RosterController> 		getRosterController()=0;
    virtual shared_ptr<SwarmsController>		getSwarmsManager()=0;
};
#endif //AppContext_H
