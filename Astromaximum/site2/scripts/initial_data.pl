#!/usr/bin/perl
use strict;
use warnings;
require '../tools.pm';

my $lang = 'ru';

my @interpret_files = glob("../Astromaximum/interpret/$lang/*.txt");

my $is_first_record = 1;
my $primary_key = 1;

my $out = "[\n";
foreach my $infile (@interpret_files) {
	open (INF, "<$infile") or die "$!: $infile";
	my @buf = <INF>;
	close (INF);
	$buf[0] =~ /\!\!type\s*(\w+)/i;
	my $evt=$1;
	my $event_type = $tools::eventType{$evt};
	if ($event_type !~ /^\d+$/) {
		echo("Event $evt not defined in $infile! Skipped\n");
		next;
	}
	$buf[2]=~/\!\!planet\s*(.+)/i;
	my $planet=$1;
	foreach my $line (@buf) {
		$line =~ s/\/\/.+//is;
		next if $line !~ /%[\d\s\,\-]+%/;
		$line =~ s/\s*\Z//is;
		$line =~ s/\.+\Z//is if $evt ne 'EV_MSG';
		$line =~ s/.*?%(.*?)%\s*//is;
		my @param = split (/,/, $1);
		for (my $i = 0; $i < 3; ++$i) {
			if (!defined ($param[$i])) {
				$param[$i] = 'null';
			}
			else {
				$param[$i] = int($param[$i]);
			}
		}
		if ($is_first_record) {
			$is_first_record = 0;
		}
		else {
			$out .= ',';
		}
		$line =~ s/"/\\"/g;
		$out .= << "END";
		{
			"pk": $primary_key,
			"model": "amax.text",
			"fields": {
				"planet": $planet,
				"event_type": $event_type,
				"language": "$lang",
				"param0": $param[0],
				"param1": $param[1],
				"param2": $param[2],
				"message": "$line"
			}
		}
END
		++$primary_key;
	}
}
$out .= "]\n";
print $out;

