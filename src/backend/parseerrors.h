#ifndef PARSEERRORS_H
#define PARSEERRORS_H

#include <QString>
#include <QMessageBox>
#include <QObject>

typedef enum { roster_conf, users_config } parsed_item_file;

void reportParseError(QString errorMessage, parsed_item_file item);

#endif // PARSEERRORS_H
