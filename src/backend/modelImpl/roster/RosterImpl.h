#ifndef ROSTERMODEL_IMPL_H
#define ROSTERMODEL_IMPL_H

#include <QTextStream>
#include <QObject>
#include <QSet>

#include "OneToOneRosterEntry.h"

class UserManager;
class AppContext;
class AbstractRosterEntry;
class UnsentChatMessagesStorage;
class SwarmType1LocalImage;

class Roster : public QObject
{
    Q_OBJECT
public:
    Roster(AppContext& chatCore_, UserManager& userManager_, UnsentChatMessagesStorage& offlineMessagesStorage_,
                         QObject *parent = nullptr);

    void invalidateContents();

signals:

public slots:
private:
    void 		save() const;
    void 		load();

    friend class UserManager;

    //forbid some operators
    Roster(const Roster&)=delete;
    Roster& operator=(const Roster&)=delete;

    AppContext& chatCore;
    UserManager& userManager;
    UnsentChatMessagesStorage& offlineMessagesStorage;

    QMap<QString,AbstractRosterEntry*> entryIdQStrToRosterEntry;

    void loadNewActor(QString& nickName,QString& i2pDest);

    void loadRosterEntry(QStringList& lookaheadTokens, QTextStream& input, parsed_item_file);
public:
    void createNewSwarmType1(SwarmType1LocalImage&swarm);
    AbstractRosterEntry* operator [](QString mapId) { return entryIdQStrToRosterEntry[mapId]; }
private:
    void insertRosterEntry(AbstractRosterEntry* entry);
public:
    //dealloc by `delete result`
    QList<AbstractRosterEntry*>* cloneRosterEntriesSet();
    void removeActorByI2PDestination(QString I2PDestination) {
        for(QMap<QString,AbstractRosterEntry*>::iterator it = entryIdQStrToRosterEntry.begin(); it!=entryIdQStrToRosterEntry.end(); ++it) {
            AbstractRosterEntry* e = *it;
            OneToOneRosterEntry * o2o = e->asOneToOne();
            if(!o2o)continue;
            if(o2o->getRemoteUser()->getI2PDestination()==I2PDestination) {
                entryIdQStrToRosterEntry.remove(o2o->getMapIdQString());
                return;
            }
        }
    }
};

#endif // ROSTERMODEL_IMPL_H
