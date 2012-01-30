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
        result = '<a href="vc">VC<br/>%s</a>' % s
    return mark_safe(result)

@register.filter
def voc(event):
    result = ''
    if event:
        s = '%s<br/>%s'  % (event.time0(), event.time1())
        s = decorate(event, s)
        result = '<a href="voc">VOC<br/>%s</a>' % s
    return mark_safe(result)

@register.filter
def sun_degree(event):
    result = ''
    if event:
        s = '%02d&deg;%s<br/>%s' % (event.degree_number(), event.degree_zodiac(), event.time0())
        s = decorate(event, s)
        result = '<a href="sun_dgr">%s<br/>%s</a>' % (Event.PLANET[event.planet0], s)
    return mark_safe(result)

@register.filter
def moon_sign(event):
    result = ''
    if event:
        s = '%s<br/>%s' % (Event.CONSTELL[event.get_degree()], event.time0())
        s = decorate(event, s)
        result = '<a href="moon_sign">%s<br/>%s</a>' % (Event.PLANET[event.planet0], s)
    return mark_safe(result)

@register.filter
def tithi(event):
    result = ''
    if event:
        s = '%s<br/>%s'  % (event.get_degree(), event.time0())
        s = decorate(event, s)
        result = '<a href="tithi">Tithi<br/>%s</a>' % s
    return mark_safe(result)

@register.filter
def hour(event):
    result = ''
    if event:
        s = '%s<br/>%s'  % (event.get_degree(), event.time0())
        s = decorate(event, s)
        result = '<a href="tithi">Tithi<br/>%s</a>' % s
    return mark_safe(result)
