#include "parseerrors.h"

#include "parseerror.h"

void reportParseError(QString errorMessage, parsed_item_file item) {
    QString parsed_item_qstr;
    switch(item) {
    case roster_conf:  parsed_item_qstr="roster.conf";  break;
    case users_config: parsed_item_qstr="users.config"; break;
    default: parsed_item_qstr=QObject::tr("unknown");   break;
    }

    QString errorMessageFull = QObject::tr("Error: ")+errorMessage+QObject::tr(" at ")+parsed_item_qstr;
    QMessageBox::critical(nullptr, QObject::tr("Error parsing"), errorMessageFull);
    throw ParseError(errorMessageFull.toStdString());
}
