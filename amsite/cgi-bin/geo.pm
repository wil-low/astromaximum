package geo;

use strict;
use warnings;
use CGI ':standard';
use DBI;
use CGI::Carp 'fatalsToBrowser';
$CGI::POST_MAX=1024 * 10;  # max 100K posts
$CGI::DISABLE_UPLOADS = 1;  # no uploads
use tools;
use amtools;


our $cur_country='';
our $cur_state='';
our $out='';
our ($dbh, $cnum, $statenum, $defyear, $userid);

sub get_content{
	$userid=$_[1];
	return '<br><p align=center>Please log in to access the cities database.</p>' unless $userid; 
	$dbh=$_[0];
	my $ADDCITY=<<ADDCITY;
	<script>
	function city_add(cname,sname){
		selc=document.getElementById("selcit");
		out="";
		frm=document.forms.namedItem("main");
		sc=frm.elements.namedItem("sc");
	//	alert(sname);
		for(i=0; i<frm.elements.length; i++){
			opt=frm.elements.item(i);
			if(opt.type=="checkbox" && opt.checked && sc.value.indexOf("."+opt.id+".")<0){
				out=out+"<input type=checkbox name=sss id="+opt.id+">"+opt.value;
	//			if(sname!=""){
	//				out=out+", "+sname;
	//			}
				out=out+", "+cname+"</input><br>";
				sc.value=sc.value+opt.id+".";
			}
		}
		selc.innerHTML=selc.innerHTML+out;
	};
	
	function show_country(country,state){
		frm=document.forms.namedItem("main");
		if(frm.elements.namedItem('cid').value!=country || frm.elements.namedItem('stateid').value!=state){
			frm.elements.namedItem('stateid').value=state;
			frm.elements.namedItem('cid').value=country;
			frm.submit();
		}
	};
	
	function city_del(){
		frm=document.forms.namedItem("main");
		sc=frm.elements.namedItem("sc");
		oldsc=sc.value;
		sc.value=".";
		for(i=0; i<frm.elements.length; i++){
			opt=frm.elements.item(i);
			if(opt.type!="checkbox") continue;
			if( opt.name!="sss") continue;
			if(!opt.checked){
				sc.value=sc.value+opt.id+".";
			}
		}
		if(oldsc!=sc.value){
			frm.submit();
		}
	};
	</script>
ADDCITY
	
	my $mode=defined(param('cid'));
	my @sel_cities=param('Sel_cities');
	

	$cnum=param('cid');
	$statenum=param('stateid');
	$statenum=0 unless defined($statenum);
	$defyear=param('year');
	$defyear=2007 unless $defyear;
	
#	print header(-expires=>'now'), start_html(-title=>'Astromaximum location archives', -script=>$ADDCITY);
	$out.=$ADDCITY;	
	
	$out.=tools::adm_panel();
	$out.=start_form(-name=>'main', -method=>'post');
	$out.="<table width=100% border=1><tr valign=top><td>"; 
	$out.=popup_menu(-name=>'year', -values=>[qw(2005 2006 2007 2008)], -default=>$defyear, 
		-onchange=>"javascript:window.navigate('start.cgi?p=geo&year='+year.value)"
		)."</td>";
	$out.="<td rowspan=2 width=25%>".selected_cities()."</td></tr>";
	$out.="<font color='red'><i>Step 1:</i></font><br><b>Year:</b> ";
	$out.= hidden(-name=>'sc', -default=>'.');
	$out.= hidden(-name=>'cid', -default=>$cnum);
	$out.= hidden(-name=>'stateid', -default=>0);
	$out.= hidden(-name=>'p', -default=>'geo');
	$out.= "<tr><td>".country_header().state_header().city_selector()."</td>";
	
	$out.= "</tr></table>";
	$out.= end_form();
	#print Dump();
	#print join('.',@sel_cities);
	
	if(param('Action') && param('Action') eq 'Get data' && param('sc')=~/\d/){
		my $id=create_jar($defyear, param('sc'));
		my $url='data.cgi?r='.$id;
		$out.= "<p><center><font color='red'><i>Step 4:</i></font>";
		$out.= "<h4>Download to PC:</h4>";
		$out.= "<b>JAR link: <a href=\'$url\'>$id</a><br><br>";
		$url=~s/\?r/\?d/is;
		$out.= "JAD link: <a href=\'$url\'>$id</a><br><br></b>";
		$url=~s/\?d/\?t/is;
		$out.= "<h4>Download to phone:</h4>";
		$out.= "<b>Direct link: <a href=\'$url\'>$id</a><br>";
		$out.= "<br><font color='red'>Attention: links are valid within next 2 hours!</font></b></center>";
	}
#	print end_html;
#	$dbh->disconnect;
	return $out;
}

