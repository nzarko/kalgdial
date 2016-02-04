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


#include "kalgdial.h"
#include "dockwidget.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

static const char description[] =
    I18N_NOOP("A KDE front end for pon and poff commands");

static const char version[] = "1.0.0";

static KCmdLineOptions options[] =
{
//    { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
    KCmdLineLastOption
};

kalgdial *p_kppp;

int main(int argc, char **argv)
{
    KAboutData about("kalgdial", I18N_NOOP("kalgdial"), version, description,
		     KAboutData::License_GPL, "(C) 2005 Nikos Zarkadas", 0, 0, "nzarko@acn.gr");
    about.addAuthor( "Nikos Zarkadas", 0, "nzarko@acn.gr" );
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions( options );
    KApplication app;
    kalgdial *mainWin = 0;

    if (app.isRestored())
    {
        RESTORE(kalgdial);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        /// @todo do something with the command line args here

        mainWin = new kalgdial();
		
		p_kppp = mainWin;
		(void)new DockWidget(p_kppp, "dockw", p_kppp->stats);
		
        app.setMainWidget( mainWin );
        mainWin->show();

        args->clear();
    }

    // mainWin has WDestructiveClose flag by default, so it will delete itself.
    return app.exec();
}

