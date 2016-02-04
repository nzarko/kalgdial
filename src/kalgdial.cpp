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
#include <qtimer.h>
#include <qcolor.h>
#include <qdatetime.h>

#include <kaction.h>
#include <kstdaction.h>
#include <kstdaccel.h>
#include <kshortcut.h>
#include <kmainwindow.h>
#include <klocale.h>
#include <kapplication.h>
#include <kmessagebox.h>
#include <kiconloader.h>
#include <ktextbrowser.h>
#include <kpushbutton.h>
#include <kdebug.h>
#include <kconfig.h>
#include <kdeversion.h>
#include <kstatusbar.h>
#include <krun.h>

#include "kalgdial.h"
#include "pppstats.h"
#include "dockwidget.h"
#include "utils.h"
#include "loginfo.h"
#include "loginfoview.h"

QString kalgdial::conStartTime = QString::null;
QString kalgdial::conEndTime = QString::null;

kalgdial::kalgdial()
    : KMainWindow( 0, "kalgdial" )
{
    // set the shell's ui resource file
    setXMLFile("kalgdialui.rc");
	m_view = new KAlgDialView (this, 0);
	setCentralWidget( m_view );
	this->setFixedSize( m_view->width() + 1, m_view->height() + 1 );
	
	// add a status bar
	statusBar()->show();
	setupActions();
	// Apply the create the main window and ask the mainwindow to
		// automatically save settings if changed: window size, toolbar
    // position, icon size, etc.  Also to add actions for the statusbar
		// toolbar, and keybindings if necessary.
	setupGUI();
	
	stats = new PPPStats;
	if_timer = new QTimer(this);
	connect( if_timer, SIGNAL( timeout() ), SLOT( if_timer_slot() ) );
	
	connect_action->setEnabled( TRUE );
	disconnect_action->setEnabled( FALSE );
	
	if ( pppdRunning() )
	{
		QString str_info =  i18n("Connection established at %1").arg(QDateTime::currentDateTime().toString()) ;
		m_view->m_textInfo->append( str_info );
		if ( !stats->ifIsUp() )
			kdDebug() << "Interface seems to be down but we continue !" << endl;
		
		stats->initStats();
		stats->start();
		
		//Show more info to user
		str_info = i18n ( "Local IP Address : %1 \nRemote IP Address : %2\n")
				.arg(stats->local_ip_address)
				.arg(stats->remote_ip_address);
		m_view->m_textInfo->append( str_info );
		
		m_view->isConnected = true;
		toggle_connect_actions_state();
		if_timer->start(2000, TRUE);
		m_view->startTimer(1000);
	}

	connect(KApplication::kApplication(), SIGNAL(saveYourself()),
			this, SLOT(saveMyself()));
	connect(KApplication::kApplication(), SIGNAL(shutDown()),
			this, SLOT(shutDown()));
	connect(m_view, SIGNAL (beginDocking()),
			this, SLOT(startConnection()));
	connect(m_view, SIGNAL(pppDisconnect()),
			this, SLOT(endConnection()) );
}

kalgdial::~kalgdial()
{
}

/*!
    \fn kalgdial::setupActions()
 */
 void kalgdial::setupActions()
{	
	using namespace KStdAction;
	quit_act = quit( kapp, SLOT(quit() ), actionCollection() );
	pref_act = preferences( this, SLOT (optionsPreferences() ), actionCollection() );

	connect_action = new KAction(i18n("Connect "), "connect_creating",
								 KShortcut::null(), m_view, 
								 SLOT(connectSlot()),
								 actionCollection(), "connect_action");
	disconnect_action = new KAction(i18n("Disconnect"),"connect_no",
									KShortcut::null(), m_view, 
									SLOT(disconnectSlot()), actionCollection(), "disconnect_action");
	viewlog_action = new KAction ( i18n ( "View Log"), "view_text",
								   KShortcut::null(), this,
								   SLOT( viewLog() ), actionCollection(), 
								   "viewlog_action" );
}