sub country_header{
#	die "header";
	my $res="<font color='red'><i>Step 2:</i></font><br><b>Country: </b>";
	my $stcou = $dbh->prepare("SELECT countries.id, countries.name FROM countries ORDER BY countries.name")|| die $dbh->errstr;
	$stcou->execute|| die $dbh->errstr;
	while(my @row = $stcou->fetchrow_array){
		if(!$cnum){
			$cnum=$row[0];
			param('cid',$cnum);
		}
		if($row[0]==$cnum){
			$cur_country=$row[1];
			$row[1]="<b>$row[1]</b>" ;
		}
		$res.="<a href='#' onclick=\"show_country($row[0],0)\">$row[1]</a>&nbsp;\n"; 
	}
	$stcou->finish;
	return $res;
}

sub state_header{
	my $stcou = $dbh->prepare("SELECT DISTINCT states.id, states.name FROM states,countries WHERE country_id=$cnum ORDER BY states.name")|| die $dbh->errstr;
	$stcou->execute|| die $dbh->errstr;
	my $res='';
	$cur_state='';
	my $allst="<a href='#' onclick=\"show_country($cnum,0)\">&lt;All states&gt;</a>&nbsp;\n";
	$allst="<b>$allst</b>" if !$statenum;
	if($stcou->rows){
		$res="<hr>$allst";
		while(my @row = $stcou->fetchrow_array){
#			if(!$statenum){
#				$statenum=$row[0];
#				param('stateid',$statenum);
#			}
			if($row[0]==$statenum){
				$cur_state=$row[1];
				$row[1]="<b>$row[1]</b>" ;
			}
			$res.="<a href='#' onclick=\"show_country($cnum,$row[0])\">$row[1]</a>&nbsp;\n"; 
		}
	}
	$stcou->finish;
	return $res;
}

sub city_selector{
	$cnum=0 unless $cnum;
	$statenum=0 unless $statenum;
	my $andst='';
	$andst=" AND state_id=$statenum" if $statenum;
	my $stat=sprintf(
		"SELECT cities.id, cities.name FROM cities,countries".
		",locations". # year condition
		" WHERE country_id=%d AND countries.id=country_id".
		" AND city_id=cities.id %s AND year=".$defyear. # year condition
		" ORDER BY cities.name",$cnum, $andst);
	my $sth = $dbh->prepare($stat)|| die $dbh->errstr;
	$sth->execute|| die $dbh->errstr;
	my $res="<hr><div id=chkcit>";
	my $i=0;
	while(my @row = $sth->fetchrow_array){
		$res.="<input type=checkbox id=$row[0] value='$row[1]'>$row[1]</input>\n"; 
		$i++;
	}
	$res.="</div>";
	$sth->finish;
	if($i>0){
		$res.=button(-value=>'Add cities', -onClick=>"city_add(\"$cur_country\",\"$cur_state\")");
	}
	else{
		$res.="<i>No cities in database.</i>";
	}
	return $res;
}

sub selected_cities{
	my $rs=restored_selection(param('sc'));
	my $res="<center><b>Selected cities:</b></center>";
#	if($rs){
		$res.="<div align=right>".button(-value=>'Delete selected', -onClick=>"city_del()")."</div>".
			"<div id=selcit>".$rs."</div><p align=center><font color='red'><i>Step 3:</i></font> ";
		$res.=submit('Action','Get data')."</p>";
#	}
#	else{
#		$res.="<center><b><i>No cities selected.</i></b></center>";
#	}
	return $res;
}

