/*
 *              kPPP: A pppd Front End for the KDE project
 *
 * $Id: docking.cpp,v 1.29 2003/11/27 22:09:44 mlaurent Exp $
 *
 *              Copyright (C) 1997 Bernd Johannes Wuebben
 *                      wuebben@math.cornell.edu
 *
 * This file was contributed by Harri Porten <porten@tu-harburg.de>
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <qtooltip.h>

#include <kapplication.h>
#include <kwin.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kpopupmenu.h>
#include <kmessagebox.h>
#include <kglobal.h>
#include <klocale.h>

#include "dockwidget.h"
#include "kalgdial.h"

#include "pppstats.h"
#include "utils.h"

extern kalgdial* p_kppp;

// static member
DockWidget *DockWidget::dock_widget = 0;

DockWidget::DockWidget(QWidget *parent, const char *name, PPPStats *st)
        : KSystemTray(parent, name), stats(st)
{

    // load pixmaps
    dock_none_pixmap = UserIcon("dock_none");
    dock_left_pixmap = UserIcon("dock_left");
    dock_right_pixmap = UserIcon("dock_right");
    dock_both_pixmap = UserIcon("dock_both");

    // popup menu for right mouse button
    popup_m = contextMenu();
    	
	popup_m->insertItem( SmallIconSet("connect_creating"),  i18n( "Connect" ), p_kppp->m_view, SLOT(connectSlot()));
	popup_m->insertItem(SmallIconSet("connect_no"), i18n("Disconnect"), p_kppp->m_view, SLOT(disconnectSlot()));
	
	popup_m->insertSeparator();
	
	toggleID = popup_m->insertItem(i18n("Restore"),
								   this, SLOT(toggle_window_state()));
	popup_m->insertSeparator();
	popup_m->insertItem(SmallIconSet("exit"), i18n("&Quit"), kapp, SLOT(quit() ));
    //popup_m->insertItem(i18n("Details"), p_kppp, SLOT(showStats()));
		
    // connect to stats for little modem animation
    connect(stats, SIGNAL(statsChanged(int)), SLOT(paintIcon(int)));

    DockWidget::dock_widget = this;
}


DockWidget::~DockWidget()
{
    DockWidget::dock_widget = 0;
}


void DockWidget::paintEvent (QPaintEvent *)
{
    paintIcon(PPPStats::BytesNone);
}


void DockWidget::paintIcon (int status)
{
    // animate modem lights

    const QPixmap *pixmap;

    if(this->isVisible())
    {
        switch(status)
        {
        case PPPStats::BytesBoth:
            pixmap = &dock_both_pixmap;
            break;
        case PPPStats::BytesIn:
            pixmap = &dock_left_pixmap;
            break;
        case PPPStats::BytesOut:
            pixmap = &dock_right_pixmap;
            break;
        case PPPStats::BytesNone:
        default:
            pixmap = &dock_none_pixmap;
            break;
        }

        bitBlt(this, 0, 0, pixmap);
    }
}

void DockWidget::enterEvent( QEvent * )
{
	QString message;
	
	if ( pppdRunning() )
	{
		static int last_bytes_in = 0;
		float baud;
		
		baud = (float)(stats->ibytes - last_bytes_in) ;
		QString baud_info = i18n("%1 kb/sec")
				.arg(KGlobal::locale()->formatNumber((float)baud / 1024.0, 1));
		
		el_time = stats->connectionTime()->toString();
		message = QString ( "Connectd to %1\n Connection Time : %2\n Speed : %3\n")
				.arg(stats->interfaceName()).arg(el_time).arg(baud_info);
		message += QString ( "Bytes in : %1\nBytes out : %2\n").arg(stats->ibytes).arg(stats->obytes);
		QToolTip::add( this, message );
		
		last_bytes_in = p_kppp->stats->ibytes; 
	}
	else
		QToolTip::add( this, QString("Disconnected!\n Connection last :%1").arg(el_time));
}

void DockWidget::take_stats()
{
    if (isVisible())
    {
        stats->initStats();
        stats->start();
    }
}


void DockWidget::stop_stats()
{
    stats->stop();
}


void DockWidget::mousePressEvent(QMouseEvent *e)
{
    // open/close connect-window on right mouse button
    if ( e->button() == LeftButton )
    {
        toggle_window_state();
    }

    // open popup menu on left mouse button
    if ( e->button() == RightButton )
    {
        QString text;
        if(p_kppp->isVisible())
            text = i18n("Minimize");
        else
            text = i18n("Restore");

        popup_m->changeItem(text, toggleID);
        popup_m->popup(e->globalPos());
        popup_m->exec();
    }
}


void DockWidget::toggle_window_state()
{
    // restore/hide connect-window
    if(p_kppp != 0L)
    {
        if (p_kppp->isVisible())
            p_kppp->hide();
        else
        {
            p_kppp->show();
            KWin::activateWindow(p_kppp->winId());
        }
    }
}


#include "dockwidget.moc"
