import struct
from calendar import timegm
from models import Event, Location
from datetime import datetime, timedelta
from eventselector import EventSelector

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
        self.fd = open(self.filename, 'rb')
        self.read_header()

    def read(self, count):
        return self.fd.read(count)

    def read_byte(self):
        val, = struct.unpack('b', self.fd.read(1))
        return val
    
    def read_ubyte(self):
        val, = struct.unpack('B', self.fd.read(1))
        return val
    
    def read_short(self):
        val, = struct.unpack('>h', self.fd.read(2))
        return val

    def read_int(self):
        val, = struct.unpack('>i', self.fd.read(4))
        return val

    def read_uint(self):
        val, = struct.unpack('>I', self.fd.read(4))
        return val

    def read_UTF(self):
        str_length = self.read_short()
        return self.read(str_length)

    def read_YMD(self):
        self.year = self.read_short()
        month = self.read_byte()
        day = self.read_byte()
        
        self.startJD = timegm((self.year, month, day, 0, 0, 0))
        if self.is_common:
            self.read(2)
        self.dayCount = self.read_short()
        self.finalJD = self.startJD + DataFile.SECINDAY * self.dayCount

        print 'Range', self.startJD, self.finalJD
        
        startDate = datetime.utcfromtimestamp(self.startJD)
        finalDate = datetime.utcfromtimestamp(self.finalJD)
        print 'Range2', startDate, finalDate
 
    def read_header(self):
        if self.is_common:
            self.read_YMD()
            self.city_id = None
        else:
            print self.read(4)  # signature
            version = self.read_byte()
            if version == 2:
                self.read_YMD()
                location = Location()
                location.id = self.read_uint()
                # latitude, longitude, altitude
                location.latitude = self.read_short() / 100.
                location.longitude = self.read_short() / 100.
                location.altitude = self.read_short()
                location.name = self.read_UTF()  # city
                location.state = self.read_UTF()  # state
                location.country = self.read_UTF()  # country
                location.timezone = self.read_UTF()  # timezone
                location.save()
                self.city_id = location
                self.read_UTF()  # custom data
                transitionCount = self.read_byte()
                self.transitionTimes = []
                self.transitionOffsets = []
                self.transitionNames = []
                for i in range(transitionCount):
                    self.transitionTimes.append(self.read_int())  # start_date
                    self.transitionOffsets.append(self.read_short() * 60)  # gmt_ofs_min
                    self.transitionNames.append(self.read_UTF())  # name
                    print self.transitionTimes[i], ", ", self.transitionTimes[i], " > ", self.transitionOffsets[i], " ", self.transitionNames[i]
            else:
                print "Unknown version ", version
        
    def read_sub_data(self, event_func):
        "call event_func with Event"
        return self.read_events(event_func)
       
    def read_events(self, event_func):
        last = Event()
        last.date0 = last.date1 = 0
     
        fnext_date2 = 0
        period = 24 * 60
        event_count = 0
        flag = 0
        try:
            while True:
                self.read_byte()
                last.event_type = self.read_byte()
                assert(Event.EV_VOC <= last.event_type <= Event.EV_LAST)
                self.read_short()
                flag = self.read_short()
                planet = self.read_byte()
                period = 24 * 60
                if last.event_type == Event.EV_ASCAPHETICS:
                    period = 2 * 60
                count = self.read_short()
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
                print 'ev_type', last.event_type, flag, planet, count
                print 'flags:', fcumul_date_b, fcumul_date_w, fdate, fplanet1, fplanet2, fdegree, fshort_degree, fnext_date2
                for i in range(count):
                    if fcumul_date_b:
                        if i:
                            cumul = self.read_byte()
                            date += (cumul + period) * 60
                        else:
                            date = self.read_int()
                    elif fcumul_date_w:
                        if i:
                            cumul = self.read_short()
                            date += (cumul + period) * 60
                        else:
                            date = self.read_int()
                    else:
                        date = self.read_int()

                    mydate0 = date
                    
                    if fdate:
                        mydate1 = self.read_int()
                    else:
                        mydate1 = mydate0
                        
                    if fplanet1:
                        myplanet0 = self.read_byte()
                        
                    if fplanet2:
                        myplanet1 = self.read_byte()
                        
                    if fdegree:
                        if fshort_degree:
                            mydgr = self.read_ubyte()
                        else:
                            mydgr = self.read_short()
                            
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

                if fnext_date2:
                    new_event = self.clone_event(last)
                    event_func(new_event)
                    event_count += 1

        except (struct.error):
            print 'EOF reached'
        return event_count
    
    def close(self):
        self.fd.close()
    
    def clone_event(self, event):
        new_event = Event()
        new_event.year = self.year
        new_event.city_id = self.city_id
        new_event.event_type = event.event_type
        new_event.datetime0 = datetime.utcfromtimestamp(event.date0).replace(tzinfo=Event.utc_tz)
        new_event.datetime1 = datetime.utcfromtimestamp(event.date1).replace(tzinfo=Event.utc_tz)
        new_event.date0 = event.date0
        new_event.date1 = event.date1
        new_event.planet0 = event.planet0
        new_event.planet1 = event.planet1
        new_event.degree = event.degree
        return new_event

    def process_event(self, event):
        if event.event_type == Event.EV_ASTRORISE:
            event.degree = Event.RS_ASC
        elif event.event_type == Event.EV_ASTROSET:
            event.event_type = Event.EV_ASTRORISE
            event.degree = Event.RS_DSC
        event.save()

    def process_mc_ic(self, event):
        event.save()

    def print_event(self, event):
        print event.__unicode__()

    def calc_mc_ic(self, event_func):
        count = 0
        Event.set_tzinfo(self.city_id)
        for planet in range(Event.SE_SUN, Event.SE_URANUS):
            rise_sets = list(Event.objects.filter(city_id__exact=self.city_id, event_type__exact=Event.EV_ASTRORISE, 
                                                  planet0__exact=planet, year__exact=self.year).order_by('datetime0'))
            for i in range(len(rise_sets) - 1):
                diff = rise_sets[i + 1].datetime0 - rise_sets[i].datetime0
                ev = Event()
                ev.event_type = Event.EV_ASTRORISE
                ev.planet0 = planet
                ev.year = self.year
                ev.city_id = self.city_id
                ev.datetime0 = ev.datetime1 = rise_sets[i].datetime0 + diff / 2
                if rise_sets[i].degree == Event.RS_ASC:
                    ev.degree = Event.RS_MC
                else:
                    ev.degree = Event.RS_IC
                event_func(ev)
                count += 1
        return count

    def calc_planet_hours(self, event_func):
        Event.set_tzinfo(self.city_id)
        day_delta = timedelta(days=1)
        period0 = datetime(self.year, 1, 1, tzinfo=Event.tzinfo).astimezone(Event.utc_tz)
        es = EventSelector(self.year, period0, period0, period0, self.city_id)
        count = 0
        for i in range(self.dayCount):
            es.set_period(period0, period0 + day_delta)
            hours = self.get_planetary_hours(es)
            for ev in hours:
                ev.year = self.year
                ev.city_id = self.city_id
                event_func(ev)
                count += 1
            period0 += day_delta
        return count
            
    WEEK_START_HOUR = [3, 6, 2, 5, 1, 4, 0]
    HOUR_SEQ = [Event.SE_SUN, Event.SE_VENUS, Event.SE_MERCURY,
        Event.SE_MOON, Event.SE_SATURN, Event.SE_JUPITER, Event.SE_MARS]

    def get_planetary_hours(self, es):
        today_rise = es.get_event_on_period(Event.EV_RISE, Event.SE_SUN)[0]
        today_set = es.get_event_on_period(Event.EV_SET, Event.SE_SUN)[0]
        day_delta = timedelta(days=1)
        es.set_period(es.period0 + day_delta, es.period1 + day_delta)
        tomorrow_rise = es.get_event_on_period(Event.EV_RISE, Event.SE_SUN)[0]
        #import pdb; pdb.set_trace()
        hours = [];
        start_hour = DataFile.WEEK_START_HOUR[today_rise.datetime0.weekday()]
        
        diff = today_set.datetime0 - today_rise.datetime0
        diff_sec = (diff.days * 86400 + diff.seconds) / 12
        day_hour = timedelta(seconds=diff_sec)
        
        diff = tomorrow_rise.datetime0 - today_set.datetime0
        diff_sec = (diff.days * 86400 + diff.seconds) / 12
        night_hour = timedelta(seconds=diff_sec)
        
        start = today_rise.datetime0
        for i in range(24):
            ev = Event()
            ev.datetime0 = start
            ev.planet0 = DataFile.HOUR_SEQ[start_hour % 7]
            if i < 12:
                start += day_hour
            else:
                start += night_hour
            ev.datetime1 = start
            ev.date0 = ev.date1 = 0
            ev.event_type = Event.EV_PLANET_HOUR
            hours.append(ev)
            start_hour += 1
        return hours

def main():
    # import amax.datafile; amax.datafile.main()

    df = DataFile('/home/willow/prj/amax-hg/Astromaximum/site2/data/commons/2012.comm', 1)
    df.read_sub_data(df.process_event)
    df.close()
    #df = DataFile('/home/willow/amax/data/archive-tzdata/2012/UA/d9d95558.dat', 0)
    #df.read_sub_data(df.print_event)

if __name__ == '__main__':
    main()

    
