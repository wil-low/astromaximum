function showc(country,state){
	frm=document.forms.namedItem("main");
	if(frm.elements.namedItem('cid').value!=country || frm.elements.namedItem('stateid').value!=state){
		frm.elements.namedItem('stateid').value=state;
		frm.elements.namedItem('cid').value=country;
		frm.submit();
	}
}

function highlight_gen(lb){
	if(lb.selectedIndex<0) return;
	btn=findObj('genbtn');
	btn.style.background="url('i/btn_on.png')";
	btn.style.fontWeight="bold";
}

function showc2(mode,val)
{ 
    var xhr; 
    try {
        xhr = new ActiveXObject('Msxml2.XMLHTTP');
    }
    catch (e) {
        try {
            xhr = new ActiveXObject('Microsoft.XMLHTTP');
        }
        catch (e2) {
            try {
                xhr = new XMLHttpRequest();
            }
            catch (e3) {
                xhr = false;
            }
        }
     }
    var cid=document.main.cid.value;
    var stateid=document.main.stateid.value;
//    document.getElementById('status').innerHTML=mode+','+val+','+cid+','+stateid;
    if(!mode){
        xhr.onreadystatechange  = function()
        { 
            if(xhr.readyState  == 4) {
                if(xhr.status  == 200) {
                    con = eval('(' + xhr.responseText + ')');
					var content=con.content;
                    fill_lb(document.main.countries, content, cid);
                    document.main.countries.selectedIndex=0;
                    document.main.cid.value=firstId(content);
                    showc2(1, document.main.cid.value);
                }
            }
            
        }; 
    }
    if(mode==1){
        document.main.cid.value=val;
        xhr.onreadystatechange  = function()
        { 
            if(xhr.readyState  == 4) {
                if(xhr.status  == 200) {
                    con = eval('(' + xhr.responseText + ')');
					var content=con.content;
                    fill_lb(document.main.states, content, document.main.stateid.value);
                    document.main.states.selectedIndex=0;
                    document.main.stateid.value=0;
                    showc2(2, firstId(content));
                }
            }
            
        }; 
    }
    if(mode==2){
        document.main.stateid.value=val;
        xhr.onreadystatechange  = function()
        { 
            if(xhr.readyState  == 4) {
                if(xhr.status  == 200) {
                    con = eval('(' + xhr.responseText + ')');
					var content=con.content;
                    fill_lb(document.main.cities, content, 0);
                }
            }
            
        }; 
    }
    var url='/mobi/html/dl2.php?lang=' + document.main.lang.value + '&ajax='+mode+'&cid=' + document.main.cid.value +
        '&stateid=' + document.main.stateid.value;
//    alert(url);
    xhr.open('GET', url,  true);
    xhr.send(null); 
}

function firstId(arr){
	return arr[0][0];
}

function fill_lb(listbox, arr, selindex)
{
    listbox.options.length=0;
    for(var i = 0; i < arr.length; i++){
        listbox.options[i] = new Option(arr[i][1], arr[i][0], arr[i][0] == selindex);
    }
}

function dl_init(){
	if(document.main.countries.length==0){
//		alert("Loaded");
	    showc2(0,0,0);
	}
}