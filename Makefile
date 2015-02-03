HDIR=c:/watcom/h
LIBDIR=c:/watcom/lib386/os2

LIBRARY=curses$A

CFLAGS+=-omaxet -zp4 -4r -fpi87 -i=.
#CFLAGS+=-d2 -i=.

HDRS=	curses.h curspriv.h

OBJS=   attrib$O   beep$O     border$O   boxes$O     \
	charadd$O  chardel$O  charget$O  charins$O   \
	charpick$O clrtobot$O clrtoeol$O endwin$O    \
	initscr$O  linedel$O  lineins$O  longname$O  \
	move$O     mvcursor$O newwin$O   options$O   \
	overlay$O  prntscan$O refresh$O  scrreg$O    \
	setmode$O  setterm$O  stradd$O   strget$O    \
	tabsize$O  termmisc$O unctrl$O   update$O    \
 	winclear$O windel$O   winerase$O winmove$O   \
 	winscrol$O wintouch$O cursesio$O getch$O     \
	kbhit$O

all: $(LIBRARY)

$(LIBRARY): $(LIBRARY)({$(OBJS)})

install: "$(LIBDIR)/$(LIBRARY)"

"$(LIBDIR)/$(LIBRARY)": $(LIBRARY)
	cp -p $(LIBRARY) $(LIBDIR)
	cp -p $(HDRS) $(HDIR)

clean:
	-rm -f $(OBJS) $(LIBRARY) *.lbc *.i *.out *.err

# Dependencies for individual modules

$(LIBRARY)(attrib$O):     $**.c $(HDRS)
$(LIBRARY)(beep$O):       $**.c $(HDRS)
$(LIBRARY)(border$O):     $**.c $(HDRS)
$(LIBRARY)(boxes$O):      $**.c $(HDRS)
$(LIBRARY)(charadd$O):    $**.c $(HDRS)
$(LIBRARY)(chardel$O):    $**.c $(HDRS)
$(LIBRARY)(charget$O):    $**.c $(HDRS)
$(LIBRARY)(charins$O):    $**.c $(HDRS)
$(LIBRARY)(charpick$O):   $**.c $(HDRS)
$(LIBRARY)(clrtobot$O):   $**.c $(HDRS)
$(LIBRARY)(clrtoeol$O):   $**.c $(HDRS)
$(LIBRARY)(endwin$O):     $**.c $(HDRS)
$(LIBRARY)(initscr$O):    $**.c $(HDRS)
$(LIBRARY)(linedel$O):    $**.c $(HDRS)
$(LIBRARY)(lineins$O):    $**.c $(HDRS)
$(LIBRARY)(longname$O):   $**.c $(HDRS)
$(LIBRARY)(move$O):       $**.c $(HDRS)
$(LIBRARY)(mvcursor$O):   $**.c $(HDRS)
$(LIBRARY)(newwin$O):     $**.c $(HDRS)
$(LIBRARY)(options$O):    $**.c $(HDRS)
$(LIBRARY)(overlay$O):    $**.c $(HDRS)
$(LIBRARY)(prntscan$O):   $**.c $(HDRS)
$(LIBRARY)(refresh$O):    $**.c $(HDRS)
$(LIBRARY)(scrreg$O):     $**.c $(HDRS)
$(LIBRARY)(setmode$O):    $**.c $(HDRS)
$(LIBRARY)(setterm$O):    $**.c $(HDRS)
$(LIBRARY)(stradd$O):     $**.c $(HDRS)
$(LIBRARY)(strget$O):     $**.c $(HDRS)
$(LIBRARY)(tabsize$O):    $**.c $(HDRS)
$(LIBRARY)(termmisc$O):   $**.c $(HDRS)
$(LIBRARY)(unctrl$O):     $**.c $(HDRS)
$(LIBRARY)(update$O):     $**.c $(HDRS)
$(LIBRARY)(winclear$O):   $**.c $(HDRS)
$(LIBRARY)(windel$O):     $**.c $(HDRS)
$(LIBRARY)(winerase$O):   $**.c $(HDRS)
$(LIBRARY)(winmove$O):    $**.c $(HDRS)
$(LIBRARY)(winscrol$O):   $**.c $(HDRS)
$(LIBRARY)(wintouch$O):   $**.c $(HDRS)
$(LIBRARY)(cursesio$O):   $**.c
