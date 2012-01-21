from django.template import RequestContext
from django.shortcuts import render_to_response
from django.db.models import Q
from amax.models import Event
import datetime
from amax.datafile import EventSelector

def today_summary(request):
    today = datetime.datetime.now()
    return summary(request, today.year, today.month, today.day)

def summary(request, year, month, day):
    "prints home page"
    current_date = datetime.datetime(int(year), int(month), int(day))
    prev_date = (current_date + datetime.timedelta(days=-1))
    next_date = (current_date + datetime.timedelta(days=1))

    es = EventSelector(int(year), current_date, next_date)
    event_list = {}
    event_list['vocs'] = es.get_event_on_period(Event.EV_VOC, Event.SE_MOON)
    event_list['vc'] = es.get_event_on_period(Event.EV_VIA_COMBUSTA, Event.SE_MOON)
    
    q_sun = Q(planet0__exact=Event.SE_SUN)
    q_moon = Q(planet0__exact=Event.SE_MOON)
    q_rise = Q(event_type__exact=Event.EV_RISE)
    q_set = Q(event_type__exact=Event.EV_SET)

    event_list['sun_rise'] = es.get_event_on_period_q(q_sun & q_rise)
    event_list['sun_set'] = es.get_event_on_period_q(q_sun & q_set)
    event_list['moon_rise'] = es.get_event_on_period_q(q_moon & q_rise)
    event_list['moon_set'] = es.get_event_on_period_q(q_moon & q_set)
    
    event_list['sun_degree'] = es.get_event_on_period(Event.EV_DEGREE_PASS, Event.SE_SUN)
    event_list['moon_sign'] = es.get_event_on_period(Event.EV_SIGN_ENTER, Event.SE_MOON)

    event_list['sun_day'] = es.get_event_on_period(Event.EV_RISE, Event.SE_SUN)
    event_list['moon_day'] = es.get_event_on_period(Event.EV_RISE, Event.SE_MOON)

    event_list['tithi'] = es.get_event_on_period(Event.EV_TITHI, Event.SE_MOON)

    # sun day with Navroz
    es.set_period(es.zeroJD(), es.finalJD())
    navroz_events = es.get_event_on_period(Event.EV_NAVROZ, Event.SE_SUN)
    navroz = navroz_events[1].date0
    sunrise = event_list['sun_rise'][0].date0
    if sunrise < navroz:
        navroz = navroz_events[0].date0
    pltDaySun = int((sunrise - navroz) / Event.SECINDAY + 0.5)
    if pltDaySun < 360:
        pltDaySun = pltDaySun % 30 + 1
    else:
        pltDaySun = -(pltDaySun - 359)
    sun_day_event = event_list['sun_rise'][0]
    sun_day_event.degree = pltDaySun
    event_list['sun_day'] = [sun_day_event,]

    #aspects
    es.set_period(prev_date, next_date)
    event_list['aspects'] = es.get_aspects_on_period(False)

    es.set_period((current_date + datetime.timedelta(days=-2)), (next_date + datetime.timedelta(days=+2)))
    event_list['moon_move'] = list(es.get_event_on_period(Event.EV_ASP_EXACT, Event.SE_MOON))

    es.set_period((current_date + datetime.timedelta(days=-2)), (next_date + datetime.timedelta(days=+4)))
    moon_sign_enter_events = list(es.get_event_on_period(Event.EV_SIGN_ENTER, Event.SE_MOON))

    event_list['moon_move'].extend(moon_sign_enter_events)
    event_list['moon_move'].sort(key=lambda event: event.date0)
    
    params = {
              'current_date': current_date.strftime("%Y-%m-%d"),
              'prev_date': prev_date.strftime("%Y-%m-%d"),
              'next_date': next_date.strftime("%Y-%m-%d"),
              'event_list': event_list,
              }
    c = RequestContext(request, params)
    return render_to_response('m/summary.html', context_instance=c)
