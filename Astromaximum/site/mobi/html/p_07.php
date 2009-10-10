<?php
    $title='share-it';
    $custom_content='-- примечания --';
    
if(!isset($EXEC)){ // don't show share-it button when in popup
        
$cur_year=$GLOBALS['amax']['year'];
$price=$GLOBALS['amax']['price'];

$admin_email='dev@astromaximum.mobi';

$products = array('ru' => 300335555, 'en' => 300334680);
$lngs = array('ru' => '12', 'en' => '1');

$svc_url='http://www.shareit.com/product.html?productid=' . $products[$lang] .
	'&stylefrom=' . $products[$lang] . 
	'&languageid=' . $lngs[$lang] .
	'&backlink=http%3A%2F%2Fastromaximum.com&pc=4995y&currencies=USD&nolselection=1';

echo <<< EOF
<a href="$svc_url" target="_blank" title="Trusted and Secure Online Payment Processing via share-it e-commerce partner"><img src="http://a124.e.akamai.net/f/124/5462/2d/images.element5.com/shareit/images/checkout_logo_shi.gif" border="0"></a>
EOF;

}else{ ?>

share-it instruction

<?php } ?>