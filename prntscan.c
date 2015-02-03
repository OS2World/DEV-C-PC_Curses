/****************************************************************/
/* Printw() and scanw() routines of the PCcurses package	*/
/*								*/
/****************************************************************/
/* This version of curses is based on ncurses, a curses version	*/
/* originally written by Pavel Curtis at Cornell University.	*/
/* I have made substantial changes to make it run on IBM PC's,	*/
/* and therefore consider myself free to make it public domain.	*/
/*		Bjorn Larsson (...mcvax!enea!infovax!bl)	*/
/****************************************************************/
/*		     IMPLEMENTATION NOTE			*/
/* These routines make a local copy of their parameter stack,	*/
/* assuming at most 5 'double' arguments were passed (== 40	*/
/* bytes == 20 int's == 10 long's == 10-20 pointers {depending	*/
/* on memory model}, etc). This means the invocation of the	*/
/* routines themselves requires at least 80 bytes of stack just	*/
/* for the parameters, and the vsprintf() and vscanf() functions	*/
/* will require more. Therefore, this module should be compiled	*/
/* with stack checking on to avoid stack overflow errors.	*/
/****************************************************************/
/* 1.0:	Release:					870515	*/
/* 1.2:	Rcsid[] string for maintenance:			881002	*/
/* 1.3:	MSC -W3, Turbo'C' -w -w-pro checkes:		881005	*/
/****************************************************************/

#include <curses.h>
#include <curspriv.h>
#include <stdarg.h>

char _curses_prntscan_rcsid[] = "@(#)prntscan.c v1.3 - 881005";

static	int	pblen();		/* gets length of buffer */
static	char	printscanbuf[513];	/* buffer used during I/O */

/****************************************************************/
/* Wprintw(win,fmt,args) does a printf() in window 'win'.	*/
/****************************************************************/

int	wprintw(WINDOW *win, char *fmt, ...)
  {
  va_list args;

  va_start(args, fmt);
  vsprintf(printscanbuf,fmt,args);
  va_end(args);

  if (waddstr(win,printscanbuf) == ERR)
    return(ERR);
  return(pblen());
  } /* wprintw */

/****************************************************************/
/* Printw(fmt,args) does a printf() in stdscr.			*/
/****************************************************************/

int	printw(char *fmt, ...)
  {
  va_list args;

  va_start(args, fmt);
  vsprintf(printscanbuf,fmt,args);
  va_end(args);

  if(waddstr(stdscr,printscanbuf) == ERR)
    return(ERR);
  return(pblen());
  } /* printw */

/****************************************************************/
/* Mvprintw(fmt,args) moves the stdscr cursor to a new posi-	*/
/* tion, then does a printf() in stdscr.			*/
/****************************************************************/

int	mvprintw(int y, int x, char *fmt, ...)
  {
  va_list args;

  if (wmove(stdscr,y,x) == ERR)
    return(ERR);

  va_start(args, fmt);
  vsprintf(printscanbuf,fmt,args);
  va_end(args);

  if(waddstr(stdscr,printscanbuf) == ERR)
    return(ERR);
  return(pblen());
  } /* mvprintw */

/****************************************************************/
/* Mvwprintw(win,fmt,args) moves the window 'win's cursor to	*/
/* a new position, then does a printf() in window 'win'.	*/
/****************************************************************/

int	mvwprintw(WINDOW *win, int y, int x, char *fmt, ...)
  {
  va_list args;

  if (wmove(win,y,x) == ERR)
    return(ERR);

  va_start(args, fmt);
  vsprintf(printscanbuf,fmt,args);
  va_end(args);

  if(waddstr(win,printscanbuf) == ERR)
    return(ERR);
  return(pblen());
  } /* mvwprintw */

/****************************************************************/
/* Wscanw(win,fmt,args) gets a string via window 'win', then	*/
/* scans the string using format 'fmt' to extract the values	*/
/* and put them in the variables pointed to the arguments.	*/
/****************************************************************/

int wscanw(WINDOW *win, char *fmt, ...)
  {
  int count;
  va_list args;

  wrefresh(win);				/* set cursor */
  if (wgetstr(win,printscanbuf) == ERR)		/* get string */
    return(ERR);

  va_start(args, fmt);
  count = vscanf(printscanbuf,fmt,args);
  va_end(args);

  return(count);
  } /* wscanw */

/****************************************************************/
/* Scanw(fmt,args) gets a string via stdscr, then scans the	*/
/* string using format 'fmt' to extract the values and put them	*/
/* in the variables pointed to the arguments.			*/
/****************************************************************/

int scanw(char *fmt, ...)
  {
  int count;
  va_list args;

  wrefresh(stdscr);				/* set cursor */
  if (wgetstr(stdscr,printscanbuf) == ERR)	/* get string */
    return(ERR);

  va_start(args, fmt);
  count = vscanf(printscanbuf,fmt,args);
  va_end(args);

  return(count);
  } /* scanw */

/****************************************************************/
/* Mvscanw(y,x,fmt,args) moves stdscr's cursor to a new posi-	*/
/* tion, then gets a string via stdscr and scans the string	*/
/* using format 'fmt' to extract the values and put them in the	*/
/* variables pointed to the arguments.				*/
/****************************************************************/

int mvscanw(int y, int x, char *fmt, ...)
  {
  int count;
  va_list args;

  if (wmove(stdscr,y,x) == ERR)
    return(ERR);
  wrefresh(stdscr);				/* set cursor */
  if (wgetstr(stdscr,printscanbuf) == ERR)	/* get string */
    return(ERR);

  va_start(args, fmt);
  count = vscanf(printscanbuf,fmt,args);
  va_end(args);

  return(count);
  } /* mvscanw */

/****************************************************************/
/* Mvwscanw(win,y,x,fmt,args) moves window 'win's cursor to a	*/
/* new position, then gets a string via 'win' and scans the	*/
/* string using format 'fmt' to extract the values and put them	*/
/* in the variables pointed to the arguments.			*/
/****************************************************************/

int mvwscanw(WINDOW *win, int y, int x, char *fmt, ...)
  {
  int count;
  va_list args;

  if (wmove(win,y,x) == ERR)
    return(ERR);
  wrefresh(win);				/* set cursor */
  if (wgetstr(win,printscanbuf) == ERR)		/* get string */
    return(ERR);

  va_start(args, fmt);
  count = vscanf(printscanbuf,fmt,args);
  va_end(args);

  return(count);
  } /* mvwscanw */

/****************************************************************/
/* Pblen() returns the length of the string in printscanbuf.	*/
/****************************************************************/

static	int pblen()
  {
  char *p = printscanbuf;
  
  while(*p++);
  return(p-printscanbuf-1);
  } /* plben */
