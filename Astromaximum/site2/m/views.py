from django.template import RequestContext
from django.http import HttpResponseRedirect
from django.shortcuts import render_to_response
from django.conf import settings
from django.contrib.auth.decorators import login_required
from django.db.models import Q
import datetime
from amax.eventselector import EventSelector
from amax.models import Event, Text, UserProfile, Location
from forms import SettingsForm

def get_user_profile(request):
    if request.user.is_anonymous():
        request.session.set_expiry(settings.ANONYMOUS_USER['session_expiry'])
        city_id = request.session.get('city_id', settings.ANONYMOUS_USER['city_id'])
        profile = UserProfile()
        locations = Location.objects.filter(id__exact=city_id)
        if not locations:
            city_id = settings.ANONYMOUS_USER['city_id']
            locations = Location.objects.filter(id__exact=city_id)
        profile.location = locations[0]
        request.session['city_id'] = city_id
        return profile
    else:
        return request.user.get_profile()

def today_summary(request):
    profile = get_user_profile(request)
    Event.set_tzinfo(profile.location)
    now = Event.fromutc(datetime.datetime.utcnow())
    return HttpResponseRedirect('%04d-%02d-%02d/summary/' % (now.year, now.month, now.day))

def call_view(request, year, month, day, view_class, event0=-1, event1=-1, direction=''):
    profile = get_user_profile(request)
    Event.set_tzinfo(profile.location)
    v = view_class(year, month, day, datetime.datetime.utcnow().replace(tzinfo=Event.utc_tz), profile.location.pk, event0, event1, direction)
    v.gather_events()
    return v.render(request, profile)

@login_required
def call_view_login_required(request, year, month, day, view_class, event0=-1, event1=-1, direction=''):
    return call_view(request, year, month, day, view_class, event0, event1, direction)

class BaseView():
    def __init__(self, year, month, day, now, city_id, event0, event1, direction):
        self.year = year
        self.month = month
        self.day = day
        self.now = now
        self.event0 = event0
        self.event1 = event1
        self.direction = direction
        self.current_date = datetime.datetime(int(year), int(month), int(day), tzinfo=Event.utc_tz)
        self.prev_date = (self.current_date + datetime.timedelta(days=-1))
        self.next_date = (self.current_date + datetime.timedelta(days=1))
        self.es = EventSelector(self.current_date.year, self.current_date, self.next_date, now, city_id)
        self.title = self.message = ''
        self.event_list = []
        self.params = {}

    def gather_events(self):
        pass
    
    def render(self, request, profile):
        params = {
                  'date_range': (self.current_date, self.next_date),
                  'prev_date': self.prev_date,
                  'next_date': self.next_date,
                  'now': self.now,
                  'event_list': self.event_list,
                  'page_name': request.path_info.split('/')[-2],
                  'user': request.user,
                  'location': profile.location.name,
                  'title': self.title,
                  'message': self.message,
                  }
        params.update(self.params)
        c = RequestContext(request, params)
        return render_to_response(self.template_name, context_instance = c)

class SummaryView(BaseView):
    def select_single_event(self, events):
        now_pos = None
        for ev in events:
            now_pos = Event.date_between(self.now, ev.datetime0, ev.datetime1)
            if now_pos == 1:  # gone
                continue
            if now_pos == -1:  # coming
                ev.state = Event.STATE_COMING
            elif now_pos == 0:  # active
                ev.state = Event.STATE_ACTIVE
            return ev
        if now_pos == 1:  #gone
            return ev  # last event
        return None

    def gather_events(self):
        self.event_list = {}
        self.template_name = 'm/summary.html'
        self.params = {
                       'settings': settings,
                       'rise_set': EventSelector.RISE_SET,
                       }

        self.event_list['vocs'] = self.es.get_vocs()

        self.event_list['vc'] = self.select_single_event(self.es.get_vc())
        self.event_list['sun_degree'] = self.select_single_event(self.es.get_sun_degree())
        self.event_list['moon_sign'] = self.select_single_event(self.es.get_moon_sign())
        self.event_list['tithi'] = self.select_single_event(self.es.get_tithi())
        self.event_list['planet_hour'] = self.select_single_event(self.es.get_planetary_hours())
        #aspects
        self.es.set_period(self.prev_date, self.next_date)
        self.event_list['aspects'] = self.es.get_aspects()
        self.event_list['moon_move'] = self.es.get_moon_move()
        self.event_list['retrograde'] = self.es.get_retrograde()
    
class AspectView(BaseView):
    def gather_events(self):
        self.template_name = 'm/lists/aspects.html'
        self.event_list = self.es.get_aspects()

class TithiView(BaseView):
    def gather_events(self):
        self.template_name = 'm/lists/tithi.html'
        self.event_list = self.es.get_tithi()

class MoonMoveView(BaseView):
    def gather_events(self):
        self.template_name = 'm/lists/moon_move.html'
        self.event_list = self.es.get_moon_move()
        self.params = {'settings': settings}

