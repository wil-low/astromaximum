function showc(mode, lb)
{
	showc2(mode, lb.item(lb.selectedIndex).value);
}

function highlight_gen(is_sel)
{
	if(findObj('rmore'))
		return;
	lb=findObj('chkcit');
	btn=findObj('genbtn');
	if(is_sel){
		btn.style.background="url('/i/btn_on.png')";
		btn.style.fontWeight="bold";
	}
	else{
		btn.style.background="url('/i/btn_off.png')";
		btn.style.fontWeight="normal";
	}
}

function showc2(mode)
{
    highlight_gen(false);
    var cb;
    var val1;
    if(!mode){
        cb=function(data){
            var content=data.content;
            fill_lb(document.main.countries, content, cid);
            document.main.countries.selectedIndex=0;
            document.main.cid.value=content[0][0];
            showc2(1);
        };
    }
    else{
	    val1=$("select").get(mode);
	    val1=val1.item(val1.selectedIndex).value;
    }
    if(mode==1){
        document.main.cid.value=val1;
        cb=function(data){
            var content=data.content;
            fill_lb(document.main.states, content, stateid);
            document.main.states.selectedIndex=0;
            document.main.stateid.value=0;
            showc2(2);
        };
    }
    if(mode==2){
        document.main.stateid.value=val1;
        cb=function(data){
        	var content=data.content;
        	fill_lb(document.main.cities, content, 0);
        }; 
    }
    var cid=document.main.cid.value;
    var stateid=document.main.stateid.value;
    var year=document.main.y_sel.value;
    var url='/dl/' + mode + '-' + cid + '-' + stateid + '-' + year;
//    alert(url);
    $.getJSON(url, {}, cb);
}

function fill_lb(listbox, arr, selindex)
{
    listbox.options.length=0;
    for(var i = 0; i < arr.length; i++){
        //alert(arr[i]);
        listbox.options[i] = new Option(arr[i].name, arr[i].id, arr[i].id == selindex);
    }
}

function dl_init(){
//	alert("Loaded");
	showc2(0);
}