<?php
$META_TITLE='share-it!';
$cur_year=$GLOBALS['amax']['year'];
$price=$GLOBALS['amax']['price'];

$admin_email='dev@astromaximum.mobi';

$products = array('ru' => 300366182, 'en' => 300366177);
$lngs = array('ru' => '12', 'en' => '1');

$svc_url='http://www.shareit.com/product.html?productid=' . $products[$lang] .
	'&stylefrom=' . $products[$lang] . 
	'&languageid=' . $lngs[$lang] .
	'&backlink=http%3A%2F%2Fastromaximum.com&pc=4995y&currencies=USD&nolselection=1';

echo <<< EOF
<h4>share-it!</h4>
{$i18['PAYDESC_07']}
<hr><p>{$i18['PAYLINK']}<br/><br/>
<a href="$svc_url" target="_blank" title="Trusted and Secure Online Payment Processing via share-it e-commerce partner"><img src="http://a124.e.akamai.net/f/124/5462/2d/images.element5.com/shareit/images/checkout_logo_shi.gif" border="0"></a></p>
EOF;
?>
