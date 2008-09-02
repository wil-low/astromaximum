<?php
if(!isset($EXEC)) die("Access restricted");
include_once("mobi/amtools.php");
include_once("mobi/ipblock.php");
allow_ip('demo',false);
if(check_access()!=-1){
	$uri=htmlentities($_SERVER['REQUEST_URI']);
	if(isset($_POST["demo"]) && isset($_POST['p_captcha'])){
		$captcha=$_POST['p_captcha'];
		if(is_captcha($captcha)){
            $is_demo=$_POST["demo"];
			$prev_year=$GLOBALS['amax']['year']-1;
			if($is_demo){ // demo
				if(isset($_POST["email"]) && check_email_address($_POST["email"])){
					$email=$_POST['email'];
					$sc=get_default_cities($GLOBALS['amax']['def_cities']); 
					$link_text=midlet_create("demo", $prev_year, $lang, $sc, "mobi/dl", false);
                    if(!$link_text){
                        echo '<p><span class="alert">Error</span></p>';
                    }
                    else{
                        $message=file_get_contents("mobi/dl/source/demo.mail");
                        $message=str_replace('[site]', $GLOBALS['amax']['mail_site'], $message);
                        $message=str_replace('[links]', $link_text, $message);
                        $mail=mailtext_w_attach($email, '', 'Astromaximum demo - download link', $message);
                        if(!$mail->ErrorInfo){
                            echo "<p>".sprintf($i18['DEMO_LINK_SENT'], $email)."</p>";
                        }
                        else{
                            echo '<p><span class="alert">'.$mail->ErrorInfo."</span></p>";
                        }
                    }
				}
				else{  # wrong email
					echo "<p><span class=\"alert\">{$i18['DEMO_EMAIL_WRONG']}</span></p>";
				}
			}
			else{ # demo cities
                $choice=0;
                if(isset($_POST["agree"]))
                    $choice=$_POST["agree"];
				if(is_numeric($choice) && ($choice>=0) && ($choice<count($GLOBALS['amax']['demo_cities']))){
					echo "<p>".sprintf($i18['READY_CITIES'], $GLOBALS['amax']['demo_cities'][$choice], $prev_year)."</p>\n";				
					$sc=get_default_cities($GLOBALS['amax']['demo_cities'][$choice]); 
					$link_text=midlet_create("geo", $prev_year, $lang, $sc, "mobi/dl", true);
					echo "<p>$link_text</p>";
				}
			}
			echo "<p><a href=\"$uri\">{$i18['BACK']}</a></p>";
			return;
		}
		else{
			echo "<p><span class=\"alert\">{$i18['CAPTCHA_WRONG']}</span></p>";
		}
	}
	$sess=session_name().'='.session_id();
	echo $i18['DEMO_HEADER'];
	$emailing=sprintf($i18['DEMO_EMAILING'], $GLOBALS['amax']['mail_office']);
	echo <<<EOF1

<form action="$uri" method="post">
<p>{$emailing} <input name="email" type="text"/></p>
<!--<p><input name="btnDemo" type="button" onclick="citySelector(this)"/></p>-->
<p>{$i18['CAPTCHA_PROMPT']}</p>
<p><img src="mobi/kcaptcha?$sess" alt="Captcha">
<input name="p_captcha" type="text"/>
</p>
<p><span class="fine">{$i18['DEMO_CALGEN']}:</span><br/><br/>
<!--<input type="radio" name="agree" value="demo" style="width:auto; border: 0px" checked="checked"/> -->
<b>ASTROMAXIMUM</b> {$i18['DEMO_DEMO']}<br/><br/>
<input type="hidden" name="demo" value="0"/>
<input type="button" class="ok_on" value="OK" onclick="this.form.demo.value=1;form.submit()"/>
<br/><br/></p>
<p></p>
<p>{$i18['DEMO_DLCITY']}</p>
<span class="fine">{$i18['SELCITY_GENERATE']}:</span><br/><br/>

EOF1;
	foreach($GLOBALS['amax']['demo_cities'] as $i=>$city){
		echo '<input type="radio" name="agree" value="'.$i.'" style="width:auto; border: 0px"/> '.$city." &nbsp; &nbsp;\n";
	}
	echo '<br/><br/><input type="submit" class="ok_on" value="OK"/></form>';
	return;
}
$press_enter=sprintf($i18['DEMO_ENTER'],$i18['LOG_IN']);
echo <<<EOF2
<p>{$i18['DEMO_NOTREG']}</p>
<ul>
<li>{$i18['LOGIN']}: {$GLOBALS['amax']['demo_login']}</li>
<li>{$i18['PWD']}: {$GLOBALS['amax']['demo_pass']}</li>
</ul>
<p>$press_enter</p>
EOF2;
?>
