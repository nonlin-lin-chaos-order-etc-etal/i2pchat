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
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of       
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "SeedlessManager.h"
#include "Core.h"
#include "SeedlessSearcher.h"
#include "SeedlessAnnouncer.h"
#include "UserManager.h"


CSeedlessManager::CSeedlessManager( CCore& Core,QString FilePathToSeedlessList,QString FilePathForSettings)
:mCore(Core),mFilePathToSeedlessList(FilePathToSeedlessList),mFilePathForSettings(FilePathForSettings)
{
  mFirstServerSearch=true;
  QSettings setting(FilePathForSettings);
  connect(&mReAnnouncingTimer,SIGNAL(timeout()),this,
	  SLOT(slotTimerForReAnncounging()));
  
  
  setting.beginGroup("Usersearch");
	mReAnnouncingTimer.setInterval(setting.value("ReAnnounceTimerInHours",1).toInt()*3600000);
  setting.endGroup();
  
  qsrand (QTime::currentTime().msec());
}

CSeedlessManager::~ CSeedlessManager()
{
  mReAnnouncingTimer.stop();
}

bool CSeedlessManager::ExtractInteressingInformation(QString dataStringIn, QString& Destination, QString& TTL, const QString Service)
{  
	Destination=dataStringIn.left(60);
	dataStringIn.remove(0,Destination.length()+1);
	  
	TTL=dataStringIn.mid(0,dataStringIn.indexOf(" "));
	dataStringIn.remove(0,TTL.length());
	return dataStringIn.contains(Service);
}

void CSeedlessManager::printSeedlessServerList()
{
  	QMapIterator<QString,SeedlessServerInformation> i(mSeedlessServerList);
	SeedlessServerInformation currentServerInformation;
  
	qDebug()<<"SeedlessServerList";
	qDebug()<<"for Service: "<<SEEDLESS_SERVICE_NAME<<endl;

	if (mSeedlessServerList.isEmpty()){
		qDebug()<<"List is empty !";
	}
	else{
	      while(i.hasNext()){
		      i.next();
		      currentServerInformation=i.value();
		      qDebug()<<"b32Dest:\t"  <<currentServerInformation.b32Dest;
		      qDebug()<<"failCount:\t"<<currentServerInformation.failCount;
	      }
	}
	
}


void CSeedlessManager::doSeedlessSearch(QByteArray Data)
{
	QString			DebugMessage;
	QString 		ServerAddress;
	QStringList allKkeys=   mSeedlessServerList.keys();
	QStringList		ServerAdressesList;
	
	DebugMessage="<-- CSeedlessManager::doSeedlessSearch -->\n";
	DebugMessage+="MetaDataForSearching:\n"+Data+"\n";
	DebugMessage+="---------------------------------------------------\n";
	
	emit signSearchState("Progress 0/"+QString::number(getCountForSeedlessMessengerSearch()));
	
	if(mSeedlessServerList.isEmpty()){
		emit signSeedlessSearchNothingFound();
		return;
	}

	DebugMessage+="using server:\n";
	
	while(ServerAdressesList.count()<getCountForSeedlessMessengerSearch()){
	   
	    int randomNumber=(qrand() % allKkeys.count());
	    ServerAddress=mSeedlessServerList.value(allKkeys.at(randomNumber)).b32Dest;
	    
	    if(ServerAdressesList.contains(ServerAddress)==false){
		ServerAdressesList.append(ServerAddress);
		DebugMessage+=ServerAddress+"\n";
	    }
	}
	
	for(int i=0; i< ServerAdressesList.count();i++){
	  CSeedlessSearcher* searcher=new CSeedlessSearcher(mCore,QString::number(i));
	  connect(searcher,SIGNAL(signDoneSuccessfully(bool,QStringList,QList<QByteArray>,QString)),
		this,SLOT(slotSeedlessSearcherFinished(bool,QStringList,QList<QByteArray>,QString)));
	
	  mSeedlessUserSearcherList.insert(QString::number(i),searcher);
	}
	
	
	for(int i=0; i< ServerAdressesList.count();i++){
	    mSeedlessUserSearcherList.value(QString::number(i))->doSearch(ServerAdressesList.at(i),SEEDLESS_SERVICE_NAME,Data);
	}
	emit DebugMessages(DebugMessage);
}


