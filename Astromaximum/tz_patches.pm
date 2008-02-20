sub do_patch{ # dataref
	my $d=shift;
	$$d=~s/(Russia)\s+(\+\d+)/$1 - GMT $2/isg;
=head
	$$d=~s/(Yemen, Sana).+?a/$1w/is;
	$$d=~s/Mexico, M[^\n]+?rida/Mexico, Merida/is;
	$$d=~s/Saint John[^\n]+s/Saint Johnes/is;
	$$d=~s/Grenada, Saint George[^\n]+s/Grenada, Saint George's/is;
	$$d=~s/Costa Rica, San Jos[^\n]+/Costa Rica, San Jose/is;
	$$d=~s/Panam[^\s]+ City/Panama City/is;
=cut
	$$d=~s/´/'/sg;
	$$d=~s/\([^\)]+?\)//isg;
	$$d=~s/\s*,/,/isg;
	return;
}
1;
