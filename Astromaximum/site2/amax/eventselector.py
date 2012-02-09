from django.db.models import Q
from datetime import datetime, timedelta
from amax.models import Event

class EventSelector():
    RISE_SET = [
                [Event.SE_SUN, Event.SE_MOON, Event.SE_MERCURY,],
                [Event.SE_VENUS, Event.SE_JUPITER,],
                [Event.SE_MARS, Event.SE_SATURN,],
                ]

    def __init__(self, year, period0, period1, now, city_id):
        self.set_year(year)
        self.set_period(period0, period1)
        self.now = now
        self.city_id = city_id

    def set_year(self, year):
        self.year = year

    def set_period(self, period0, period1):
        self.period0 = period0
        self.period1 = period1
        self.weekday = self.period0.weekday()

    def get_city(self, event_type):
        city_id = None
        if event_type in [Event.EV_RISE, Event.EV_SET, Event.EV_ASTRORISE, Event.EV_ASTROSET, Event.EV_PLANET_HOUR]:
            city_id = self.city_id
        #import pdb; pdb.set_trace()
        return city_id

    def get_event_on_period(self, event_type, planet):
        city_id = self.get_city(event_type)
        return list(Event.objects.filter(year__exact=self.year, datetime0__gte=self.period0, datetime0__lt=self.period1,
            event_type__exact=event_type, planet0__exact=planet, city_id__exact=city_id).order_by('datetime0'))
        
    def get_crossing_event(self, event_type, planet):
        city_id = self.get_city(event_type)
        q_outside_range = Q(datetime0__gte=self.period1) | Q(datetime1__lt=self.period0)
        return Event.objects.filter(year__exact=self.year, city_id__exact=city_id, event_type__exact=event_type, planet0__exact=planet).\
            filter(~q_outside_range).order_by('datetime0')

    def zeroJD(self):
        return datetime(1900, 1, 1)

    def finalJD(self):
        return datetime(self.year + 1, 1, 1)

    def get_aspects_on_period(self, is_moon):
        q = Q(planet0__exact=Event.SE_MOON) | Q(planet1__exact=Event.SE_MOON)
        if not is_moon:
            q = ~q
            
        return list(Event.objects.filter(year__exact=self.year,
                                    datetime0__gte=self.period0, datetime0__lt=self.period1,
                                    event_type__exact=Event.EV_ASP_EXACT). \
            filter(q).order_by('datetime0'))
    
    def get_vocs(self):
        return self.get_event_on_period(Event.EV_VOC, Event.SE_MOON)
    
    def get_vc(self):
        return self.get_event_on_period(Event.EV_VIA_COMBUSTA, Event.SE_MOON)

    def get_rise_sets(self, planet_list):
        city_id = self.get_city(Event.EV_ASTRORISE)
        q_type = Q(event_type__exact=Event.EV_ASTRORISE) | Q(event_type__exact=Event.EV_ASTROSET)
        q_planet = Q()
        for planet in planet_list:
            q_planet |= Q(planet0__exact=planet)
        q_inside_range = Q(datetime0__gte=self.period0) & Q(datetime0__lt=self.period1)
        rise_list = list(Event.objects.filter(year__exact=self.year, city_id__exact=city_id).\
            filter(q_inside_range & q_type & q_planet).order_by('planet0', 'datetime0', 'event_type'))
        return rise_list
    
    def get_sun_degree(self):
        return self.get_event_on_period(Event.EV_DEGREE_PASS, Event.SE_SUN)

    def get_moon_sign(self):
        return self.get_crossing_event(Event.EV_SIGN_ENTER, Event.SE_MOON)

    def get_aspects(self):
        return self.get_aspects_on_period(False)

    def get_moon_move(self):
        self.set_period(self.period0 + timedelta(days=-1), self.period1 + timedelta(days=+1))
        moon_aspects = list(self.get_event_on_period(Event.EV_ASP_EXACT, Event.SE_MOON))
    
        moon_sign_enter_events = list(self.get_event_on_period(Event.EV_SIGN_ENTER, Event.SE_MOON))
    
        moon_aspects.extend(moon_sign_enter_events)
        moon_aspects.sort(key=lambda event: event.datetime0)
        return moon_aspects

    def get_tithi(self):
        return self.get_event_on_period(Event.EV_TITHI, Event.SE_MOON)
    
    @staticmethod
    def get_event(event_id):
        return Event.objects.filter(id__exact=int(event_id))

    def get_neighbour_event(self, ev, direction, planet):
        q = Q()
        if direction == 'b':
            q &= Q(datetime0__lt=ev.datetime0)
            ordering = '-datetime0'
        elif direction == 'a':
            q &= Q(datetime0__gt=ev.datetime0)
            ordering = 'datetime0'
        if planet is not None:
            q &= Q(planet0__exact=planet)
        event_list = Event.objects.filter(event_type__exact=ev.event_type, \
                                          year__exact=ev.year, city_id__exact=self.get_city(ev.event_type)).\
                                          filter(q).order_by(ordering)
        if event_list:
            return event_list[0]
        return None

    @staticmethod
    def get_event_text(event):
        return str(event)

    def get_planetary_hours(self):
        city_id = self.get_city(Event.EV_PLANET_HOUR)
        return list(Event.objects.filter(year__exact=self.year, datetime0__gte=self.period0, datetime0__lt=self.period1,
            event_type__exact=Event.EV_PLANET_HOUR, city_id__exact=city_id).order_by('datetime0'))
