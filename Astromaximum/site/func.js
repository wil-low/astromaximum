function findObj(id) {
  return (document.all?document.all[id]:document.getElementById(id));
}

function checkCheckBox(b){
	if(b.form.agree.checked==false)
	{
		alert('Please check the box to continue.');
	}
	else{
		b.form.submit();
	}
}

function checklogin(){
  	if(!findObj('ilog').value || !findObj('ipwd').value){
  		return false;
  	}
	findObj("flog").submit();
}

function is_empty(id){
	return findObj(id).value.length==0;
}

function check_notify(){
	if(!findObj('u_notify').checked) return true;
	return !is_empty('u_pwd1') && !is_empty('u_email') && !is_empty('u_login');
}

function check_user(){
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
	if(check_notify()){
		findObj('usredit').submit();
	}
	else{
		alert("Notify user: Missing login or password or email");
	}
}

function do_random(input_id){
	var str='';
	for(i=0; i<9; i++){
		str=str.concat(Math.floor(Math.random()*9));
	}
	findObj(input_id).value=str;
}