void CSeedlessManager::doSeedlessAnnounce()
{
	QString		MetaDataForAnnouncing;
	QString		DebugMessage;
	
	QString 	ServerAddress;
	QStringList	ServerAdressesList;
	QByteArray	data;
	
	MetaDataForAnnouncing=getMetaDataForAnnouncing();
	
	DebugMessage="<-- CSeedlessManager::doSeedlessAnnounce() -->\n";
	DebugMessage+="MetaDataForAnnouncing:\n"+MetaDataForAnnouncing+"\n";
	DebugMessage+="---------------------------------------------------\n";
	data.append(MetaDataForAnnouncing);
	
	DebugMessage+="using server:\n";
	
	QStringList allKkeys= mSeedlessServerList.keys();
	
	while(ServerAdressesList.count()<getCountForSeedlessAnncounce()){
	    int randomNumber=(qrand() % allKkeys.count());
	    ServerAddress=mSeedlessServerList.value(allKkeys.at(randomNumber)).b32Dest;
	
	    if(ServerAdressesList.contains(ServerAddress)==false){
		ServerAdressesList.append(ServerAddress);
		DebugMessage+=ServerAddress+"\n";
	    }
	}
	
	for(int i=0; i< ServerAdressesList.count();i++){
	    CSeedlessAnnouncer* announcer= new CSeedlessAnnouncer(mCore,QString::number(i));
	    connect(announcer,SIGNAL(signDoneSuccessfully(bool,QStringList,QByteArray,QString)),
		    this,SLOT(slotSeedlessAnnouncerFinished(bool,QStringList,QByteArray,QString)));
	    
	    mSeedlessAnnouncerList.insert(QString::number(i),announcer)	;
	}
	
	for(int i=0; i< ServerAdressesList.count();i++){
	    mSeedlessAnnouncerList.value(QString::number(i))->doAnnounce( ServerAdressesList.at(i),SEEDLESS_SERVICE_NAME,data);
	}
	emit DebugMessages(DebugMessage);
}


void CSeedlessManager::doStart()
{	
	emit DebugMessages("<-- CSeedlessManager::doStart -->");
	mReAnnouncingTimer.start();
	LoadSeedlessServerList();
	SearchForMoreUseFullSeedlessServer();
	
}

void CSeedlessManager::slotSeedlessAnnouncerFinished(bool t,QStringList Header, QByteArray Data,QString ID)
{
	QString ServerDestination;
	QString DebugMessage;
	
	if(mSeedlessAnnouncerList.contains(ID)==false){  
	      return;
	}
	
	foreach(QString line,Header){
	  if(line.contains("X-Seedless: ")==true){
	      line.replace("X-Seedless: ","");
	      if(line!=mCore.getMyDestinationB32()){
		  mCore.setMyDestinationB32(line);
	      }
	  }
	}
	
	CSeedlessAnnouncer* currentAnnouncer=mSeedlessAnnouncerList.take(ID);
	ServerDestination=currentAnnouncer->getDestination();
	
	DebugMessage="<-- CSeedlessManager::slotSeedlessAnnouncerFinished -->\n";
	
	if(t==true){
		DebugMessage+="Anouncing OK ( "+ID+" )\n";
		DebugMessage+="Header start:\n";
	 
		for(int i=0;i<Header.count();i++){
		    DebugMessage+="Line \t"+ QString::number(i,10)+": "+Header.at(i)+"\n";
		}
		DebugMessage+="-------------------------------\n";
		
		DebugMessage+="Data Recived(length: "+QString::number(Data.length());
		DebugMessage+="):\n"+Data+"\n";
		
		//announcing worked,- so reset the failcount
		SeedlessServerInformation tmp=mSeedlessServerList.take(ServerDestination);
		tmp.failCount=0;
		mSeedlessServerList.insert(tmp.b32Dest,tmp);
	}
	else{	
		DebugMessage+="Anouncing Not OK ( "+ID+")\n";
		for(int i=0;i<Header.count();i++){
		    DebugMessage+="Line \t"+ QString::number(i,10)+": "+Header.at(i)+"\n";
		}
		DebugMessage+="-------------------------------\n";
		
		DebugMessage+="Data Recived(length: "+QString::number(Data.length());
		DebugMessage+="):\n"+Data+"\n";
		
		//announcing don't worked so , increase the failCount    
		SeedlessServerInformation tmp=mSeedlessServerList.take(ServerDestination);
		tmp.failCount++;
		mSeedlessServerList.insert(tmp.b32Dest,tmp);
		CheckForMax_FailCount();
	}

	emit DebugMessages(DebugMessage);
	currentAnnouncer->deleteLater();
	if(mSeedlessAnnouncerList.isEmpty()==true){
	      emit DebugMessages("<-- the Seedless announcing complead finished -->");
	}
}

