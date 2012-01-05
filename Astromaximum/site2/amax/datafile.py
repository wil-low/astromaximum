import struct
from calendar import timegm
from pprint import pprint
from models import Event


class DataFile:
    EF_DATE = 0x1  # contains 2nd date - 4b
    EF_PLANET1 = 0x2  # contains 1nd planet - 1b
    EF_PLANET2 = 0x4  # contains 2nd planet - 1b
    EF_DEGREE = 0x8  # contains degree or angle - 2b
    EF_CUMUL_DATE_B = 0x10  # date are cumulative from 1st 4b - 1b
    EF_CUMUL_DATE_W = 0x20  # date are cumulative from 1st 4b - 2b
    EF_SHORT_DEGREE = 0x40  # contains angle 0..180 - 1b
    EF_NEXT_DATE2 = 0x80  # 2nd date is 1st in next event

    MSECINDAY = 86400 * 1000
    
    def __init__(self, filename, is_common):
        self.filename = filename
        self.is_common = is_common

    def read_byte(self, fd):
        val, = struct.unpack('B', fd.read(1))
        return val
    
    def read_short(self, fd):
        val, = struct.unpack('>h', fd.read(2))
        return val

    def read_int(self, fd):
        val, = struct.unpack('>i', fd.read(4))
        return val

    def read_uint(self, fd):
        val, = struct.unpack('>I', fd.read(4))
        return val

    def read_UTF(self, fd):
        str_length = self.read_short(fd)
        return fd.read(str_length)

    def read_YMD(self, fd):
        year = self.read_short(fd)
        month = self.read_byte(fd)
        day = self.read_byte(fd)
        
        self.startJD = timegm((year, month, day, 0, 0, 0))

        print 'custom_data_len', year, month, day
        self.dayCount = self.read_short(fd)
        self.finalJD = self.startJD + DataFile.MSECINDAY * self.dayCount
 
    def read_sub_data(self, event_func):
        "parse datafile header and call event_func with Event"
        
        fd = file(self.filename, 'rb')
        
        if self.is_common:
            self.read_YMD(fd)
        else:
            print fd.read(4)  # signature
            version = self.read_byte(fd)
            coords = []
            if version == 2:
                self.read_YMD(fd)
                city_id = self.read_int(fd)  # city id
                # latitude, longitude, altitude
                coords = [self.read_short(fd), self.read_short(fd), self.read_short(fd)]
                print self.startJD, self.finalJD, self.dayCount
                print self.read_UTF(fd)  # city
                self.read_UTF(fd)  # state
                self.read_UTF(fd)  # country
                self.read_UTF(fd)  # timezone
                self.read_UTF(fd)  # custom data
                transitionCount = self.read_byte(fd)
                self.transitionTimes = []
                self.transitionOffsets = []
                self.transitionNames = []
                for i in range(transitionCount):
                    self.transitionTimes.append(self.read_int(fd) * 1000)  # start_date
                    self.transitionOffsets.append(self.read_short(fd) * 60000)  # gmt_ofs_min
                    self.transitionNames.append(self.read_UTF(fd))  # name
                    print self.transitionTimes[i], ", ", self.transitionTimes[i], " > ", self.transitionOffsets[i], " ", self.transitionNames[i]
            else:
                print "Unknown version ", version
            
        pprint(vars(self))
        self.read_events(fd, event_func)
        fd.close()
       
    def read_events(self, fd, event_func):
        print __file__,  self.startJD, self.finalJD
        flag = 0
        last = Event()
        fnext_date2 = 0
        period = 24 * 60
        event_count = 0
        try:
            while True:
                self.read_byte(fd)
                last.type = self.read_byte(fd)
                self.read_short(fd)
                flag = self.read_short(fd)
                planet = self.read_byte(fd)
                period = 24 * 60
                if last.type == Event.EV_ASCAPHETICS:
                    period = 2 * 60
                count = self.read_short(fd)
                fcumul_date_b = flag & DataFile.EF_CUMUL_DATE_B
                fcumul_date_w = flag & DataFile.EF_CUMUL_DATE_W
                fdate = flag & DataFile.EF_DATE
                fplanet1 = flag & DataFile.EF_PLANET1
                fplanet2 = flag & DataFile.EF_PLANET2
                fdegree = flag & DataFile.EF_DEGREE
                fshort_degree = flag & DataFile.EF_SHORT_DEGREE
                fnext_date2 = flag & DataFile.EF_NEXT_DATE2

                myplanet0 = planet
                myplanet1 = -1
                mydgr = 127
                mydate0 = 0
                mydate1 = 0
                cumul = 0
                date = 0
                print last.type, flag, planet, count
                for i in range(count):
                    if fcumul_date_b:
                        if i:
                            cumul = self.read_byte(fd)
                            date += (cumul + period) * 60
                        else:
                            date = self.read_int(fd)
                    elif fcumul_date_w:
                        if i:
                            cumul = self.read_short(fd)
                            date += (cumul + period) * 60
                        else:
                            date = self.read_int(fd)
                    else:
                        date = self.read_int(fd)

                    mydate0 = date * 1000
                    if fdate:
                        mydate1 = self.read_int(fd) * 1000
                    else:
                        mydate1 = mydate0
                    if fplanet1:
                        myplanet0 = self.read_byte(fd)
                    if fplanet2:
                        myplanet1 = self.read_byte(fd)
                    if fdegree:
                        if fshort_degree:
                            mydgr = self.read_byte(fd)
                        else:
                            mydgr = self.read_short(fd)
                    if fnext_date2:
                        last.date1 = mydate0
                        mydate1 = self.finalJD
                    last.planet0 = myplanet0
                    last.planet1 = myplanet1
                    last.degree = mydgr
                    last.date0 = mydate0
                    last.date1 = mydate1
                    event_func(last)
                    event_count += 1
        except (struct.error):
            print 'EOF reached', event_count

    def print_event(self, event):
        1#print event

if __name__ == '__main__':
    df = DataFile('/home/willow/prj/amax-hg/Astromaximum/2012.comm', 1)
    df.read_sub_data(df.print_event)
    #df = DataFile('/home/willow/amax/data/archive-tzdata/2012/ancients/09fcc911.dat', 0)
    #df.read_sub_data(df.print_event)

    
