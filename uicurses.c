/**
 * nmond_ui_curses.c -- Ncurses user interface elements of nmond
 *  Copyright (c) 2015 Christopher Stoll (https://github.com/stollcri)
 *   (for license, see included LICENSE file)
 */

#include "uicurses.h"

#define COLOUR if(usecolor) /* Only use this for single line color curses calls */

static inline void uibanner(int cols, WINDOW *pad, char *string)
{
	mvwhline(pad, 0, 0, ACS_HLINE, cols-2);
	wmove(pad, 0, 0);
	wattron(pad, A_STANDOUT);
	wprintw(pad, " ");
	wprintw(pad, string);
	wprintw(pad, " ");
	wattroff(pad, A_STANDOUT);
}


static inline void uidisplay(int *xin, int cols, int rows, WINDOW *pad, int lines)
{
	int x = *xin;

	if(x+2+(rows) > lines) {
		pnoutrefresh(pad, 0, 0, x, 1, lines-2, cols-2);
	} else {
		pnoutrefresh(pad, 0, 0, x, 1, x+rows+1, cols-2);
	}

	x=x+(rows);
	if(x+4 > lines) {
		mvwprintw(stdscr, lines-1, 10, MSG_WRN_NOT_SHOWN);
	}

	*xin = x;
}

inline void uiheader(int *xin, int usecolor, int useblink, char *version, char *hostname, double elapsed, time_t timer)
{
	int x = *xin;
	struct tm *tim = localtime(&timer);

	box(stdscr, 0, 0);
	mvprintw(x, 2, "nmond");
	mvprintw(x, 8, "%s", version);
	if(useblink) {
		mvprintw(x, 14,"[H for help]");
	}
	mvprintw(x, 29, "%s", hostname);
	mvprintw(x, 52, "Refresh=%2.0fsecs ", elapsed);
	mvprintw(x, 70, "%02d:%02d.%02d", tim->tm_hour, tim->tm_min, tim->tm_sec);
	wnoutrefresh(stdscr);

	*xin = x + 1;
}

void uiwelcome(WINDOW **padwelcomein, int *xin, int cols, int rows, int usecolor, struct syshw hw)
{
	WINDOW *padwelcome = *padwelcomein;
	if (padwelcome == NULL) {
		return;
	}
	int x = *xin;

	if(usecolor) {
		wattrset(padwelcome, COLOR_PAIR(2));
	}
	mvwprintw(padwelcome, x+1, 1, "                                         __");
	mvwprintw(padwelcome, x+2, 1, "    ____   ____ ___   ____   ____   ____/ /");
	mvwprintw(padwelcome, x+3, 1, "   / __ \\ / __ `__ \\ / __ \\ / __ \\ / __  / ");
	mvwprintw(padwelcome, x+4, 1, "  / / / // / / / / // /_/ // / / // /_/ /  ");
	mvwprintw(padwelcome, x+5, 1, " /_/ /_//_/ /_/ /_/ \\____//_/ /_/ \\____/   ");
	mvwprintw(padwelcome, x+6, 1, "=======================================    ");
	if(usecolor) {
		wattrset(padwelcome, COLOR_PAIR(0));
	}
	mvwprintw(padwelcome, x+1, 45, "For help type H or ...");
	mvwprintw(padwelcome, x+2, 45, " nmond -?  - hint");
	mvwprintw(padwelcome, x+3, 45, " nmond -h  - full");
	mvwprintw(padwelcome, x+5, 45, "To start the same way every time");
	mvwprintw(padwelcome, x+6, 45, " set the NMOND shell variable");
	if(usecolor) {
		wattrset(padwelcome, COLOR_PAIR(1));
	}
	mvwprintw(padwelcome, x+8,  2, "%s", hw.model);
	mvwprintw(padwelcome, x+9,  2, "%s", hw.cpubrand);
	mvwprintw(padwelcome, x+10, 2, "CPU Count    = %2d   Physcal Cores = %d", hw.cpucount, hw.physicalcpucount);
	mvwprintw(padwelcome, x+11, 2, "Hyperthreads = %2d   Virtual CPUs  = %d", hw.hyperthreads, hw.logicalcpucount);
	if(usecolor) {
		wattrset(padwelcome, COLOR_PAIR(0));
	}
	mvwprintw(padwelcome, x+15, 3, "Use these keys to toggle statistics on/off:");
	mvwprintw(padwelcome, x+16, 3, "   c = CPU        l = CPU Long-term   - = Faster screen updates");
	mvwprintw(padwelcome, x+17, 3, "   m = Memory     j = Filesystems     + = Slower screen updates");
	mvwprintw(padwelcome, x+18, 3, "   d = Disks      n = Network         V = Virtual Memory");
	mvwprintw(padwelcome, x+19, 3, "   r = Resource   N = NFS             v = Verbose hints");
	mvwprintw(padwelcome, x+20, 3, "   k = kernel     t = Top-processes   . = only busy disks/procs");
	mvwprintw(padwelcome, x+21, 3, "   h = more options                   q = Quit");
	pnoutrefresh(padwelcome, 0, 0, x, 1, rows-2, cols-2);
	wnoutrefresh(stdscr);
	
	*xin = x + 22;
}

