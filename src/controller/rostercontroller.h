#ifndef ROSTERCONTROLLER_H
#define ROSTERCONTROLLER_H

#include <QObject>
#include <QString>
#include <QListWidgetItem>

#include "Core.h"
#include "User.h"
#include "abstractrosterentryview.h"
#include "abstractrosterentry.h"
#include "gui_icons.h"
#include "actorrosterentry.h"
#include "swarmtype1rosterentry.h"
#include "SwarmType1.h"

class RosterModel;

class RosterController
{
public:
    RosterController(RosterModel & rosterModel);

    enum USER_DATA_ROLES {
        RosterEntryModelRole = Qt::UserRole
    };

    void removeActorByI2PDestination(QString I2PDestination);

    QString getDisplayNameQStringForRosterEntry(AbstractRosterEntry * rosterEntryModel) {
        {
            ActorRosterEntry * actor = rosterEntryModel->asActor();
            if(actor) {
                QString nick = actor->getUser().getName();
                return nick.trimmed();
            }
        }
        {
            SwarmType1RosterEntry * swarm = rosterEntryModel->asSwarmType1();
            if(swarm) {
                QString nick = swarm->getSwarm().getLocalSwarmNick();
                return nick.trimmed();
            }
        }
        return QObject::tr("Unknown type of roster entry");
    }

    void shapeRosterEntryGUI(AbstractRosterEntry * rosterEntryModel,
                             QListWidgetItem* newItem,
                             bool & showUnreadMessageAtTray,
                             QString & mLastRosterEntryWithUnreadMessages,
                             QListWidgetItem* ChildWidthUserData,
                             QListWidgetItem* ChildWidthTyp) {
        if(!rosterEntryModel)return;
        if(rosterEntryModel->hasUnreadChatMessages()){
            newItem->setIcon(QIcon(ICON_NEWUNREADMESSAGE));
            showUnreadMessageAtTray=true;
            mLastRosterEntryWithUnreadMessages = rosterEntryModel->getMapIdQString();
        } else {
            if(rosterEntryModel->asGroup()) {
                newItem->setIcon(QIcon(ICON_USERSEARCH));
            }else {
                ActorRosterEntry * actor = rosterEntryModel->asActor();
                if(actor) {
                    switch(actor->getUser().getOnlineState())
                    {

                    case USERTRYTOCONNECT:
                    case USERINVISIBLE:
                    case USEROFFLINE:
                    {
                        newItem->setIcon(QIcon(ICON_USER_OFFLINE));
                        break;
                    }
                    case USERONLINE:
                    {
                        newItem->setIcon(QIcon(ICON_USER_ONLINE));
                        break;
                    }
                    case USERWANTTOCHAT:
                    {
                        newItem->setIcon(QIcon(ICON_USER_WANTTOCHAT));
                        break;
                    }
                    case USERAWAY:
                    {
                        newItem->setIcon(QIcon(ICON_USER_AWAY));
                        break;
                    }
                    case USERDONT_DISTURB:
                    {
                        newItem->setIcon(QIcon(ICON_USER_DONT_DUSTURB));
                        break;
                    }
                    case USERBLOCKEDYOU:
                    {
                        newItem->setIcon(QIcon(ICON_USER_BLOCKED_YOU));
                        break;
                    }
                    }
                }
            }
        }

        newItem->setTextAlignment(Qt::AlignLeft);
        QFont currentFont=newItem->font();
        newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        ActorRosterEntry * actor = rosterEntryModel->asActor();
        if(actor && actor->getUser().getIsInvisible())	{
            currentFont.setItalic(true);
            newItem->setFont(currentFont);
        } else {
            currentFont.setItalic(false);
            newItem->setFont(currentFont);
        }

        newItem->setText(getDisplayNameQStringForRosterEntry(rosterEntryModel));

        ChildWidthUserData->setHidden(true);
        ChildWidthUserData->setData(RosterEntryModelRole, rosterEntryModel->getMapIdQString());
        ChildWidthTyp->setText("A");
        ChildWidthTyp->setHidden(true);
    }


    //void attachEntryViewToModel(AbstractRosterEntry * model, AbstractRosterEntryView * view);

    RosterModel & getRosterModel();

    QStringList getNewMessages(AbstractGroupRosterEntry * group) {
        if(!group)return QStringList();
        return group->pickNewIncomingMessagesAndClear();//FIXME unreliable, can lose business data
    }


private:
    RosterModel & rosterModel;
};

#endif // ROSTERCONTROLLER_H
