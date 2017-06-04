#include "AbstractGroupRosterEntry.h"

#include "AppContext.h"

AbstractGroupRosterEntry::AbstractGroupRosterEntry(AppContext& chatCore):textFont(nullptr),textColor(nullptr) {
    QSettings settings(chatCore.getConfigPath()+"/application.ini",QSettings::IniFormat);

    settings.beginGroup("Chat");
        QFont f;
        f.fromString(settings.value("DefaultFont","Comic Sans MS,10").toString());
        setTextFont(f);
        QColor c;
        c.setNamedColor(settings.value("DefaultColor","#000000").toString());
        setTextColor(c);
    settings.endGroup();
    settings.sync();
}

AbstractGroupRosterEntry::~AbstractGroupRosterEntry() {
    if(textFont){delete textFont;textFont=nullptr;}
    if(textColor){delete textColor;textColor=nullptr;}
}

QFont& AbstractGroupRosterEntry::getTextFont() { return *textFont; }
QColor& AbstractGroupRosterEntry::getTextColor() { return *textColor; }
void AbstractGroupRosterEntry::setTextFont(QFont f)  {
    if(textFont){delete textFont;textFont=nullptr;}
    textFont=new QFont(f);
}//TODO persist
void AbstractGroupRosterEntry::setTextColor(QColor c)  {
    if(textColor){delete textColor;textColor=nullptr;}
    textColor=new QColor(c);
}//TODO persist
