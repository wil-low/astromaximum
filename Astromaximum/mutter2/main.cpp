
#include "datafile.h"

//---------------------------------------------------------------------------
#include "events.h"
#include "evclass.h"
#include "datafile.h"
#include <time.h>
#include "errno.h"
#include <fstream>
#include <unistd.h>
#include <assert.h>
using namespace std;

const int NOT_ENOUGH_PARAMS = -1,
        INVALID_YEAR = -2,
        INVALID_EVENT = -3;

char ephemPath[] = "../swiss"; // relative to program dir
char mypath[PATH_MAX];
const char outFile[] = "output.txt";

int test();

sAphRecord aphetics[SE_SATURN + 1];
clock_t c_start, c_end;
sEphRecord *ephData = NULL;

DataFile df;

void myexit(int ret) {
    chdir(mypath);
    c_end = clock();
    //    long cps = CLOCKS_PER_SEC;
    //    if(!ret)
    //        printf("\nExecution time %d clocks (cps=%d)\n", c_end-c_start, cps);
    //    printf("\nExit code: %d. Restored curdir: %s\n", ret, mypath);
    delete[] ephData;
    exit(ret);
}

int main(int argc, char* argv[]) {
    /*
      char szInput [256];

      long dif;
      long cps=CLOCKS_PER_SEC;
      start=clock ();
      printf ("Please, enter your name: ");
      gets (szInput);
      end=clock();
      dif = end-start;
      printf ("Hi %s.\n", szInput);
      printf ("It took you %d  clocks at %d.\n", dif, cps);

      return 0;

     */
    char path[255], serr[256];

    /*
     * double test_day=swe_julday(2007, 4, 7, 0, SE_GREG_CAL);
     * double result;
     * double geopos[3]={30.51,50.43,200};
     * swe_rise_trans(test_day,SE_MOON,NULL,0,SE_CALC_SET| SE_BIT_DISC_CENTER ,geopos,0,20,&result,path);
     * int y,m,d; double hour;
     * swe_revjul(result, SE_GREG_CAL, &y, &m, &d, &hour);
     * printf("%02d.%02d.%04d %02d:%02d",d, m, y, (int)hour, (int)((hour-(int)hour)*60));
     * scanf("%s",path);
     * myexit(0);
     */
    if ((argc == 1) || (strcmp(argv[1], "--help")) == 0) {
        printf("Usage:  mutter2 <year> [options]\n");
        printf(" options:\n");
        printf("   <empty> - calculate ephemeris if none, and common.dat\n");
        printf("   asctest - ascending test for events in archive/<year>/*.bin\n");
        printf("   <prefix> electio - calculate APHETICS with prefix\n");
        printf("   <prefix> <lon> <lat> [alt] [electio]- calc locations on coords with prefix\n");
        printf("   view <file.bin> <count> - view events of datafile\n");
        printf("   dump <country> <filenum> <secnum> - dump sections of location file\n");
        printf("   sql - create year.sql for website import\n");
        printf("   jul <month> <day> <hour> - julian date from ephemeris\n");
        printf("   revjul <float> - date from ephemeris\n");
        printf("   dow <double> - day of week from ephemeris\n");
        printf("   _test_ - run test() routine\n");
        exit(0);
    }
    strcpy(mypath, argv[0]);
    char *pos = strrchr(mypath, '\\');
    if (!pos) {
        pos = strrchr(mypath, '/');
    }
    if (pos) {
        *pos = 0;
    } else {
        mypath[0] = 0;
    }

    //    getcwd(mypath, PATH_MAX);
    //    printf("App path=%s\n", mypath);
    sprintf(path, "%s/%s", mypath, "../data");
    chdir(path);
    getcwd(mypath, PATH_MAX);
    //    printf("chdir to %s\n", mypath);
    sprintf(serr, "%s/%s", path, ephemPath);
    while (pos = strchr(serr, '\\')) {
        *pos = '/';
    }
    swe_set_ephe_path(ephemPath);
    Event::EPOCH = swe_julday(1970, 1, 1, 0, SE_GREG_CAL);
    double outr[6];
    int res = swe_calc_ut(Event::EPOCH, SE_SUN, SEFLG_BARYCTR, outr, serr);
    if (res < 0) {
        printf("%s\n", serr);
        myexit(-1); // Sweph not found, exit
    }
    struct tm now;
    now.tm_year = 2006 - 1900;
    now.tm_mon = 11;
    now.tm_mday = 8;
    now.tm_hour = 18;
    now.tm_min = 3;
    now.tm_sec = 0;
    now.tm_isdst = 0;

#ifdef ANSITZ
    time_t loo = mktime(&now); //-_timezone;
    tm *st = gmtime(&loo);
    time_t loo1 = mktime(st);
    Event::_timezone_ = loo1 - loo;
#else
    time_t loo = mktime(&now) - _timezone;
    tm *st = gmtime(&loo);
    loo = mktime(st);
#endif

    assert(sizeof (sMatrix) == 9);
    assert(EV_LAST == 52);
    if (argc < 2) myexit(NOT_ENOUGH_PARAMS);
//    char buf[20];
    int year;
    if (sscanf(argv[1], "%4d", &year) != 1)
        myexit(INVALID_YEAR);
    Event::startYear = year;
    if (argc > 2) {
        if (strcmp(argv[2], "jul") == 0) {
            int mon, day;
            float hr;
            if (sscanf(argv[3], "%02d-%*02d*%*02d", &mon) != 1) {
                myexit(NOT_ENOUGH_PARAMS);
            }
            if (sscanf(argv[4], "%02d-%*02d*%*02d", &day) != 1) {
                myexit(NOT_ENOUGH_PARAMS);
            }
            if (sscanf(argv[5], "%f", &hr) != 1) {
                myexit(NOT_ENOUGH_PARAMS);
            }

            double jd = swe_julday(year, mon, day, hr, SE_GREG_CAL);
            printf("%f\n", jd);
            myexit(0);
        }
        if (strcmp(argv[2], "revjul") == 0) {
            double jd;
            if (sscanf(argv[3], "%lf", &jd) != 1) {
                myexit(NOT_ENOUGH_PARAMS);
            }
            Event ev(jd, 0);
            ev.print_date(0);
            printf("\n");
            myexit(0);
        }

        if (strcmp(argv[2], "dow") == 0) {
            double jd;
            if (sscanf(argv[3], "%lf", &jd) != 1) {
                myexit(NOT_ENOUGH_PARAMS);
            }
            int dow = swe_day_of_week(jd);
            printf("%d\n", dow);
            myexit(0);
        }
    }
    printf("Local timezone offset %ld\n", Event::_timezone_);
    //    printf("Curdir: %s\n", mypath);
    //    printf("argv[0] is: %s\n", path);
    //    printf("Chdir to %s\n", path);
    //    printf("Year = %d\t", year);
    if ((argc > 2) && (strcmp(argv[2], "asctest") == 0)) {
        int err=df.AscendingTest();
        if(!err)
            printf("\nErrors found %d\n", err);
        printf("\n%s\n", "Finished.");
        myexit(err);
    }
    if ((argc == 5) && (strcmp(argv[2], "view") == 0)) {
        int count = 0;
        sscanf(argv[4], "%d", &count);
        df.view(argv[3], count);
        printf("\nFinished\n");
        myexit(0);
    }
    if (argc == 3 && strcmp(argv[2], "sql") == 0) { // Creating SQL file
        VAE work;
        char fn[100];
        char buf0[100], buf1[100];
        sprintf(fn, "../site/%04d.sql", year);
        FILE *sql = fopen(fn, "w");
        if (!sql) {
            int ern = errno;
            printf("Cannot create file %s: %s", fn, strerror(ern));
            myexit(-1);
        }
        writeSQL(sql, "voc01.bin", EV_VOC);
        writeSQL(sql, "degpass00.bin", EV_VOC);
        fprintf(sql, "TRUNCATE TABLE `_voc`; BEGIN;\n");
        if (df.readSubData("voc01.bin", work)) {
            for (uint i = 0; i < work.size(); i++) {
                Event *ev = work[i];
                fprintf(sql, "INSERT INTO `_voc` VALUES (%s, %s);\n",
                        ev->date_sql(buf0, 0), ev->date_sql(buf1, 1));
                //                ev->dump();
            }
            fprintf(sql, "COMMIT;\n\n");
        } else {
            printf("\nVOC file error!");
        }
        df.release(work);
        /*
                fprintf(sql, "TRUNCATE TABLE `_vc`; BEGIN;\n");
                if(df.readSubData("via01.bin", work)){
                    for(int i=0; i<work.size(); i++){
                        Event *ev=work[i];
                        fprintf(sql, "INSERT INTO `_vc` VALUES (%s, %s);\n",
                                ev->date_sql(buf0, 0), ev->date_sql(buf1, 1));
        //                ev->dump();
                    }
                    fprintf(sql, "COMMIT;\n\n");
                }
                else{
                    printf("\nVC file error!");
                }
                df.release(work);
         */
        fprintf(sql, "TRUNCATE TABLE `_sundgr`; BEGIN;\n");
        if (df.readSubData("degpass00.bin", work)) {
            for (uint i = 0; i < work.size(); i++) {
                Event *ev = work[i];
                fprintf(sql, "INSERT INTO `_sundgr` VALUES (%s, %s, %d);\n",
                        ev->date_sql(buf0, 0), ev->date_sql(buf1, 1), ev->degree & 0x3fff);
                //                ev->dump();
            }
            fprintf(sql, "COMMIT;\n\n");
        } else {
            printf("\nSun degree file error!");
        }
        df.release(work);

        fclose(sql);
        printf("\nSQL created: %s\n", fn);
        myexit(0);
    }
    if ((argc == 6) && (strcmp(argv[2], "dump") == 0)) {
        int num = 0, secnum = -2;
        sscanf(argv[4], "%d", &num);
        sscanf(argv[5], "%d", &secnum);
        df.dump_location(argv[3], num, secnum);
        myexit(0);
    }
    double startJD = swe_julday(year - 1, 12, 31, 0, SE_GREG_CAL);
    printf("startJD=%f\t", startJD);
    double endJD = swe_julday(year + 1, 2, 1, 0, SE_GREG_CAL);

    int dayCount = (int) (endJD - startJD);
    unsigned int stepCount = (int) (dayCount / MINUTE_STEP);
    printf("Steps = %d\n", stepCount);
    double data[6];
    char ephf[255];
    ephData = new sEphRecord [stepCount];
    endJD = startJD;
    int size = sizeof (sEphRecord) * stepCount;
    sprintf(ephf, "ephdata/ephdata%04d.dat", year);

    FILE *fin = fopen(ephf, "rb");
    int fsz = 0;
    if (fin) {
        fseek(fin, 0, SEEK_END);
        fsz = ftell(fin);
    }
    if (fsz) {
        printf("\nValid cached ephdata found. Loading...");
        rewind(fin);
        fread(ephData, size, 1, fin);
        fclose(fin);
        printf("Done.\n");
    } else {
        printf("\nCalculating ephdata...");
        if (fin) {
            fclose(fin);
        }

        printf("\nSteps = %d\n", stepCount);
        for (uint i = 0; i < stepCount; i++) {
            for (int body = 0; body < 13; body++) {
                swe_calc_ut(endJD, PLANETS[body], SEFLG_SWIEPH, data, serr);
                ephData[i].data[body] = data[0];
            }
            endJD += MINUTE_STEP;
            if (i % 10000 == 0)
                printf("%d.", i / 10000);
            fflush(stdout);
        }
        printf("\nSaving cached ephemeris...");
        FILE *fout = fopen(ephf, "wb");
        fwrite(ephData, size, 1, fout);
        fclose(fout);
        printf("Done. Restart with the same parameters to calculate common.dat\n");
        myexit(0);
    }
    int Err=0;
    df.init(ephData, startJD, dayCount);
    if (argc == 2) { // only year specified
        df.AAA(); // calculate common.dat
        //  df.saveFile(outFile);
        Err=df.AscendingTest();
        printf("Done.\n");
    } else {
        VAE work, assist, vout, work2;
        if (strcmp(argv[2], "_test_") == 0) {
            myexit(test());
        }
        if (strcmp(argv[2], "electio") == 0) {
            //      df.loadAphetics(aphetics);
            df.choice(EV_APHETICS, work, assist, vout, work2, argv[3]);
            //      scanf("%s",buf);
        } else {
            if (strcmp(argv[2], "vocsql") == 0) {

            } else {
                if (argc < 5)
                    myexit(NOT_ENOUGH_PARAMS);
                df.Lon = strtod(argv[3], NULL);
                df.Lat = strtod(argv[4], NULL);
                int alt = 0;
                if (argc > 5 && sscanf(argv[5], "%d", &alt) == 1) {
                    df.Alt = alt;
                }
                df.choice(EV_NAVROZ, work, assist, vout, work2, argv[2]);
                df.calcAscData();

                df.choice(EV_ASTRORISE, work, assist, vout, work2, argv[2]);
                df.choice(EV_RISE, work, assist, vout, work2, argv[2]);
                if (argc > 5 && (strcmp(argv[5], "electio") == 0)) {
                    //                    df.choice(EV_ASCAPHETICS, work, assist, vout, work2, argv[2]);
                }
            }
        }
    }
    if(!Err)
        printf("\nErrors found %d\n", Err);
    printf("\nFinished.\n");
    myexit(Err);
}

