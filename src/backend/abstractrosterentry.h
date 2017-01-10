#ifndef ABSTRACTROSTERENTRY_H
#define ABSTRACTROSTERENTRY_H

#include <QTextStream>
#include <QString>

class CUnsentChatMessageStorage;

class AbstractRosterEntry
{
public:
    AbstractRosterEntry();
    virtual ~AbstractRosterEntry();

    virtual QString getClassTag()=0;
    virtual QString getMapIdQString()=0;
    virtual void save(QTextStream& out, CUnsentChatMessageStorage& offlineMessagesStorage)=0;
};

#endif // ABSTRACTROSTERENTRY_H