void kalgdial::toggle_connect_actions_state()
{
	if ( connect_action->isEnabled() )
		connect_action->setEnabled( FALSE );
	else connect_action->setEnabled( TRUE );
	
	if ( disconnect_action->isEnabled() )
		disconnect_action->setEnabled( FALSE );
	else disconnect_action->setEnabled( TRUE );
}

/*!
    \fn kalgdial::startConnection()
 */
void kalgdial::startConnection()
{
	QString str_info =  i18n("Connection established at %1").arg( QDateTime::currentDateTime().toString()) ;
		kalgdial::conStartTime = QTime::currentTime().toString();
		
		m_view->m_textInfo->append( str_info );
		if ( !stats->ifIsUp() )
			kdDebug() << "Interface seems to be down but we continue !" << endl;
		
		stats->initStats();
		stats->start();
		
		//Show more info to user
		str_info = QString ( "Local IP Address : %1 \nRemote IP Address : %2\n")
				.arg(stats->local_ip_address)
				.arg(stats->remote_ip_address);
		m_view->m_textInfo->append( str_info );
		
		m_view->isConnected = true;
		toggle_connect_actions_state();
		if_timer->start(2000, TRUE);
}


/*!
    \fn kalgdial::endConnection()
 */
void kalgdial::endConnection()
{
	QString str_info = i18n("Connection terminated at %1").arg (QDateTime::currentDateTime().toString()) ;
	kalgdial::conEndTime = QTime::currentTime().toString();
	
	m_view->m_textInfo->append( str_info );
	
	LogInfo loginfo(stats);
	if ( loginfo.write() )
	{
		str_info = i18n("Log file wrote");
		m_view->colorMessage(Qt::blue, str_info);
	}
	else
	{
		str_info = i18n("Error!!! Can't write to log file");
		m_view->colorMessage(Qt::red, str_info);
	}
	stats->stop();
	DockWidget::dock_widget->stop_stats();

        // Update ui.
	m_view->isConnected = false;
	toggle_connect_actions_state();
}

/**
 * \fn kzdial::if_timer_slot()
 */
 void kalgdial::if_timer_slot()
{
	DockWidget::dock_widget->show();
	DockWidget::dock_widget->take_stats();
	this->hide();
}

/*!
    \fn kalgdial::shutDown()
 */
void kalgdial::shutDown()
{
    /// @todo implement me
}

/*!
    \fn kalgdial::saveMyself()
 */
void kalgdial::saveMyself()
{
    /// @todo implement me
}

bool kalgdial::queryClose()
{
	KMessageBox::information( this,
							  i18n( "<qt>Closing the main-window will keep kalgdial running in the System Tray. "
									  "Use <B>Quit</B> from the menu, or the kalgdial tray-icon to exit the application.</qt>" ),
							  i18n( "Docking in System Tray" ), "hideOnCloseInfo" );
	DockWidget::dock_widget->show();
	this->hide();
	
	return false;
}

bool kalgdial::queryExit()
{
	if ( pppdRunning() )
	{
		if ( KMessageBox::questionYesNo(this, i18n("There is an interface online. Do you want to shutdown this interface?"),
			 i18n("Question")) == KMessageBox::Yes )
			interruptConnection();
		else
			KMessageBox::information(this, i18n("You can shutdown your connection with poff command!"), 
									 i18n("Information"));
	}
	return true;
}

/*!
    \fn kalgdial::viewLog()
 */
 void kalgdial::viewLog()
{
	LogInfo *log_info = new LogInfo(stats);
	LogInfoView *liv = new LogInfoView(log_info);
	liv->showNormal();
}

/*!
    \fn kalgdial::optionsPreferences()
 */
void kalgdial::optionsPreferences()
{
    /// @todo implement me
}



#include "kalgdial.moc"