int test() {
    VAE allDegPass, work, work2;
    int i=6;
    char fname[]="06test_degpass.binn";

// TODO error - strange degree in event #7
    df.calcDegPass(allDegPass, i);
    df.clearDegPass(allDegPass, work, i, work2);
    for (int i = 0; i < 11; i++) {
        work[i]->dump();
    }
    if (!df.writeSubData(work, EV_DEGREE_PASS, EF_CUMUL_DATE_W | EF_DEGREE | EF_NEXT_DATE2, i, fname)) {
        df.writeSubData(work, EV_DEGREE_PASS, EF_DEGREE | EF_NEXT_DATE2, i, fname);
    }
    df.release(work);
    printf("\n\n----View:\n");
    df.view(fname, 100);
    printf("\n\n----Alternate:\n");
    
    df.readSubData(fname, work);
    for (uint i = 0; i < work.size(); i++) {
        work[i]->dump();
        printf("\n");
    }

    df.release(work);
    /*    
    double tm = 2451655.252083;
    Event ev(tm, 0);
    ev.dump();
    printf("\nPacked date: %ld\n", ev.packDate(tm));
    double tm2 = ev.calcJD(ev.date[0]);
    printf("tm %f, tm2 %f\n", tm, tm2);
    assert(tm == tm2);
    Event ev2(tm2, 0);
    ev2.dump();
 */
    return 0;
}

// # vi:et:ts=4:sw=4
