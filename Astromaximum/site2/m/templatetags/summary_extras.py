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
def vc(event):
    result = ''
    if event:
        s = '%s<br/>%s'  % (event.time0(), event.time1())
        s = decorate(event, s)
        result = '<a href="../text/e%s">VC<br/>%s</a>' % (event.pk, s)
    return mark_safe(result)

@register.filter
def voc(event):
    result = ''
    if event:
        s = '%s<br/>%s'  % (event.time0(), event.time1())
        s = decorate(event, s)
        result = '<a href="../text/e%s/">VOC<br/>%s</a>' % (event.pk, s)
    return mark_safe(result)

@register.filter
def sun_degree(event):
    result = ''
    if event:
        s = '%02d&deg;%s<br/>%s' % (event.degree_number(), event.degree_zodiac(), event.time0())
        s = decorate(event, s)
        result = '<a href="../text/e%s/">%s<br/>%s</a>' % (event.pk, Event.PLANET[event.planet0], s)
    return mark_safe(result)

@register.filter
def moon_sign(event):
    result = ''
    if event:
        s = '%s<br/>%s' % (Event.CONSTELL[event.get_degree()], event.time0())
        s = decorate(event, s)
        result = '<a href="../text/e%s/">%s<br/>%s</a>' % (event.pk, Event.PLANET[event.planet0], s)
    return mark_safe(result)

@register.filter
def tithi(event):
    result = ''
    if event:
        s = '%s<br/>%s'  % (event.get_degree(), event.time0())
        s = decorate(event, s)
        result = '<a href="../tithi/">Tithi<br/>%s</a>' % s
    return mark_safe(result)

@register.filter
def moon_phase(event):
    result = ''
    if event:
        s = '%s<br/>%s'  % (event.get_degree(), event.time0())
        s = decorate(event, s)
        result = '<a href="../tithi/"><img src="/i/phases/ph50-%02d.png"/></a>' % event.get_degree()
    return mark_safe(result)

@register.filter
def hour(event):
    result = ''
    if event:
        result = '<a href="../hour/">%s<br/>%s<br/>%s</a>' % (event.time0(), Event.PLANET[event.planet0], event.time1())
    return mark_safe(result)

@register.filter
def rise_set(rise_set_list):
    result = '&nbsp;'.join(map(lambda planet: Event.PLANET[planet], rise_set_list))
    return mark_safe(result)

@register.filter
def retrograde(retrograde_list):
    result = 'R:&nbsp;'
    result += '&nbsp;'.join(map(lambda item: Event.PLANET[item.planet0], retrograde_list))
    return mark_safe(result)