sub restored_selection{ # ids
	my $ids=shift;
	$ids=~/^\.(.*?)\.?$/is;
	$ids=$1;
	$ids=~s/\./\,/isg;
	my $res='';
	return $res if !$ids;
	my $stat="SELECT cities.id, cities.name, countries.name FROM cities,countries WHERE cities.id IN ($ids) and countries.id=country_id ORDER BY countries.name,cities.name";
#	print $stat;
	my $sth = $dbh->prepare($stat)|| die $dbh->errstr;
	$sth->execute|| die $dbh->errstr;
	while(my @row = $sth->fetchrow_array){
		$res.="<input type=checkbox name=sss id=$row[0]></input>$row[1], $row[2]<br>\n";	
	}
	$sth->finish;
	return $res;
	
}

sub create_jar{ # $year, $city_ids
	my($year, $ids, $outfile)=@_;
	$ids=~/^\.(.+?)\.?$/is;
	$ids=$1;
	$ids=~s/\./\,/isg;
	my ($dir,$fn)=amtools::random($tools::dir_files,'.r');
	my $srcdir='';
	$srcdir="/tmp/$fn";
	mkdir $srcdir or die $!;
	open(INF, "<$tools::dir_source/template.jad") or die $!;
	my @data=<INF>;
	close(INF);
	my $template=join("",@data);
	@data=();
	$fn=~/(\d{4})$/is;
	my $code="-$1";
	my $fname="Cities$code";
	$year=~/\d\d(\d\d)/is;
	my $ye=$1;
	$template=~s/<YEAR>/$ye/isg;
#	$jad=~s/<REGION>/$reg/isg;
	$template=~s/<CODE>/$code/isg;
#	$jad=~s/<DESC>/$desc/isg;
	$template=~s/<JAR>/$fname\.jar/isg;

	my $cmd=sprintf($amtools::unzip, "$tools::dir_source/template.zip", $srcdir);
	system($cmd);
	open(INF, ">$srcdir/META-INF/MANIFEST.MF") or die $!;
		print INF $template;
	close(INF);
	my $stat="SELECT DISTINCT cities.name, data FROM cities, locations ".
		"WHERE cities.id IN ($ids) AND city_id=cities.id AND year=$year".
		" ORDER BY cities.name";
#	print $stat;
	my $sth = $dbh->prepare($stat)|| die $dbh->errstr;
	$sth->execute|| die $dbh->errstr;
	my $i=0;
	while(my @row = $sth->fetchrow_array){
		push(@data, $row[1]);		
	}
	$sth->finish;
	amtools::join_datafiles2("$srcdir/locations.dat", \@data);
	$cmd=sprintf($amtools::zip, $srcdir, "$tools::dir_files/$fn");
	system($cmd);
	#die $cmd;
	my $asize= -s "$tools::dir_files/$fn.r";
	$template.="MIDlet-Jar-Size: $asize\n";
	open(FFF, ">$tools::dir_files/$fn.d") or die "$tools::dir_files/$fn.d: $!";
	print(FFF $template);
	close(FFF);
	my $server="http://".server_name();
	$template=~s%(MIDlet-Jar-URL: ).+?\n%$1$server/cgi-bin/data.cgi\?r=$fn\n%is;
	open(FFF, ">$tools::dir_files/$fn.t");
	print(FFF $template);
	close(FFF);
	system("rm -R $srcdir/*");
	rmdir $srcdir;
	my $sql='INSERT INTO files (id, type, user_id, end_tm) VALUES';
	foreach (('r','d','t')){
		$sql.=" ($fn, \'$_\', ".$userid.", NOW()+ INTERVAL 2 HOUR),";
	}
	$sql=~s/,$//is;
	$sth = $dbh->prepare($sql)|| die $dbh->errstr;
	$sth->execute|| die $dbh->errstr;
	$sth->finish;
	return $fn;
}

1;