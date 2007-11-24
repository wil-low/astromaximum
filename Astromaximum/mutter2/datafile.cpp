//---------------------------------------------------------------------------
#include <fstream>
#include <algorithm>
#include "assert.h"
#include "errno.h"

using namespace std;
#pragma hdrstop

#include "events.h"
#include "datafile.h"
#include <dirent.h>
// 2007 geo0- 30.51 50.43 electio
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define Dgr(deg, min, sec) (deg)+((min)/60.)+((sec)/3600.)

static sMatrix matrix[PLANET_COUNT][PLANET_COUNT];
static const unsigned char ASP_ANGLES[]={0,180,90,120,60};

static const unsigned char OWN_SIGN[2][7]=
  {{4,3,2,1,0,8,9},
   {100,100,5,6,7,11,10}};
static const unsigned char OWN_SIGN_REVERSE[12]=
  {SE_MARS,SE_VENUS,SE_MERCURY,SE_MOON,SE_SUN,SE_MERCURY,
   SE_VENUS,SE_MARS,SE_JUPITER,SE_SATURN,SE_SATURN,SE_JUPITER};
static const unsigned char EXALTATION[7]=
  {0,1,5,11,9,3,6};
static const unsigned char EXALTATION_REVERSE[12]=
  {SE_SUN,SE_MOON,100,SE_JUPITER,100,SE_MERCURY,
   SE_SATURN,100,100,SE_MARS,100,SE_VENUS};
static const unsigned char TRIPLICITY[3][7]={
  {0,1,2,1,3,0,2},
  {4,5,6,5,7,4,6},
  {8,9,10,9,11,8,10}
};

static const char APH_ORBIS[7] = {8,6,4,4,4,5,5};
static const char DECANES[7]=
  {SE_MARS,SE_SUN,SE_VENUS,SE_MERCURY,SE_MOON,SE_SATURN,SE_JUPITER};

static const double AVG_SPEED[7]={
  Dgr(0,59,8), // Sun
  Dgr(13,10,36), // Moon
  Dgr(0,59,8), // Mercury
  Dgr(0,59,8), // Venus
  Dgr(0,31,27), // Mars
  Dgr(0,4,59), // Jupiter
  Dgr(0,2,1), // Sun
};

static const int BURNT_ORBIS[7]={0,7,8,8,11,7,7};

DataFile::DataFile()
{
  Lon=30.51; Lat=50.43; ascData=NULL;
}

void DataFile::init(sEphRecord *ephdata, double start, unsigned int count)
{
  Event::startJD=start;
  startJD=start; dayCount=count; stepCount=(int)(count/MINUTE_STEP);
  ephData=ephdata;
}

struct less_event  {
  bool operator()(Event *c1, Event *c2)
  {
    return c1->date[0]<c2->date[0];
  }
};

void DataFile::view(const char* fname, int count){
  VAE work;
  readSubData(fname, work);
  if(count>work.size()){
    count=work.size();
  }
  printf("\nContents of %s (%d of %d):\n", fname, count, work.size());
  for(int i=0; i<count; i++){
    work[i]->dump();
  }
  release(work);
  printf("\nFinished\n");
}

void DataFile::AscendingTest(const char* dirname)
{
  DIR *dir;
  struct dirent *ent;

  printf("First pass on '%s':\n",dirname);
  if ((dir = opendir(dirname)) == NULL){
    perror("Unable to open directory");
    exit(1);
  }

  while ((ent = readdir(dir)) != NULL)
    if(strstr(ent->d_name,".bin")){
//      printf("\n%s",ent->d_name);
      VAE work;
      readSubData(ent->d_name,work);
      long cur=0;
      for(int i=0; i<work.size(); i++){
        if(work[i]->date[0]<cur){
          printf("\n%s\n", "*****Ascension order is broken!*****");
          break;
        }
        cur=work[i]->date[0];
      }
    }
  closedir(dir);

}

void DataFile::sortVAE(VAE &work)
{
  sort(work.begin(), work.end(), less_event());
}

void DataFile::AAA()
{
  VAE work, assist, vout, work2;
	
/*
  choice(EV_MOON_PHASE, work, assist, vout, work2);
  release(work);
  readSubData("phase01.bin",work);
  for(int i=0; i<2; i++){
    work[i]->dump();
  }
	
  return;
*/
// -----------------------
  choice(EV_ASP_EXACT, work, assist, vout, work2);

  choice(EV_DEGREE_PASS, work, assist, vout, work2);
  choice(EV_VIA_COMBUSTA, work, assist, vout, work2);
//  choice(EV_ASP_EXACT, work, assist, vout, work2);
  choice(EV_SIGN_ENTER, work, assist, vout, work2);
  choice(EV_VOC, work, assist, vout, work2);
  choice(EV_RETROGRADE, work, assist, vout, work2);
  choice(EV_ECLIPSE, work, assist, vout, work2);
  choice(EV_TITHI, work, assist, vout, work2);
  choice(EV_MOON_PHASE, work, assist, vout, work2);

//  choice(EV_RETROGRADE, work, assist, vout, work2);
// ------------------------
//  choice(EV_NAKSHATRA, work, assist, vout, work2);
//  choice(EV_RISE, work, assist, vout, work2);
//  choice(EV_DECL_EXACT, work, assist, vout, work2);
//  choice(EV_NAVROZ, work, assist, vout, work2);
//  choice(EV_APHETICS, work, assist, vout, work2);

//  choice(EV_ASP_EXACT, work, assist, vout, work2);

//  choice(EV_DEGREE_PASS, work, assist, vout, work2);
//  choice(EV_TITHI, work, assist, vout, work2);
//  choice(EV_SIGN_ENTER, work, assist, vout, work2);
//  release(work);
//  return;

//  readSubData("aphetics00.bin",work);
//  readSubData("geo0-rise00.bin",work);
  release(work);
  for(int i=0; i<work.size(); i++){
    work[i]->dump();
  }
  
//    work[i]->dump();
// -----------------------
//  release(work);
//  release(work2);
//  release(assist);

}


void DataFile::calcAspExact(VAE & moonvae,VAE & vae)
{
  // [0] - aspect angle, [1] - counter
  for(int i=0; i<PLANET_COUNT; i++)
    for(int j=0; j<PLANET_COUNT; j++){
      matrix[i][j].ang=1;
      matrix[i][j].counter=0;
  }

  for(int c=0; c<stepCount; c++){
    for(int i=0; i<PLANET_COUNT; i++)
      for(int j=i+1; j<PLANET_COUNT; j++){
        int aspindex=aspectExists(c,i,j,0.01);
        sMatrix &mtx=matrix[i][j];
        if(aspindex!=-1){ // exact - yes
          if(mtx.ang==ASP_ANGLES[aspindex]){ // aspect persists
            ++mtx.counter;
          }
          else{ // new aspect
            registerAspect(moonvae, vae,i,j);
            mtx.counter=1; mtx.step=c; mtx.ang=ASP_ANGLES[aspindex];
          }
        }
        else{ // exact - no
          registerAspect(moonvae,vae,i,j);
          mtx.counter=0; mtx.ang=1;
        }
      }
      if(c%10000==0)
        printf("\b\b%02d",c/10000); fflush(stdout);
    }
  for(int i=0; i<PLANET_COUNT; i++)
    for(int j=0; j<PLANET_COUNT; j++)
      registerAspect(moonvae, vae,i,j);
}

