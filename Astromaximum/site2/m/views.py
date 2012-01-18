from django.template import RequestContext
from django.shortcuts import render_to_response
from django.db.models import Q
from amax.models import Event
import datetime
from amax.datafile import get_event_on_period, get_event_on_period_q

def summary(request, year, month, day):
    "prints home page"
    current_date = datetime.datetime(int(year), int(month), int(day))
    prev_date = (current_date + datetime.timedelta(days=-1))
    next_date = (current_date + datetime.timedelta(days=1))

    event_list = {}
    event_list['vocs'] = get_event_on_period(current_date, next_date, Event.EV_VOC, Event.SE_MOON)
    event_list['vc'] = get_event_on_period(current_date, next_date, Event.EV_VIA_COMBUSTA, Event.SE_MOON)
    
    q = (Q(planet0__exact=Event.SE_SUN) | Q(planet0__exact=Event.SE_MOON)) \
         & (Q(event_type__exact=Event.EV_RISE) | Q(event_type__exact=Event.EV_SET)) 
    event_list['rise_set'] = get_event_on_period_q(current_date, next_date, q)
    
    event_list['sun_degree'] = get_event_on_period(current_date, next_date, Event.EV_DEGREE_PASS, Event.SE_SUN)
    event_list['moon_sign'] = get_event_on_period(current_date, next_date, Event.EV_SIGN_ENTER, Event.SE_MOON)

    event_list['sun_day'] = get_event_on_period(current_date, next_date, Event.EV_RISE, Event.SE_SUN)
    event_list['moon_day'] = get_event_on_period(current_date, next_date, Event.EV_RISE, Event.SE_MOON)

    event_list['tithi'] = get_event_on_period(current_date, next_date, Event.EV_TITHI, Event.SE_MOON)

    date_begin = (current_date + datetime.timedelta(days=0))
    date_end = (current_date + datetime.timedelta(days=365))

    params = {
              'current_date': current_date.strftime("%Y-%m-%d"),
              'prev_date': prev_date.strftime("%Y-%m-%d"),
              'next_date': next_date.strftime("%Y-%m-%d"),
              'event_list': event_list,
              }
    c = RequestContext(request, params)
    return render_to_response('m/summary.html', context_instance=c)
