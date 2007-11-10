package const;

our $DIR_TEMPLATE='templates';
our $DIR_OUTPUT='Astromaximum/deploy';
our $DIR_TEMP='.temp';
our $PRODUCT='Astromaximum';
our $VENDOR='S&W Axis';
our $VERSION='1.0.10';

our $UNZIP=q(unzip -q %s -d %s );
#our $unzip=q("d:/Program Files/WinRAR/WinRar.exe" x %s * %s\ );
#our $ZIP=q(fastjar cvfM %s -C %s .);
our $ZIP=q(cd %s; zip -vr %s *);
#our $zip=q("d:/Program Files/WinRAR/WinRar.exe" a -afzip -r -ep1 %s.r %s/*);

1;