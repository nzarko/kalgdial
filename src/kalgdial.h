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


#ifndef _KALGDIAL_H_
#define _KALGDIAL_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "kalgdialview.h"
#include <qstringlist.h>
#include <kmainwindow.h>

class QTimer;
class PPPStats;

/**
 * @short Application Main Window
 * @author Nikos Zarkadas <nzarko@acn.gr>
 * @version 0.99
 */
class kalgdial : public KMainWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    kalgdial();

    /**
     * Default Destructor
     */
    virtual ~kalgdial();
	static QStringList conTimeStartEnd()
	{
		QStringList lst;
		lst.push_back(conStartTime);
		lst.push_back(conEndTime);
		return lst;
	}
private:
	static QString conStartTime;
	static QString conEndTime;
	
    QTimer *if_timer;
    KAction* quit_act;
    KAction* pref_act;
    //custom :
    KAction* connect_action;
    KAction *disconnect_action;
    KAction *viewlog_action;

    //private methods ::
    void setupActions();
    void toggle_connect_actions_state();
public:
    KAlgDialView *m_view;
    PPPStats *stats;
public slots:
    void if_timer_slot();
    void shutDown();
    void saveMyself();
    /**
     * Called before the window is closed, either by the user or indirectly by the session manager. 
     * The purpose of this function is to prepare the window in a way that it is safe to close it, i.e. 
     * without the user losing some data. In this case we use it to hide the main window in tray icon 
     * instead of close it.
     */
    bool queryClose();
	bool queryExit();
    void viewLog();
    void optionsPreferences();
    void startConnection();
    void endConnection();
};

#endif // _KALGDIAL_H_