void CSeedlessManager::slotSeedlessSearcherFinished(bool t ,QStringList Header,QList<QByteArray> Data,QString ID)
{
	QString ServerDestination;
	SeedlessSearchStruct resultStr;
	QString DebugMessage;
	int lineCount=1;
	
	
	if(mSeedlessUserSearcherList.contains(ID)==false){
	    qint32 iID=ID.toInt();
	    mCore.getConnectionManager()->doDestroyStreamObjectByID(iID);
	    return;
	}
	
	foreach(QString line,Header){
	  if(line.contains("X-Seedless: ")==true){
	      line.replace("X-Seedless: ","");
	      if(line!=mCore.getMyDestinationB32()){
		  mCore.setMyDestinationB32(line);
	      }
	  }
	}
	
	
	DebugMessage="<-- CSeedlessManager::slotSeedlessSearcherFinished -->\n";
	ServerDestination=mSeedlessUserSearcherList.value(ID)->getDestination();
	
	if(t==true && Header.at(0)=="HTTP/1.1 200 OK" ){
	        bool OK;
		
		DebugMessage+="Searching OK (ID: "+ID+")\n";
		DebugMessage+="Header start:\n";
	    
		for(int i=0;i<Header.count();i++){
		    DebugMessage+="Line \t"+QString::number(i)+": "+Header.at(i)+"\n";
		}
		DebugMessage+="-------------------------------\n";
		
		for(int responsCount=0;responsCount<Data.count();responsCount++){
		      DebugMessage+="Respons\t"+QString::number(responsCount)+":\n";
		      QTextStream	in(Data.at(responsCount));
		      in.setCodec(QTextCodec::codecForName("utf-8")); 

		      DebugMessage+="Decoded Respons(length: "+QString::number(Data.at(responsCount).length())+"):\n";
		      while(!in.atEnd()){
			  DebugMessage+="Line \t"+QString::number(lineCount)+":"+in.readLine()+"\n";
			  lineCount++;
		      }
		      lineCount=1;
		      DebugMessage+="-------------------------------\n";
		      
		      resultStr=ExtractMessengerInformation(Data.at(responsCount),OK);
		      if(OK==true){  
			  if(mSearchResult.contains(resultStr.b32Dest)==false){
				mSearchResult.insert(resultStr.b32Dest,resultStr);
			  }else{
			    if(mSearchResult.value(resultStr.b32Dest).TTL < resultStr.TTL){
				mSearchResult.remove(resultStr.b32Dest);
				mSearchResult.insert(resultStr.b32Dest,resultStr);
			    }
			  }
		      }else{
			  DebugMessage+="Unsuported SERVICE_STRUCT_VERSION\nSearchResult ignored\n";
			  //Ignore it
		      }
		}
		  //search worked,- so reset the failcount
		  SeedlessServerInformation tmp=mSeedlessServerList.take(ServerDestination);
		  tmp.failCount=0;
		  mSeedlessServerList.insert(tmp.b32Dest,tmp);
	}
	else{
		DebugMessage+="Searching not OK (ID: ";
		DebugMessage+=ID +")\n";
		DebugMessage+="Header start:\n";
	    
		for(int i=0;i<Header.count();i++){
		    DebugMessage+="Line \t"+QString::number(i)+": "+Header.at(i)+"\n";
		}
		DebugMessage+="-------------------------------\n";
		for(int responsCount=0;responsCount<Data.count();responsCount++){
		      DebugMessage+="Respons\t"+QString::number(responsCount)+":\n";
		      QTextStream	in(Data.at(responsCount));
		      in.setCodec(QTextCodec::codecForName("utf-8")); 

		      DebugMessage+="Respons(length: "+QString::number(Data.at(responsCount).length())+"):\n";
		      while(!in.atEnd()){
			  DebugMessage+="Line \t"+QString::number(lineCount)+":"+in.readLine()+"\n";
			  lineCount++;
		      }
		      lineCount=1;
		      DebugMessage+="-------------------------------\n";
		}
		  //search don't worked so , increase the failCount    
		  SeedlessServerInformation tmp=mSeedlessServerList.take(ServerDestination);
		  tmp.failCount++;
		  mSeedlessServerList.insert(tmp.b32Dest,tmp);
		  CheckForMax_FailCount();
	}
	
	CSeedlessSearcher* currentSearcher=mSeedlessUserSearcherList.value(ID);
	currentSearcher->deleteLater();
	mSeedlessUserSearcherList.remove(ID);
	
	if(mSeedlessUserSearcherList.isEmpty()){
	    DebugMessage+="Search compleaded\n";
	    CheckAndFilterResult();
	    
	    emit signSearchState("Finished"); 
	    emit signSeedlessSearchResults(mSearchResult);
	    mSearchResult.clear();
	}else{
	    emit signSearchState("Progress " +QString::number(getCountForSeedlessMessengerSearch() - mSeedlessUserSearcherList.count())+"/"+QString::number(getCountForSeedlessMessengerSearch())); 
	}
	emit DebugMessages(DebugMessage);
	
}


