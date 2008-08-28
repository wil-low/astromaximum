function showc(country,state){
	frm=document.forms.namedItem("main");
	if(frm.elements.namedItem('cid').value!=country || frm.elements.namedItem('stateid').value!=state){
		frm.elements.namedItem('stateid').value=state;
		frm.elements.namedItem('cid').value=country;
		frm.submit();
	}
}
function generate(country){
/*    
	lst=findObj("chkcit");
	ind=lst.selectedIndex;
	if(ind<0){
		alert("<?php echo $i18['SELCITY_ALERT']?>");
		return;
	}
	if(confirm("<?php echo $i18['SELCITY_GENERATE']?>:\n"+lst.item(ind).text+", "+country+"?")){
		frm=document.forms.namedItem("main");
		frm.elements.namedItem("sc").value=lst.item(ind).value;
		frm.elements.namedItem("Action").value=1;
		frm.submit();
	}
*/
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
    document.getElementById('status').innerHTML=mode+','+val+','+cid+','+stateid;
    if(!mode){
        xhr.onreadystatechange  = function()
        { 
            if(xhr.readyState  == 4) {
                if(xhr.status  == 200) {
                    doc = eval('(' + xhr.responseText + ')');
                    fill_lb(document.main.countries, doc.countries, cid);
                    document.main.countries.selectedIndex=0;
                    document.main.cid.value=doc.countries[0].id;
                    showc2(1, doc.countries[0].id);
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
                    doc = eval('(' + xhr.responseText + ')');
                    fill_lb(document.main.states, doc.states, document.main.stateid.value);
                    document.main.states.selectedIndex=0;
                    document.main.stateid.value=0;
                    showc2(2, doc.states[0].id);
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
                    var doc = eval('(' + xhr.responseText + ')');
                    fill_lb(document.main.cities, doc.cities, 0);
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
if(document.main.countries.length==0){
    showc2(0,0,0);
}

function fill_lb(listbox, arr, selindex)
{
    listbox.options.length=0;
    for(var i = 0; i < arr.length; i++){
        listbox.options[i] = new Option(arr[i].name, arr[i].id, arr[i].id == selindex);
    }
}