void DataFile::registerAspect(VAE &moonvae, VAE &vae, int i, int j)
{
  const sMatrix &mtx=matrix[i][j];
  if(mtx.counter){ // there was previous aspect
    double tm=startJD+(mtx.step+(mtx.counter-1)/2.)*MINUTE_STEP;
    Event *ec=new Event(tm, i);
    ec->degree=mtx.ang; ec->planetId[1]=j;
    if((i==SE_MOON)||(j==SE_MOON)){
      if(j==SE_MOON){
        ec->planetId[0]=j;
        ec->planetId[1]=i;
      }
      moonvae.push_back(ec);
    }
    else
      vae.push_back(ec);
  }
}

void DataFile::calcDegPass(VAE & vae, int planet)
{
  double endJD=startJD+dayCount;
  Event *ev=new Event(startJD,planet);
  ev->date[1]=ev->packDate(endJD);
  ev->degree=(int)ephData[0].data[planet];
  vae.push_back(ev);
  int lastd=ev->degree;
  double cur=startJD;
  for(int i=1; i<stepCount; i++){
    cur+=MINUTE_STEP;
    int dgr=(int)ephData[i].data[planet];
    if(lastd!=dgr){
      ev->date[1]=ev->packDate(cur);
      ev=new Event(cur,planet);
      ev->date[1]=ev->packDate(endJD);
      ev->degree=dgr;
      lastd=dgr;
      vae.push_back(ev);
    }
  }
}

DataFile::~DataFile()
{
  release(events);
}

void DataFile::release(VAE & v)
{
  for(int i=0; i<v.size(); i++)
    delete v[i];
  v.clear();
}

void DataFile::clearDegPass(VAE & src, VAE & dest, int id, VAE & destall)
{
  static const int degarray[14]={17,68,126,174,222,280,329,22,72,129,180,228,288,333};
  for(int i=0; i<src.size(); i++){
    Event *ev=src[i];
    int degree=ev->degree & 0x3fff;
    int idx=-1;
    for(int i=0; i<14; i++)
      if(degree==degarray[i]){
        idx=i; break;
      }
    if(id==SE_MOON)
      destall.push_back(ev);
    if(idx!=-1){ // selected degree
      degree+=((idx>=7? 1: 2)<<14);
      ev->degree=degree;
      dest.push_back(ev);
    }
    else
      if(id!=SE_MOON)
        dest.push_back(ev);

  }
}

void DataFile::clearSignEnter(VAE & src, VAE & dest)
{
  int sign=-1;
  for(int i=0; i<src.size(); i++){
    Event *ev=src[i];
    int dgr=ev->degree;
    int new_sign=dgr/30;
    if(sign!=new_sign){
      sign=new_sign;
      ev->degree=sign;
      dest.push_back(ev);
    }
  }
}

void DataFile::clearViaCombusta(VAE & src, VAE & dest)
{
  static const int limit[]={6*30+24, 7*30+6};
  Event *tmp;
  if((src[0]->degree>=limit[0])&&(src[0]->degree<limit[1]))
    tmp=new Event(startJD,0); // starting may be in VC
  for(int i=1; i<src.size(); i++){
    Event *ev=src[i];
    if(ev->degree==limit[0])
      tmp=new Event(ev->julianDay,0);
    if(ev->degree==limit[1]){
      tmp->date[1]=tmp->packDate(ev->julianDay);
      dest.push_back(tmp);
      tmp=NULL;
    }
  }
  if(tmp){
    tmp->date[1]=tmp->packDate(startJD+dayCount);
    dest.push_back(tmp);
  }
}

bool DataFile::writeSubData(const VAE & v, EventType evtype, int evflags, int planet, char* fname)
{
  char buf[200];
  sprintf(buf,"archive/%d/%s",Event::startYear,fname);
  printf("\n%s: ",buf);
  FILE *fout=fopen(buf,"wb");
	if(!fout){
		int ern=errno;
		printf("Cannot create file %s: %s",buf,strerror(ern));
		return false;
	}
  fwrite(&evtype, 1, 1, fout);
  long start=ftell(fout);
  long cumul=v[0]->date[0];
  short sBuf; int iBuf;
  fseek(fout,2,SEEK_CUR);
  sBuf=swapShort(evflags);
  fwrite(&sBuf, 2, 1, fout);
  fwrite(&planet, 1, 1, fout);
  int sz=v.size();
  sBuf=swapShort(sz);
  fwrite(&sBuf, 2, 1, fout);
  printf("%u records",v.size());
  int PERIOD=24*60*60;
  if(evtype==EV_ASCAPHETICS) PERIOD=2*60*60;
//  if(evtype==EV_ASTRORISE) PERIOD=6*60*60;
//  v[0]->dump();
//  v[1]->dump();
  for(int i=0; i<v.size(); i++){
    Event *ev=v[i];
    if((evflags & EF_CUMUL_DATE_W)&& (i>0)){
      int delta=(ev->date[0]-cumul-PERIOD)/60;
      if(abs(delta)>32767){
        printf("\nError overflow %d at:",delta);
        ev->dump();
        return false;
      }
      short d=delta;
      cumul=ev->date[0];
      sBuf=swapShort(d);
      fwrite(&sBuf, 1, 2, fout);
    }
    else if((evflags & EF_CUMUL_DATE_B)&& (i>0)){
      int delta=(ev->date[0]-cumul-PERIOD)/60;
      if(abs(delta)>127){
        printf("\nError overflow %d at:",delta);
        ev->dump();
        return false;
      }
      char d=delta;
      cumul=ev->date[0];
      fwrite(&d, 1, 1, fout);
    }
    else{
      iBuf=swapInt(ev->date[0]);
      fwrite(&iBuf, 1, 4, fout);
    }
    if(evflags & EF_DATE){
      iBuf=swapInt(ev->date[1]);
      fwrite(&iBuf, 4, 1, fout);
    }
    if(evflags & EF_PLANET1)
      fwrite(&ev->planetId[0], 1, 1, fout);
    if(evflags & EF_PLANET2)
      fwrite(&ev->planetId[1], 1, 1, fout);
    if(evflags & EF_DEGREE)
      if(evflags & EF_SHORT_DEGREE)
        fwrite(&ev->degree, 1, 1, fout);
      else{
        sBuf=swapShort(ev->degree);
        fwrite(&sBuf, 2, 1, fout);
      }
  }
  long fsize=ftell(fout);
  fseek(fout,start,SEEK_SET);
  sBuf=swapShort(fsize);
  fwrite(&sBuf, 2, 1, fout);
  fclose(fout);
  printf(" saved.");
  return true;
}