void CSeedlessManager::LoadSeedlessServerList()
{
	QString DebugMessage;
	
    	QFile file(mFilePathToSeedlessList);
	DebugMessage="<-- CSeedlessManager::LoadSeedlessServerList() -->\nFile:\t"+mFilePathToSeedlessList+"\n";
	
     	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		DebugMessage+="File empty or don't exist, load FallBackSeedlessServerList\n";
		emit DebugMessages(DebugMessage);
		LoadFallBackSeedlessServerList();
		return;
	}
	
	QByteArray bSeedlessList=file.readAll();
        QTextStream in(bSeedlessList);
	QString line;
	QStringList temp;
	SeedlessServerInformation currentServer;
	
	in.skipWhiteSpace();
	mSeedlessServerList.clear();
	
	while (!in.atEnd()) {
		line = in.readLine(550);
		temp=line.split("\t");
	
		if(temp[0]=="b32Dest:"){
		    currentServer.b32Dest=temp[1];
		}
		else if(temp[0]=="failCount:"){
		   currentServer.failCount=temp[1].toInt(0,10);
		   mSeedlessServerList.insert(currentServer.b32Dest,currentServer);
		} 
	}
	file.close();
	
	if(mSeedlessServerList.isEmpty()==true){
	    LoadFallBackSeedlessServerList();
	}
}
void CSeedlessManager::SaveSeedlessServerList()
{
	QFile file(mFilePathToSeedlessList);
	file.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&file);

	QMapIterator<QString,SeedlessServerInformation> i(mSeedlessServerList);
	SeedlessServerInformation currentServerInformation;
      
	emit DebugMessages("<--  CSeedlessManager::SaveSeedlessServerList() -->\n");
	
	while(i.hasNext()){
	  	i.next();
		currentServerInformation=i.value();
		out<<"b32Dest:\t"   <<currentServerInformation.b32Dest  <<endl
		   <<"failCount:\t" <<currentServerInformation.failCount<<endl;
	}
	out.flush();
	file.close();
}
int CSeedlessManager::doCheckForValidB32(const QString B32Address) const
{
    QString tmp=B32Address.right(8);
  
    if(tmp.compare(".b32.i2p",Qt::CaseInsensitive)==0){
      return true;
    }
    
    return false;
}

