package const;

our $DIR_TEMPLATE='templates';
our $DIR_OUTPUT='Astromaximum/deploy';
our $DIR_TEMP='.temp';
our $PRODUCT='Astromaximum';
our $VENDOR='S&W Axis';
our $VERSION='1.0.011';

our $UNZIP=q(unzip -q %s -d %s );
#our $unzip=q("d:/Program Files/WinRAR/WinRar.exe" x %s * %s\ );
#our $ZIP=q(fastjar cvfM %s -C %s .);
#our $ZIP=q(cd %s & zip -vr %s *);
#our $JAR=q(jar cvf %s -C %s .);
#our $zip=q("d:/Program Files/WinRAR/WinRar.exe" a -afzip -r -ep1 %s.r %s/*);

sub JAR{
	my ($jarpath, $out, $srcdir)=@_;
	$jarpath=~s/[\\\/]+$//is;
	my $jarchiver='jar';
	$jarchiver='fastjar' unless $winda;
	return sprintf("%s/%s cvfm %s %s/META-INF/MANIFEST.MF -C %s .", $jarpath, $jarchiver, $out, $srcdir, $srcdir);
}

1;