void uihelp(WINDOW **padhelpin, int *xin, int cols, int rows)
{
	WINDOW *padhelp = *padhelpin;
	if (padhelp == NULL) {
		return;
	}
	int x = *xin;
	uibanner(cols, padhelp, "HELP");
	mvwprintw(padhelp,  1, 5, "key  --- statistics which toggle on/off ---");
	mvwprintw(padhelp,  2, 5, "h = This help information");
	mvwprintw(padhelp,  3, 5, "r = RS6000/pSeries CPU/cache/OS/kernel/hostname details + LPAR");
	mvwprintw(padhelp,  4, 5, "t = Top Process Stats 1=basic 3=CPU");
	mvwprintw(padhelp,  5, 5, "    u = shows command arguments (hit twice to refresh)");
	mvwprintw(padhelp,  6, 5, "c = CPU by processor             l = longer term CPU averages");
	mvwprintw(padhelp,  7, 5, "m = Memory & Swap stats L=Huge   j = JFS Usage Stats");
	mvwprintw(padhelp,  8, 5, "n = Network stats                N = NFS");
	mvwprintw(padhelp,  9, 5, "d = Disk I/O Graphs D=Stats      o = Disks %%Busy Map");
	mvwprintw(padhelp, 10, 5, "k = Kernel stats & loadavg       V = Virtual Memory");
	mvwprintw(padhelp, 11, 5, "g = User Defined Disk Groups [start nmon with -g <filename>]");
	mvwprintw(padhelp, 12, 5, "v = Verbose Simple Checks - OK/Warnings/Danger");
	mvwprintw(padhelp, 13, 5, "b = black & white mode");
	mvwprintw(padhelp, 14, 5, "--- controls ---");
	mvwprintw(padhelp, 15, 5, "+ and - = double or half the screen refresh time");
	mvwprintw(padhelp, 16, 5, "q = quit                     space = refresh screen now");
	mvwprintw(padhelp, 17, 5, ". = Minimum Mode =display only busy disks and processes");
	mvwprintw(padhelp, 18, 5, "0 = reset peak counts to zero (peak = \">\")");
	mvwprintw(padhelp, 19, 5, "Chrisotpher Stoll, 2015 (https://github.com/stollcri)");
	pnoutrefresh(padhelp, 0, 0, x, 1, rows-2, cols-2);
	uidisplay(&x, cols, 20, padhelp, rows);
	
	*xin = x;
}

