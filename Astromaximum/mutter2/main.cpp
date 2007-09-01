//---------------------------------------------------------------------------
#include "events.h"
#include "datafile.h"
#include "assert.h"
#include <time.h>
#ifdef _WIN32_
	#include <dir.h>
#endif
#include <fstream>
#include <assert.h>
using namespace std;
#pragma hdrstop

//---------------------------------------------------------------------------

#pragma argsused

const int NOT_ENOUGH_PARAMS=-1,
      INVALID_YEAR=-2,
      INVALID_EVENT=-3;

;
char ephemPath[]="swiss";
const char outFile[]="output.txt";

sAphRecord aphetics[SE_SATURN+1];

int main(int argc, char* argv[])
{
  char path[255];

  swe_set_ephe_path(ephemPath);
/*
  double test_day=swe_julday(2007, 4, 7, 0, SE_GREG_CAL);
  double result;
  double geopos[3]={30.51,50.43,200};
  swe_rise_trans(test_day,SE_MOON,NULL,0,SE_CALC_SET| SE_BIT_DISC_CENTER ,geopos,0,20,&result,path);
  int y,m,d; double hour;
  swe_revjul(result, SE_GREG_CAL, &y, &m, &d, &hour);
  printf("%02d.%02d.%04d %02d:%02d",d, m, y, (int)hour, (int)((hour-(int)hour)*60));
  scanf("%s",path);
  return 0;
*/
	if((argc==2)&&(strcmp(argv[1],"--help"))==0){
		printf("Usage:  mutter2 [options]\n");
		printf(" options:\n");
		printf("   asctest <dir> - ascending test for events in dir/*.bin\n");
		printf("   <year> - calculate ephemeris if none, and common.dat\n");
		printf("   year <prefix> electio - calculate APHETICS with prefix\n");
		printf("   year <prefix> <lon> <lat> [electio]- calc locations on coords with prefix\n");
		printf("   year view <file.bin> <count> - view events of datafile\n");
		exit(0);
	}
  strcpy(path,argv[0]);
  printf("argv[0] is: %s\n", path);
  char *pos=strrchr(path,'\\');
  if(!pos){
    pos=strrchr(path,'/');
  }
  if(pos){
      *(pos+1)=0;
  }
	else{
		path[0]=0;
	}
  strcat(path,"../mutter/");
  chdir(path);
  printf("Current directory is: %s\n", path);
//  return 0;
  struct tm now;
  now.tm_year=2006-1900;
  now.tm_mon=11;
  now.tm_mday=8;
  now.tm_hour=18;
  now.tm_min=3;
  now.tm_sec=0;
  now.tm_isdst = 0;

#ifdef ANSITZ
  time_t loo=mktime(&now);//-_timezone;
  tm *st=gmtime(&loo);
  time_t loo1=mktime(st);
  Event::_timezone_=loo1-loo;
  printf("Local timezone offset %d\n",Event::_timezone_);
#else  
  time_t loo=mktime(&now)-_timezone;
  tm *st=gmtime(&loo);
  loo=mktime(st);
#endif  
  
  assert(sizeof(sMatrix)==9);
  assert(EV_LAST==50);
  if(argc<2) return NOT_ENOUGH_PARAMS;
  DataFile df;
  char buf[20];
  if(strcmp(argv[1],"asctest")==0){
    df.AscendingTest(argv[2]);
    printf("\n%s\n","Finished.");
    scanf("%s",buf);
    return 0;
  }
  sEphRecord *ephData=NULL;
  int year;
  if(sscanf(argv[1],"%4d",&year)!=1)
    return INVALID_YEAR;
  printf("Year = %d\n",year);
  Event::startYear=year;
  if((argc==5)&&(strcmp(argv[2],"view")==0)){
    int count=0;
    sscanf(argv[4],"%d",&count);
    df.view(argv[3], count);
    return 0;
  }
  double startJD=swe_julday(year-1,12,31,0,SE_GREG_CAL);
  printf("startJD=%f\n",startJD);
  double endJD=swe_julday(year+1,2,1,0,SE_GREG_CAL);


//  double endJD=swe_julday(year,2,1,0,SE_GREG_CAL);


  int dayCount=endJD-startJD;
  unsigned int stepCount=dayCount/MINUTE_STEP;
  printf("Steps = %d\n", stepCount);
  double data[6]; char serr[255],ephf[255];
  ephData=new sEphRecord [stepCount];
  endJD=startJD;
  int size=sizeof(sEphRecord)*stepCount;
  sprintf(ephf,"../ephdata%04d.dat",year);

  FILE *fin=fopen(ephf, "rb");
  int fsz=0;
  if(fin){
    fseek(fin,0,SEEK_END);
    fsz=ftell(fin);
  }
  if(fsz){
    printf("\nValid cached ephdata found. Loading...");
    rewind(fin);
    fread(ephData,size,1,fin);
    fclose(fin);
    printf("Done.\n");
  }
  else{
    printf("\nCalculating ephdata...");
    if(fin){
        fclose(fin);
    }

    printf("\nSteps = %d\n",stepCount);
    for(int i=0; i<stepCount; i++){
      for(int body=0; body<13; body++){
        swe_calc_ut(endJD, PLANETS[body], EFLAG, data, serr);
        ephData[i].data[body]=data[0];
      }
      endJD+=MINUTE_STEP;
      if(i%10000==0)
        printf("%d...",i/10000);
      	fflush(stdout);
    }
    printf("\nSaving cached ephemeris...");
    FILE *fout=fopen(ephf,"wb");
    fwrite(ephData,size,1,fout);
    fclose(fout);
    printf("Done.\n");
    return 0;
  }
  df.init(ephData,startJD, dayCount);
  if(argc==2){ // only year specified
    df.AAA();  // calculate common.dat
  //  df.saveFile(outFile);
    delete[] ephData;
    ephData=NULL;
    printf("\nPress any key...");
    scanf("%s",buf);
  }
  else{
    VAE work, assist, vout, work2;
    if(strcmp(argv[2],"electio")==0){
//      df.loadAphetics(aphetics);
      df.choice(EV_APHETICS, work, assist, vout, work2, argv[3]);
//      scanf("%s",buf);
    }
    else{
			if(strcmp(argv[2],"vocsql")==0){
				
			}
			else{
				if(argc<5)
					return NOT_ENOUGH_PARAMS;
				df.Lon=strtod(argv[3],NULL);
				df.Lat=strtod(argv[4],NULL);
		// TODO remove this line
	//      df.stepCount=8000;
				df.choice(EV_NAVROZ, work, assist, vout, work2, argv[2]);
				df.calcAscData();

				df.choice(EV_ASTRORISE, work, assist, vout, work2, argv[2]);
				df.choice(EV_RISE, work, assist, vout, work2, argv[2]);
				if(argc>5 && (strcmp(argv[5],"electio")==0)){
					df.choice(EV_ASCAPHETICS, work, assist, vout, work2, argv[2]);
				}
      }
    }
    delete[] ephData;
  }
  if(df.ascData){
    delete[] df.ascData;
  }
  printf("\nFinished.\n");
  return 0;
}



