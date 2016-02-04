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
#include "loginfo.h"
#include "kalgdial.h"

#include <qfile.h>
#include <kstddirs.h>
#include <kglobal.h>
#include <kurl.h>
#include <kio/job.h>
#include <kio/jobclasses.h>
#include <klocale.h>
#include <kdebug.h>

const char  FIELD_SEP = '|';

/** 
 * Used to store Log info data after a succesfull 
 * parsing  of a log file.
 * @author Nikos Zarkadas
 */
struct InfoData
{
	QString cur_date;
	QString interface_name; 
	QString con_start;
	QString con_end;
	QString con_duration;
	//QString ed_mes; /** browser view messages */
	int ibytes;	/** Total bytes in */
	int obytes; /** Total bytes out */
	int totalbytes;
};

LogInfo::LogInfo()
{
	stats = NULL;
}

LogInfo::LogInfo(PPPStats *st)
{
	init();
	stats = st;
	cur_date = QDate::currentDate().toString();
	interface_name = stats->interfaceName();
	QStringList tmp = kalgdial::conTimeStartEnd();
	con_start = tmp[0];
	con_end = tmp[1];
	con_duration = stats->connectionTime()->toString();
	ibytes = stats->ibytes;
	obytes = stats->obytes;
	totalbytes = stats->totalbytes;
}

/*!
    \fn LogInfo::LogInfo ( const LogInfo & )
 */
 LogInfo::LogInfo ( const LogInfo &li )
{
	_curLogFileName = li._curLogFileName;
	m_logFile = li.m_logFile;
	stats = li.stats;
	interface_name = li.interface_name;
	con_start = li.con_start;
	con_end = li.con_end;
	con_duration = li.con_duration;
	ibytes = li.ibytes;
	obytes = li.obytes;
	totalbytes = li.totalbytes;
}

LogInfo::~LogInfo()
{
}
/*!
  *  \fn LogInfo::setFileName(QString fname)
 * Change the name of the log file. For read only.
 * Don't use this function to write log.
 */
 void LogInfo::setFileName(QString fname)
{
	m_logFile = locateLocal ("appdata", fname);
	kdDebug() << "Log file to read : " << m_logFile << endl;
}

/*!
    \fn LogInfo::read()
 */
LIVector* LogInfo::read(const QString &filename)
{
	li_vector.clear();
	InfoData id;
	bool ok;
	int errors = 0;
	int line_no = 1;
	
	QString fn =filename;
	 fn = fn.insert(0, "Log/");
	fn = locateLocal("appdata", fn);
	
	QFile log_f(fn);
	log_f.flush();
	if ( !log_f.open(IO_ReadOnly))
	{
		kdError() << "Can't open " << fn << " for read." << endl;
		return  &li_vector; //if something goes wrong an empty vector is returned.
	}
	
	QTextStream ts ( &log_f );
	while ( !ts.eof() )
	{
		QString line = ts.readLine();
		QStringList fields = QStringList::split(FIELD_SEP, line );
		id.cur_date = fields[0];
		id.interface_name = fields[1];
		id.con_start = fields[2];
		id.con_end = fields[3];
		id.con_duration = fields[4];
		id.ibytes = fields[5].toInt( &ok );
		if ( !ok )
			errors++;
		id.obytes = fields[6].toInt( &ok );
		if ( !ok )
			errors++;
		id.totalbytes = fields[7].toInt (&ok );
		if ( !ok )
			errors++;
	
		if (!errors)
		{
			LogInfo li;
			li.setValues( id );
			li_vector.push_back(li);
		}
		else 
		{
			kdError() << "LogInfo::read(const QString &) --> Can't read properly\n"
					<< errors << " errors made\n";
			kdError() << "LogInfo::read(const QString &) --> Possible error on : "
					<< fn << " near to line " << line_no << endl;
		}
		line_no++;
	}
	
	log_f.close();
	return &li_vector;
}

/*!
    \fn LogInfo::write()
 */
bool LogInfo::write() const 
{ 
	QFile log_f(_curLogFileName);
	if (!log_f.open(IO_WriteOnly | IO_Append))
	{
		kdError() << "Can't open " << _curLogFileName << " for write" << endl;
		return false;
	}
	
	QTextStream ts (&log_f);
	
	ts << cur_date << FIELD_SEP << interface_name << FIELD_SEP 
			<< con_start << FIELD_SEP << con_end << FIELD_SEP << con_duration << FIELD_SEP
			<< ibytes << FIELD_SEP << obytes << FIELD_SEP << totalbytes << '\n';
	
	log_f.close();
	return true;
}

/*!
    \fn LogInfo::init()
 */
void LogInfo::init()
{
	QString tmp_file = "Log/" + QDate::currentDate().toString("MMM-yy") + ".log";
	
	m_logFile = locateLocal("appdata", tmp_file, TRUE);
	//Use this helper variable to avoid read write confusions.
	//We want to write only in current logfile witch name format is 
	// cur_month-cur_year.log but when we want to read we need to know
	//exactly the file name. For example after three months of program usage
	//in Log directory will be three differrents log files. So that way we give  to user an 
	//oportinity to read all the log files changing every time the log 
	//file name with setFileName member function and call after that the read  function.
	_curLogFileName = m_logFile;
	kdDebug() << "Log file = " << _curLogFileName << endl;
}

/*!
    \fn LogInfo::setValues( InfoData & )
 */
 void LogInfo::setValues( InfoData &id )
{
	cur_date = id.cur_date;
	interface_name = id.interface_name;
	con_start = id.con_start;
	con_end = id.con_end;
	con_duration = id.con_duration;
	ibytes = id.ibytes;
	obytes = id.obytes;
	totalbytes = id.totalbytes;
}

QTextStream &operator >> (QTextStream &ts, LogInfo &info)
{
	bool ok;
	InfoData id;
	int errors = 0;
	
	QString line = ts.readLine();
	QStringList fields = QStringList::split( FIELD_SEP, line );
	id.cur_date = fields[0];
	id.interface_name = fields[1];
	id.con_start = fields[2];
	id.con_end = fields[3];
	id.con_duration = fields[4];
	id.ibytes = fields[5].toInt( &ok );
	if ( !ok )
		errors++;
	id.obytes = fields[6].toInt( &ok );
	if ( !ok )
		errors++;
	id.totalbytes = fields[7].toInt (&ok );
	if ( !ok )
		errors++;
	
	if (!errors)
		info.setValues( id );
	
	return ts;
}

QTextStream &operator << (QTextStream &ts, const LogInfo &info )
{
	ts << info.cur_date << FIELD_SEP << info.interface_name << FIELD_SEP 
			<< info.con_start << FIELD_SEP << info.con_end << FIELD_SEP << info.con_duration << FIELD_SEP
			<< info.ibytes << FIELD_SEP << info.obytes << FIELD_SEP << info.totalbytes << '\n';
	return ts;
}



