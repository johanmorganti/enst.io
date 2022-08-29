# ##########################################################
#
#  Makefile

# ##########################################################


# ##########################################################
# Emplacement des includes et des librairies liees a X et Motif
# Note:
# - 2 cas prevus : SunOS 5.3 (Solaris) ou SunOS 4.X

XDIR = /usr/local/X11
PIXRECT = /usr/openwin/include/pixrect
SCANDIR = /sig/garfield/ricoh-fs2

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
	then echo -L\${XDIR} -R\${XDIR} -lUil -lMrm -lXm -lXt -lX11 -lsocket -lnsl -lgen ; \
	else echo -L\${XDIR} -lXm -lXt -lX11 ; \
	fi

LDLIBS += -lm


# ##########################################################
#
# Regles de generation des executables:
#

OBJS = mre.o mre_color.o mre_db.o mre_font.o mre_util.o

# make sans argument construira les programmes indiques ci-dessous
#
mre : $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)
