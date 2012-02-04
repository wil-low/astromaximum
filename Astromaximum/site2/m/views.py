from django.template import RequestContext
from django.http import HttpResponseRedirect
from django.shortcuts import render_to_response
from django.conf import settings
from django.contrib.auth.decorators import login_required
import dateutil.tz
import datetime
from eventselector import EventSelector
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
    Event.tzinfo = dateutil.tz.gettz(profile.location.timezone)
    now = Event.fromutc(datetime.datetime.utcnow())
    return HttpResponseRedirect('%04d-%02d-%02d/summary/' % (now.year, now.month, now.day))

def call_view(request, year, month, day, view_class):
    profile = get_user_profile(request)
    Event.tzinfo = dateutil.tz.gettz(profile.location.timezone)
    v = view_class(year, month, day, datetime.datetime.utcnow(), profile.location.pk)
    v.gather_events()
    return v.render(request, profile)

class BaseView():
    def __init__(self, year, month, day, now, city_id):
        self.year = year
        self.month = month
        self.day = day
        self.now = now
        self.current_date = datetime.datetime(int(year), int(month), int(day), tzinfo=Event.tzinfo).astimezone(Event.utc_tz)
        self.prev_date = (self.current_date + datetime.timedelta(days=-1))
        self.next_date = (self.current_date + datetime.timedelta(days=1))
        self.es = EventSelector(self.current_date.year, self.current_date, self.next_date, now, city_id)

    def gather_events(self):
        pass
    
    def render(self, request, profile):
        params = {
                  'date_range': (self.current_date, self.next_date),
                  'prev_date': self.prev_date,#.strftime('%Y-%m-%d'),
                  'next_date': self.next_date,#.strftime('%Y-%m-%d'),
                  'now': self.now,
                  'event_list': self.event_list,
                  'settings': settings,
                  'page_name': request.path_info.split('/')[-2],
                  'user': request.user,
                  'location': profile.location,
                  }
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
        self.event_list['vocs'] = self.es.get_vocs()

        self.event_list['vc'] = self.select_single_event(self.es.get_vc())
        #self.event_list['vocs'][0].state = Event.STATE_ACTIVE
        self.event_list['sun_degree'] = self.select_single_event(self.es.get_sun_degree())
        self.event_list['moon_sign'] = self.select_single_event(self.es.get_moon_sign())
        self.event_list['tithi'] = self.select_single_event(self.es.get_tithi())
        self.event_list['planet_hour'] = self.select_single_event(self.es.get_planetary_hours())
#        self.event_list['planet_hour'] = self.es.get_planetary_hours()
    #    event_list['sun_day'] = 
    #    event_list['moon_day'] = 
        #aspects
        self.es.set_period(self.prev_date, self.next_date)
        self.event_list['aspects'] = self.es.get_aspects()
        self.event_list['moon_move'] = self.es.get_moon_move()
    
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

class PlanetHourView(BaseView):
    def gather_events(self):
        self.template_name = 'm/lists/planet_hour.html'
        self.event_list = self.es.get_planetary_hours()

class RiseSetView(BaseView):
    def gather_events(self):
        self.event_list = []
        self.template_name = 'm/lists/rise_set.html'
        self.es.set_period(self.current_date, self.next_date)
        self.event_list = self.es.get_rise_sets()

@login_required
def event_text(request, year, month, day, event_id):
    ev = EventSelector.get_event(event_id)[0]
    caption = text = ''
    text_list = []
    if ev:
        caption = ev
        if ev.event_type == Event.EV_TITHI:
            text_list = Text.objects.filter(event_type__exact=ev.event_type, param0__exact=ev.degree).\
                values_list('message', flat=True)
        elif ev.event_type == Event.EV_ASP_EXACT:
            aspect_goodness = Event.ASPECT[ev.degree][1]
            if ev.planet0 == Event.SE_MOON:
                text_list = Text.objects.filter(event_type__exact=Event.EV_ASP_EXACT_MOON, 
                    param0__exact=ev.planet1, param1__exact=aspect_goodness).\
                    values_list('message', flat=True)
            else:
                text_list = Text.objects.filter(event_type__exact=ev.event_type, 
                    param0__exact=ev.planet0, param1__exact=ev.planet1, param2__exact=aspect_goodness).\
                    values_list('message', flat=True)
        elif ev.event_type == Event.EV_SIGN_ENTER:
            text_list = Text.objects.filter(event_type__exact=ev.event_type, 
                param0__exact=ev.degree).values_list('message', flat=True)
        elif ev.event_type == Event.EV_ASTRORISE:
            text_list = Text.objects.filter(event_type__exact=Event.EV_RISE, 
                param0__exact=ev.planet0, param1__exact=1).values_list('message', flat=True)
        elif ev.event_type == Event.EV_ASTROSET:
            text_list = Text.objects.filter(event_type__exact=Event.EV_RISE, 
                param0__exact=ev.planet0, param1__exact=3).values_list('message', flat=True)
        if text_list:
            text = text_list[0]
        params = {
                  'caption': caption,
                  'text': text,
                  }
    c = RequestContext(request, params)
    return render_to_response('m/text.html', context_instance=c)

@login_required
def hour_text(request, year, month, day, planet):
    caption = text = ''
    text_list = Text.objects.filter(event_type__exact=Event.EV_PLANET_HOUR, param0__exact=planet).\
        values_list('message', flat=True)
    if text_list:
        text = text_list[0]
    params = {
              'caption': caption,
              'text': text,
              }
    c = RequestContext(request, params)
    return render_to_response('m/text.html', context_instance=c)

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