short DataFile::swapShort(short var)
{
  var=(var & 0xff)<<8 | ((var >> 8) & 0xff);
  return var;
}

int DataFile::swapInt(int var)
{
  int res=0;
  for(int i=0; i<4; i++){
    res<<=8;
    res|=(var & 0xff);
    var>>=8;
  }
  return res;
}

bool DataFile::readSubData(const char* fname, VAE & v)
{
  char buf[200];
  sprintf(buf,"archive/%d/%s",Event::startYear,fname);
  printf("\nReading %s: ",buf);
  FILE *fin=fopen(buf,"rb");

  if(!fin)
    return false;
  fseek(fin,0,SEEK_END);
  long realsz=ftell(fin);
  fseek(fin,0,SEEK_SET);
  EventType evtype;
  fread(&evtype, 1, 1, fin);
  long fsize=0;
  int evflags=0, recCount=0;
  char planet; int date;
  fread(&fsize, 2, 1, fin);
  fsize=(unsigned short)swapShort(fsize);
  int PERIOD=24*60;
  if(evtype==EV_ASCAPHETICS) PERIOD=2*60;
//  if(evtype==EV_ASTRORISE) PERIOD=6*60;

  if(fsize!=realsz) goto err;
  fread(&evflags, 2, 1, fin);
  evflags=swapShort(evflags);
  fread(&planet, 1, 1, fin);
  fread(&recCount, 2, 1, fin);
  recCount=swapShort(recCount);
  printf("%u records...",recCount);
  int cumul;
  for(int i=0; i<recCount; i++){
    if(ferror(fin)) goto err;
    if(evflags & EF_CUMUL_DATE_B){
      if(i){
        char d;
        fread(&d, 1, 1, fin);
        cumul=d;
        date+=(cumul+PERIOD)*60;
      }
      else{
        fread(&date, 1, 4, fin);
        date=swapInt(date);
      }
    }
    else if(evflags & EF_CUMUL_DATE_W){
      if(i){
        short d;
        fread(&d, 1, 2, fin);
        d=swapShort(d);
        cumul=d;
        date+=(cumul+PERIOD)*60;
      }
      else{
        fread(&date, 1, 4, fin);
        date=swapInt(date);
      }
    }
    else{
      fread(&date, 1, 4, fin);
      date=swapInt(date);
    }

    Event *ev=new Event(Event::calcJD(date),planet);
    int iBuf; short sBuf;
    if(evflags & EF_DATE){
      fread(&iBuf, 1, 4, fin);
      ev->date[1]=swapInt(iBuf);
    }
    if(evflags & EF_PLANET1)
      fread(&ev->planetId[0], 1, 1, fin);
    if(evflags & EF_PLANET2)
      fread(&ev->planetId[1], 1, 1, fin);
    if(evflags & EF_DEGREE)
      if(evflags & EF_SHORT_DEGREE)
        fread(&ev->degree, 1, 1, fin);
      else{
        fread(&sBuf, 2, 1, fin);
        ev->degree=swapShort(sBuf);
      }
    if(evflags & EF_NEXT_DATE2)
      if(v.size()>0)
        v[v.size()-1]->date[1]=ev->date[0];
    v.push_back(ev);
  }
  v[v.size()-1]->date[1]=Event::packDate(startJD+dayCount);
  fclose(fin);
  return true;
err:
  fclose(fin);
  printf("Error!");
  return false;
}

int DataFile::select(VAE & src, double jdstart, double jdend, char planet, bool both, VAE & dest)
{
  for(int i=0; i<src.size(); i++){
    Event *ev=src[i];
    double evd=ev->julianDay;
    bool save=false;
    if((evd>=jdstart)&&(evd<jdend))
      if(planet!=-1){
        if(both){
          if((ev->planetId[0]==planet)||(ev->planetId[1]==planet))
            save=true;
        }
        else
          if(ev->planetId[0]==planet)
            save=true;
      }
      else
        save=true;
    if(save)
      dest.push_back(ev);
  }
  return dest.size();
}

void DataFile::clearAphetics(aphRecord *arr, int planet, int mins, VAE &dest)
{
  VAE work;
  int value=arr[0].data[planet];
  int idx=0;
  int i=1;
  for(; i<stepCount; i++){
    if(value!=arr[i].data[planet]){
      Event* ev=new Event(idx*MINUTE_STEP+startJD,planet);
      ev->date[1]=ev->packDate(i*MINUTE_STEP+startJD);
      ev->degree=value;
      work.push_back(ev);
      value=arr[i].data[planet];
      idx=i;
    }
  }
  if(idx!=i){
    Event* ev=new Event(idx*MINUTE_STEP+startJD,planet);
    ev->date[1]=ev->packDate(i*MINUTE_STEP+startJD);
    ev->degree=value;
    work.push_back(ev);
  }
  if(mins){
    for(VAE::iterator i=work.begin(); i!=work.end(); i++){
      Event *ev=*i;
      if(ev->date[1]-ev->date[0]<mins*60){
        delete ev;
        work.erase(i);
      }
    }
  }
  int sign=-1;
  for(int i=0; i<work.size(); i++){
    Event *ev=work[i];
//    int new_sign=ev->degree;
//    if(sign!=new_sign){
//      sign=new_sign;
      dest.push_back(ev);
//    }
  }
  // ball calculation
  for(int i=0; i<dest.size(); i++){
    Event *ev=dest[i];
    int peregr=0;
    int ball= 64 + 4-2-2; // DIRECT+SLOW+RAISEMOON + 50
    if(!(ev->degree & AF_PEREGRINE)){
      ball-=5;
      peregr=1;
    }
    for(int j=0; j<sizeof(ApheBalls)/sizeof(int); j++){
      if(ev->degree & (1<<j)){
        ball+=ApheBalls[j];
      }
    }
    ev->planetId[1]=ball+(peregr<<7);
  }
}

