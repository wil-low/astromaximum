package const;

our $DIR_TEMPLATE='templates';
our $DIR_OUTPUT='Astromaximum/deploy';
our $DIR_TEMP='.temp';
our $PRODUCT='Astromaximum';
our $VENDOR='S&W Axis';
our $VERSION='1.0.013';

our $UNZIP=q(unzip  %s -x *META-INF* -d %s );
#our $unzip=q("d:/Program Files/WinRAR/WinRar.exe" x %s * %s\ );
#our $ZIP=q(cd %s & zip -vr %s *);
#our $JAR=q(jar cvf %s -C %s .);
#our $zip=q("d:/Program Files/WinRAR/WinRar.exe" a -afzip -r -ep1 %s.r %s/*);

sub JAR{
	my ($jarpath, $out, $manifest, $srcdir, $winda)=@_;
	$jarpath=~s/[\\\/]+$//is;
	my $jarchiver='jar';
	$jarchiver='fastjar' unless $winda;
	return sprintf("%s/%s cvfm %s %s -C %s .", $jarpath, $jarchiver, $out, $manifest, $srcdir);
}

1;