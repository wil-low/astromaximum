from django.template import RequestContext
from django.shortcuts import render_to_response
from django.conf import settings
import datetime
from eventselector import EventSelector
from amax.models import Event

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
        self.event_list = {}
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
        self.template_name = 'm/summary.html'
        self.event_list['vocs'] = self.select_single_event(self.es.get_vocs())

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

def event_text(request, year, month, day, event_id):
    ev = EventSelector.get_event(event_id)
    if ev:
        event_text = EventSelector.get_event_text(ev)
    params = {
              'event_text': event_text,
              }
    c = RequestContext(request, params)
    return render_to_response('m/text.html', context_instance=c)
