from django import template
from django.utils.safestring import mark_safe
from amax.models import Event

register = template.Library()

def decorate(event, s):
    if event.state == Event.STATE_GONE:
        s = '<del>%s</del>' % s
    elif event.state == Event.STATE_COMING:
        s = '<i>%s</i>' % s
    elif event.state == Event.STATE_ACTIVE:
        s = '<b>%s</b>' % s
    return s

@register.filter
def aspect(event):
    result = ''
    if event:
        s = '%s %s %d&deg; %s' % (event.datetime0.strftime('%H:%M %d %b'), Event.PLANET[event.planet0], event.degree, Event.PLANET[event.planet1])
        s = decorate(event, s)
        result = '<a href="text/%s" class="aspects">%s</a>' % (event.id, s)
    return mark_safe(result)

@register.filter
def tithi(event):
    result = ''
    if event:
        s = '%d %s' % (event.degree, event.datetime0.strftime('%H:%M %d %b'))
        s = decorate(event, s)
        result = '<a href="text/%s" class="aspects">%s</a>' % (event.id, s)
    return mark_safe(result)

@register.filter
def moon_move_list(events, date_range):
    is_id_set = False
    output = "<ul>\n"
    previous = None
    for event in events:
        if previous:
            output += '<a href="text/%s-%s"> &gt;&gt;</a></li>' % (previous.id, event.id)            
            output += "\n"
        if event.event_type == Event.EV_ASP_EXACT:
            s = '%s %d&deg; ' % (Event.PLANET[event.planet1], event.degree)
        else:
            s = '%s ' % (Event.CONSTELL[event.degree])
        s += event.datetime0.strftime('%H:%M %d %b')
        if Event.date_between(event.datetime0, date_range[0], date_range[1]) == 0:
            s = '<b>%s</b>' % s
            if not is_id_set:
                output += '<li id="start">'
                is_id_set = True
            else:
                output += '<li>'
        else:
            output += '<li>'
        output += '<a href="text/%s">%s</a>' % (event.id, s)
        previous = event
    output += "</li>\n</ul>"
    return mark_safe(output)

@register.filter
def rise_set(event):
    result = ''
    if event:
        s = '%d %s' % (event.degree, event.datetime0.strftime('%H:%M %d %b'))
        s = decorate(event, s)
        result = '<a href="text/%s" class="aspects">%s</a>' % (event.id, s)
    return mark_safe(result)
