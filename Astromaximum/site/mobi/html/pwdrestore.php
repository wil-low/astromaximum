<?php
print_r($_REQUEST);
?>
<h4>Восстановление пароля</h4>
<p>Введите e-mail, указанный Вами при регистрации:</p>
<form action="<?php echo $_SERVER['REQUEST_URI']?>" method="post">
<input name="p_email" type="text" style="width: auto"/>
<p>Введите символы, указанные на рисунке:</p>
<p><img src="capcha.png">
<input name="p_capcha" type="text"/>
</p>
<input name="action" type="submit"/>
</form>
