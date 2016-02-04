#ifndef UTILS_H
#define UTILS_H

class PPPStats;

struct pppData
{
		char ppp_pid_path[4096];
		int ppp_id;
		int baud;
		pppData();
};

int pppInterfaceNumber();
void ppp_pid_path ( char *pid_path, int ppp_id );
bool pppdRunning();
void interruptConnection();
#endif
