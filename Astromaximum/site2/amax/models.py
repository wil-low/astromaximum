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
    SE_TRUE_NODE = 10
    SE_MEAN_APOG = 11
    SE_WHITE_MOON = 12
    
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

#    def __init__(self, *args, **kwargs): 
#        super(Event, self).__init__(*args, **kwargs)
#        self.dates = Event.date_to_string(self.datetime0) + '/' + Event.date_to_string(self.datetime1)

    def __unicode__(self):
        return u"Event y %s, city %s type %s %s,%s %s: (%s %s)" % (
            self.year, self.city_id, self.event_type,
            self.planet0, self.planet1, self.degree,
            #self.date0, self.date1,
            self.datetime0, self.datetime1)
    
    def time0(self):
        return "%s" % self.datetime0.strftime('%H:%M')
    
    def time1(self):
        return "%s" % self.datetime1.strftime('%H:%M')

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