void DataFile::doAphetics(VAE &work)
{
  aphRecord *balls;
  char fname[200];

  FILE *terma=fopen("terma.bin","rb");
  short count=0;
  fread(&count,1,2,terma);
  short buf[2];
  int ii=0;
  for(int i=0; i<count; i++){
    fread(buf,2,2,terma);
    for(int j=0; j<buf[0]; j++){
      terms[ii++]=buf[1];
    }
  }
  fclose(terma);

  balls=new aphRecord[stepCount]; // balls for all steps
  memset(balls,0,stepCount*sizeof(aphRecord));

  // general
  for(int i=0; i<7; i++){
    if(i==SE_MOON){
      sprintf(fname,"degall01.bin");
    }
    else{
      sprintf(fname,"degpass%02d.bin",i);
    }
    readSubData(fname,work);
    for(int j=0; j<work.size(); j++){
//      work[j]->dump2();
      calcAphetics(balls, work[j]);
    }
    release(work);
  }
  

// ____________________
//      sun heart
  
  for(int i=0; i<stepCount; i++){
    double sun=ephData[i].data[SE_SUN];
    for(int j=SE_MOON; j<=SE_SATURN; j++){
      double aspa=(ephData[i].data[j]-sun);
      if(aspa<0) aspa+=360;
      if(aspa>180) aspa=360-aspa;
      if(aspa<17/60.){
        balls[i].data[j]|=(1<<AF_SUNHEART);
      }
      else
        if(aspa<BURNT_ORBIS[j]){
          balls[i].data[j]|=(1<<AF_BURNT);
        }

    }
  }
  
// ____________________
//     retrograde
  for(int i=SE_MERCURY; i<=SE_SATURN; i++){
    sprintf(fname,"retro%02d.bin",i);
    readSubData(fname,work);
    for(int j=0; j<work.size(); j++){
      addBalls(balls,work[j],(1<<AF_RETRO));
    }
    release(work);
  }

  // faster
  for(int i=SE_SUN; i<=SE_SATURN; i++){
    sprintf(fname,"fast%02d.bin",i);
    readSubData(fname,work);
    for(int j=0; j<work.size(); j++){
      addBalls(balls,work[j],(1<<AF_FAST));
    }
    release(work);
  }
  // growing moon
  sprintf(fname,"phase01.bin");
  readSubData(fname,work);
  for(int j=0; j<work.size(); j++){
    if(work[j]->planetId[1]<=1){
      addBalls(balls,work[j],(1<<AF_GROWINGMOON));
    }
  }
  release(work);
  
  for(int i=0; i<stepCount; i++){
    for(int j=SE_SUN; j<=SE_SATURN; j++){
      int dj=(int)ephData[i].data[j];
      int sj=dj/30;
      // sign reception
      int k=OWN_SIGN_REVERSE[sj];
      if(k>j){
        int dk=(int)ephData[i].data[k];
        int sk=dk/30;
        if((sk==OWN_SIGN[0][j])||(sk==OWN_SIGN[1][j])){
          balls[i].data[j]|=(1<<AF_RECSIGN);
          balls[i].data[k]|=(1<<AF_RECSIGN);
        }
      }
      // exalt. reception
      k=EXALTATION_REVERSE[sj];
      if((k!=100) && (k>j)){
        int dk=(int)ephData[i].data[k];
        int sk=dk/30;
        if(sk==EXALTATION[j]){
          balls[i].data[j]|=(1<<AF_RECEXALT);
          balls[i].data[k]|=(1<<AF_RECEXALT);
        }
      }
    }
  }
  
/*
  for(int i=0; i<stepCount; i++){
    printf("\n %7d", i);
    for(int j=SE_SUN; j<=SE_SATURN; j++){
      printf(" %08u",balls[i].data[j]);
    }
  }
*/
  for(int i=SE_SUN; i<=SE_SATURN; i++){
    clearAphetics(balls, i, 0, work);
    sprintf(fname,"aphetics%02d.bin", i);
    writeSubData(work,EV_APHETICS,EF_PLANET2|EF_NEXT_DATE2,i,fname);
    if(i==SE_MERCURY){
      for(int i=0; i<10; i++){
        work[i]->dump2();
      }
    }
    release(work);
  }
  delete[] balls;
}

