from django.template import RequestContext
from django.shortcuts import render_to_response
from django.db.models import Q
from amax.models import Event
import datetime
import amax.datafile

def summary(request, year, month, day):
    "prints home page"
    current_date = datetime.datetime(int(year), int(month), int(day))
    prev_date = (current_date + datetime.timedelta(days=-1))
    next_date = (current_date + datetime.timedelta(days=1))
#    event_list = Event.objects.filter(datetime0__gte=current_date).filter(datetime0__lt=next_date).order_by('datetime0')
    event_list = {}
    event_list['vocs'] = amax.datafile.get_event_on_period(current_date, next_date, Event.EV_VOC, Event.SE_MOON)
    event_list['vc'] = amax.datafile.get_event_on_period(current_date, next_date, Event.EV_VIA_COMBUSTA, Event.SE_MOON)
    
    q = (Q(planet0__exact=Event.SE_SUN) | Q(planet0__exact=Event.SE_MOON)) \
         & (Q(event_type__exact=Event.EV_RISE) | Q(event_type__exact=Event.EV_SET)) 
    event_list['rise_set'] = amax.datafile.get_event_on_period_q(current_date, next_date, q)
    
    event_list['sun_degree'] = amax.datafile.get_event_on_period(current_date, next_date, Event.EV_DEGREE_PASS, Event.SE_SUN)
    event_list['moon_sign'] = amax.datafile.get_event_on_period(current_date, next_date, Event.EV_SIGN_ENTER, Event.SE_MOON)

    event_list['sun_day'] = amax.datafile.get_event_on_period(current_date, next_date, Event.EV_RISE, Event.SE_SUN)
    event_list['moon_day'] = amax.datafile.get_event_on_period(current_date, next_date, Event.EV_RISE, Event.SE_MOON)

    params = {
              'current_date': current_date.strftime("%Y-%m-%d"),
              'prev_date': prev_date.strftime("%Y-%m-%d"),
              'next_date': next_date.strftime("%Y-%m-%d"),
              'event_list': event_list,
              }
    c = RequestContext(request, params)
    #raise Exception(c)
    return render_to_response('summary.html', context_instance=c)