void uisys(WINDOW **padsysin, int *xin, int cols, int rows, struct syskern kern)
{
	WINDOW *padsys = *padsysin;
	if (padsys == NULL) {
		return;
	}
	int x = *xin;
	uibanner(cols, padsys, "Kernel and Processor Details");
	mvwprintw(padsys, 1, 2, "%s", kern.version);
	mvwprintw(padsys, 2, 2, "Release  : %s", kern.osrelease);
	mvwprintw(padsys, 3, 2, "Version  : %s", kern.osversion);
	mvwprintw(padsys, 4, 2, "# of CPUs: %d", kern.corecount);
	mvwprintw(padsys, 5, 2, "Nodename : %s", kern.hostname);
	mvwprintw(padsys, 6, 2, "Domain   : %s", kern.domainname);
	mvwprintw(padsys, 7, 2, "Booted   : %s", kern.boottimestring);
	uidisplay(&x, cols, 8, padsys, rows);
	
	*xin = x;
}

void uiverbose(WINDOW **padverbin, int *xin, int cols)
{
	WINDOW *padverb = *padverbin;
	if (padverb == NULL) {
		return;
	}
	int x = *xin;

	uibanner(cols, padverb, "Verbose Mode");
	mvwprintw(padverb, 1, 0, " Code    Resource            Stats   Now\tWarn\tDanger ");

	*xin = x + 6;
}






void plot_smp(WINDOW *pad, int cpuno, int row, int usecolor, double user, double sys, double wait, double idle, double steal)
{
	int	i;
	int	peak_col;
		
	// if(cpu_peak[cpuno] < (user + kernel + iowait) )
	// 	cpu_peak[cpuno] = (double)((int)user/2 + (int)kernel/2 + (int)iowait/2)*2.0;
	
	if(cpuno == 0) {
		mvwprintw(pad,row, 0, "Avg");
	} else {
		mvwprintw(pad,row, 0, "%3d", cpuno);
	}
	mvwprintw(pad,row,  4, "%2.2f", user);
	mvwprintw(pad,row, 10, "%2.2f", sys);
	mvwprintw(pad,row, 16, "%2.2f", wait);
	if(steal) {
		mvwprintw(pad,row, 22, "%2.2f", steal);
	} else {
		mvwprintw(pad,row, 22, "%2.2f", idle);
	}

	mvwprintw(pad,row, 27, "|");

	wmove(pad,row, 28);
	int userquant = (int)(user / 2);
	int systquant = (int)(sys / 2);
	int waitquant = (int)(wait / 2);
	for(int i=28; i<=77; ++i){
		if(userquant) {
			if(usecolor) {
				wattrset(pad,COLOR_PAIR(10));
				wprintw(pad,"|");
			} else {
				wprintw(pad,"U");
			}
			--userquant;
		} else {
			if(systquant) {
				if(usecolor) {
					wattrset(pad,COLOR_PAIR(8));
					wprintw(pad,"|");
				} else {
					wprintw(pad,"S");
				}
				--systquant;
			} else {
				if(systquant) {
					if(usecolor) {
						wattrset(pad,COLOR_PAIR(9));
						wprintw(pad,"|");
					} else {
						wprintw(pad,"W");
					}
					--systquant;
				} else {
					wattrset(pad,COLOR_PAIR(0));
					wprintw(pad,"=");
				}
			}
		}
	}
	/*
	for (i = 0; i < (int)(user / 2); i++){
		if(usecolor) {
			wattrset(pad,COLOR_PAIR(9));
		}
		wprintw(pad,"U");
	}
	for (i = 0; i < (int)(kernel / 2); i++){
		if(usecolor) {
			wattrset(pad,COLOR_PAIR(8));
		}
		wprintw(pad,"S");
	}
	for (i = 0; i < (int)(iowait / 2); i++) {
		if(usecolor) {
			wattrset(pad,COLOR_PAIR(8));
		}
		wprintw(pad,"W");
	}
	if(usecolor) {
		wattrset(pad,COLOR_PAIR(0));
	}
	for (i = 0; i <= (int)(idle / 2); i++) {  // added "=" to try to conteract missing halves
			wprintw(pad," ");
	}
	for (i = 0; i < (int)((steal+1)  / 2); i++) {
		if(usecolor) {
			wattrset(pad,COLOR_PAIR(5));
		}
		wprintw(pad,"S");
	}
	*/
	if(usecolor) {
		wattrset(pad,COLOR_PAIR(0));
	}
	mvwprintw(pad,row, 77, "| ");
	
	// peak_col = 28 +(int)(cpu_peak[cpuno]/2);
	// if(peak_col > 77)
	// 	peak_col=77;
	// mvwprintw(pad,row, peak_col, ">");
}