void DataFile::choice(EventType et, VAE & work, VAE & assist, VAE & vout, VAE & work2,
  char* prefix)
{
  char extra_plt[]={SE_TRUE_NODE,SE_MEAN_APOG,17};
  char fname[200];
  double endJD;
  double geopos[3]={Lon,Lat,0},tret[2]; char serr[255];
  release(work); release(assist); release(vout); release(work2);
  Event *ev;
  VAE allDegPass;
  printf("\n");
  switch(et){
    case EV_APHETICS:
      printf("Aphetics...");
      doAphetics(work);
      break;
    case EV_ASCAPHETICS:
      printf("Ascendent aphetics...");
      doAscAphetics(work);
      sprintf(fname, "%sascaph.bin", prefix);
      writeSubData(work,EV_ASCAPHETICS,EF_NEXT_DATE2|EF_PLANET1|
        EF_PLANET2|EF_DEGREE,-1,fname);
      release(work);
      readSubData(fname,work);
      for(int i=0; i<30; i++){
        work[i]->dump2();
      }
      break;
    case EV_VIA_COMBUSTA:
      printf("Via Combusta...");
      calcDegPass(allDegPass,SE_MOON);
      clearViaCombusta(allDegPass,work);
      sprintf(fname,"via01.bin");
      writeSubData(work,EV_VIA_COMBUSTA,EF_DATE,SE_MOON,fname);
      release(work); release(allDegPass);
      break;
    case EV_ASP_EXACT:
      printf("AspExact...");
      calcAspExact(work2,work);
      printf("calcAspExact =  %d events\n",work.size());
      sprintf(fname,"aspects.bin");
      sortVAE(work);
      printf(fname);
      writeSubData(work,EV_ASP_EXACT,EF_CUMUL_DATE_W|EF_PLANET1|EF_PLANET2|EF_DEGREE|EF_SHORT_DEGREE,-1,fname);
      release(work);
      sprintf(fname,"aspects01.bin");
      writeSubData(work2,EV_ASP_EXACT,EF_CUMUL_DATE_W|EF_PLANET2|EF_DEGREE|EF_SHORT_DEGREE,SE_MOON,fname);
      release(work2);
      break;
    case EV_SIGN_ENTER:
      printf("SignEnter...");
      for(int i=0; i<PLANET_COUNT; i++){
        calcDegPass(allDegPass,i);
        clearSignEnter(allDegPass,work);
        sprintf(fname,"signenter%02u.bin",i);
        writeSubData(work,EV_SIGN_ENTER,EF_DEGREE|EF_SHORT_DEGREE|EF_NEXT_DATE2,i,fname);
        work.clear(); release(allDegPass);
      }
      break;
    case EV_VOC:
      printf("VOC...");
      readSubData("signenter01.bin",work);
      readSubData("aspects01.bin",assist);
      double st;
      st=startJD;
      for(int i=0; i<work.size(); i++){
        int sz;
        sz=select(assist,st,work[i]->julianDay,SE_MOON,true,vout);
        Event *last;
        if(sz) last=vout[sz-1];
        double evstart;
        evstart=sz? last->julianDay: st;
        char lastPlt,lastAsp;
        lastPlt=SE_MOON;
        if(sz)
          if(last->planetId[0]==lastPlt)
            lastPlt=last->planetId[1];
          else
            lastPlt=last->planetId[0];
        lastAsp=0;
        if(sz)
          lastAsp=last->degree;
        ev=new Event(evstart,SE_MOON);
        ev->date[1]=ev->packDate(work[i]->julianDay);
        ev->degree=lastAsp; ev->planetId[1]=lastPlt;
        if(ev->date[0]!=ev->date[1])
          work2.push_back(ev);
        st=work[i]->julianDay;
        vout.clear();
      }
      sprintf(fname,"voc01.bin");
      writeSubData(work2,EV_VOC,EF_DATE|EF_DEGREE|EF_SHORT_DEGREE|EF_PLANET2,SE_MOON,fname);

      break;
    case EV_DEGREE_PASS:
      printf("DegPass...");
      for(int i=0; i<13; i++){
        calcDegPass(allDegPass,i);
        clearDegPass(allDegPass,work,i,work2);
        sprintf(fname,"degpass%02u.bin",i);
        if(i==SE_MOON){
          writeSubData(work,EV_DEGREE_PASS,EF_CUMUL_DATE_W|EF_DATE|EF_DEGREE,i,fname);
          sprintf(fname,"degall01.bin");
          writeSubData(work2,EV_DEGREE_PASS,EF_CUMUL_DATE_W|EF_DATE|EF_DEGREE,i,fname);
        }
        else{
          if(!writeSubData(work,EV_DEGREE_PASS,EF_DEGREE|EF_NEXT_DATE2,i,fname)){
						writeSubData(work,EV_DEGREE_PASS,EF_DEGREE|EF_NEXT_DATE2,i,fname);
					}
        }
        work.clear(); work2.clear(); release(allDegPass);
      }
      break;
    case EV_RISE:
      printf("RiseSets & moon days:  ");
      sprintf(fname,"new01.bin");
      double novol, ddd[6]; novol=startJD-31; int ii; ii=0;
      if(!readSubData(fname, vout)){
        while(novol<startJD+dayCount){
          swe_calc_ut(novol, SE_SUN, EFLAG, ddd, serr);
          double sun_ang=ddd[0];
          swe_calc_ut(novol, SE_MOON, EFLAG, ddd, serr);
          double aspa=fabs(sun_ang-ddd[0]);
          if(aspa>195) aspa-=180;
          if(aspa<0.01){
            Event *ev=new Event(novol,SE_MOON);
            ev->planetId[1]=255;
            vout.push_back(ev);
            novol+=27;
          }
          novol+=MINUTE_STEP;
          ++ii;
          if(ii%10000==0)
            printf("\b\b%02d",ii/10000); fflush(stdout);

        }
        writeSubData(vout,EV_STATUS,0,SE_MOON,fname);
      }
//      for(int i=0; i<vout.size(); i++)
//        vout[i]->dump();
//      return;
      for(int j=SE_SUN; j<=SE_MOON; j++){
        sprintf(fname,"%srise%02u.bin",prefix,j);
        if(j==SE_MOON){
          endJD=startJD-31;
          while(endJD<startJD+dayCount+31){
            swe_rise_trans(endJD,SE_MOON,NULL,EFLAG,SE_CALC_RISE|SE_BIT_DISC_CENTER,geopos,0,20,&tret[0],serr);
            Event *ev=new Event(tret[0],j);
            work.push_back(ev);
            endJD=tret[0]+0.01;
          }
          endJD=startJD-31;
          while(endJD<startJD+dayCount+31){
            swe_rise_trans(endJD,SE_MOON,NULL,EFLAG,SE_CALC_SET|SE_BIT_DISC_CENTER,geopos,0,20,&tret[1],serr);
            Event *ev=new Event(tret[1],j);
            work2.push_back(ev);
            endJD=tret[1]+0.01;
          }
          VAE output;
          for(int k=0; k<vout.size(); k++){
            double jstart=vout[k]->julianDay;
            double jend=(k==vout.size()-1)? startJD+dayCount:vout[k+1]->julianDay;
            select(work,jstart,jend,-1,false,assist);
            assist.insert(assist.begin(),vout[k]);
            int md=1;
            for(int i=0; i<assist.size(); i++){
              assist[i]->degree=md;
              md++;
              output.push_back(assist[i]);
            }
            assist.clear();
          }
          select(output,startJD,startJD+dayCount,-1,false,assist);
          if(!writeSubData(assist,EV_RISE,EF_CUMUL_DATE_W|EF_NEXT_DATE2|EF_DEGREE|EF_SHORT_DEGREE,SE_MOON,fname));
            writeSubData(assist,EV_RISE,EF_NEXT_DATE2|EF_DEGREE|EF_SHORT_DEGREE,SE_MOON,fname);
          assist.clear();
          release(vout);
        }
        else{
          endJD=startJD-31;
          while(endJD<startJD+dayCount+31){
            swe_rise_trans(endJD,j,NULL,EFLAG,SE_CALC_RISE|SE_BIT_DISC_CENTER,geopos,0,20,&tret[0],serr);
            Event *ev=new Event(tret[0],j);
            work.push_back(ev);
            endJD=tret[0]+0.2;
            swe_rise_trans(endJD,j,NULL,EFLAG,SE_CALC_SET|SE_BIT_DISC_CENTER,geopos,0,20,&tret[1],serr);
            ev=new Event(tret[1],j);
            work2.push_back(ev);
            endJD=tret[1]+0.2;
          }
          select(work,startJD,startJD+dayCount,-1,false,assist);
          if(!writeSubData(assist,EV_RISE,EF_CUMUL_DATE_B|EF_NEXT_DATE2,j,fname))
            if(!writeSubData(assist,EV_RISE,EF_CUMUL_DATE_W|EF_NEXT_DATE2,j,fname))
		if(writeSubData(assist,EV_RISE,EF_NEXT_DATE2,j,fname)){
		    printf("Fatal error overflow\n");
		    exit(5);
		}
          assist.clear();
        }
        sprintf(fname,"%sset%02u.bin",prefix,j);
        assist.clear();
        select(work2,startJD,startJD+dayCount,-1,false,assist);
        if(!writeSubData(assist,EV_SET,EF_CUMUL_DATE_B,j,fname))
          if(!writeSubData(assist,EV_SET,EF_CUMUL_DATE_W,j,fname))
	      if(!writeSubData(assist,EV_SET,0,j,fname)){
		printf("Fatal error overflow\n");
		exit(5);
	      }
        assist.clear();
        release(work);
        release(work2);
      }
      break;
    case EV_ASTRORISE:
      printf("Astro risesets...");
      for(int i=SE_SUN; i<=SE_SATURN; i++){
        endJD=startJD;
        int phase=-1; double min=360;
        for(int j=0; j<stepCount; j++){
          double pos=ephData[j].data[i];
          if(phase>=0){
            double aspa=(pos-ascData[j].data[phase]);
            if(aspa<0) aspa+=360;
            if(aspa>180) aspa=360-aspa;
            if(aspa<3)
              if(aspa>min){
                Event *ev=new Event(endJD-MINUTE_STEP,i);
                if(phase)
                  work2.push_back(ev);
                else
                  work.push_back(ev);
//                ev->dump();
                phase=1-phase;
                min=360;
              }
              else
                min=aspa;
          }
          else
            for(int cnt=0; cnt<2; cnt++){
              double aspa=(pos-ascData[j].data[cnt]);
              if(aspa<0) aspa+=360;
              if(aspa>180) aspa=360-aspa;
              if(aspa<3)
                if(aspa>min){
                  Event *ev=new Event(endJD-MINUTE_STEP,i);
                  phase=cnt;
                  if(phase)
                    work2.push_back(ev);
                  else
                    work.push_back(ev);
//                  ev->dump();
                  phase=1-phase;
                  min=360;
                  break;
                }
                else
                  min=aspa;
            }
          endJD+=MINUTE_STEP;
        }
        sprintf(fname,"%sarise%02u.bin",prefix,i);
        if(!writeSubData(work,EV_ASTRORISE,EF_CUMUL_DATE_B|EF_NEXT_DATE2,i,fname))
          writeSubData(work,EV_ASTRORISE,EF_CUMUL_DATE_W|EF_NEXT_DATE2,i,fname);
        sprintf(fname,"%saset%02u.bin",prefix,i);
        if(!writeSubData(work2,EV_ASTROSET,EF_CUMUL_DATE_B,i,fname))
          writeSubData(work2,EV_ASTROSET,EF_CUMUL_DATE_W,i,fname);
        release(work);
        release(work2);
      }
/*      readSubData("~arise01.bin",work);
      for(int i=0; i<50; i++)
        work[i]->dump();
      printf("\n  ET=%ld\n",GetTickCount()-tm);
      scanf("%s",fname);
*/      break;
    case EV_RETROGRADE:
      printf("Retrograde and avg speed:  ");
      bool isRetro, isFaster;
      double data[6];
      for(int body=SE_SUN; body<=SE_PLUTO; body++){
        isRetro=isFaster=false; endJD=startJD; ev=NULL;
        Event *evFast=NULL;
        for(int i=0; i<stepCount; i++){
          swe_calc_ut(endJD, body, EFLAG, data, serr);
          if(body>=SE_MERCURY){
            isRetro=data[3]<0;
            if(isRetro){  // retrograde
              if(!ev)   // was direct
                ev=new Event(endJD,body);
            }
            else  // direct
              if(ev){  //was retrograde
                ev->date[1]=ev->packDate(endJD);
                work.push_back(ev);
                ev=NULL;
              }
          }
          isFaster=fabs(data[3])>AVG_SPEED[body];
          if(isFaster){  // retrograde
            if(!evFast)   // was direct
              evFast=new Event(endJD,body);
          }
          else  // direct
            if(evFast){  //was retrograde
              evFast->date[1]=evFast->packDate(endJD);
              work2.push_back(evFast);
              evFast=NULL;
            }
          endJD+=MINUTE_STEP;
          if(i%10000==0)
            printf("\b\b%02d",i/10000); fflush(stdout);
        }
        if(ev){  //was retrograde
          ev->date[1]=ev->packDate(endJD);
          work.push_back(ev);
        }
        if(evFast){  //was faster
          evFast->date[1]=evFast->packDate(endJD);
          work2.push_back(evFast);
        }
        if(work.size()){
          sprintf(fname,"retro%02u.bin",body);
          writeSubData(work,EV_RETROGRADE,EF_DATE,body,fname);
        }
        sprintf(fname,"fast%02u.bin",body);
        writeSubData(work2,EV_FAST,EF_DATE,body,fname);
        release(work);
        release(work2);
      }
      break;
    case EV_ECLIPSE:
      for(int i=SE_SUN; i<=SE_MOON; i++){
        endJD=startJD; double tret[10];
        do{
          if(i==SE_SUN)
            swe_sol_eclipse_when_glob(endJD, EFLAG, 0, tret, false, serr);
          else
            swe_lun_eclipse_when(endJD, EFLAG, 0, tret, false, serr);
          endJD=tret[0];
          work.push_back(new Event(endJD,i));
        }while(endJD<startJD+dayCount);
        delete work.back();
        work.pop_back();
        sprintf(fname,"eclipse%02u.bin",i);
        writeSubData(work,EV_ECLIPSE,EF_DEGREE|EF_SHORT_DEGREE,i,fname);
        release(work);
      }
      break;
    case EV_TITHI:
      printf("Tithi:  ");
      int tith; tith=-1;
      st=startJD;
      for(int i=0; i<stepCount; i++){
        double delta=ephData[i].data[SE_MOON]-ephData[i].data[SE_SUN];
        NormAngle(delta);
        int new_tith=(int)(delta/12)+1;
        if(tith!=new_tith){
          ev=new Event(st,SE_MOON);
          tith=new_tith;
          ev->degree=tith;
          work.push_back(ev);
        }
        st+=MINUTE_STEP;
        if(i%10000==0)
          printf("\b\b%02d",i/10000); fflush(stdout);
      }
      sprintf(fname,"tithi.bin");
      writeSubData(work,EV_TITHI,EF_CUMUL_DATE_W|EF_NEXT_DATE2|EF_DEGREE|EF_SHORT_DEGREE,SE_MOON,fname);
      break;
    case EV_NAKSHATRA:
      printf("Nakshatra:  ");
      tith=-1;
      st=startJD;
      for(int i=0; i<stepCount; i++){
        int new_tith=(int)(ephData[i].data[SE_MOON]*28./360.)+1;
        if(tith!=new_tith){
          ev=new Event(st,SE_MOON);
          tith=new_tith;
          ev->degree=tith;
          work.push_back(ev);
        }
        st+=MINUTE_STEP;
        if(i%10000==0)
          printf("\b\b%02d",i/10000); fflush(stdout);
      }
      sprintf(fname,"nakshatra.bin");
      writeSubData(work,EV_NAKSHATRA,EF_CUMUL_DATE_W|EF_NEXT_DATE2|EF_DEGREE|EF_SHORT_DEGREE,SE_MOON,fname);
      break;
    case EV_DECL_EXACT:
      printf("Declination:  ");
      st=startJD; double decl; bool flag;
      flag=false; ev=NULL;
      for(int i=0; i<stepCount; i++){
        swe_calc_ut(st, SE_SUN, EFLAG|SEFLG_EQUATORIAL, data, serr);
        decl=data[1];
        swe_calc_ut(st, SE_MOON, EFLAG|SEFLG_EQUATORIAL, data, serr);

        if(fabs(decl-data[1])<0.0004){ // exact
          if(!ev)   // no event
            ev=new Event(st,-1);
        }
        else  // not exact
          if(ev){   // event exists
            ev->date[1]=ev->packDate(st);
            work.push_back(ev);
            ev=NULL;
          }
        st+=MINUTE_STEP;
        if(i%10000==0)
          printf("\b\b%02d",i/10000); fflush(stdout);
      }
      sprintf(fname,"decl.bin");
      writeSubData(work,EV_DECL_EXACT,EF_DATE,-1,fname);
      break;
    case EV_NAVROZ:
      printf("Navroz..."); fflush(stdout);
      st=startJD-dayCount; int deg;
      deg=-1; ev=NULL;
      do{
        swe_calc_ut(st, SE_SUN, EFLAG, data, serr);
        if(deg!=int(data[0])){
          deg=(int)data[0];
          if(!deg) break;
        }
        st+=MINUTE_STEP;
      }while(true);
      swe_rise_trans(st,SE_SUN,NULL,EFLAG,SE_CALC_RISE,geopos,0,20,&tret[0],serr);
      ev=new Event(tret[0],SE_SUN);
      work.push_back(ev);
      readSubData("signenter00.bin",assist);
			assert(assist.size()>0);
      for(int i=0; i<assist.size(); i++)
        if(assist[i]->degree==0){
          swe_rise_trans(assist[i]->julianDay,SE_SUN,NULL,EFLAG,SE_CALC_RISE,geopos,0,20,&tret[0],serr);
          ev=new Event(tret[0],SE_SUN);
          work.push_back(ev);
          break;
        }

      sprintf(fname,"%snavroz.bin",prefix);
      writeSubData(work,EV_NAVROZ,0,SE_SUN,fname);
      release(assist);
      break;
    case EV_MOON_PHASE:
      printf("Moon phases...");
      readSubData("aspects01.bin",work);
      int idx;
      for(int i=0; i<work.size(); i++){
        if(work[i]->planetId[1]!=SE_SUN)
          continue;
        int dgr=work[i]->degree;
        idx=getAspIndex(dgr);
        if((idx>=0)&&(idx<=2)){
          work[i]->planetId[1]=dgr;
          assist.push_back(work[i]);
        }
      }
      for(int i=0; i<assist.size(); i++)
        if(assist[i]->planetId[1]==0){
          idx=i;
          break;
        }
      idx=4-(idx%4);
      for(int i=0; i<assist.size(); i++){
        assist[i]->planetId[1]=idx%4;
        idx++;
      }
      sprintf(fname,"phase01.bin");
      writeSubData(assist,EV_MOON_PHASE,EF_NEXT_DATE2|EF_PLANET2,SE_MOON,fname);
      assist.clear();
      release(work);
      break;
  }

}

