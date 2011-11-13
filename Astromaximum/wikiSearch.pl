#!/usr/bin/perl
use strict;
use POSIX;
use warnings;
use Data::Dumper;

my %unknown_country = (
	'Jerusalem' => 'Israel',
	'Sarıkamış' => 'Turkey',
	'Side' => 'Turkey',
	'Singapore' => 'Singapore',
	'Sydney' => 'Australia',
	'Melbourne' => 'Australia',
	'Perth, Western Australia' => 'Australia',
	'Canberra' => 'Australia',
	'Serhetabat' => 'Turkmenistan',
	'Istarawshan' => 'Tajikistan',
	'London' => 'United Kingdom',
	'Gibraltar' => 'Gibraltar',
	'Luxembourg' => 'Luxembourg',
);

# Create a user agent object
use LWP::UserAgent;
my $ua = LWP::UserAgent->new;
$ua->agent("Opera");

if ($ARGV[0]) {
	my $ini = "./data/$ARGV[0].ini";
	process_ini ($ini);
}
else {
	my @inifiles = glob ('./data/*.ini');
	foreach my $ini (@inifiles) {
		process_ini ($ini);
		last;
	}
}

sub process_ini { # filename
	my $ini = shift;
	
	$ini =~ /data\/(.+?)\.ini/;
	my $region = $1;
	mkdir "./data/wikipages/$region";
	
	open (INF, "<$ini") or die "$!: $ini";
	open (OUTF, ">$ini.world");
	while (my $line = <INF>) {
		print ("\t$line");
		next if $line =~ /^[#&]/;
		next if $line =~ /^\s*$/;
		if ($line =~ /^@/) {
			print (OUTF "# $line");
			next;
		}
		$line =~ s/|.+//;
		chomp ($line);
		my ($city, $country, $latitude, $longitude, $altitude) = city_query ($line, $region);
		print (OUTF "$city;;$country;$latitude;$longitude;$altitude;TIMEZONE;\n");
	}
	close (OUTF);
	print ("--- $ini.world written ---\n");
}

sub city_query { # city, region
	my ($city, $region, $country, $latitude, $longitude, $altitude) = (shift, shift, '', '', '', '');
	$city =~ s/[\r\n]//sg;
	my $real_name = $city;
	if ($city =~ s/(.+?)\!//s) {
		$real_name = $1;
	}
	my $wikifile = "./data/wikipages/$region/$city.html";
	my $content = '';
	if (-f $wikifile) {
		open (WIKIFILE, "<$wikifile") or die "$!: $wikifile";
		my @data = <WIKIFILE>;
		close (WIKIFILE);
		$content = join ('', @data);
	}
	else {
		# Create a request
		my $url = "http://en.wikipedia.org/wiki/$city";
		print "$url\n";
		my $req = HTTP::Request->new(GET => $url);
		# Pass request to the user agent and get a response back
		my $res = $ua->request($req);
		$res->decode();
		# Check the outcome of the response
		if ($res->is_success) {
			$content = $res->content;
			open (WIKIFILE, ">$wikifile") or die "$!: $wikifile";
			print (WIKIFILE $content);
			close (WIKIFILE);
		}
		else {
			die "$city error: " . $res->status_line . "\n";
		}
	}
	$content =~ s/&#160;/ /sg;
	if ($content =~ /class="firstHeading">([^<]+)<\/h1>/s) {
		$city = $1;
	}
	else {
		dump_contents($content);
		die "Cannot detect city: $city";
	}
	if (defined ($unknown_country{$city})) {
		$country = $unknown_country{$city};
	}
	else {
		if ($content =~ />(?:Location|Country)<.+?(<td.+?<\/td>)/s) {
			$country = $1;
			$country =~ s/<.+?>//isg;
			$country =~ s/[\n\r]//isg;
			$country =~ s/^\s+//g;
			$country =~ s/\s+$//g;
		}
		else {
			dump_contents($content);
			die "Cannot detect country: $city";
		}
	}
	warn "DISAMBIGUATION: $city" if $content =~ /disambiguation/;
	if ($content =~ />Elevation<.+?>([-,\d]+)(\&#160;|\s)+m/s) {
		$altitude = $1;
		$altitude =~ s/[,\s]//sg;
	}
	else {
		dump_contents($content);
		warn "Cannot detect elevation: $city";
	}
	if ($content =~ /<span class="latitude">(.+?)(\w)<\/span> <span class="longitude">(.+?)(\w)<\/span>/s) {
		my ($lat, $latl, $lon, $lonl) = ($1, $2, $3, $4);
		if ($lat =~ /([\d\.]+)°([\d\.]+)′(?:([\d\.]+)″)?/) {
			my ($latd, $latm, $lats) = ($1, $2, $3);
			$lats = 0 if !defined ($lats);
			$latitude = $latd + $latm / 60 + $lats / 3600;
			$latitude = -$latitude if $latl eq 'S';
			$latitude = int ($latitude * 100 + 0.5) / 100;
		}
		else {
			dump_contents($content);
			unlink ($wikifile);
			die "Cannot detect latitude: $city: '$lat'";
		}
		if ($lon =~ /([\d\.]+)°([\d\.]+)′(?:([\d\.]+)″)?/) {
			my ($lond, $lonm, $lons) = ($1, $2, $3);
			$lons = 0 if !defined ($lons);
			$longitude = $lond + $lonm / 60 + $lons / 3600;
			$longitude = -$longitude if $lonl eq 'W';
			$longitude = int ($longitude * 100 + 0.5) / 100;
		}
		else {
			dump_contents($content);
			unlink ($wikifile);
			die "Cannot detect longitude: $city: '$lon'";
		}
	}
	else {
		dump_contents($content);
		unlink ($wikifile);
		warn "Cannot detect coords1: $city";
	}
	return ($real_name, $country, $latitude, $longitude, $altitude);
}

sub dump_contents {
	open (OFILE, ">contents.log");
	print (OFILE $_[0]);
	close (OFILE);
}
