#$Id: tcpsockets.pl,v 1.1.1.1 1993/03/07 08:30:51 frankj Exp $
package main;
require 'sys/socket.ph';
package tcpsockets;

require 'gensym.pl';

# Config Stuff
$listenqueue = 5;
# End of Config Stuff

$sockaddr = 'S n a4 x8';

$localhostname = `/bin/hostname`;

chop($localhostname);

($name,$aliases,$type,$len,$thisaddr) = gethostbyname($localhostname);

die("Couldn't get address of $localhostname\n") if ! defined $thisaddr;

($name, $aliases, $proto) = getprotobyname('tcp');

sub getsocket {
    local($port) = @_;
    $port = $port || 0;
    $socket = &gensym'get;

    socket($socket, &main'PF_INET, &main'SOCK_STREAM, $proto) 
	|| die "$0 - socket: $!";
    $this = pack($sockaddr, &main'AF_INET, $port, $thisaddr);
    $foo = setsockopt($socket,&main'SOL_SOCKET,&main'SO_REUSEADDR,1);
    die("Unable to setsockopt;$!\n") unless defined $foo;
    $foo = setsockopt($socket,&main'SOL_SOCKET,&main'SO_DONTLINGER,1);
    die("Unable to setsockopt;$!\n") unless defined $foo;
    bind($socket, $this) || die "$0 - bind: $!";
    return $socket;
}

sub getport {
    die ("Internal Error:getport called wrong\n") if $#_ !=0;
    ($name,$aliases,$port) = getservbyname($_[0],'tcp')
	unless $port =~ /^\d+$/o;
    return $port;
}

sub getconnection {
    local($them,$port) = @_;
    
    if ($them =~ /^\d+\.\d+\.\d+\.\d+$/o) {
	($one,$two,$three,$four) = split(/\./o,$them);
	$thataddr = pack('C4',$one,$two,$three,$four);
    } else {
	($name,$aliases,$type,$len,$thataddr) = gethostbyname($them);
    }

    $port = &getport($port);

    warn("Thataddr undefined -- $them\n") if ! defined $thataddr;

    $that = pack($sockaddr, &main'AF_INET, $port, $thataddr);

    $socket = &getsocket(0);

    connect($socket, $that) || die "$0: connection refused by $them:$!";

    $old = select($socket); $|=1;select($old);

    return $socket;
}

sub getacceptor {
    local($port) = @_;

    $port = &getport($port);

    $socket = &getsocket($port);
    listen($socket,$listenqueue);

    $old = select($socket); $|=1;select($old);

    return $socket;
}

sub getportnumber {
    $mysockaddr = getsockname($_[0]);
    ($family , $port, $myaddr) = unpack($sockaddr,$mysockaddr);
    return $port;
}

sub getinetaddr {
    ($af,$port,$inetaddr) = unpack($sockaddr,$_[0]);
    return unpack('C4',$inetaddr);
}

sub getlocaladdress {
    return unpack ('C4',$thisaddr);
}

    
#Revision 1.1  1992/06/08  17:02:15  eanders
#Initial revision
