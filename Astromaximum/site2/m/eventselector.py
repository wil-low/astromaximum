from django.db.models import Q
from datetime import datetime, timedelta
from amax.models import Event

class EventSelector():
    q_sun = Q(planet0__exact=Event.SE_SUN)
    q_moon = Q(planet0__exact=Event.SE_MOON)
    q_rise = Q(event_type__exact=Event.EV_RISE)
    q_set = Q(event_type__exact=Event.EV_SET)

    def __init__(self, year, period0, period1, now):
        self.set_year(year)
        self.set_period(period0, period1)
        self.now = now

    def set_year(self, year):
        self.year = year

    def set_period(self, period0, period1):
        self.period0 = period0
        self.period1 = period1

    def get_event_on_period(self, event_type, planet):
        return Event.objects.filter(year__exact=self.year, datetime0__gte=self.period0, datetime0__lt=self.period1,
            event_type__exact=event_type, planet0__exact=planet).order_by('datetime0')
        
    def get_event_on_period_q(self, q):
        return Event.objects.filter(year__exact=self.year, datetime0__gte=self.period0, datetime0__lt=self.period1). \
            filter(q).order_by('datetime0')

    def zeroJD(self):
        return datetime(1900, 1, 1)

    def finalJD(self):
        return datetime(self.year + 1, 1, 1)

    def get_aspects_on_period(self, is_moon):
        q = Q(planet0__exact=Event.SE_MOON) | Q(planet1__exact=Event.SE_MOON)
        if not is_moon:
            q = ~q
            
        return Event.objects.filter(year__exact=self.year,
                                    datetime0__gte=self.period0, datetime0__lt=self.period1,
                                    event_type__exact=Event.EV_ASP_EXACT). \
            filter(q).order_by('datetime0')
    
    def get_vocs(self):
        return self.get_event_on_period(Event.EV_VOC, Event.SE_MOON)
    
    def get_vc(self):
        return self.get_event_on_period(Event.EV_VIA_COMBUSTA, Event.SE_MOON)

    def get_rise_set(self, planet):
        q_planet = Q(planet0__exact=planet)
        ev_rise = self.get_event_on_period_q(q_planet & EventSelector.q_rise)[0]
        ev_set = self.get_event_on_period_q(q_planet & EventSelector.q_set)[0]
        ev_rise.datetime1 = ev_set.datetime0
        return [ev_rise,]
    
    def get_sun_degree(self):
        return self.get_event_on_period(Event.EV_DEGREE_PASS, Event.SE_SUN)

    def get_moon_sign(self):
        return self.get_event_on_period(Event.EV_SIGN_ENTER, Event.SE_MOON)

#    event_list['sun_day'] = es.get_event_on_period(Event.EV_RISE, Event.SE_SUN)
#    event_list['moon_day'] = es.get_event_on_period(Event.EV_RISE, Event.SE_MOON)
#
#
#    # sun day with Navroz
#    es.set_period(es.zeroJD(), es.finalJD())
#    navroz_events = es.get_event_on_period(Event.EV_NAVROZ, Event.SE_SUN)
#    navroz = navroz_events[1].date0
#    sunrise = event_list['sun_rise'][0].date0
#    if sunrise < navroz:
#        navroz = navroz_events[0].date0
#    pltDaySun = int((sunrise - navroz) / Event.SECINDAY + 0.5)
#    if pltDaySun < 360:
#        pltDaySun = pltDaySun % 30 + 1
#    else:
#        pltDaySun = -(pltDaySun - 359)
#    sun_day_event = event_list['sun_rise'][0]
#    sun_day_event.degree = pltDaySun
#    event_list['sun_day'] = [sun_day_event,]

    def get_aspects(self):
        return self.get_aspects_on_period(False)

    def get_moon_move(self):
        self.set_period(self.period0 + timedelta(days=-2), self.period1 + timedelta(days=+2))
        moon_aspects = list(self.get_event_on_period(Event.EV_ASP_EXACT, Event.SE_MOON))
    
        self.set_period(self.period0, self.period1 + timedelta(days=+2))
        moon_sign_enter_events = list(self.get_event_on_period(Event.EV_SIGN_ENTER, Event.SE_MOON))
    
        moon_aspects.extend(moon_sign_enter_events)
        moon_aspects.sort(key=lambda event: event.date0)
        return moon_aspects

    def get_tithi(self):
        return self.get_event_on_period(Event.EV_TITHI, Event.SE_MOON)
    
    @staticmethod
    def get_event(event_id):
        return Event.objects.filter(id__exact=int(event_id))

    @staticmethod
    def get_event_text(event):
        return str(event)
