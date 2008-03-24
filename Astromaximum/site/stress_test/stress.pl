#!/opt/lampp/bin/perl

use strict;
use CGI;
use LWP::UserAgent;
use IO::Handle;
STDOUT->autoflush(1);

my $ua=LWP::UserAgent->new;
$ua->agent("Stress agent/1.0");
my $req; # HTTP::Request
my $resp; # HTTP::Response

my($tm_start, $tm_elapsed);

print "*** Starting stress tests ***\n";

#die 11234%100;
#stress_request(100, "captcha", 1, \&stress_captcha);
stress_request(100, "login", 0, \&stress_login);

# stress_* w/o args creates Request
# stress_*(Response) returns if it is succeeded

sub stress_captcha {
	if(defined($_[0])){
#		die $_[0];
		return $_[0]->is_success();
	}
	$req=HTTP::Request->new(GET=>'http://astromaximum.de/mobi/kcaptcha/?PHPSESSID=1234567890');
	$req->header(Referer=>'http://astromaximum.de/index.php?lang=ru&p=pwdrestore');
	return $req;
}

sub stress_login {
	if(defined($_[0])){
#		die $_[0];
		return $_[0]->is_success();
	}
	$req=HTTP::Request->new(GET=>'http://astromaximum.de/');
	return $req;
}

sub stress_request {
	my($count, $name, $die, $ctr)=@_;
	print "\t<<< $name >>>\n";
	my ($succ, $fail)=(0, 0);
	clock_start();
	for(my $i=0; $i<$count; $i++){
		$resp=$ua->request(&$ctr());
		if(&$ctr($resp)){
			$succ++;
#			print $resp->status_line, "\n", $resp->content();
#			die;
		}
		else{
			if($die){
				print "$i\t", $resp->status_line(), "\n";
				die;
			}
			$fail++;
		}
		if($i%100==0){
			print "$i.";
		}
	}
	clock_finish();
	print "\nElapsed time: $tm_elapsed\tSuccess: $succ\tFails: $fail\n";
	print "Average responce time: ", $tm_elapsed/$count, "\n";
	print "\t<<< $name end >>>\n";
}

sub clock_start {
	$tm_start=time();
}

sub clock_finish {
	$tm_elapsed=time()-$tm_start;
}