class PlanetHourView(BaseView):
    def gather_events(self):
        self.template_name = 'm/lists/planet_hour.html'
        self.event_list = self.es.get_planetary_hours()

class RetrogradeView(BaseView):
    def gather_events(self):
        self.template_name = 'm/lists/retrograde.html'
        self.event_list = self.es.get_retrograde()

class RiseSetView(BaseView):
    def gather_events(self):
        self.event_list = []
        self.template_name = 'm/lists/rise_set.html'
        self.es.set_period(self.current_date, self.next_date)
        for planet in EventSelector.RISE_SET[int(self.direction)]:
            self.event_list.append(self.es.get_rise_sets(planet))

class TextView(BaseView):
    def gather_events(self):
        self.template_name = 'm/text.html'
        event_list = EventSelector.get_event(self.event0)
        planet = None
        use_neighbour_navigation = True
        if event_list:
            ev = event_list[0]
            
            if ev.event_type == Event.EV_ASP_EXACT:
                if ev.planet0 == Event.SE_MOON:
                    planet = ev.planet0
                else:
                    use_neighbour_navigation = False
            elif ev.event_type == Event.EV_SIGN_ENTER:
                planet = ev.planet0
            elif ev.event_type == Event.EV_ASTRORISE:
                planet = ev.planet0
            elif ev.event_type == Event.EV_DEGREE_PASS:
                planet = ev.planet0
            elif ev.event_type == Event.EV_RETROGRADE:
                use_neighbour_navigation = False

            if use_neighbour_navigation and self.direction != 'e':
                ev = self.es.get_neighbour_event(ev, self.direction, planet)
            text_list = []
            if ev:
                if use_neighbour_navigation:
                    self.event_list = [ev,]
                self.title = ev
                
                if ev.event_type == Event.EV_TITHI:
                    q = Q(event_type__exact=ev.event_type, param0__exact=ev.degree)
                elif ev.event_type == Event.EV_ASP_EXACT:
                    aspect_goodness = Event.ASPECT[ev.degree][1]
                    if ev.planet0 == Event.SE_MOON:
                        q = Q(event_type__exact=Event.EV_ASP_EXACT_MOON, param0__exact=ev.planet1, param1__exact=aspect_goodness)
                    else:
                        q = Q(event_type__exact=ev.event_type, param0__exact=ev.planet0, param1__exact=ev.planet1, param2__exact=aspect_goodness)
                elif ev.event_type == Event.EV_SIGN_ENTER:
                    q = Q(event_type__exact=ev.event_type, param0__exact=ev.degree)
                elif ev.event_type == Event.EV_ASTRORISE:
                    q = Q(event_type__exact=Event.EV_RISE, param0__exact=ev.planet0, param1__exact=ev.degree + 1)
                elif ev.event_type == Event.EV_PLANET_HOUR:
                    q = Q(event_type__exact=ev.event_type, param0__exact=ev.planet0)
                elif ev.event_type == Event.EV_VOC:
                    q = Q(event_type__exact=ev.event_type, planet__exact=ev.planet0)
                elif ev.event_type == Event.EV_VIA_COMBUSTA:
                    q = Q(event_type__exact=ev.event_type, planet__exact=ev.planet0)
                elif ev.event_type == Event.EV_DEGREE_PASS:
                    q = Q(event_type__exact=ev.event_type, param0__exact=ev.degree)
                elif ev.event_type == Event.EV_RETROGRADE:
                    q = Q(event_type__exact=ev.event_type, param0__exact=ev.planet0)

                text_list = Text.objects.filter(q).values_list('message', flat=True)
                if text_list:
                    self.message = text_list[0]

class TextMoonMoveView(BaseView):
    def gather_events(self):
        self.template_name = 'm/text_moon_move.html'
        try:
            event0 = EventSelector.get_event(self.event0)[0]
        except IndexError:
            return
        try:
            event1 = EventSelector.get_event(self.event1)[0]
        except IndexError:
            return
        planet0 = event0.planet1
        planet1 = event1.planet1
        if event1.event_type == Event.EV_SIGN_ENTER:
            planet0 = 255
            planet1 = Event.SE_MOON
        elif event0.event_type == Event.EV_SIGN_ENTER:
            planet0 = Event.SE_MOON
        q = Q(event_type__exact=Event.EV_MOON_MOVE, param0__exact=planet0, param1__exact=planet1)
        self.title = [event0, event1]
        text_list = Text.objects.filter(q).values_list('message', flat=True)
        if text_list:
            self.message = text_list[0]

class SettingsView(BaseView):
    def render(self, request, profile):
        if request.method == 'POST':
            form = SettingsForm(request.POST, instance=profile)
            if request.user.is_authenticated():
                form.save()
            else:
                new_profile = form.save(commit=False)
                request.session['city_id'] = new_profile.location.pk
            return HttpResponseRedirect('../summary/')

        form = SettingsForm(initial = {'location': profile.location.pk})
        params = {
                  'form': form,
                  'user': request.user,
                  'session': request.session,
                  }
        c = RequestContext(request, params)
        return render_to_response('m/settings.html', context_instance=c)
