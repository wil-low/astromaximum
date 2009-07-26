package const;

our $DIR_TEMPLATE='templates';
our $DIR_OUTPUT='Astromaximum/deploy';
our $DIR_TEMP='.temp';
our $DIR_IMG='images/icons'; 
our $DIR_INTERPRET='Astromaximum/interpret';
our $MIDLET='Amax';
our $PRODUCT='Astromaximum';
our $VENDOR='S&W Axis';
our $VERSION='1.1.7';
our $DESCR_CALENDAR='Astrological calendar';
our $DESCR_GEO='Astromaximum city file';
our $USE_AMTEXT=0;

our $UNZIP=q("%s" -qq %s -x *META-INF* -d %s);
#our $unzip=q("d:/Program Files/WinRAR/WinRar.exe" x %s * %s\ );
#our $ZIP=q(cd %s & zip -vr %s *);
#our $JAR=q(jar cvf %s -C %s .);
#our $zip=q("d:/Program Files/WinRAR/WinRar.exe" a -afzip -r -ep1 %s.r %s/*);

sub JAR{
	my ($jarpath, $out, $manifest, $srcdir, $winda)=@_;
	$jarpath=~s/[\\\/]+$//is;
	my $jarchiver='jar';
	$jarchiver='fastjar' unless $winda;
	return sprintf("%s/%s cfm %s %s -C %s .", $jarpath, $jarchiver, $out, $manifest, $srcdir);
}

1;
