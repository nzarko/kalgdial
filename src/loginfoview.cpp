/***************************************************************************
 *   Copyright (C) 2005 by Nikos Zarkadas   *
 *   nzarko@acn.gr   *
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
#include <qdir.h>

#include <kstddirs.h>
#include <kglobal.h>
#include <kcombobox.h>
#include <klistview.h>
#include <klocale.h>
#include <kdebug.h>

#include "loginfoview.h"

LogInfoView::LogInfoView(LogInfo *log_info, QWidget *parent, const char *name)
 : ViewLogInfoBase(parent, name)
{
	m_logInfo = new LogInfo (*log_info);
	QString dir_name = locateLocal("appdata", "Log/");
	kdDebug() << "dir_name = " << dir_name << endl;
	
	//Find all the file names in Log directory
	QDir log_dir(dir_name);
	m_fileList = log_dir.entryList("*.log");
	m_logCB->setEditable( FALSE );
	m_logCB->insertStringList(m_fileList, 0);
	m_logCB->setEnabled( TRUE );
	m_logCB->setCurrentItem(0);
	prevCBSelection = m_logCB->currentText();
	connect (m_logCB, SIGNAL (activated(const QString& )), this,
			 SLOT (showInfo (const QString &)));
	
	//Show info to user.
	showInfo(m_logCB->currentText());
}


LogInfoView::~LogInfoView()
{
	delete m_logInfo;
}

void LogInfoView::showInfo( const QString & filename)
{
	//Read the log file.
	kdDebug(20) << "combo active file name : " << filename << endl;
	
	m_logLV->clear();
	m_liVector = m_logInfo->read(filename);
	LIVectorIter iter;
	QString str;
	
	for(iter = m_liVector->begin(); iter != m_liVector->end(); iter ++ )
	{
		KListViewItem *listItem = new KListViewItem(m_logLV, iter->curDate());
		listItem->setText(1, iter->interfaceName());
		listItem->setText(2, iter->conStart());
		listItem->setText(3, iter->conEnd());
		listItem->setText(4, iter->conDuration());
		listItem->setText(5, str.setNum(iter->iBytes()));
		listItem->setText(6, str.setNum(iter->oBytes()));
		listItem->setText(7, str.setNum(iter->totalBytes()));
	}
	
	statistics();
}

/*!
    \fn LogInfoView::statistics()
 */
 void LogInfoView::statistics()
{
	//How many connections have done already
	QString str;
	m_logTotalLV->clear();
	uint how_many = m_liVector->count();
	
	//Total duration ibytes. obytes, totalbytes
	QTime dur(0,0,0);
	LIVectorIter iter;
	int ib = 0, ob = 0, tb = 0;
	int total_secs = 0;
	for(iter = m_liVector->begin(); iter != m_liVector->end(); iter ++ )
	{
		dur = QTime::fromString(iter->conDuration());
		total_secs += dur.hour()*3600 +  dur.minute()*60 + dur.second();
		ib += iter->iBytes();
		ob += iter->oBytes();
		tb += iter->totalBytes();
	}
	
	//Convert total_secs to valid time.
	int hour, min, sec;
	hour = (total_secs / 3600) % 24;
	min = (total_secs / 60) % 60;
	sec = total_secs % 60;
	dur.setHMS( hour, min, sec );
	//Fill the statistics List view
	KListViewItem *listItem = new KListViewItem(m_logTotalLV);
	str = str.setNum(how_many);
	listItem->setText(0, str);
	str = dur.toString();
	listItem ->setText(1, str);
	str = str.setNum(ib);
	listItem->setText(2, str);
	str = str.setNum(ob);
	listItem->setText(3, str);
	str = str.setNum(tb);
	listItem->setText(4, str);
}
#include "loginfoview.moc"



