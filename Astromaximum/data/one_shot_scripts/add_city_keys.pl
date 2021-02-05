#!/usr/bin/perl
use strict;
use warnings;
use File::Find;

my $CALCULATIONS_DIR = '/home/willow/prj/amax/amax-calculations/compressed/tmp';

print "START TRANSACTION;\n";

find ({wanted => \&location_found, no_chdir => 1}, $CALCULATIONS_DIR);
print "COMMIT;\n";

sub location_found {
	if ($File::Find::name =~ /\.txt$/) {
		open (INF, "<$File::Find::name") or die "$!: $File::Find::name";
		while (my $line = <INF>) {
			$line =~ s/'/''/g;
			my @fields = split (/;/, $line);
			if (scalar(@fields) < 8) {
				next;
			}
			my ($city, $state, $country, $key) = ($fields[0], $fields[1], $fields[2], $fields[7]);
			my $query = "UPDATE cities, countries, states SET cities.key='$key' where cities.name = '$city' and cities.country_id = countries.id and countries.name = '$country'";
			if ($state) {
				$query .= " and cities.state_id = states.id and states.name = '$state'"
			}
			else {
				$query .= " and state_id = 0";
			}
			print "$query;\n";
		}
	}
}