void uicpu(WINDOW **padsmpin, int *xin, int cols, int rows, int usecolor, struct sysres res, int show_raw)
{
	WINDOW *padsmp = *padsmpin;
	if (padsmp == NULL) {
		return;
	}
	int x = *xin;

	double cpu_busy = res.busy;
	double cpu_scaled_user = res.percentuser;
	double cpu_scaled_sys = res.percentsys;
	double cpu_scaled_wait = 0;
	double cpu_scaled_idle = res.percentidle;
	double cpu_scaled_steal = 0;

	uibanner(cols, padsmp, "CPU Utilisation");
	// BANNER(padsmp,"CPU Utilisation");
	// /* mvwprintw(padsmp,1, 0, cpu_line);*/
	// /*
	//  *mvwprintw(padsmp,2, 0, "CPU  User%%  Sys%% Wait%% Idle|0          |25         |50          |75       100|");
	//  */
	char cpu_line[78] = "---------------------------+-------------------------------------------------+";
	mvwprintw(padsmp, 1, 0, cpu_line);
	mvwprintw(padsmp, 2, 0, "CPU  ");
	if(usecolor) {
		wattrset(padsmp, COLOR_BLUE);
	}
	mvwprintw(padsmp, 2, 4, "User%%");
	if(usecolor) {
		wattrset(padsmp, COLOR_RED);
	}
	mvwprintw(padsmp, 2, 10, "Sys %%");
	if(usecolor) {
		wattrset(padsmp, COLOR_GREEN);
	}
	mvwprintw(padsmp, 2, 16, "Wait%%");
	if(usecolor) {
		wattrset(padsmp, COLOR_PAIR(0));
	}
	mvwprintw(padsmp, 2, 22, "Idle");
	if(usecolor) {
		wattrset(padsmp, COLOR_PAIR(0));
	}
	mvwprintw(padsmp, 2, 27, "|0          |25         |50          |75       100|");

	int i = 0;
	for (i = 0; i < res.count; i++) {
	 	mvwprintw(padsmp,3 + i, 77, "|");
		// if(!show_raw)
			plot_smp(padsmp,i+1, 3 + i, usecolor,
				cpu_scaled_user,cpu_scaled_sys, cpu_scaled_wait,
				cpu_scaled_idle, cpu_scaled_steal);
		// else
			// save_smp(padsmp,i+1, 3+i,
			// 		 RAW(user) - RAW(nice),
			// 		 RAW(sys),
			// 		 RAW(wait),
			// 		 RAW(idle),
			// 		 RAW(nice),
			// 		 RAW(irq),
			// 		 RAW(softirq),
			// 		 RAW(steal));
	}
	mvwprintw(padsmp,i + 3, 0, cpu_line);
	
	if (res.count > 1) {
		// 	if(!show_raw) {
				plot_smp(padsmp,0, 4 + i, usecolor,
					cpu_scaled_user, cpu_scaled_sys, cpu_scaled_wait,
					cpu_scaled_idle, cpu_scaled_steal);
		// 	} else {
		// 		save_smp(padsmp,0, 4+i,
		// 				 RAWTOTAL(user) - RAWTOTAL(nice),
		// 				 RAWTOTAL(sys),
		// 				 RAWTOTAL(wait),
		// 				 RAWTOTAL(idle),
		// 				 RAWTOTAL(nice),
		// 				 RAWTOTAL(irq),
		// 				 RAWTOTAL(softirq),
		// 				 RAWTOTAL(steal));
		// 	}
		
		mvwprintw(padsmp, i + 5, 0, cpu_line);
		i = i + 2;
	}
	uidisplay(&x, cols, (i+4), padsmp, rows);

	*xin = x;
}
