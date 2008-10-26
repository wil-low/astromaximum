<?php
if(!isset($EXEC)) die("Access restricted");
include_once("mobi/amtools.php");
include_once("mobi/ipblock.php");
allow_ip('demo',false);
//print_r($_REQUEST);
if(check_access()!=-1){
	$uri=htmlentities($_SERVER['REQUEST_URI']);
	$prev_year=$GLOBALS['amax']['year']-1;
	if(isset($_POST['p_captcha'])){
		$captcha=$_POST['p_captcha'];
		if(is_captcha($captcha)){
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
                        echo "<p><a href=\"$uri\">{$i18['BACK']}</a></p>";
                        return;
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
		else{
			echo "<p><span class=\"alert\">{$i18['CAPTCHA_WRONG']}</span></p>";
		}
	}
	$sess=session_name().'='.session_id();
	echo $i18['DEMO_HEADER'];
	$emailing=sprintf($i18['DEMO_EMAILING'], $GLOBALS['amax']['mail_office']);
	$msg_dlcity=sprintf($i18['DEMO_DLCITY'], anchor("dl"), $prev_year);
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
<b>ASTROMAXIMUM</b> {$i18['DEMO_DEMO']} <b>{$prev_year}</b><br/><br/>
<input type="button" class="ok_on" value="OK" onclick="form.submit()"/>
<br/><br/></p>
<p></p>
<p>{$msg_dlcity}</p>

EOF1;
return;
}
// guest user warning
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
