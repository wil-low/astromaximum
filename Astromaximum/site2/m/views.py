from django.template import RequestContext
from django.shortcuts import render_to_response
from django.conf import settings
import datetime
from eventselector import EventSelector

def call_view_today(request):
    now = datetime.datetime.now()
    return call_view(request, now.year, now.month, now.day, now)

def call_view(request, year, month, day, view_class):
    v = view_class(year, month, day, datetime.datetime.now())
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
        self.event_list = {}
        self.es = EventSelector(self.current_date.year, self.current_date, self.next_date)

    def gather_events(self):
        pass
    
    def render(self, request):
        params = {
                  'current_date': self.current_date.strftime("%Y-%m-%d"),
                  'prev_date': self.prev_date.strftime("%Y-%m-%d"),
                  'next_date': self.next_date.strftime("%Y-%m-%d"),
                  'now': self.now,
                  'event_list': self.event_list,
                  'settings': settings,
                  }
        c = RequestContext(request, params)
        return render_to_response(self.template_name, context_instance = c)

class SummaryView(BaseView):
    def gather_events(self):
        self.template_name = 'm/summary.html'
        self.event_list['vocs'] = self.es.get_vocs()
        self.event_list['vc'] = self.es.get_vc()
        self.event_list['sun_rise'] = self.es.get_sun_rise()
        self.event_list['sun_set'] = self.es.get_sun_set()
        self.event_list['moon_rise'] = self.es.get_moon_rise()
        self.event_list['moon_set'] = self.es.get_moon_set()
        self.event_list['sun_degree'] = self.es.get_sun_degree()
        self.event_list['moon_sign'] = self.es.get_moon_sign()
        self.event_list['tithi'] = self.es.get_tithi()
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

def event_text(request, year, month, day, event_id):
    ev = EventSelector.get_event(event_id)
    if ev:
        event_text = EventSelector.get_event_text(ev)
    params = {
              'event_text': event_text,
              }
    c = RequestContext(request, params)
    return render_to_response('m/text.html', context_instance=c)
