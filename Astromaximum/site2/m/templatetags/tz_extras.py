from django import template
from amax.models import Event

register = template.Library()

@register.filter
def tz(dtime):
    return Event.fromutc(dtime)
