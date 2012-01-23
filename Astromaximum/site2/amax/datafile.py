import struct
from calendar import timegm
from pprint import pprint
from models import Event
from datetime import datetime

class DataFile:
    EF_DATE = 0x1  # contains 2nd date - 4b
    EF_PLANET1 = 0x2  # contains 1nd planet - 1b
    EF_PLANET2 = 0x4  # contains 2nd planet - 1b
    EF_DEGREE = 0x8  # contains degree or angle - 2b
    EF_CUMUL_DATE_B = 0x10  # date are cumulative from 1st 4b - 1b
    EF_CUMUL_DATE_W = 0x20  # date are cumulative from 1st 4b - 2b
    EF_SHORT_DEGREE = 0x40  # contains angle 0..180 - 1b
    EF_NEXT_DATE2 = 0x80  # 2nd date is 1st in next event

    SECINDAY = 86400
    
    def __init__(self, filename, is_common):
        self.filename = filename
        self.is_common = is_common
        self.coords = []
        self.year = 0
        self.city_id = 0

    def read_byte(self, fd):
        val, = struct.unpack('b', fd.read(1))
        return val
    
    def read_ubyte(self, fd):
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
        self.year = self.read_short(fd)
        month = self.read_byte(fd)
        day = self.read_byte(fd)
        
        self.startJD = timegm((self.year, month, day, 0, 0, 0))
        self.dayCount = self.read_short(fd)
        self.finalJD = self.startJD + DataFile.SECINDAY * self.dayCount

        print 'Range', self.startJD, self.finalJD
        
        startDate = datetime.utcfromtimestamp(self.startJD)
        finalDate = datetime.utcfromtimestamp(self.finalJD)
        print 'Range2', startDate, finalDate
 
    def read_sub_data(self, event_func):
        "parse datafile header and call event_func with Event"
        
        fd = file(self.filename, 'rb')
        
        if self.is_common:
            self.read_YMD(fd)
            self.city_id = None
            fd.read(2)  # custom data length
        else:
            print fd.read(4)  # signature
            version = self.read_byte(fd)
            if version == 2:
                self.read_YMD(fd)
                self.city_id = '%08x' % self.read_uint(fd)  # city id
                # latitude, longitude, altitude
                self.coords = [self.read_short(fd), self.read_short(fd), self.read_short(fd)]
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
                    self.transitionTimes.append(self.read_int(fd))  # start_date
                    self.transitionOffsets.append(self.read_short(fd) * 60)  # gmt_ofs_min
                    self.transitionNames.append(self.read_UTF(fd))  # name
                    print self.transitionTimes[i], ", ", self.transitionTimes[i], " > ", self.transitionOffsets[i], " ", self.transitionNames[i]
            else:
                print "Unknown version ", version
            
        pprint(vars(self))
        event_count = self.read_events(fd, event_func)
        fd.close()
        return event_count
       
    def read_events(self, fd, event_func):
        last = Event()
        last.date0 = last.date1 = 0
     
        fnext_date2 = 0
        period = 24 * 60
        event_count = 0
        flag = 0
        try:
            while True:
                self.read_byte(fd)
                last.event_type = self.read_byte(fd)
                self.read_short(fd)
                flag = self.read_short(fd)
                planet = self.read_byte(fd)
                period = 24 * 60
                if last.event_type == Event.EV_ASCAPHETICS:
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
                print last.event_type, flag, planet, count
                print 'flags:', fcumul_date_b, fcumul_date_w, fdate, fplanet1, fplanet2, fdegree, fshort_degree, fnext_date2
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

                    mydate0 = date
                    
                    if fdate:
                        mydate1 = self.read_int(fd)
                    else:
                        mydate1 = mydate0
                        
                    if fplanet1:
                        myplanet0 = self.read_byte(fd)
                        
                    if fplanet2:
                        myplanet1 = self.read_byte(fd)
                        
                    if fdegree:
                        if fshort_degree:
                            mydgr = self.read_ubyte(fd)
                        else:
                            mydgr = self.read_short(fd)
                            
                    if fnext_date2:
                        last.date1 = mydate0
                        mydate1 = self.finalJD
                        
                    new_event = self.clone_event(last)
                    event_func(new_event)
                    event_count += 1
                    
                    last.date0 = mydate0
                    last.date1 = mydate1
                    last.planet0 = myplanet0
                    last.planet1 = myplanet1
                    last.degree = mydgr

                new_event = self.clone_event(last)
                event_func(new_event)
                event_count += 1

        except (struct.error):
            print 'EOF reached'
        return event_count

    def clone_event(self, event):
        new_event = Event()
        new_event.year = self.year
        new_event.city_id = self.city_id
        new_event.event_type = event.event_type
        new_event.datetime0 = datetime.utcfromtimestamp(event.date0)
        new_event.datetime1 = datetime.utcfromtimestamp(event.date1)
        new_event.date0 = event.date0
        new_event.date1 = event.date1
        new_event.planet0 = event.planet0
        new_event.planet1 = event.planet1
        new_event.degree = event.degree
        return new_event

    def process_event(self, event):
        event.save()

    def print_event(self, event):
        print event.__unicode__()


def main():
    # import amax.datafile; amax.datafile.main()

    #df = DataFile('/home/willow/prj/amax-hg/Astromaximum/2012.comm', 1)
    #df.read_sub_data(df.process_event)
    df = DataFile('/home/willow/amax/data/archive-tzdata/2012/UA/d9d95558.dat', 0)
    df.read_sub_data(df.print_event)

if __name__ == '__main__':
    main()

    
