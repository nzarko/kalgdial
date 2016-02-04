/* -*- C++ -*-
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id: kpppconfig.h,v 1.29 2003/03/07 22:06:51 waba Exp $
 *
 *            Copyright (C) 1997 Bernd Johannes Wuebben
 *                   wuebben@math.cornell.edu
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


#ifndef _KALGPPPCONFIG_H_
#define _KALGPPPCONFIG_H_

#if defined(__svr4__)
#define STREAMS
#define _XOPEN_SOURCE 1
#define _XOPEN_SOURCE_EXTENDED 1
#define __EXTENSIONS__
#endif

#include <config.h>

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif


// Every PPP_STATS_INTERVAL milli seconds kppp will read
// and display the ppp statistics IF the stats window
// is visible. If the stats window is not visible
// the stats are not taken.
// 200 milli secs is 5 times per second and results in
// 0 load on my machine. Play with this parameter if
// you feel like it.

#define PPP_STATS_INTERVAL 200

#if defined(__linux__) || defined(BSD)
#define PPP_PID_DIR "/var/run"
#else
#define PPP_PID_DIR "/etc/ppp/"
#endif

#endif
