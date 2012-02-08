from django import template
from django.utils.safestring import mark_safe
from amax.models import Event

register = template.Library()

def decorate(event, s, now):
    now_pos = Event.date_between(now, event.datetime0, event.datetime1)
    if now_pos == 0:
        s = '<b>%s</b>' % s

#    if event.state == Event.STATE_GONE:
#        s = '<del>%s</del>' % s
#    elif event.state == Event.STATE_COMING:
#        s = '<i>%s</i>' % s
#    elif event.state == Event.STATE_ACTIVE:
#        s = '<b>%s</b>' % s
    return s

@register.filter
def aspect(event):
    result = ''
    if event:
        s = '%s %s %d&deg; %s' % (Event.fromutc(event.datetime0).strftime('%H:%M %d %b'), Event.PLANET[event.planet0], event.degree, Event.PLANET[event.planet1])
        #s = decorate(event, s)
        result = '<a href="../text/e%s/" class="aspects">%s</a>' % (event.id, s)
    return mark_safe(result)

@register.filter
def tithi(event):
    result = ''
    if event:
        s = '%d %s' % (event.degree, Event.fromutc(event.datetime0).strftime('%H:%M %d %b'))
        #s = decorate(event, s)
        result = '<a href="../text/e%s/" class="aspects">%s</a>' % (event.id, s)
    return mark_safe(result)

@register.filter
def moon_move_list(events, date_range):
    is_anchor_set = is_preceding_found = is_following_found = False
    previous = None
    output = ''
    for event in events:
        if previous:
            output += '</div><div><a class="move" href="../text/m%s-%s/"> &gt;&gt;</a></div>' % (previous.id, event.id)            
            output += "\n"
        if event.event_type == Event.EV_ASP_EXACT:
            s = '%d&deg; %s<br/>' % (event.degree, Event.PLANET[event.planet1])
        else:
            s = '%s<br/>' % (Event.CONSTELL[event.degree])
        s += Event.fromutc(event.datetime0).strftime('%H:%M')
        if Event.date_between(Event.fromutc(event.datetime0), date_range[0], date_range[1]) == 0:
            s = '<b>%s</b>' % s
            if not is_anchor_set:
                output += '<div id="start">'
                is_anchor_set = True
            else:
                output += '<div>'
        else:
            output += '<div>'
        output += '<a href="../text/e%s/">%s</a>' % (event.id, s)
        previous = event
    output += "</div>\n"
    return mark_safe(output)

@register.filter
def rise_set(event):
    result = ''
    if event:
        s = '%s %s %s' % (Event.PLANET[event.planet0], Event.EVENT_TYPE[event.event_type],
            Event.fromutc(event.datetime0).strftime('%H:%M %d %b'))
        result = '<a href="../text/e%s/" class="rise_set">%s</a>' % (event.id, s)
    return mark_safe(result)

@register.filter
def hour(event, now):
    result = ''
    if event:
        s = '%s %s' % (Event.fromutc(event.datetime0).strftime('%H:%M'), Event.PLANET[event.planet0])
        s = decorate(event, s, now)
        result = '<a href="../text/e%s/" class="hour">%s</a>' % (event.id, s)
    return mark_safe(result)
