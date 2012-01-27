from amax.models import Event, Location, Text
from django.contrib import admin

class EventAdmin(admin.ModelAdmin):
    pass
    #fields = ['datetime0', 'planet0']
    #list_display = ('event_type', 'datetime0', 'datetime0', 'planet0', 'planet1')
    #date_hierarchy = 'datetime0'

admin.site.register(Event, EventAdmin)

class LocationAdmin(admin.ModelAdmin):
    pass
    #fields = ['datetime0', 'planet0']
    #list_display = ('event_type', 'datetime0', 'datetime0', 'planet0', 'planet1')
    #date_hierarchy = 'datetime0'

admin.site.register(Location, LocationAdmin)

class TextAdmin(admin.ModelAdmin):
    pass
    #fields = ['datetime0', 'planet0']
    #list_display = ('event_type', 'datetime0', 'datetime0', 'planet0', 'planet1')
    #date_hierarchy = 'datetime0'

admin.site.register(Text, TextAdmin)