int DataFile::getAspIndex(int angle)
{
  int idx=-1;
  for(int i=0; i<sizeof(ASP_ANGLES)/sizeof(char); i++)
    if(ASP_ANGLES[i]==angle){
      idx=i;
      break;
    }
  return idx;
}

void DataFile::NormAngle(double &a)
{
  while(a<0) a+=360.L;
  while(a>=360) a-=360.L;
}


int DataFile::calcAphetics(aphRecord *balls, const Event *ev)
{
  int aph=0;
  int degree=ev->degree & 0x3fff;
  int sign=degree/30;
  int planet=ev->planetId[0];
  // domicile
  if(sign==OWN_SIGN[0][planet]){
    aph|=(1<<AF_DOMICILE);
  }
  if(sign==OWN_SIGN[1][planet]){
    aph|=(1<<AF_DOMICILE);
  }

  // exaltation
  if(sign==EXALTATION[planet]){
    aph|=(1<<AF_EXALT);
  }

  // fall
  if(sign==(EXALTATION[planet]+6)%12){
    aph|=(1<<AF_FALL);
  }

  // detriment
  if(sign==(OWN_SIGN[0][planet]+6)%12){
    aph|=(1<<AF_DETRIMENT);
  }
  if(planet>SE_MOON){
    if(sign==(OWN_SIGN[1][planet]+6)%12){
      aph|=(1<<AF_DETRIMENT);
    }
  }


  // triplicity
  for(int i=0; i<3; i++){
    if(sign==TRIPLICITY[i][planet]){
      aph|=(1<<AF_TRIPL);
      break;
    }
  }

  // terms
  if(planet==terms[degree]){
    aph|=(1<<AF_TERM);
  }

  // decanes
  if(planet==DECANES[(degree/10)%7]){
    aph|=(1<<AF_DECANE);
  }
  addBalls(balls,ev,aph);
}

