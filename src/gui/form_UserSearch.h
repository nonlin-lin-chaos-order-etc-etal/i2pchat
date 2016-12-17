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

#ifndef FORM_USERSEARCH_H
#define FORM_USERSEARCH_H

#include <QtGui>
#include <QtGlobal> 
#include <QMap>

#include "ui_form_userSearch.h"

#include "SeedlessManager.h"
#include "gui_icons.h"

class CCore;
class form_userSearch : public QDialog,private Ui::form_userSearch
{

  Q_OBJECT

  public:
      form_userSearch(CCore& Core,CSeedlessManager& SeedlessManage);
     ~form_userSearch();
      void getFocus();
     
  signals:
      void signClosingUserSearchWindow();
      
  private slots:
      void slot_cmdSearch();
      void slot_SeedlessSearchFinished(QMap<QString,CSeedlessManager::SeedlessSearchStruct> SearchResults);
      void slot_showContextMenu(const QPoint &);
      void slot_addUser();  
      void slot_onlineStateChanged();
      void slot_genderMale();
      void slot_genderFemale();
      
  private:
      CSeedlessManager& mSeedlessManager;  
      CCore&		mCore;
      void init();
      void closeEvent(QCloseEvent *e);
      void keyPressEvent(QKeyEvent* event); 
      QMap< QString, CSeedlessManager::SeedlessSearchStruct > mSearchResults;
};
#endif