void CSeedlessManager::LoadFallBackSeedlessServerList()
{
	QString DebugMessage;
	QString FallbackList=mCore.getConfigPath();
	FallbackList.append("/SeedlessFallbackList.txt");
	
	QFile file(FallbackList);
	
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		DebugMessage+="Can't load fallback-seedlessserver-list because the file: SeedlessFallbackList.txt don't exits\n";
		emit DebugMessages(DebugMessage);
		
		QMessageBox* msgBox= new QMessageBox(NULL);
		msgBox->setIcon(QMessageBox::Critical);
		msgBox->setText("CSeedlessManager(LoadFallBackSeedlessServerList)");
		msgBox->setInformativeText("Can't load fallbacklist because the file: SeedlessFallbackList.txt don't exits");
		msgBox->setStandardButtons(QMessageBox::Ok);
		msgBox->setDefaultButton(QMessageBox::Ok);
		msgBox->setWindowModality(Qt::NonModal);
		msgBox->show();
		return ;
	}
	
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream in(&file);
	
	DebugMessage="<--LoadFallBackSeedlessServerList-->\n";
        DebugMessage.append("added Server:\n");
        mSeedlessServerList.clear();
	
	while(!in.atEnd()){
	   QString textLine=in.readLine().trimmed();
	   if(textLine.startsWith("//")==true || textLine.startsWith("#")){continue;}
	   
	   if(doCheckForValidB32(textLine)==true && mSeedlessServerList.contains(textLine)==false){
		//add new SeedlessServer
		SeedlessServerInformation newServer;
		newServer.b32Dest=textLine;
		newServer.failCount=0;
		mSeedlessServerList.insert(newServer.b32Dest,newServer);
		
		DebugMessage+=textLine;
		DebugMessage+="\n";
	   } 
	}
	file.close();
	emit DebugMessages(DebugMessage);
}

void CSeedlessManager::slotSeedlessServerSearchFinished(bool t, QStringList Header, QList< QByteArray > Data, QString ID)
{
  QString DebugMessage;
  QString line;
  QString Destination;
  QString ServerDestination;
  QString TTL;
  
      if(mSeedlessServerSearchList.contains(ID)==false){
	  return;
      }
      
      foreach(QString line,Header){
	  if(line.contains("X-Seedless: ")==true){
	      line.replace("X-Seedless: ","");
	      if(line!=mCore.getMyDestinationB32()){
		  mCore.setMyDestinationB32(line);
	      }
	  }
      }
  
      DebugMessage="<-- CSeedlessManager::slotSeedlessServerSearchFinished ( "+ID+" ) -->\n";
  
      CSeedlessSearcher* currentSearcher=mSeedlessServerSearchList.value(ID);
      ServerDestination=currentSearcher->getDestination();
  
      if(t==true){
		DebugMessage+="Searching OK (ID: "+ID+")\n";
		DebugMessage+="Header start:\n";
	    
		for(int i=0;i<Header.count();i++){
		    DebugMessage+="Line \t"+QString::number(i)+": "+Header.at(i)+"\n";
		}
		DebugMessage+="-------------------------------\n";
	
	
	    for(int responsCount=0;responsCount<Data.count();responsCount++){
	      line=QString(Data.at(responsCount));
	      
		if(ExtractInteressingInformation(line,Destination,TTL,SEEDLESS_SERVICE_NAME)==true){
		      if(mSeedlessServerList.contains(Destination)==false){
			    //create new Seedless value (mSeedlessServerList)
			    SeedlessServerInformation newServer;
			    newServer.b32Dest=Destination;
			    newServer.failCount=0;
			    DebugMessage+="New Server found: "+newServer.b32Dest+"\n";
			    mSeedlessServerList.insert(newServer.b32Dest,newServer);
		      }
		}
	    }
    
	    //search worked,- so reset the failcount
	    SeedlessServerInformation tmp=mSeedlessServerList.take(ServerDestination);
	    tmp.failCount=0;
	    mSeedlessServerList.insert(tmp.b32Dest,tmp);

      }else
      {	
	    int lineCount=1;
	    DebugMessage+="Searching not OK (ID: ";
	    DebugMessage+=ID +")\n";
	    DebugMessage+="Header start:\n";
		for(int i=0;i<Header.count();i++){
		    DebugMessage+="Line \t"+QString::number(i)+": "+Header.at(i)+"\n";
		}
		DebugMessage+="-------------------------------\n";
	    
	    for(int responsCount=0;responsCount<Data.count();responsCount++){
		  DebugMessage+="Respons\t"+QString::number(responsCount)+":\n";
		  QTextStream	in(Data.at(responsCount));
		  in.setCodec(QTextCodec::codecForName("utf-8")); 

		  DebugMessage+="Respons(length: "+QString::number(Data.at(responsCount).length())+"):\n";
		  while(!in.atEnd()){
		      DebugMessage+="Line \t"+QString::number(lineCount)+":"+in.readLine()+"\n";
		      lineCount++;
		  }
		  lineCount=1;
		  DebugMessage+="-------------------------------\n";
	    }

	    //search don't worked so , increase the failCount    
	    SeedlessServerInformation tmp=mSeedlessServerList.take(ServerDestination);
	    tmp.failCount++;
	    mSeedlessServerList.insert(tmp.b32Dest,tmp);
	    CheckForMax_FailCount();
      }
      
     
      
      currentSearcher->deleteLater();
      mSeedlessServerSearchList.remove(ID);
      
      if(mSeedlessServerSearchList.isEmpty()==true){
	  DebugMessage.append("<-- the SeedlessServerSearch complead finished -->");
	  SaveSeedlessServerList();
	  
	  if(mFirstServerSearch==true){
	      mFirstServerSearch=false;
	      doSeedlessAnnounce();
	  }
      } 
      
       emit DebugMessages(DebugMessage);
}

