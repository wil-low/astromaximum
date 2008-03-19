#include "main.h"
#include "fMain.h"
#include <stdio.h>
#include <unistd.h>
#include <fltk/run.h>
#include <fltk/filename.h>
#include <fltk/file_chooser.h>
#include <fltk/ask.h>
#include <time.h>
#include <algorithm>
using namespace std;
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

fltk::Window *wnd=(fltk::Window *)0;
const int IMEI_LEN=15;
const char LOCL_TEMP[]=".locl.txt";
char path[256];
int year=0;
bool sortbystate=false;

LocRec selected, avail;

char* add_messjar_switch(char* str);
    
void refresh_lbsize() {
	int sz=selected.size();
	if(!sz){
		lbSize->copy_label("");
	}
	else{
		float locsize=(5100+(sz)*8300/2.25)/1024;
		char lbl[100];
		sprintf(lbl, "Cities = %d,   file size ~ %.2f kb",sz, locsize);
		lbSize->text(lbl);
	}
}

void move_record(LocRec &src, LocRec &dest, int index){
	if((index>=0) && (index<src.size())){
		dest.push_back(src[index]);
		LocRec::iterator iter=src.begin()+index;
		src.erase(iter);
		refresh_lbsize();
	}
}

void cb_sort(fltk::RadioButton *chk, void*) {
//	printf("%s\n", chk->label());
	sortbystate=(int)chk->user_data()==1;
	do_sort();
}

void cb_select(MyBrowser *lst, void*) {
	move_record(avail, selected, lst->value());
	refill_all();
}

void cb_unselect(MyBrowser *lst, void*) {
	move_record(selected, avail, lst->value());
	refill_all();
}

void cb_imei_changed(fltk::IntInput* input, void*){
	int tmp;
	bool act=strlen(input->text())==IMEI_LEN;
	bImei->activate(act);
	bImeiLogger->activate(act);
}

void cb_year_changed(fltk::Input* input, void*){
	bool act=strlen(input->text())==4;
	bSetYear->activate(act);
}

void set_year(const char *str) {
	static char title[80];
	int tmp;
	if(sscanf(str, "%4d", &tmp)==1){
		snprintf(title, 80, "RelGui - %s", str);
		year=tmp;
		wnd->label(title);
		get_city_list(avail);
		refill_all();
	}
	else{
		fltk::alert("Invalid year - %s", str);
	}
}

void refill_list(fltk::Browser *lst, LocRec &v) {
	char item_name[300];
	lst->clear();
	for(int i=0; i<v.size(); i++){
		if(sortbystate){
			sprintf(item_name, "%s, %s", v[i].state.c_str(), v[i].city.c_str());
		}
		else{
			sprintf(item_name, "%s, %s", v[i].city.c_str(), v[i].state.c_str());
		}
		lst->add(item_name);
	}
}

void refill_all() {
	refill_list(lbSelected, selected);
	do_sort();
}


void get_city_list(LocRec &v) {
	v.clear();
	struct dirent **namelist;
	char cur_txt[1000];
	char ini[100];
	const char dir[]="data/archive";
	City rec;

	int n = fltk::filename_list(dir, &namelist, 0);
	if (!n)
		perror("scandir");
	else {
		for(int i=0; i<n; i++) {
			strcpy(ini, namelist[i]->d_name);
			free(namelist[i]);
			sprintf(cur_txt, "%s/%s", dir, ini);
			if(!fltk::filename_match(cur_txt, "*.txt")){
				continue;
			}
			*(fltk::filename_ext(ini))=0;
/*			strcpy(fltk::filename_ext(cur_txt), ".txt");
			if(!fltk::filename_exist(cur_txt)){
				printf("No region: %s\n", cur_txt);
				continue;
			}
*/			FILE *intxt=fopen(cur_txt, "r");
			if(!intxt){
				perror("txt open");
				continue;
			}
			int ii=0;
			char *city, *state, *ind;
			while(fgets(cur_txt, 1000, intxt)){
				if(strstr(cur_txt, "#"))
					continue;
				if(ind=strchr(cur_txt, '\r'))
					*ind=0;
				if(ind=strchr(cur_txt, '\n'))
					*ind=0;
				cur_txt[strlen(cur_txt)-4]='-';
				state=strrchr(cur_txt, '|')+1;
				if(ind=strchr(cur_txt, '|'))
					*ind=0;
				city=cur_txt;
				ind=strchr(cur_txt, '!');
				if(ind){
					city=ind+1;
				}
				ind=strchr(state, '$');
				if(ind){
					state=ind+1;
				}
//		printf("\nC: %s\tS: %s", city, state);
				char dpath[1000];
				sprintf(dpath, "%s/%04d/%s/Data%04d.dat", dir, year, ini, ii);
				if(fltk::filename_exist(dpath)){
					sprintf(dpath,"%s:Data%04d %s", ini, ii, city);
					rec.city=city; rec.state=state; rec.datapath=dpath;
					v.push_back(rec);
				}
				else{
					printf("No datafile %s\n", dpath);
				}
				ii++;
			}
		}
		free(namelist);
	}
}

bool comparator(const City &lhs, const City &rhs){
	if(sortbystate){
		return strcmp(lhs.state.c_str(), rhs.state.c_str())<0;
	}
	else{
		return strcmp(lhs.city.c_str(), rhs.city.c_str())<0;
	}
}

void do_sort(){
	sort(avail.begin(), avail.end(), comparator);
	refill_list(lbAvailable, avail);
}
// Callbacks
static void cb_SetYear(fltk::Button*, void*) {
	set_year(txtYear->text());
}

