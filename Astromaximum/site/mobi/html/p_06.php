<?php
$META_TITLE='Plimus';
$cur_year=$GLOBALS['amax']['year'];
$price=$GLOBALS['amax']['price'];

$admin_email='dev@astromaximum.mobi';

$contracts = array('ru' => 2528726, 'en' => 2528838);
$lngs = array('ru' => 'RUSSIAN', 'en' => 'ENGLISH');

$svc_url='https://www.plimus.com/jsp/buynow.jsp?contractId=' . $contracts[$lang] . '&language=' . $lngs[$lang];

echo <<< EOF
<h4>Plimus</h4>
{$i18['PAYDESC_06']}
<hr><p>{$i18['PAYLINK']}<br/><br/>
<a href="$svc_url" target="_blank" title="Trusted and Secure Online Payment Processing via PLIMUS">
<img src="http://www.plimus.com/images/icons_wizard/icons/cards/cards_type2_2-5.gif" border="0">
</a>
EOF;

?>
