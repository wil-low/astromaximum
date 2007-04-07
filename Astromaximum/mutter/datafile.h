//---------------------------------------------------------------------------

#ifndef datafileH
#define datafileH
#include "events.h"
#include "evclass.h"
#include <vector>
using namespace std;
//---------------------------------------------------------------------------
typedef enum{
  EF_DATE=0x1, // contains 2nd date - 4b
  EF_PLANET1=0x2, // contains 1nd planet - 1b
  EF_PLANET2=0x4, // contains 2nd planet - 1b
  EF_DEGREE=0x8, // contains degree or angle - 2b
  EF_CUMUL_DATE_B=0x10, // date are cumulative from 1st 4b - 1b
  EF_CUMUL_DATE_W=0x20, // date are cumulative from 1st 4b - 2b
  EF_SHORT_DEGREE=0x40, // contains angle 0..180 - 1b
  EF_NEXT_DATE2=0x80 // 2nd date is 1st in next event
} EventFlag;

static const char PLANETS[]={SE_SUN,SE_MOON,SE_MERCURY,SE_VENUS,SE_MARS,
  SE_JUPITER,SE_SATURN,SE_URANUS,SE_NEPTUNE,SE_PLUTO,SE_TRUE_NODE,SE_MEAN_APOG,
  SE_FICT_OFFSET_1+17
};
typedef vector<Event*> VAE;

class DataFile
{
private:
  sEphRecord *ephData;
  double startJD;
  unsigned int dayCount, stepCount;
  VAE events;
  void calcAspExact(VAE & moonvae,VAE & vae);
  void calcDegPass(VAE & vae, int planet);
  void NormAngle(double &a);

  void registerAspect(VAE & moonvae,VAE &vae, int i, int j);
  void registerDegPass(VAE &dpe, int deg, int body, int interval);
  void release(VAE & v);
  void clearDegPass(VAE & src, VAE & dest, int id);
  void clearSignEnter(VAE & src, VAE & dest);
  void clearViaCombusta(VAE & src, VAE & dest);
  bool writeSubData(const VAE & v, EventType evtype, int evflags, int planet, char* fname);
  bool readSubData(char* fname, VAE & v);
  int select(VAE & src, double jdstart, double jdend, char planet, bool both, VAE & dest);
  int getAspIndex(int angle);
  void geopos(char* city, double lat, double lon, char* suffix);
  short swapShort(short var);
  int swapInt(int var);
public:
  sAscRecord *ascData;
  void sortVAE(VAE &work);
  void AscendingTest(const char* dirname);
  DataFile();
  double Lon, Lat;
  void choice(EventType et, VAE & work, VAE & assist, VAE & vout, VAE & work2,
    char* prefix="");
  void init(sEphRecord *ephdata, double start, unsigned int count);
  void AAA();
  ~DataFile();
};
#endif