void cb_do_demo(fltk::Button*, void*){
	char cmd[200];
	sprintf(cmd, "perl %s/gen_amax.pl demo %d %s %s -",
		path, year, pbLang->label(), lbLoclist->text());
	int result=run_exe(add_messjar_switch(cmd));
}

void cb_do_geo(fltk::Button*, void*){
	FILE *locl=fopen(LOCL_TEMP, "w");
	for(int i=0; i<selected.size(); i++){
		fprintf(locl,"%s\n", selected[i].datapath.c_str());
	}
	fclose(locl);
	char cmd[200];
	sprintf(cmd, "perl %s/gen_amax.pl geo- %d %s %s -",
		path, year, pbLang->label(), LOCL_TEMP);
	int result=run_exe(add_messjar_switch(cmd));
}

void cb_do_timebomb(fltk::Button*, void*){
	char cmd[200];
	sprintf(cmd, "perl %s/gen_amax.pl tb %d %s %s - %s %s",
		path, year, pbLang->label(), lbLoclist->text(), 
                txtTimeOffset->text(), txtTimeDelta->text());
	int result=run_exe(add_messjar_switch(cmd));
}

void cb_do_timebomb_logger(fltk::Button*, void*){
	char cmd[200];
	sprintf(cmd, "perl %s/gen_amax.pl tb-logger %d %s %s - %s %s",
		path, year, pbLang->label(), lbLoclist->text(), 
                txtTimeOffset->text(), txtTimeDelta->text());
	int result=run_exe(add_messjar_switch(cmd));
}

void cb_do_imei(fltk::Button*, void*){
	char cmd[200];
	sprintf(cmd, "perl %s/gen_amax.pl imei %d %s %s - %s",
		path, year, pbLang->label(), lbLoclist->text(), txtImei->text());
	int result=run_exe(add_messjar_switch(cmd));
}

void cb_load_loclist(fltk::Button*, void* udata){
	const char *locfile=fltk::file_chooser("Load loclist", "*.txt", "", 0);
	fltk::message("%s", locfile);
}

void cb_save_loclist(fltk::Button*, void* udata){

}

void cb_clear_loclist(fltk::Button*, void* udata){
	
}

char* add_messjar_switch(char* str){
    if(!ckMessjar->value()){
        strcat(str, " nomessjar");
    }
    return str;
}

int run_exe(const char *cmd){
	wnd->activate(0);
#ifdef _WIN32
    STARTUPINFO		suInfo;		// Process startup information
    PROCESS_INFORMATION	prInfo;		// Process information

    memset(&suInfo, 0, sizeof(suInfo));
    suInfo.cb = sizeof(suInfo);

    int icommand_length = strlen(cmd);

    char* copy_of_icommand = new char[icommand_length+1];
    strcpy(copy_of_icommand, cmd);

    // On _WIN32 the .exe suffix needs to be appended to the command
    // whilst leaving any additional parameters unchanged - this
    // is required to handle the correct conversion of cases such as : 
    // `../fluid/fluid valuators.fl' to '../fluid/fluid.exe valuators.fl'.

    // skip leading spaces.
    char* start_command = copy_of_icommand;
    while(*start_command == ' ') ++start_command;

    // find the space between the command and parameters if one exists.
    char* start_parameters = strchr(start_command,' ');

    char* command = new char[icommand_length+6]; // 6 for extra 'd.exe\0'

    if (start_parameters==NULL) { // no parameters required.
#  ifdef _DEBUG
      sprintf(command, "%sd.exe", start_command);
#  else
      sprintf(command, "%s.exe", start_command);
#  endif // _DEBUG
    } else { // parameters required.
      // break the start_command at the intermediate space between
      // start_command and start_parameters.
      *start_parameters = 0;
      // move start_paremeters to skip over the intermediate space.
      ++start_parameters;

#  ifdef _DEBUG
      sprintf(command, "%sd.exe %s", start_command, start_parameters);
#  else
      sprintf(command, "%s.exe %s", start_command, start_parameters);
#  endif // _DEBUG
    }

    DWORD result=CreateProcess(NULL, command, NULL, NULL, FALSE,
                  NORMAL_PRIORITY_CLASS, NULL, NULL, &suInfo, &prInfo);
		if(result){
	    delete command;
	    delete copy_of_icommand;
			WaitForSingleObject(prInfo.hProcess,INFINITE);
			GetExitCodeProcess(prInfo.hProcess, &result);
	    CloseHandle(prInfo.hProcess);
	    CloseHandle(prInfo.hThread);
	  }
#else // NON _WIN32 systems.

	printf("Cmd=%s\n", cmd);
    int result=system(cmd);
#endif // _WIN32
	printf("Result=%d\n", result);
	if(result){
		fltk::alert("gen_max.pl error occured.");
	}
	else{
		fltk::message("gen_max.pl completed successfully.");
	}
	wnd->activate(1);
	return result;
}

int main(int argc, char** argv) {
	char ystr[5];
	time_t t;
	struct tm *tmp;
	char fullpath[256];
	
	t = time(NULL);
	tmp = localtime(&t);
	year=tmp->tm_year+1900;
	sprintf(ystr, "%04d", year);
	int len=fltk::filename_absolute(path, 255, "", NULL);
	strcat(path, "..");
	chdir(path);
	printf("Current directory is: %s\n", path);
	wnd=make_window();
	txtYear->text(ystr);
	txtImei->text("359593001109710");
	lbLoclist->text("loclist_default.txt");
	cb_SetYear(0, 0);
	cb_imei_changed(txtImei, 0);
	wnd->x(50);	wnd->y(50);
	wnd->show();
	return fltk::run();
}