void DataFile::addBalls(aphRecord *balls, const Event *ev, int value)
{
  int ind0=(int)((ev->julianDay-startJD)/MINUTE_STEP);
  int ind1=(int)((ev->calcJD(ev->date[1])-startJD)/MINUTE_STEP+0.5);
  int planet=ev->planetId[0];
  if(value){
    for(int i=ind0; i<ind1; i++){
      balls[i].data[planet]|=value;
    }
  }
}

bool DataFile::loadAphetics(sAphRecord *data)
{
  static const char PLANET_BALLS[SE_SATURN+1][12]={
    {10, 6,11, 7, 3, 5, 1, 9, 2, 8, 0, 4},
    { 9, 7, 0, 8, 4, 2,10, 6, 5,11, 1, 3},
    {11, 8, 3, 7, 1, 9, 4, 0, 6,10, 2, 5},
    { 0, 7, 5, 2,10, 8, 4, 3, 9,11, 6, 1},
    { 1, 6, 3,11, 5, 2,10, 8, 4, 9, 7, 0},
    { 5, 2, 9, 7, 1,10, 6, 4, 0, 3,11, 8},
    { 4, 3, 0, 2, 8,11, 7, 5, 1, 6,10, 9},
  };
  static const char DECANES[3][12]={
    {4,2,5,3,6,0,1,4,2,5,3,6},
    {0,1,4,2,5,3,6,0,1,4,2,5},
    {3,6,0,1,4,2,5,3,6,0,1,4},
  };
  FILE *aphet=fopen("aphetics.bin","rb");
  if(aphet){
    fread(data,SE_SATURN+1,360,aphet);
  }
  else{
    printf("\nCalculating aphetics...");
    for(int i=0; i<=SE_SATURN; i++){
      for(int j=0; j<12; j++){
        int sign=PLANET_BALLS[i][j]*30;
        int ball=j-6;
        if(ball>=0) ball++;
        for(int k=0; k<30; k++){
          data[i].data[sign+k]=ball;
        }
      }
    }
    FILE *terma=fopen("terma.bin","rb");
    short count=0;
    fread(&count,1,2,terma);
    short buf[2];
    int ii=0;
    for(int i=0; i<count; i++){
      fread(buf,2,2,terma);
      for(int j=0; j<buf[0]; j++){
        data[buf[1]].data[ii++]+=2;
      }
    }
    fclose(terma);
    for(int i=0; i<360; i++){
      int j=i/30;
      data[DECANES[(i-(j*30))/10][j]].data[i]++;
    }
    aphet=fopen("aphetics.bin","wb");
    fwrite(data,SE_SATURN+1,360,aphet);
  }
  fclose(aphet);
  return true;
}

