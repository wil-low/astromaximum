#!/usr/bin/perl
use strict;
use POSIX;
use warnings;
use Data::Dumper;
use File::Path;
require './genconst.pm';
require './Crc32.pm';

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

my %id_hash;
my %city_hash;

# Create a user agent object
use LWP::UserAgent;
my $ua = LWP::UserAgent->new;
$ua->agent("Opera");

File::Path::make_path ("./data/world");

if ($ARGV[0]) {
	my $ini = "./data/$ARGV[0].ini";
	process_ini ($ini);
}
else {
	my @inifiles = glob ('./data/*.ini');
	foreach my $ini (@inifiles) {
		process_ini ($ini);
	}
}

sub process_ini { # filename
	my $ini = shift;
	
	$ini =~ /data\/(.+?)\.ini/;
	my $region = $1;
	File::Path::make_path ("./data/wikipages/$region");

	open (ERRFILE, ">err.log") or die "$!: err.log";
	my $outfile = "data/world/$region.world";
	open (INF, "<$ini") or die "$!: $ini";
	open (OUTF, ">$outfile");
	print (OUTF "## city, state, country, latitude, longitude, altitude, timezone\n");
	my ($cur_country, $cur_state, $zone) = ('', '');
	while (my $line = <INF>) {
		print ("\t$line");
		$line =~ s/[\r\n]//sg;
		next if $line =~ /^[#&]/;
		next if $line =~ /^\s*$/;
		if ($line =~ /^@\s(.+?), (.+)/) {
			($cur_country, $zone) = ($1, $2);
			if (! -f "$const::TIMEZONE_DIR/$zone") {
				die "Unknown timezone '$const::TIMEZONE_DIR/$zone'\n";
			}
			if ($cur_country =~ s/\s\-\s(.+)//s) {
				$cur_state = $1;
			}
			else {
				$cur_state = '';
			}
			print (OUTF "\n# $line\n");
			next;
		}
		$line =~ s/|.+//;
		
		my $is_check_disambiguation = $line !~ s/^\-//;

		if ($cur_country eq 'USA') {
			if ($line !~ /!$/) {
				$line .= ", $cur_state";
			}
		}
		elsif ($cur_country eq 'Canada') {
			if ($line !~ /!$/) {
				$line .= ", $cur_state";
			}
			warn ">$line<";
		}
		else {
			if ($line !~ /[?!]/) {
				$line .= ", $cur_country";
			}
		}
		my ($city, $state, $country, $latitude, $longitude, $altitude, $zone, $wikifile) = 
			city_query ($line, $region, $cur_country, $cur_state, $zone, $is_check_disambiguation);
		if ($wikifile) {
			#system ("opera \"$wikifile\"");
			print (ERRFILE "$line -- $city, $country\n");
		}
		$city =~ s/, .+//; # strip country or state name
		$city =~ s/ +\(.+//; # strip garbage
		my $coords = "$latitude|$longitude";
		my $crc32 = new Digest::Crc32();
		my $crc32hex = sprintf ("%08x", $crc32->strcrc32($coords));
		
		my $city1_str = "$city;$state;$country;$region";
		if (exists ($id_hash{$crc32hex})) {
			die "Duplicate id $crc32hex for cities '$city1_str', '$id_hash{$crc32hex}'";
		}
		else {
			$id_hash{$crc32hex} = $city1_str;
		}
		
		my $city_compare = "$city;$state;$country";
		my $city2_str = "$city;$region";
		if (exists ($id_hash{$city_compare})) {
			die "Duplicate cities '$city2_str', '$id_hash{$city_compare}'";
		}
		else {
			$id_hash{$city_compare} = $city2_str;
		}

		print (OUTF "$city;$state;$country;$latitude;$longitude;$altitude;$zone;$crc32hex;\n");
	}
	close (OUTF);
	close (ERRFILE);
	print ("--- $outfile written ---\n");
}

sub city_query { # city, region, cur_country, cur_state, zone, is_check_disambiguation
	my ($city, $region, $cur_country, $cur_state, $zone, $is_check_disambiguation) = @_;
	my ($country, $latitude, $longitude, $altitude) = ('', '', '', '');
#	warn "($city, $region, $cur_country, $cur_state, $is_check_disambiguation)";
	my $real_name = '';
	if ($city =~ s/(.+?)\!//s) {
		$real_name = $1;
	}
	if (!$city) { # ends with !
		$city = $real_name;
	}
	$city =~ s/\?$//;
	my $wikifile = "./data/wikipages/$region/$city.html";
	my $content = '';
	my $url = "http://en.wikipedia.org/wiki/$city";
	print "$url\n";
	if (-f $wikifile) {
		open (WIKIFILE, "<$wikifile") or die "$!: $wikifile";
		my @data = <WIKIFILE>;
		close (WIKIFILE);
		$content = join ('', @data);
	}
	else {
		# Create a request
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
	if ($content =~ /class="firstHeading">(?:<span dir="auto">)?([^<]+)(?:<\/span>)?<\/h1>/s) {
		$city = $1;
		if ($real_name eq '') {
			$real_name = $city;
		}
	}
	else {
		dump_contents($content);
		die "Cannot detect city: $city";
	}
=head	
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
			system ("opera '$wikifile'");
			die "Cannot detect country: $city";
		}
	}
=cut
	if ($content =~ />Elevation<.+?>([-,\d]+)(\&#160;|\s)+m/s) {
		$altitude = $1;
		$altitude =~ s/[,\s]//sg;
	}
	else {
		dump_contents($content);
		warn "Cannot detect elevation: $city";
	}
	if ($content =~ /(<span class="latitude">(.+?)(\w)<\/span> <span class="longitude">(.+?)(\w)<\/span>)/s) {
		my ($lat, $latl, $lon, $lonl) = ($2, $3, $4, $5);
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
			die "Cannot detect latitude: $city: '$lat', '$latl'";
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
			die "Cannot detect longitude: $city: '$lon', '$lonl'";
		}
	}
	else {
		dump_contents($content);
		unlink ($wikifile);
		die "Cannot detect coords1: $city";
	}
	my $is_redirected = $content =~ /Redirected from/s;
	
	if ($is_check_disambiguation and $content =~ /disambiguation/) {
	}
	else {
		$wikifile = '';
	}
	return ($real_name, $cur_state, $cur_country, $latitude, $longitude, $altitude, $zone, $wikifile);
}

sub dump_contents {
	open (OFILE, ">contents.log");
	print (OFILE $_[0]);
	close (OFILE);
}
