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
#ifndef LOGINFO_H
#define LOGINFO_H

#include <qstring.h>
#include <qtextstream.h>
#include <qvaluevector.h>

#include "pppstats.h"

struct InfoData;
class LogInfo;
typedef QValueVector<LogInfo> LIVector;
typedef QValueVector<LogInfo>::iterator LIVectorIter;
typedef QValueVector<LogInfo>::size_type qt_int;

/**
@author Nikos Zarkadas
*/
class LogInfo
{
	private:
		//For internal use
		QString m_logFile; /** log file only for read */
		QString _curLogFileName; /** log file only for write logs */
		LIVector li_vector;
		PPPStats *stats;
		//Info data.
		QString cur_date;
		QString interface_name; 
		QString con_start; /** The time that connection was started */
		QString con_end; /** The time that connection was ended */
		QString con_duration; /** The connection duration. */
		int ibytes;	/** Total bytes in */
		int obytes; /** Total bytes out */
		int totalbytes; /** Total bytes. */
public:
	LogInfo();
    LogInfo(PPPStats *st);
	LogInfo ( const LogInfo &);
    ~LogInfo();
    LIVector* read(const QString &);
    bool write() const; 
    void setFileName(QString fname);
    void setValues( InfoData & );
	// getter functions.
	QString curDate() const { return cur_date; } 
	QString interfaceName() const { return interface_name; }
	QString conStart() const { return con_start; }
	QString conEnd() const { return con_end; }
	QString conDuration() const { return con_duration; }
	int oBytes() const { return obytes; }
	int iBytes() const { return ibytes; }
	int totalBytes() const { return totalbytes; }
	
	//Friends
	friend QTextStream &operator >> (QTextStream & , LogInfo &);
	friend QTextStream &operator  << ( QTextStream & , const LogInfo &);
private:
    void init();
};

#endif
