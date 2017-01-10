#ifndef ROSTERMODEL_H
#define ROSTERMODEL_H

#include <QTextStream>
#include <QObject>
#include <QSet>

class CUserManager;
class CCore;
class AbstractRosterEntry;
class CUnsentChatMessageStorage;
class SwarmType1;

class RosterModel : public QObject
{
    Q_OBJECT
public:
    RosterModel(CCore& chatCore_, CUserManager& userManager_, CUnsentChatMessageStorage& offlineMessagesStorage_,
                         QObject *parent = nullptr);

    void invalidateContents();

signals:

public slots:
private:
    void 		save() const;
    void 		load();

    friend class CUserManager;

    //forbid some operators
    RosterModel(const RosterModel&)=delete;
    RosterModel& operator=(const RosterModel&)=delete;

    CCore& chatCore;
    CUserManager& userManager;
    CUnsentChatMessageStorage& offlineMessagesStorage;

    QMap<QString,AbstractRosterEntry*> entryIdQStrToRosterEntry;

    void loadNewActor(QString& nickName,QString& i2pDest);

    void loadRosterEntry(QStringList& lookaheadTokens, QTextStream& input);
public:
    void createNewSwarmType1(SwarmType1&swarm);

    void insertRosterEntry(AbstractRosterEntry* entry);
};

#endif // ROSTERMODEL_H
