from django.template import RequestContext
from django.shortcuts import render_to_response
from django.conf import settings
import datetime
from eventselector import EventSelector

def today_summary(request):
    today = datetime.datetime.now()
    return summary(request, today.year, today.month, today.day)

def summary(request, year, month, day):
    "prints home page"
    current_date = datetime.datetime(int(year), int(month), int(day))
    prev_date = (current_date + datetime.timedelta(days=-1))
    next_date = (current_date + datetime.timedelta(days=1))

    event_list = {}

    es = EventSelector(int(year), current_date, next_date)
    event_list['vocs'] = es.get_vocs()
    event_list['vc'] = es.get_vc()
    
    event_list['sun_rise'] = es.get_sun_rise()
    event_list['sun_set'] = es.get_sun_set()
    event_list['moon_rise'] = es.get_moon_rise()
    event_list['moon_set'] = es.get_moon_set()
    
    event_list['sun_degree'] = es.get_sun_degree()
    event_list['moon_sign'] = es.get_moon_sign()

    event_list['tithi'] = es.get_tithi()

#    event_list['sun_day'] = 
#    event_list['moon_day'] = 
    
    #aspects
    es.set_period(prev_date, next_date)
    event_list['aspects'] = es.get_aspects()

    event_list['moon_move'] = es.get_moon_move()
    
    params = {
              'current_date': current_date.strftime("%Y-%m-%d"),
              'prev_date': prev_date.strftime("%Y-%m-%d"),
              'next_date': next_date.strftime("%Y-%m-%d"),
              'event_list': event_list,
              'settings': settings,
              }
    c = RequestContext(request, params)
    return render_to_response('m/summary.html', context_instance=c)
