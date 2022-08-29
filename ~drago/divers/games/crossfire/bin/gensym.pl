package gensym;

# $Id: gensym.pl,v 1.1.1.1 1993/03/07 08:30:50 frankj Exp $

# Revision 1.7  1992/06/23  21:15:53  jmalcolm
# Null out things instead of undef'ing them.
#
# Revision 1.6  1991/06/22  23:55:15  jmalcolm
# changed order of nreturn's parameters.
#
# Revision 1.5  1991/06/22  23:45:21  jmalcolm
# nget and nreturn now accept and keep track of
# a function to print the gensym meaningfully.
#
# Revision 1.4  1991/05/31  10:56:23  jmalcolm
# added nget and nreturn to enable more accurate tracking of
# gensym use.
#
# Revision 1.3  1991/05/26  01:40:28  jmalcolm
# fixed get. I had forgetten to change a variable
# everywhere when I changed its name..
#
# Revision 1.2  1991/05/26  01:13:00  jmalcolm
# added stats and returning gensyms.
#
# Revision 1.1  1991/05/26  00:23:55  jmalcolm
# Initial revision
#

$gensym = 'gensym0000000000000001';
$totalout = 0;
$totalin = 0;
$totalcreated = 0;
@list = ();
%howtoprint = ();

sub stats {
	($totalout,$totalin,$totalcreated,$#list+1);
}

sub nget {
	local($useid,$printfunc) = @_;
	$ind_out{$useid}++;
	local($gs) = &get;
	if ($_[1]) {
		$howtoprint{$gs} = $printfunc;
	}
	$gs;
}

sub nreturn {
	local($gs,$useid) = @_;
	$ind_in{$useid}++;
	undef($howtoprint{$gs});
	&return($gs);
}

sub get {
	local($gs);
	++$totalout;
	if ($#list < 0) {
		++$gensym;
		++$totalcreated;
		$gs = "gensyms'".$gensym;
	} else {
		$gs = shift(@list);
	}
	$gs;
}

sub return {
	++$totalin;
	local(*GS) = local($gs) = @_;
	$GS = '';
	%GS = @GS = ();
#	undef($GS);
#	undef(@GS);
#	undef(%GS);
	unshift(@list,$gs);
	1;
}

sub main'getgensym {	# old, deprecated interface
	&get;
}

1;
