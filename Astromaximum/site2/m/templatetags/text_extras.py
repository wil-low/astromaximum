from django import template
from django.utils.safestring import mark_safe
from django.utils.translation import ugettext as _
from amax.models import Event

register = template.Library()

DATE_FORMAT='%H:%M %a %d %b'

def time_range_str(event):
    return '%s - %s' % (Event.fromutc(event.datetime0).strftime(DATE_FORMAT),
                        Event.fromutc(event.datetime1).strftime(DATE_FORMAT))

def time_tithi_str(event):
    return '%s - %s' % (Event.fromutc(event.datetime0).strftime(DATE_FORMAT),
                        Event.fromutc(event.datetime1).strftime(DATE_FORMAT))

def time_moment_str(event):
    return Event.fromutc(event.datetime0).strftime(DATE_FORMAT)

@register.filter
def title(ev):
    result = None
    if ev.event_type == Event.EV_TITHI:
        result = '%s<br/>%s%s' % (time_tithi_str(ev), _('Tithi #'), ev.degree)
    elif ev.event_type == Event.EV_ASP_EXACT:
        result = '%s<br/>%s-%s&deg;-%s %s' % (time_moment_str(ev),
                                              Event.PLANET[ev.planet0],
                                              ev.degree,
                                              Event.PLANET[ev.planet1],
                                              'duration')
    elif ev.event_type == Event.EV_SIGN_ENTER:
        result = '%s<br/>%s %s %s' % (time_range_str(ev),
                                      Event.PLANET[ev.planet0],
                                      _('in'),
                                      Event.CONSTELL[ev.degree])
    elif ev.event_type == Event.EV_ASTRORISE:
        result = '%s<br/>%s %s %s' % (time_moment_str(ev),
                                   _('(-40/+28 min)'),
                                   Event.PLANET[ev.planet0],
                                   Event.RISE_SET_STR[ev.degree])
    elif ev.event_type == Event.EV_PLANET_HOUR:
        result = '%s<br/>%s %s' % (time_range_str(ev), _('Hour of'), Event.PLANET[ev.planet0])
    elif ev.event_type == Event.EV_VOC:
        result = '%s<br/>%s' % (time_range_str(ev), _('Moon Void of Course'))
    elif ev.event_type == Event.EV_VIA_COMBUSTA:
        result = '%s<br/>%s' % (time_range_str(ev), _('Via Combusta'))
    elif ev.event_type == Event.EV_DEGREE_PASS:
        result = '%s<br/>%s %s&deg;%s' % (time_range_str(ev), Event.PLANET[ev.planet0], ev.degree_number(), ev.degree_zodiac())
    elif ev.event_type == Event.EV_RETROGRADE:
        result = '%s<br/>%s %s' % (time_range_str(ev), _('Retrograde'), Event.PLANET[ev.planet0])
    return mark_safe(result)

@register.filter
def title_move(event_list):
    if event_list[0].event_type == Event.EV_SIGN_ENTER:
        start = Event.PLANET[Event.SE_MOON]
    else:
        start = Event.PLANET[event_list[0].planet1]
    if event_list[1].event_type == Event.EV_SIGN_ENTER:
        finish = 'VOC'
    else:
        finish = Event.PLANET[event_list[1].planet1]
    return mark_safe('%s - %s<br/>%s &gt; %s' % (time_moment_str(event_list[0]),
                                                 time_moment_str(event_list[1]),
                                                 start,
                                                 finish))
