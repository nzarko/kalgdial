#include "utils.h"
#include "kalgpppconfig.h"
#include "pppstats.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <krun.h>
#include <kdebug.h>

using namespace std;

pppData::pppData()
{
}

/**
 * \fn pppInterfaceNumber()
		* Finds the interface number.
		* @return The ppp interface number. E.g if the active interface is ppp0 then 
		* this function return 0.
 */
 int pppInterfaceNumber()
{
	char ifname[4096];
	struct stat st;
	sleep(1);
	for(int i = 0; i < 8; i++)
	{
		snprintf(ifname, sizeof(ifname), "%s/ppp%d.pid", PPP_PID_DIR, i);
		if(!stat(ifname, &st))
		{
			kdDebug( ) << "pppInterfaceNumber()::ifname = " << ifname << endl;
			return i;
		}
	}

    // panic! we return 0 and hope that's ok :)
	fprintf(stderr, "pppInterfaceNumber: cannot detect ppp interface number!\n");
	return 0;
}


/**
 * \fn ppp_pid_path ( char *pid_path , int ppp_id)
		* Retrieve the full path of interface pid file and save it to \b pid_path.
		* @param pid_path : Contains the full path of the interface pid file.
		* @param ppp_id : The ppp interface number.
 * For better performance call this function like this :
		* \code
			   char buff[4096];
			   ppp_pid_path(buff, pppInterfaceNumber() );
		* \endcode
 */
void ppp_pid_path( char *pid_path, int ppp_id )
{
	char ifname[4096];
	snprintf(ifname, sizeof(ifname), "%s/ppp%d.pid", PPP_PID_DIR, ppp_id);
	strncpy(pid_path, ifname, sizeof (ifname ));
}

/**
 * \fn pppdRunning()
		* @return : true if pppd is running or false if pppd is not running.
 */

bool pppdRunning()
{
	bool is_running = false;
	
	char ifname[4096];
	char ifname_up[4096];
	char tmp_pid_path[4096];
	struct stat st;
	
	int ppp_id = pppInterfaceNumber();
	ppp_pid_path(tmp_pid_path, ppp_id );
	
	strncpy ( ifname, tmp_pid_path, sizeof ( ifname ) );
	
	if ( !stat ( ifname, &st ))
	{
		snprintf ( ifname_up, sizeof ( ifname_up ), "%s/ppp%d%s", PPP_PID_DIR, ppp_id, "-up");
		kdDebug() << "pppdRunning() : ifname_up = " << ifname_up << endl;
		if ( st.st_size ) //Does it contains anything?
		{
			if ( (access ( ifname_up, F_OK ) == 0 ) )
			{
				kdDebug() << " pppd is running " << endl;
				kdDebug() << "With pid = " << ppp_id << endl;
				is_running = true;
			}
			else 
			{
				kdDebug() << "It seems that " << ifname_up << " doesn't exist." << endl;
				kdDebug () << "Let's try another way!" << endl;
				struct stat stat_up;
				mode_t modes;
				
				stat ( ifname_up, &stat_up );
				modes = stat_up.st_mode;
				if ( !S_ISREG(modes) ) //if there isn't a regular file then we are not online.
					is_running = false;
				else is_running = true;
			}
		}
		else 
		{
			kdDebug() << " pppd is not running " << endl;
			is_running = false;
		}	
	}
	return is_running;
}	

/**
 * \fn interruptConnection()
 * Terminates an existing connection.
 * This function is called by the application when something goes 
 * wrong and there is no way to fix the situation.
 */
void interruptConnection()
{
	KRun::runCommand("/usr/bin/poff");
}

