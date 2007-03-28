//---------------------------------------------------------------------------
#include "events.h"
#include "datafile.h"
#include <time.h>
#include <dir.h>
#include <fstream>
#include <assert>
using namespace std;
#pragma hdrstop

//---------------------------------------------------------------------------

#pragma argsused

const NOT_ENOUGH_PARAMS=-1,
      INVALID_YEAR=-2,
      INVALID_EVENT=-3;

;
char ephemPath[]="swiss";
const char outFile[]="output.txt";

int main(int argc, char* argv[])
{
  char path[_MAX_PATH];
  strcpy(path,argv[0]);
  char *pos=strrchr(path,'\\');
  *(pos+1)=0;
  printf("Current directory is: %s\n", path);
  chdir(path);
//  return 0;
  struct tm now;
  now.tm_year=2006-1900;
  now.tm_mon=11;
  now.tm_mday=8;
  now.tm_hour=18;
  now.tm_min=3;
  now.tm_sec=0;
  now.tm_isdst = 0;


  time_t loo=mktime(&now)-_timezone;
  tm *st=gmtime(&loo);
  loo=mktime(st);
  assert(sizeof(sMatrix)==9);
  assert(EV_LAST==30);
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
  swe_set_ephe_path(ephemPath);
  double startJD=swe_julday(year-1,12,31,0,SE_GREG_CAL);
  printf("startJD=%f\n",startJD);
  double endJD=swe_julday(year+1,1,2,0,SE_GREG_CAL);


//  double endJD=swe_julday(year,2,1,0,SE_GREG_CAL);


  int dayCount=endJD-startJD;
  if(argc==2){
    double data[6]; char serr[255];
    unsigned int stepCount=dayCount/MINUTE_STEP;
    ephData=new sEphRecord [stepCount];
    endJD=startJD;
    int size=sizeof(sEphRecord)*stepCount;

    FILE *fin=fopen("..\\ephdata.dat", "rb");
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
      fclose(fin);

      printf("\nSteps = %d\n",stepCount);
      for(int i=0; i<stepCount; i++){
        for(int body=0; body<13; body++){
          swe_calc_ut(endJD, PLANETS[body], EFLAG, data, serr);
          ephData[i].data[body]=data[0];
        }
        endJD+=MINUTE_STEP;
        if(i%10000==0)
          printf("%d...",i/10000);
      }
      printf("\nSaving cached ephemeris...");
      FILE *fout=fopen("..\\ephdata.dat","wb");
      fwrite(ephData,size,1,fout);
      fclose(fout);
      printf("Done.\n");
      return 0;
    }
    df.init(ephData,startJD, dayCount);
    df.AAA();
  //  df.saveFile(outFile);
    delete[] ephData;
    printf("\nPress any key...");
    scanf("%s",buf);
  }
  else{
    if(argc!=5)
      return NOT_ENOUGH_PARAMS;
    df.init(ephData,startJD, dayCount);
    df.Lon=strtod(argv[3],NULL);
    df.Lat=strtod(argv[4],NULL);
    VAE work, assist, vout, work2;
    df.choice(EV_RISE, work, assist, vout, work2, argv[2]);
    df.choice(EV_NAVROZ, work, assist, vout, work2, argv[2]);
  }
  printf("Finished.\n");
  return 0;
}