void CSeedlessManager::SearchForMoreUseFullSeedlessServer()
{
      QString	  DebugMessage;
      QStringList MapForSearchkeys;
      QStringList SeedlessServerListkeys= mSeedlessServerList.keys();
      QMap<QString,QString> MapForSearch;
      int randomNumber;
      QString sID;
      
      DebugMessage="<-- CSeedlessManager::SearchForMoreUseFullSeedlessServer -->\n";
      DebugMessage+="asking Server:\n";
      
      if(mSeedlessServerList.count()==getCountForSeedlessserverSearch()){
	    for(int i=0; i< getCountForSeedlessserverSearch();i++){	
		DebugMessage+= SeedlessServerListkeys.at(i)+"\n";
		MapForSearch.insert(SeedlessServerListkeys.at(i),sID.number(i,10));
	    }
      }else if (mSeedlessServerList.count() > getCountForSeedlessserverSearch()){
	      //choice random SeedlessServer
	      for(int i=0; i< getCountForSeedlessserverSearch();i++){
		 randomNumber =(qrand() % SeedlessServerListkeys.count());
		 while(MapForSearch.contains(SeedlessServerListkeys.at(randomNumber))==true){
			randomNumber =(qrand() % SeedlessServerListkeys.count());
		 }
		
		DebugMessage+=SeedlessServerListkeys.at(randomNumber)+"\n";
		MapForSearch.insert(SeedlessServerListkeys.at(randomNumber),sID.number(i,10));
	      }
      }
      else{
	  qCritical()<<"File\t"<<__FILE__<<endl
		   <<"Line:\t"<<__LINE__<<endl
		   <<"Function:\t"<<"CSeedlessManager::SearchForMoreUseFullSeedlessServer"<<endl
		   <<"Message:\t"<<"mSeedlessServerList.count() < COUNT_FOR_SEEDLESSSERVER_SEARCH"<<endl;
	  return;
      }
      
      	QMapIterator<QString,QString> i(MapForSearch);

	while(i.hasNext()){
	  	i.next();
		
		CSeedlessSearcher* searcher=new CSeedlessSearcher(mCore,i.value());
		
		connect(searcher,SIGNAL(signDoneSuccessfully(bool,QStringList,QList<QByteArray>,QString)),
			this,SLOT(slotSeedlessServerSearchFinished(bool,QStringList,QList<QByteArray>,QString)));
		
		mSeedlessServerSearchList.insert(i.value(),searcher);
		searcher->doSearch(i.key(),"seedless",QByteArray(" "));
	}
	emit DebugMessages(DebugMessage);
}

