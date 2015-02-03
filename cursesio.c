/****************************************************************/
/* Low-level I/O functions of the PCcurses package, 'C' version */
/*                                                              */
/****************************************************************/
/* This version of curses is based on ncurses, a curses version */
/* originally written by Pavel Curtis at Cornell University.    */
/* I have made substantial changes to make it run on IBM PC's,  */
/* and therefore consider myself free make it public domain.    */
/*              Bjorn Larsson (...mcvax!enea!infovax!bl)        */
/****************************************************************/
/* BUT: this particular module was written by                   */
/*      Steve Creps (creps@silver.bacs.indiana.edu)             */
/* It provides 'C' versions of the low-level I/O functions      */
/* that are also available in assembler in cursesio.asm.        */
/* B. Larsson took the liberty of modifying its style slightly  */
/* when incorporating it into PCcurses v.1.2.                   */
/****************************************************************/
/* 1.0: Original, S. Creps:                             880827  */
/* 1.2: Style clean-up, rcsid[] string for maintenance: 881002  */
/* 1.3: MSC -W3, Turbo'C' -w -w-pro checkes:            881005  */
/****************************************************************/

#ifdef __OS2__
# define INCL_SUB
# define INCL_DOS
# include <os2.h>
# include <signal.h>
#else
# include <dos.h>
static union REGS regs;
#endif

#include <curses.h>
#include <curspriv.h>

char _curses_curseio_rcsid[] = "@(#)cursesio.c v1.3 - 881005";

/****************************************************************/
/* _Cursescattr() writes char 'chr' with attributes 'attr' to   */
/* the current cursor location.                                 */
/****************************************************************/

void _cursescattr(chr, attr)
  char  chr;
  char  attr;
  {
#ifdef __OS2__
  USHORT x, y;
  UCHAR buf[2];
  
  buf[0]=chr; buf[1]=attr;
  VioGetCurPos(&y, &x, (HVIO)0);
  VioWrtCellStr(buf, 2, y, x, 0);
#else
  regs.h.ah = 0x09;
  regs.h.al = (unsigned char)chr;
  regs.h.bh = 0x00;
  regs.h.bl = (unsigned char)attr;
  regs.x.cx = 0x01;
  int86(0x10, &regs, &regs);
#endif
  } /* _cursescattr */

/****************************************************************/
/* _Cursescursor() sets the cursor position in video page 0.    */
/* 'row' and 'column' are the cursor address. If 'row' is set   */
/*  to _cursesgrows(), no cursor at all is displayed.           */
/****************************************************************/

void _cursescursor(row, column)
  int   row;
  int   column;
  {
#ifdef __OS2__
  VioSetCurPos(row, column, 0);
#else
  regs.h.ah = 0x02;
  regs.h.bh = 0x00;
  regs.h.dh = (unsigned char)row;
  regs.h.dl = (unsigned char)column;
  int86(0x10, &regs, &regs);
#endif
  }/* _cursescursor */

/****************************************************************/
/* _Cursesgcols() returns the current number of columns on the  */
/* screen.                                                      */
/****************************************************************/

#ifdef __OS2__
static VIOMODEINFO *modeinfo = NULL;
#endif

int _cursesgcols()
  {
#ifdef __OS2__
  if (modeinfo == NULL)
	modeinfo = (VIOMODEINFO *)malloc(sizeof(VIOMODEINFO));

  VioGetMode((PVIOMODEINFO)&modeinfo,0);
  if (modeinfo->col == 0) return 80;
  return modeinfo->col;
#else
  regs.h.ah = 0x0f;
  int86(0x10, &regs, &regs);
  return (int)regs.h.ah;
#endif
  } /* _cursesgcols */

/****************************************************************/
/* _Cursesgrows() returns the current number of rows on the     */
/* screen.                                                      */
/****************************************************************/

int _cursesgrows()
  {
#ifdef __OS2__
  if (modeinfo == NULL)
	modeinfo = (VIOMODEINFO *)malloc(sizeof(VIOMODEINFO));

  VioGetMode(modeinfo,0);
  if (modeinfo->row == 0) return 25;
  return modeinfo->row;
#else
  return 25;
#endif
  } /* _cursesgcols */

/****************************************************************/
/* _Cursesputc() outputs character 'chr' to screen in tty       */
/* fashion. If a colour mode is active, the character is writ-  */
/* ten with colour 'colour'.                                    */
/****************************************************************/

void _cursesputc(chr, color)
  char  chr;
  char  color;
  {
#ifdef __OS2__
  USHORT x, y;

  VioGetCurPos(&y, &x, (HVIO)0);
  VioWrtNAttr(&color,1,y,x,0);
  VioWrtTTY(&chr,1,0);
#else
  regs.h.ah = 0x0e;
  regs.h.al = (unsigned char)chr;
  regs.h.bh = 0x00;
  regs.h.bl = (unsigned char)color;
  int86(0x10, &regs, &regs);
#endif
  } /* _cursesputc */

/****************************************************************/
/* _Cursesscroll() scrolls a window in the current page up or   */
/*  down. Urow, lcol, lrow,rcol are the window coordinates.     */
/* Lines is the number of lines to scroll. If 0, clears the     */
/* window, if < 0 scrolls down, > 0 scrolls up. Blanks areas    */
/* that are left, and sets character attributes to attr. If in  */
/* a colour graphics mode, fills them with the colour 'attr'    */
/* instead.                                                     */
/****************************************************************/

