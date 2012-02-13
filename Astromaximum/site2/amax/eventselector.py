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

    def get_rise_sets(self, planet):
        city_id = self.get_city(Event.EV_ASTRORISE)
        q_inside_range = Q(datetime0__gte=self.period0) & Q(datetime0__lt=self.period1)
        rise_list = list(Event.objects.filter(year__exact=self.year, city_id__exact=city_id,
                                              event_type__exact=Event.EV_ASTRORISE,
                                              planet0__exact=planet).filter(q_inside_range).\
                         order_by('datetime0', 'event_type'))
        return rise_list
    
    def get_sun_degree(self):
        return self.get_event_on_period(Event.EV_DEGREE_PASS, Event.SE_SUN)

    def get_moon_sign(self):
        return self.get_crossing_event(Event.EV_SIGN_ENTER, Event.SE_MOON)

    def get_aspects(self):
        return self.get_aspects_on_period(False)

    def get_retrograde(self):
        q_outside_range = Q(datetime0__gte=self.period1) | Q(datetime1__lt=self.period0)
        return Event.objects.filter(year__exact=self.year, city_id__exact=None, event_type__exact=Event.EV_RETROGRADE).\
            filter(~q_outside_range).order_by('datetime0')

    def get_moon_move(self):
        period0 = self.period0
        period1 = self.period1
        self.set_period(self.period0 + timedelta(days=-1), self.period1 + timedelta(days=+1))
        moon_aspects = list(self.get_event_on_period(Event.EV_ASP_EXACT, Event.SE_MOON))
    
        moon_sign_enter_events = list(self.get_event_on_period(Event.EV_SIGN_ENTER, Event.SE_MOON))
    
        moon_aspects.extend(moon_sign_enter_events)
        moon_aspects.sort(key=lambda event: event.datetime0)
        
        moon_move = []
        first_in_period = None
        last_in_period = None
        for i in range(len(moon_aspects) - 1):
            current = moon_aspects[i]
            if Event.date_between(Event.fromutc(current.datetime0), period0, period1) == 0:
                if first_in_period is None:
                    first_in_period = i
                last_in_period = i
            transition_event = Event()
            transition_event.event_type = Event.EV_MOON_MOVE
            if current.event_type == Event.EV_SIGN_ENTER:
                transition_event.datetime0 = current.datetime0
            else:
                transition_event.datetime0 = current.datetime1
            transition_event.datetime1 = moon_aspects[i + 1].datetime0
            id0 = None
            id1 = None
            j = i
            while(j >= 0):
                if moon_aspects[j].planet1 <= Event.SE_SATURN:
                    id0 = moon_aspects[j].pk
                    break
                j -= 1
            j = i + 1
            while(j < len(moon_aspects)):
                if moon_aspects[j].planet1 <= Event.SE_SATURN:
                    id1 = moon_aspects[j].pk
                    break
                j += 1
            transition_event.id0 = id0
            transition_event.id1 = id1
            moon_move.append(current)
            moon_move.append(transition_event)

        moon_move.append(moon_aspects[-1])
        first_in_period -= 1
        last_in_period += 1
        moon_move = moon_move[first_in_period * 2:last_in_period * 2 + 1]
        return moon_move

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
