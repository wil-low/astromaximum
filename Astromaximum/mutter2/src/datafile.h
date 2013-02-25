//---------------------------------------------------------------------------

#ifndef datafileH
#define datafileH
#include "events.h"
#include "evclass.h"
#include <vector>
using namespace std;
//---------------------------------------------------------------------------

typedef enum {
    EF_DATE = 0x1, // contains 2nd date - 4b
    EF_PLANET1 = 0x2, // contains 1nd planet - 1b
    EF_PLANET2 = 0x4, // contains 2nd planet - 1b
    EF_DEGREE = 0x8, // contains degree or angle - 2b
    EF_CUMUL_DATE_B = 0x10, // date are cumulative from 1st 4b - 1b
    EF_CUMUL_DATE_W = 0x20, // date are cumulative from 1st 4b - 2b
    EF_SHORT_DEGREE = 0x40, // contains angle 0..180 - 1b
    EF_NEXT_DATE2 = 0x80 // 2nd date is 1st in next event
} EventFlag;

typedef enum {
    AF_DOMICILE = 0,
    AF_EXALT,
    AF_TRIPL,
    AF_TERM,
    AF_DECANE,
    AF_RECSIGN,
    AF_RECEXALT,
    AF_SUNHEART,
    AF_DETRIMENT,
    AF_FALL,
    AF_RETRO,
    AF_BURNT,
    AF_FAST,
    AF_GROWINGMOON,
} ApheFlag;

static const int AF_PEREGRINE = (1 << AF_DOMICILE)+(1 << AF_EXALT)+(1 << AF_RECSIGN)+(1 << AF_RECEXALT)+(1 << AF_TRIPL);

static const int ApheBalls[] = {
    5, // AF_DOMICILE
    4, // AF_EXALT
    3, // AF_TRIPL
    2, // AF_TERM
    1, // AF_DECANE
    5, // AF_RECSIGN
    4, // AF_RECEXALT
    5, // AF_SUNHEART
    -5, // AF_DETRIMENT
    -4, // AF_FALL
    -9, // AF_RETRO (-5 against DIRECT +4)
    -5, // AF_BURNT
    4, // AF_FAST (+2 against SLOW -2)
    4, // AF_GROWINGMOON (+2 against  -2)
};
static const char PLANETS[] = {SE_SUN, SE_MOON, SE_MERCURY, SE_VENUS, SE_MARS,
    SE_JUPITER, SE_SATURN, SE_URANUS, SE_NEPTUNE, SE_PLUTO, SE_TRUE_NODE, SE_MEAN_APOG,
    SE_FICT_OFFSET_1 + 17
};
typedef vector<Event*> VAE;
typedef vector<pair<int, int> > LOC_CONTENTS;

class DataFile {
private:

    struct aphRecord {
        unsigned int data[7];
    };
    sEphRecord *ephData;
    char terms[360];
    double startJD;
    VAE events;
    int year;
    void calcAspExact(VAE & moonvae, VAE & vae);
    void NormAngle(double &a);

    void VOC_generate(EventType et, VAE & work, VAE & assist, VAE & vout, VAE & work2);
    void registerAspect(VAE & moonvae, VAE &vae, int i, int j);
    void registerDegPass(VAE &dpe, int deg, int body, int interval);
    void clearSignEnter(VAE & src, VAE & dest);
    void clearViaCombusta(VAE & src, VAE & dest);
    int select(VAE & src, double jdstart, double jdend, char planet, bool both, VAE & dest);
    int getAspIndex(int angle);
    short swapShort(short var);
    int swapInt(int var);
    int calcAphetics(aphRecord *balls, const Event *ev);
    void clearAphetics(aphRecord *arr, int planet, int mins, VAE &dest);
    void doAphetics(VAE &work);
    void doAscAphetics(VAE &work);
    void addBalls(aphRecord *balls, const Event *ev, int value);
    double getPrevious0dgr();
    void get_loc_contents(const char* fname, bool is_output, LOC_CONTENTS &v);
    void dump_section(const char* fname, pair<int, int> sec);
public:
    uint dayCount, stepCount;
    void writeSQL (FILE* fout, const char* bin_fname, EventType type);
    bool readSubData(const char* fname, VAE & v);
    bool writeSubData(const VAE & v, EventType evtype, int evflags, int planet, char* fname);
    void release(VAE & v);
    bool aph_ne(const Event* ev0, const Event* ev1);
    int aspectExists(uint step, int p0, int p1, double delta);
    Event* eventContains(const VAE &work, double moment);
    sAscRecord *ascData;
    void sortVAE(VAE &work);
    int AscendingTest();
    DataFile();
    double Lon, Lat, Alt;
    void choice(EventType et, VAE & work, VAE & assist, VAE & vout, VAE & work2,
            const char* prefix = "");
    void init(sEphRecord *ephdata, double start, unsigned int count);
    void AAA();
    void view(const char* fname, uint count);
    void dump_location(const char* fname, int num, int secnum);
    ~DataFile();
    bool loadAphetics(sAphRecord *data);
    void calcAscData();
    void calcDegPass(VAE & vae, int planet);
    void clearDegPass(VAE & src, VAE & dest, int id, VAE & destall);
};
#endif

// # vi:et:ts=4:sw=4
