from django.template import RequestContext
from django.shortcuts import render_to_response
from django.conf import settings
from django.contrib.auth.decorators import login_required
import datetime
from eventselector import EventSelector
from amax.models import Event, Text

def call_view_today(request):
    now = datetime.datetime.now()
    return call_view(request, now.year, now.month, now.day, now)

def call_view(request, year, month, day, view_class):
    v = view_class(year, month, day, datetime.datetime.utcnow())
    v.gather_events()
    return v.render(request)

class BaseView():
    def __init__(self, year, month, day, now):
        self.year = year
        self.month = month
        self.day = day
        self.now = now
        self.current_date = datetime.datetime(int(year), int(month), int(day))
        self.prev_date = (self.current_date + datetime.timedelta(days=-1))
        self.next_date = (self.current_date + datetime.timedelta(days=1))
        self.es = EventSelector(self.current_date.year, self.current_date, self.next_date, now)

    def gather_events(self):
        pass
    
    def render(self, request):
        params = {
                  'date_range': (self.current_date, self.next_date),
                  'prev_date': self.prev_date.strftime('%Y-%m-%d'),
                  'next_date': self.next_date.strftime('%Y-%m-%d'),
                  'now': self.now,
                  'event_list': self.event_list,
                  'settings': settings,
                  'page_name': request.path_info.split('/')[-1]
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


class RiseSetView(BaseView):
    def gather_events(self):
        self.event_list = []
        self.template_name = 'm/lists/rise_set.html'
        self.es.set_period(self.prev_date, self.next_date)
        for planet in range(Event.SE_SUN, Event.SE_PLUTO + 1):
            ev = self.es.get_rise_set(planet)
            if ev:
                self.event_list.append(ev)

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
        if text_list:
            text = text_list[0]
        params = {
                  'caption': caption,
                  'text': text,
                  }
    c = RequestContext(request, params)
    return render_to_response('m/text.html', context_instance=c)