void _cursesscroll(urow, lcol, lrow, rcol, lines, attr)
  int   urow;
  int   lcol;
  int   lrow;
  int   rcol;
  int   lines;
  char  attr;
  {
#ifdef __OS2__
  char  buf[2];
 
  buf[0]=' '; buf[1]=attr;
  if(lines == 0)    /* Clear window */
     {
     lines=-1;
     VioScrollDn(urow,lcol,lrow,rcol,lines,buf,0);
     }
  else if (lines < 0) 
     {
     VioScrollDn(urow,lcol,lrow,rcol,-lines,buf,0);
     }
  else
     {
     VioScrollUp(urow,lcol,lrow,rcol,lines,buf,0);
     }
#else
  if (lines >= 0)
    {
    regs.h.ah = 0x06;
    regs.h.al = (unsigned char)lines;
    } /* if */
  else
    {
    regs.h.ah = 0x07;
    regs.h.al = (unsigned char)(-lines);
    } /* else */
  regs.h.bh = (unsigned char)attr;
  regs.h.ch = (unsigned char)urow;
  regs.h.cl = (unsigned char)lcol;
  regs.h.dh = (unsigned char)lrow;
  regs.h.dl = (unsigned char)rcol;
  int86(0x10, &regs, &regs);
#endif
  } /* _cursesscroll */

/****************************************************************/
/* _Cursesgcmode() returns the current cursor type. Bits 8-15   */
/* of the return value is the start scan row, and bits 0-7 is   */
/* the end scan row.                                            */
/****************************************************************/

int _cursesgcmode()
  {
#ifdef __OS2__
VIOCURSORINFO curinfo;

  VioGetCurType(&curinfo,0);
  return ((curinfo.yStart & 0xFF)*256+(curinfo.cEnd & 0xFF));
#else
  regs.h.ah = 0x03;
  regs.h.bh = 0x00;
  int86(0x10, &regs, &regs);
  return (int)regs.x.cx;
#endif
  } /* _cursesgcmode */

/****************************************************************/
/* _Cursescmode() sets the cursor type to begin in scan line    */
/* startrow and end in scan line endrow. Both values should be  */
/* 0-31.                                                        */
/****************************************************************/

void _cursescmode(startrow, endrow)
  int   startrow;
  int   endrow;
  {
#ifdef __OS2__
VIOCURSORINFO curinfo;

  curinfo.yStart=startrow;
  curinfo.cEnd=endrow;
  curinfo.cx=0;
  curinfo.attr=0;
  VioSetCurType(&curinfo,0);
#else
  regs.h.ah = 0x01;
  regs.h.ch = (unsigned char)startrow;
  regs.h.cl = (unsigned char)endrow;
  int86(0x10, &regs, &regs);
#endif
  } /* _cursescmode */

/****************************************************************/
/* _Curseskey() returns the next key code struck at the key-    */
/* board. If the low 8 bits are 0, the upper bits contain the   */
/* extended character code. If bit 0-7 are non-zero, the upper  */
/* bits = 0.                                                    */
/****************************************************************/

int _curseskey()
  {
#ifdef __OS2__
KBDKEYINFO keyinfo;
  KbdCharIn(&keyinfo,0,0);
  if (keyinfo.chChar != 0)
     return((int)keyinfo.chChar & 0xFF);
  return((int)keyinfo.chScan<<8);
#else
  regs.h.ah = 0x00;
  int86(0x16, &regs, &regs);
  if (regs.h.al != 0)
    return (int)(regs.x.ax & 0x00ff);
  return (int)regs.x.ax;
#endif
  } /* _curseskey */

/****************************************************************/
/* _Curseskeytst() returns 1 if a keyboard character is avail-  */
/* able, 0 otherwise.                                           */
/****************************************************************/

bool _curseskeytst()
  {
#ifdef __OS2__
  if (kbhit())
     return 1;
  else
     return 0;
#else
  regs.h.ah = 0x01;
  int86(0x16, &regs, &regs);
  return ((bool)((regs.x.cflag & 0x40) ? 1 : 0));
#endif
  } /*_curseskeytst */

/****************************************************************/
/* _Cursesgcb() returns 1 if MSDOS BREAK CHECK is on, else 0.   */
/****************************************************************/
#ifdef __OS2__
static int signal_enab = 1;
void FAR null_hand()
{
}
#endif

int _cursesgcb()
  {
#ifdef __OS2__
  return signal_enab;
#else
  regs.h.ah = 0x33;
  regs.h.al = 0x00;
  int86(0x21, &regs, &regs);
  return (int)regs.h.dl;
#endif
  } /* _cursesgcb */

/****************************************************************/
/* _Cursesscb() sets MSDOS BREAK CHECK according to 'setting'.  */
/****************************************************************/

void _cursesscb(setting)
  int setting;
  {
#ifdef __OS2__
static void *old_action;

  if((setting == 0)&&(signal_enab == 1))
     {
     old_action = signal(SIGBREAK, SIG_IGN);
     signal_enab = 0;
     }
  else if ((setting == 1)&&(signal_enab == 0))
     {
     (void) signal(SIGBREAK, old_action);
     signal_enab = 1;
     }
#else
  regs.h.ah = 0x33;
  regs.h.al = 0x00;
  regs.h.dl = (unsigned char)(setting ? 1 : 0);
  int86(0x21, &regs, &regs);
#endif
  } /* _cursesscb */