void DataFile::calcAscData()
{
  double cusps[13], ascmc[10];
  sAscRecord *myascData=new sAscRecord[stepCount];
  double endJD=startJD;
  printf("\n AscData for %.2f, %.2f:    ", Lat, Lon);
  for(int i=0; i<stepCount; i++){
    swe_houses(endJD, Lat, Lon, 'P', cusps, ascmc);
    myascData[i].data[0]=cusps[1];  //asc
    myascData[i].data[1]=cusps[7];  //dsc
    endJD+=MINUTE_STEP;
    if(i%10000==0)
      printf("\b\b%02d",i/10000); fflush(stdout); 
  }
  ascData=myascData;
}

void DataFile::doAscAphetics(VAE &work)
{
  char fname[200];
  VAE vaes[7];
  for(int i=0; i<7; i++){
    sprintf(fname,"aphetics%02d.bin",i);
    readSubData(fname,vaes[i]);
  }
  double endJD=startJD;
  Event *evhist[2]={NULL,NULL}; Event *evcur[2]; 
  double oldtm=endJD; int oldstep=0;
  int ascsign[2], ascplt[2]; 
  for(int i=0; i<stepCount; i++){
    for(int j=0; j<2; j++){
      ascsign[j]=(int)(ascData[i].data[j]/30);
      ascplt[j]=OWN_SIGN_REVERSE[ascsign[j]];
      evcur[j]=eventContains(vaes[ascplt[j]],endJD);
    };
    if(!evcur[0] || !evcur[1]){
      break;
    }
//    if(aph_ne(evhist[0],evcur[0]) || aph_ne(evhist[1],evcur[1])){
    if((evhist[0]!=evcur[0]) || (evhist[1]!=evcur[1])){
      if(evhist[0]){
        Event *ascev=new Event(oldtm,evhist[0]->planetId[1]);
        ascev->planetId[1]=evhist[1]->planetId[1];
        int delta=max(APH_ORBIS[ascplt[0]],APH_ORBIS[ascplt[1]]);
        int aspind=aspectExists(oldstep,ascplt[0],ascplt[1],delta);
        ascev->degree=(ascsign[0] & 0xf) + ((ascsign[1]<<4)& 0xf0) + ((aspind<<8)& 0xff00);
        work.push_back(ascev);
        oldtm=endJD;
        oldstep=i;
//        printf("\n\n %d %d -- %d",ascplt[0],ascplt[1],i);
//        ascev->dump2();
      }
      evhist[0]=evcur[0];
      evhist[1]=evcur[1];
      evcur[0]=NULL;
    }
    endJD+=MINUTE_STEP;
  }
  if(evcur[0]){
    Event *ascev=new Event(oldtm,evhist[0]->planetId[1]);
    ascev->planetId[1]=evhist[1]->planetId[1];
    int delta=max(APH_ORBIS[ascplt[0]],APH_ORBIS[ascplt[1]]);
    int aspind=aspectExists(oldstep,ascplt[0],ascplt[1],delta);
    ascev->degree=(ascsign[0] & 0xf) + ((ascsign[1]<<4)& 0xf0) + ((aspind<<8)& 0xff00);
    work.push_back(ascev);
//    printf("\n\n %d %d --",ascplt[0],ascplt[1]);
//    ascev->dump2();
  }
  for(int i=0; i<7; i++){
    release(vaes[i]);
  }
  for(int i=0; i<30; i++){
    work[i]->dump2();
  }
  
}

Event* DataFile::eventContains(const VAE &work, double moment)
{
  for(int i=0; i<work.size(); i++){
    if(work[i]->julianDay>moment){
      return work[i-1];
    }
  }
  return NULL;
}

int DataFile::aspectExists(int step, int p0, int p1, double delta)
{
  int aspindex=-1;
  if(step<stepCount){
    double ang0=ephData[step].data[p0], ang1=ephData[step].data[p1];
    double aspa=(ang0-ang1);
    if(aspa<0) aspa+=360;
    if(aspa>180) aspa=360-aspa;
    for(int cnt=0; cnt<sizeof(ASP_ANGLES)/sizeof(char); cnt++){
      double d=aspa-ASP_ANGLES[cnt];
      d=d-int(d/360.);
      if(fabs(d)<delta){
        aspindex=cnt; break;
      }
    }
  }
  return aspindex;
}

bool DataFile::aph_ne(const Event* ev0, const Event* ev1)
{
  return (ev0->degree != ev1->degree) || (ev0->planetId[0] != ev1->planetId[0])
    || (ev0->planetId[1] != ev1->planetId[1]);
}

void DataFile::VOC_generate(EventType et, VAE & work, VAE & assist, VAE & vout, VAE & work2){
	readSubData("signenter01.bin",work);
	readSubData("aspects01.bin",assist);
	double st;
	st=startJD;
	for(int i=0; i<work.size(); i++){
		int sz;
		sz=select(assist,st,work[i]->julianDay,SE_MOON,true,vout);
		Event *last;
		if(sz) last=vout[sz-1];
		double evstart;
		evstart=sz? last->julianDay: st;
		char lastPlt,lastAsp;
		lastPlt=SE_MOON;
		if(sz)
			if(last->planetId[0]==lastPlt)
				lastPlt=last->planetId[1];
			else
				lastPlt=last->planetId[0];
		lastAsp=0;
		if(sz)
			lastAsp=last->degree;
		Event* ev=new Event(evstart,SE_MOON);
		ev->date[1]=ev->packDate(work[i]->julianDay);
		ev->degree=lastAsp; ev->planetId[1]=lastPlt;
		if(ev->date[0]!=ev->date[1])
			work2.push_back(ev);
		st=work[i]->julianDay;
		vout.clear();
	}
}
