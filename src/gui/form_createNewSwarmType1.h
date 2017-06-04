/***************************************************************************
 *   Copyright (C) 2008 by I2P-Messenger   				   *
 *   Messenger-Dev@I2P-Messenger   					   *
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

#ifndef FORM_createNewSwarmType1_H
#define FORM_createNewSwarmType1_H

#include <QtGui>
#include <QtGlobal> 
#include <QMap>

#include "ui_form_createSwarmType1.h"

#include "gui_icons.h"

class AppContext;
class form_createNewSwarmType1: public QDialog,private Ui::form_createNewSwarmType1
{

    Q_OBJECT

public:
    form_createNewSwarmType1(AppContext & appCtx);
    ~form_createNewSwarmType1();

    //forbid some operators
    form_createNewSwarmType1(const form_createNewSwarmType1&)=delete;
    form_createNewSwarmType1& operator=(const form_createNewSwarmType1&)=delete;

    void requestFocus();

signals:
    void signClosingWindow();

private slots:
    void slot_cmdCreate();
    void swarmLabelInputTextChanged(QString text);

private:
    AppContext            & mCore;
    void closeEvent(QCloseEvent *e);
    void keyPressEvent(QKeyEvent* event);
    void updateCmdCreate();
    QString canonicalizedSwarmLabel();
};
#endif /* of FORM_createNewSwarmType1_H */
