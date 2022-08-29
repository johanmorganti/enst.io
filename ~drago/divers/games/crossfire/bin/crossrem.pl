#!/usr/local/bin/perl
push(@INC,"BINDIR");
require('tcpsockets.pl');
require "sys/socket.ph";

chop($server = `/bin/hostname`);
$crosservport = 3326;
$display = $ENV{'DISPLAY'};

if ($ARGV[0] eq "-server") {
    shift @ARGV;
    $server = shift @ARGV;
}

die("Unknown cmd $_, usage $0 [-server host]\n") if defined($_ = shift @ARGV);

$fh = &tcpsockets'getconnection($server,$crosservport);

print "Hi:$fh\n";
print "Xhosting your machine to $server\n";

&system("xhost +$server\n");

chop($hostname = `hostname`);

($name,$aliases,$addrtype,$length,@addrs) = gethostbyname($hostname);

die("Can't gethostbyname of $hostname??") unless defined $addrs[0];

($a, $b, $c, $d) = unpack('C4',$addrs[0]);

print "Cleaning up old font things\n";

opendir(XDIR,"/tmp");
while ($_ = readdir(XDIR)) {
    next unless /^crossfire-font\.\d+$/o;
    next unless -d "/tmp/$_";
    print "rm -rf /tmp/$_\n";
    system("rm -rf /tmp/$_");
}
closedir(XDIR);

$localdir = "/tmp/crossfire-font.$$";

mkdir($localdir,0777);

chmod 0777 , $localdir;

$localname = "$localdir/crossfire.bdf";

print "Getting Font...\n";
print "If this doesn't do anything by 5 seconds from now, the server is\n";
print "probably down so you will want to ctrl-c this command\n";

&getfont;

print "Converting Font...\n";

&convertfont;

print "Getting old fontpath...\n";

open(FONTINFO,"xset q |") || die("Unable to read from pipe of xset q\n");

$oldfontpath = undef;

while(<FONTINFO>) {
    next unless /^Font Path:$/o;
    $oldfontpath = <FONTINFO>;
}
close(FONTINFO);

die("Old fontpath not defined\n") unless defined $oldfontpath;

chop ($oldfontpath);

@fpaths = split(/,/,$oldfontpath);

foreach $path (@fpaths) {
    if ($path =~ m!^/tmp/crossfire-font!) {
	print "Removing $path from font path\n";
	next;
    }
    push(@newpath,$path);
}

$newpath = join(',',@newpath);

print "Adding to fontpath...\n";

&system("xset fp $newpath,$localdir");

print "Waiting 5 seconds ...\n";
sleep(5);

print "Adding Window...\n";

#print $fh "window $a.$b.$c.$d\n";
print $fh "window $display\n";

print $fh "quit\n";

close($fh);

print "Your window should appear in a little while\n";
print "If it doesn't, you can try this command again.\n";

sub getfont {
    &open_recieve_data_socket;
    open (OUTFILE,">$localname") || die("Unable to open file to write.\n");
    $loop=0;
    print "BYTES:";
    $foo='';
    while ($size = sysread(DS,$foo,4096)) {
#       $diff = ($size-$loop)<$blocksize?$size-$loop:$blocksize;
#       print "$size,$loop,$diff\n";
        syswrite(OUTFILE,$foo,$size);
        $loop += $size;
        print "$loop";
        print "\b" x length("$loop");
    }
    print "\n";
#    syswrite(DS,$foo,$len) while($len=sysread(DS,$foo,$blocksize));
    close (OUTFILE);
    close(DS);
    close($acceptsocket);
}

sub open_recieve_data_socket
{
    ($a,$b,$c,$d) = &tcpsockets'getlocaladdress;
    
    $acceptsocket = &tcpsockets'getacceptor(0);
    
    # get the port number
    $port = &tcpsockets'getportnumber($acceptsocket);
    
    unless(print $fh "sendfont $a.$b.$c.$d $port\n") {
	die("Unable to ask for font from server.  It died??\n");
    }
    
    ($addr = accept(DS,$acceptsocket)) || die("accept failed $!");
    $oldfh = select(DS);$|=1;select($oldfh);
    return $addr;
}

sub convertfont {
    foreach $path (split(/:/,$ENV{'PATH'})) {
	$usepcf = 1 if (-x "$path/bdftopcf");
	$usesnf = 1 if (-x "$path/bdftosnf");
	$hasmkfontdir = 1 if (-x "$path/mkfontdir");
    }
    unless ($hasmkfontdir) {
	die("Unable to find mkfontdir in your path\n");
    }
    unless ($usepcf||$usesnf) {
	die("Unable to find either bdftopcf or bdftosnf in your path\n");
    }
    if ($usepcf) {
	$finalname = $localname;
	$finalname =~ s/\.bdf/.pcf/;

	&system("bdftopcf $localname >$finalname");
    }
    if ($usesnf) {
	$finalname = $localname;
	$finalname =~ s/\.bdf/.snf/;

	&system("bdftosnf $localname >$finalname");
    }
    &system("mkfontdir $localdir");
    open(BEH,">$localdir/fonts.alias")
	|| die("Unable to open $locldir/fonts.alias for write\n");
    close(BEH);
}

sub system {
    local($cmd) = @_;

    print "$cmd\n";

    $retval = system($cmd)>>8;

    die("Command failed($retval)") unless $retval==0;
}
