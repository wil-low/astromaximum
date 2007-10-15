<?php
function amtools_random($path, $ext){
	do{
		$id=''; $flag=1;
		for($i=0; $i<12; $i++){
			$id.=(int)(rand(0,9));
		}
		$fn="$path/$id$ext";
		if($ext){
			if(is_file($fn)){
				$flag=0;
			}
		}
		else{
			if(is_dir($fn)){
				$flag=0;
			}
		}
	}while(!$flag);
	return array($fn, $id);
}

$UNZIP="unzip %s -d %s > /dev/null";
$UNTAR="tar xvf %s -C %s";
#our $unzip=q("d:/Program Files/WinRAR/WinRar.exe" x %s * %s\ );
$ZIP='sh source/dozip.sh %s ../../%s.r *';
#our $zip=q(zip -r %s.r %s/*);

function join_datafiles2($destfile, $a_data) # destfile, data_listref
{
	$outf=fopen($destfile,'wb');
	$count=0;
	$data='';
	$a_len=array();
	foreach ($a_data as $i => $value) {
    	$data.=$value;
    	$a_len[$count]=strlen($value);
		$count++;
 	}
	fwrite($outf, pack('n',$count));
	foreach ($a_len as $i => $value) {
		fwrite($outf, pack('n',$value));
	}
	fwrite($outf, $data);
	fclose($outf);
}

function rm_all($dir)
{
	foreach(glob("$dir/*.*") as $fname){
		unlink($fname);
	}
	rmdir($dir);
}

?>