void CSeedlessManager::CheckForMax_FailCount()
{
	QString DebugMessage;
	QMapIterator<QString,SeedlessServerInformation> i(mSeedlessServerList);
	SeedlessServerInformation currentServerInformation;
	
	while(i.hasNext()){
	  	i.next();
		currentServerInformation=i.value();
		
		if(currentServerInformation.failCount>= MAX_FAILCOUNT){
		      DebugMessage="<-- CSeedlessManager::CheckForMax_FailCount -->\n";
		      DebugMessage+="remove Server:\n";
		      DebugMessage+=currentServerInformation.b32Dest+"\n";
		      
		      mSeedlessServerList.remove(currentServerInformation.b32Dest);
		      emit DebugMessages(DebugMessage);
		      SaveSeedlessServerList();
		}
	}
	
	if(mSeedlessServerList.isEmpty()==true){
	    LoadFallBackSeedlessServerList();
	    SaveSeedlessServerList();
	}	
}

QString CSeedlessManager::getMetaDataForAnnouncing()
{
    QString 	MetaDataForAnnouncing;
    QString 	sAge;    
    sAge.setNum(mCore.getUserInfos().Age,10);
    MetaDataForAnnouncing= SEEDLESS_SERVICE_STRUCT_VERSION;
    MetaDataForAnnouncing+="\n";
    MetaDataForAnnouncing+=mCore.getUserInfos().Nickname+"\n";
    MetaDataForAnnouncing+=sAge+"\n";
    
    if(mCore.getUserInfos().Gender=="Male"){
	    MetaDataForAnnouncing+="M\n";
    }
    else if(mCore.getUserInfos().Gender=="Female"){
	    MetaDataForAnnouncing+="W\n";
    }
    else{
	    MetaDataForAnnouncing+=" \n";
    }
    MetaDataForAnnouncing+= mCore.getUserInfos().Interests+"\n";

    return MetaDataForAnnouncing;
}

void CSeedlessManager::doSeedlessMessangerSearch(SeedlessSearchStruct str)
{
    QByteArray MetaDataForSearch;
    
    mCurrentSearch=str;
    mSearchResult.clear();
    
    if(mSeedlessUserSearcherList.isEmpty()==false){
	QStringList allSeedlessUserSearcherListKeys=mSeedlessUserSearcherList.keys();

	for(int i=0;i<allSeedlessUserSearcherListKeys.count();i++){
	      CSeedlessSearcher* tmp=mSeedlessUserSearcherList.take(allSeedlessUserSearcherListKeys.at(i));
	      delete tmp;
	}
    }
    
    
    if(str.NickName.isEmpty()==false){
      MetaDataForSearch.append(str.NickName);
    }else if(str.Interests.isEmpty()==false){
       MetaDataForSearch.append(str.Interests);
    }else if(str.Age!=0){
       MetaDataForSearch.append(QString::number(str.Age,10));
    }
    else if(str.Gender.isEmpty()==false){
      MetaDataForSearch.append(str.Gender);
    }
    else{
      MetaDataForSearch.append(" ");
    }
    
    doSeedlessSearch(MetaDataForSearch);
    
}
CSeedlessManager::SeedlessSearchStruct CSeedlessManager::ExtractMessengerInformation(QByteArray data,bool& OK)
{
    SeedlessSearchStruct str;
    int lineCount=1;
    
    QString CurrentLine;
    QTextStream	in(data);
    in.setCodec(QTextCodec::codecForName("utf-8")); 
    while(!in.atEnd()){
	CurrentLine=in.readLine();
	
	if(lineCount==1){	//b32dest,TTL & Structversion
	    str.b32Dest=CurrentLine.mid(0,CurrentLine.indexOf(".b32.i2p")+8);
	    CurrentLine.remove(0, str.b32Dest.length()+1);
	    str.TTL=CurrentLine.mid(0,CurrentLine.indexOf(" ")).toInt();
	    CurrentLine.remove(0,CurrentLine.indexOf(" ")+1);
	    
	    if(CurrentLine==SEEDLESS_SERVICE_STRUCT_VERSION){
		OK=true;
	    }else{
		OK=false;
		break;
	    }
	}
	else if(lineCount==2){//Nickname
	    str.NickName=CurrentLine;
	}
	else if(lineCount==3){//Age
	    str.Age=CurrentLine.toInt();
	}
	else if(lineCount==4){//Gender
	    str.Gender=CurrentLine;
	}
	else{//Interests
	  str.Interests.append(CurrentLine+"\n");
	}
	lineCount++;
    }
    return str;
}
void CSeedlessManager::CheckAndFilterResult()
{
    //mSearchResult
    SeedlessSearchStruct str;
    QMapIterator<QString,SeedlessSearchStruct> i(mSearchResult);
    
    
    while(i.hasNext()){
      i.next();
      str=i.value();
      
      //not complead -> Nickname and b32 mast be set
      if(str.NickName.isEmpty()==true || str.b32Dest.isEmpty()==true){
	  mSearchResult.remove(i.key());
	     continue;
      }
      //-------------------------
      
      
      //Filter
      if(mCurrentSearch.NickName.isEmpty()==false){
	  if(str.NickName.contains(mCurrentSearch.NickName)==false){
	     mSearchResult.remove(i.key());
	     continue;
	  }
      }
      
      if(mCurrentSearch.Age!=0){
	if(str.Age!=mCurrentSearch.Age){
	      mSearchResult.remove(i.key());
	      continue;
	}
      }
      
      if(mCurrentSearch.Gender !=" "){
	  if(str.Gender!=mCurrentSearch.Gender){
	      mSearchResult.remove(i.key());	 
	      continue;
	  }
      }
      
      if(mCurrentSearch.Interests.isEmpty()==false){
	  if(str.Interests.contains(mCurrentSearch.Interests)==false){
	     mSearchResult.remove(i.key());
	     continue;
	  }
      }
    }//while
}

