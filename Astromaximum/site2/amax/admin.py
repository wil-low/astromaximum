from amax.models import Event
from amax.models import Location
from django.contrib import admin

class EventAdmin(admin.ModelAdmin):
    1
    #fields = ['datetime0', 'planet0']
    #list_display = ('event_type', 'datetime0', 'datetime0', 'planet0', 'planet1')
    #date_hierarchy = 'datetime0'

admin.site.register(Event, EventAdmin)

class LocationAdmin(admin.ModelAdmin):
    1
    #fields = ['datetime0', 'planet0']
    #list_display = ('event_type', 'datetime0', 'datetime0', 'planet0', 'planet1')
    #date_hierarchy = 'datetime0'

admin.site.register(Location, LocationAdmin)
