# ##########################################################
#
#  Makefile pour compiler xcurves
#
# ##########################################################

# Emplacement des includes et des librairies liees a X et Motif
# Note:
# - 2 cas prevus : SunOS 5.3 (Solaris) ou SunOS 4.X

XDIR = /usr/local/X11
PIXRECT = /usr/openwin/include/pixrect


# ##########################################################
# Mise a jour automatique
#
# KEEP_STATE => verification automatique des dependances
# et des options de compilation. ==> ne PAS faire "make depend".
#
.KEEP_STATE:


# ##########################################################
# Compilation:
#
# Notes:
# - gcc = compilateur ANSI C
# - l'option -g est necessaire pour utiliser le debugger
# - l'option -O permet d'optimiser l'executable
#
CC = gcc

CFLAGS:sh =if [ `uname -r` = 5.3 ] ; \
	then echo -I\${XDIR}/include -I\${PIXRECT} ; \
	else echo -I\${XDIR}/include ; \
	fi

CFLAGS += -g


# ##########################################################
# Edition de Liens
#
# Notes options:
# - option -L : path des librairies a l'edition de liens
# - option -R : path des librairies a l'execution (QUE POUR SOLARIS !)
#
LDLIBS:sh =if [ `uname -r` = 5.3 ] ; \
	then echo -L\${XDIR}/lib -R\${XDIR}/lib -lXm -lXt -lX11 -lsocket -lnsl -lgen ; \
	else echo -L\${XDIR}/lib -lXm -lXt -lX11 ; \
	fi

LDLIBS += -lm

# ##########################################################
#
# Regles de generation des executables:
#

XCURVES = xcurves.o xmutil.o xplus.o

PROG1 = prog1.o truc.o bidule.o chose.o

PROG2 = prog2.o chose.o


# make sans argument construira les programmes indiques ci-dessous
#
all :  xcurves # prog1 prog2

xcurves : $(XCURVES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(XCURVES) $(LDLIBS)

prog1 : $(OBJS1)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS1) $(LDLIBS)

prog2 : $(OBJS2)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS2) $(LDLIBS)