bool CSeedlessManager::addNewUser(const SeedlessSearchStruct& str)
{
      if(str.b32Dest==mCore.getMyDestinationB32()){
	  QMessageBox::information(NULL, tr("I2P Messenger"),
	    tr("Sorry you can't add yourself !"),QMessageBox::Close);
	    return false;
      }else{
	  mCore.getUserManager()->addNewUser(str.NickName,str.b32Dest,0,true);
	  return true;
      }
}

void CSeedlessManager::slotTimerForReAnncounging()
{
      emit DebugMessages("<-- CSeedlessManager::slotTimerForReAnncounging -->\nReAnnouncing Started\n");
      doSeedlessAnnounce();
}
void CSeedlessManager::doStop()
{
    emit DebugMessages("<-- CSeedlessManager::doStop -->");
    mReAnnouncingTimer.stop();
    
    //clear mSeedlessUserSearcherList 
    QStringList allSeedlessUserSearcherKeys=mSeedlessUserSearcherList.keys();
    for(int i=0;i<allSeedlessUserSearcherKeys.count();i++){
	mSeedlessUserSearcherList.value(allSeedlessUserSearcherKeys.at(i))->deleteLater();
	mSeedlessUserSearcherList.remove(allSeedlessUserSearcherKeys.at(i));
    }
    
    //clear mSeedlessAnnouncerList
    QStringList allSeedlessAnnouncerKeys=mSeedlessAnnouncerList.keys();
    for(int i=0;i<allSeedlessAnnouncerKeys.count();i++){
	mSeedlessAnnouncerList.value(allSeedlessAnnouncerKeys.at(i))->deleteLater();
	mSeedlessAnnouncerList.remove(allSeedlessAnnouncerKeys.at(i));
    }
    
    //clear mSeedlessServerSearchList
    QStringList allSeedlessServerSearchKeys=mSeedlessServerSearchList.keys();
    for(int i=0;i<allSeedlessServerSearchKeys.count();i++){
	mSeedlessServerSearchList.value(allSeedlessServerSearchKeys.at(i))->deleteLater();
	mSeedlessServerSearchList.remove(allSeedlessServerSearchKeys.at(i));
    }
}
int CSeedlessManager::getCountForSeedlessserverSearch() const
{
    if(mSeedlessServerList.count()>=3){return 3;}
    else{
      return mSeedlessServerList.count();
    }
}

