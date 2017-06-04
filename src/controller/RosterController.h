#ifndef ROSTERCONTROLLER_H
#define ROSTERCONTROLLER_H

#include <QObject>
#include <QString>
#include <QListWidgetItem>

#include "AbstractRosterEntry.h"
#include "AppContext.h"
#include "OneToOneRosterEntry.h"
#include "SwarmType1LocalImage.h"
#include "SwarmType1RosterEntry.h"
#include "AbstractUserLocalImage.h"
#include "abstractrosterentryview.h"
#include "gui_icons.h"

class Roster;

class RosterController
{
public:
    RosterController(shared_ptr<Roster> rosterModel);

    enum USER_DATA_ROLES {
        RosterEntryModelRole = Qt::UserRole
    };

    void removeActorByI2PDestination(QString I2PDestination);

    QString getDisplayNameQStringForRosterEntry(shared_ptr<AbstractRosterEntry> rosterEntryModel) {
        {
            shared_ptr<OneToOneRosterEntry> o2o = rosterEntryModel->asOneToOne();
            if(o2o) {
                const QString nick = o2o->getRemoteUser()->getNickName();
                return nick.trimmed();
            }
        }
        {
            shared_ptr<SwarmType1RosterEntry> swarm = rosterEntryModel->asSwarmType1();
            if(swarm) {
                QString nick = swarm->getSwarm()->getLocalSwarmNick();
                return nick.trimmed();
            }
        }
        return QObject::tr("Unknown type of roster entry");
    }

    void shapeRosterEntryGUI(shared_ptr<AbstractRosterEntry> rosterEntryModel,
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
                shared_ptr<OneToOneRosterEntry> o2o = rosterEntryModel->asOneToOne();
                if(o2o) {
                    switch(o2o->getRemoteUser()->getOnlineStatus())
                    {

                    case USERUNKNOWNONLINESTATUS:
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
                    }
                }
            }
        }

        newItem->setTextAlignment(Qt::AlignLeft);
        //QFont currentFont=newItem->font();
        newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        /*
        auto o2o = rosterEntryModel->asOneToOne();
        if(o2o && o2o->getLocalUser()->isInvisible())	{
            currentFont.setItalic(true);
            newItem->setFont(currentFont);
        } else {
            currentFont.setItalic(false);
            newItem->setFont(currentFont);
        }
        */

        newItem->setText(getDisplayNameQStringForRosterEntry(rosterEntryModel));

        ChildWidthUserData->setHidden(true);
        ChildWidthUserData->setData(RosterEntryModelRole, rosterEntryModel->getMapIdQString());
        ChildWidthTyp->setText("A");
        ChildWidthTyp->setHidden(true);
    }


    //void attachEntryViewToModel(AbstractRosterEntry * model, AbstractRosterEntryView * view);

    Roster & getRosterModel();

    QStringList getNewMessages(shared_ptr<AbstractGroupRosterEntry> group);


private:
    Roster & rosterModel;
    shared_ptr<AppContext> appCtx;
};

#endif // ROSTERCONTROLLER_H
