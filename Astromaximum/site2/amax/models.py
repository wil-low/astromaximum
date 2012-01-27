from django.db import models

class Event(models.Model):
    SE_SUN = 0
    SE_MOON = 1
    SE_MERCURY = 2
    SE_VENUS = 3
    SE_MARS = 4
    SE_JUPITER = 5
    SE_SATURN = 6
    SE_URANUS = 7
    SE_NEPTUNE = 8
    SE_PLUTO = 9
    
    EV_VOC = 0  # void of course
    EV_SIGN_ENTER = 1  # enter into sign
    EV_ASP_EXACT = 2  # exact aspect
    EV_RISE = 3   # rising & setting
    EV_DEGREE_PASS = 4   # entering degree
    EV_VIA_COMBUSTA = 5   # good & bad degrees
    EV_RETROGRADE = 6
    EV_ECLIPSE = 7
    EV_TITHI = 8
    EV_NAKSHATRA = 9
    EV_SET = 10   # rising & setting
    EV_DECL_EXACT = 11   # declination
    EV_NAVROZ = 12   # Navroz
    EV_TOP_DAY = 13   # week days
    EV_PLANET_HOUR = 14   # planetary hours
    EV_STATUS = 15
    EV_SUN_RISE = 16
    EV_MOON_RISE = 17
    EV_MOON_MOVE = 18
    EV_SEL_DEGREES = 19
    EV_DAY_HOURS = 20
    EV_NIGHT_HOURS = 21
    EV_SUN_DAY = 22
    EV_MOON_DAY = 23
    EV_TOP_MONTH = 24
    EV_MOON_PHASE = 25
    EV_ZODIAC_SIGN = 26
    EV_PANEL = 27
    EV_TOPIC_BUTTON = 28
    EV_DEG_2ND = 29  # degrees on second page
    EV_WEEK_GRID = 30
    EV_MONTH_GRID = 31
    EV_DECUMBITURE = 32
    EV_DECUMB_ASPECT = 33
    EV_DECUMB_BEGIN = 34
    EV_SUN_DEGREE_LARGE = 35
    EV_MOON_SIGN_LARGE = 36
    EV_HELP = 37
    EV_ASP_EXACT_MOON = 38
    EV_DEGPASS0 = 39
    EV_DEGPASS1 = 40
    EV_DEGPASS2 = 41
    EV_DEGPASS3 = 42
    EV_HELP0 = 43
    EV_HELP1 = 44
    EV_ASTRORISE = 45
    EV_ASTROSET = 46
    EV_APHETICS = 47
    EV_FAST = 48
    EV_ASCAPHETICS = 49
    EV_MSG = 50
    EV_BACK = 51
    EV_TATTVAS = 52
    EV_LAST = 53   # last - do not use
    
    STATE_UNDEFINED = 0
    STATE_GONE = 1
    STATE_COMING = 2
    STATE_ACTIVE = 3
    
    SECINDAY = 24 * 60 * 60
    
    CONSTELL = ["Ari", "Tau", "Gem", "Cnc", "Leo", "Vir", "Lib", "Sco", "Sgr", "Cap", "Aqu", "Psc"]
    
    PLANET = ["SO", "MO", "ME", "VE", "MA", "JU", "SA", "UR", "NE", "PL"]

    # angle: (ordinal number, aspect goodness(0 - conjunction, 1 - bad, 2 - good))
    ASPECT = {0: (0, 0), 180: (1, 1), 120: (2, 2), 90: (3, 1), 60: (4, 2), 45: (5, 2)}
    
    EVENT_TYPE = ['EV_VOC', 'EV_SIGN_ENTER', 'EV_ASP_EXACT', 'EV_RISE', 'EV_DEGREE_PASS',
        'EV_VIA_COMBUSTA', 'EV_RETROGRADE', 'EV_ECLIPSE', 'EV_TITHI', 'EV_NAKSHATRA', 'EV_SET',
        'EV_DECL_EXACT', 'EV_NAVROZ', 'EV_TOP_DAY', 'EV_PLANET_HOUR', 'EV_STATUS', 'EV_SUN_RISE',
        'EV_MOON_RISE', 'EV_MOON_MOVE', 'EV_SEL_DEGREES', 'EV_DAY_HOURS', 'EV_NIGHT_HOURS',
        'EV_SUN_DAY', 'EV_MOON_DAY', 'EV_TOP_MONTH', 'EV_MOON_PHASE', 'EV_ZODIAC_SIGN',
        'EV_PANEL', 'EV_TOPIC_BUTTON', 'EV_DEG_2ND', 'EV_WEEK_GRID', 'EV_MONTH_GRID',
        'EV_DECUMBITURE', 'EV_DECUMB_ASPECT', 'EV_DECUMB_BEGIN', 'EV_SUN_DEGREE_LARGE',
        'EV_MOON_SIGN_LARGE', 'EV_HELP', 'EV_ASP_EXACT_MOON', 'EV_DEGPASS0', 'EV_DEGPASS1',
        'EV_DEGPASS2', 'EV_DEGPASS3', 'EV_HELP0', 'EV_HELP1', 'EV_ASTRORISE', 'EV_ASTROSET',
        'EV_APHETICS', 'EV_FAST', 'EV_ASCAPHETICS', 'EV_MSG', 'EV_BACK', 'EV_TATTVAS', 'EV_LAST']

    year = models.IntegerField(default=-1)
    city_id = models.TextField(null=True)
    
    event_type = models.IntegerField(default=EV_LAST)
    
    # these fields can be removed later
    date0 = models.IntegerField()
    date1 = models.IntegerField()
    
    datetime0 = models.DateTimeField()
    datetime1 = models.DateTimeField()
    planet0 = models.IntegerField(default=-1)
    planet1 = models.IntegerField(default=-1)
    degree = models.IntegerField(default=127)

    @staticmethod
    def date_to_string(date):
        "converts datetime to YYYY-MM-DD string"
        return "%04d-%02d-%02d" % (date.year, date.month, date.day)

    def __init__(self, *args, **kwargs): 
        super(Event, self).__init__(*args, **kwargs)
        self.state = Event.STATE_UNDEFINED

    def __unicode__(self):
        print_raw_date = True
        planet0_str = str(self.planet0)
        planet1_str = str(self.planet1)
        if Event.SE_SUN <= self.planet0 <= Event.SE_PLUTO:
            planet0_str = Event.PLANET[self.planet0]
        if Event.SE_SUN <= self.planet1 <= Event.SE_PLUTO:
            planet1_str = Event.PLANET[self.planet1]
        if print_raw_date:
            return u"%s %s/%s %s : (%s %s)(%s %s) y%s %s state %d" % (
                Event.EVENT_TYPE[self.event_type],
                planet0_str, planet1_str, self.degree,
                self.date0, self.date1,
                self.datetime0, self.datetime1, self.year, self.city_id, self.state)
        else:
            return u"%s %s/%s %s : (%s %s) y%s %s" % (
                Event.EVENT_TYPE[self.event_type],
                planet0_str, planet1_str, self.degree,
                self.datetime0, self.datetime1, self.year, self.city_id)
    
    def time0(self):
        return "%s" % self.datetime0.strftime('%H:%M')
    
    def time1(self):
        return "%s" % self.datetime1.strftime('%H:%M')
    
    def get_degree(self):
        return self.degree & 0x3ff

    def get_degree_type(self):
        return (self.degree >> 14) & 0x3

    def degree_number(self):
        return self.get_degree() % 30 + 1

    def degree_zodiac(self):
        return Event.CONSTELL[self.get_degree() / 30]

    def phase_url(self):
        return "/i/phases/ph50-%02d.png" % self.get_degree()

    def zodiac_url(self):
        return "/i/z%d.png" % self.get_degree()
    
    def planet0_url(self):
        return "/i/p%d.png" % self.planet0

    def planet1_url(self):
        return "/i/p%d.png" % self.planet1

    def aspect_url(self):
        return "/i/a%d.png" % Event.ASPECT[self.degree]
    
    def planet_in_degree_str(self):
        return "%s<br/>%02d&deg;%s" % (Event.PLANET[self.planet0], self.degree_number(), self.degree_zodiac())

    def planet_in_sign_str(self):
        return "%s<br/>%s" % (Event.PLANET[self.planet0], Event.CONSTELL[self.get_degree()])

    def aspect_str(self):
        return "%s<sub>%d&deg;</sub>%s" % (Event.PLANET[self.planet0], self.degree, Event.PLANET[self.planet1])

    def list_aspect_str(self):
        return "%s %s %d&deg; %s" % (self.datetime0, Event.PLANET[self.planet0], self.degree, Event.PLANET[self.planet1])

    def riseset_str(self):
        return "%s<br/>%s<br/>%s" % (Event.PLANET[self.planet0], self.time0(), self.time1())
        
    @staticmethod
    def date_between(date, start, end):
        if date < start:
            return -1
        if date >= end:
            return 1
        return 0

    def is_in_period(self, start, end, is_special):
        if self.date0 == 0:
            return False
        f = Event.date_between(self.date0, start, end) + Event.date_between(self.date1, start, end)
        if f == 2 or f == -2:
            return False
        if is_special:
            if f == -1:
                return False
        return True
        
    class Meta:
        ordering = ['datetime0']
        
class Location(models.Model):
    city_hash = models.TextField()
    name = models.TextField()
    state = models.TextField(null=True)
    country = models.TextField()
    timezone =  models.TextField()
    latitude = models.FloatField()
    longitude = models.FloatField()
    altitude = models.FloatField()
    
    def __unicode__(self):
        return self.city_hash


class Text(models.Model):
    language = models.TextField()
    event_type = models.IntegerField()
    planet = models.IntegerField()
    param0 = models.IntegerField(null=True)
    param1 = models.IntegerField(null=True)
    param2 = models.IntegerField(null=True)
    message = models.TextField()

    def __unicode__(self):
        return u'%s %s %s (%s, %s, %s)' % (Event.EVENT_TYPE[self.event_type], self.language,
            self.planet, self.param0, self.param1, self.param2)
