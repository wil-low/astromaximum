function findObj(id) {
  return (document.all?document.all[id]:document.getElementById(id));
}

function checkCheckBox(b){
	if(b.form.agree.checked==false)
	{
		alert('Please check the box to continue.');
	}
	else{
        b.form.demo.value=b.form.agree.value;
		b.form.submit();
	}
}

function checklogin(){
  	if(!findObj('ilog').value || !findObj('ipwd').value){
  		return false;
  	}
	findObj('flog').submit();
  	return true;
}

function is_empty(id){
	return findObj(id).value.length==0;
}

function check_notify(){
	if(!findObj('u_notify').checked) return true;
	return !is_empty('u_pwd1') && !is_empty('u_email') && !is_empty('u_login');
}

function check_user(){
	if(is_empty('u_login') || is_empty('u_realname')){
		alert("Missing login or realname");
		return;
	}
	if(!check_notify()){
		alert("Notify user: Missing login or password or email");
		return;
	}
	upw1=findObj('u_pwd1').value;
	upw2=findObj('u_pwd2').value;
	if(upw1!=upw2){
		alert("Passwords do not match!"); return;
	}
	else{
		if(upw1.length>0 && upw1.length!=9){
			alert("Password must be 9 digits long!"); return;
		}
	}
	if(findObj('u_paymode').selectedIndex==0){
		alert("Please select paymode!"); return;
	}
	findObj('usredit').submit();
}

function do_random(input_id){
	var str='';
	for(i=0; i<9; i++){
		str=str.concat(Math.floor(Math.random()*9));
	}
	findObj(input_id).value=str;
}

function open_scr(lang, n){
	window.open("/shot.php?lang="+lang+"&n="+n, "", "target=blank, width=320, height=530");
}

function citySelector(lang, input_id){
    window.open("/mobi/html/citysel.php?lang="+lang, "", "target=blank, width=500, height=420");
}