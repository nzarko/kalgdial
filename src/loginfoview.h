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
#ifndef LOGINFOVIEW_H
#define LOGINFOVIEW_H

#include <viewloginfobase.h>

#include "loginfo.h"

/**
@author Nikos Zarkadas
*/
class LogInfoView : public ViewLogInfoBase
{
Q_OBJECT
public:
    LogInfoView(LogInfo *log_info, QWidget *parent = 0, const char *name = 0);

    ~LogInfoView();
	public slots:
		void showInfo ( const QString &);
	private:
		QString prevCBSelection;
		QStringList m_fileList;
		LIVector *m_liVector;
		LogInfo *m_logInfo;
private:
    void statistics();
};

#endif
