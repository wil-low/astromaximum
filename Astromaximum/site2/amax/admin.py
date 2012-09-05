from amax.models import Event, Location, Text, UserProfile, Country, State
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

class UserProfileAdmin(admin.ModelAdmin):
    pass
    #fields = ['datetime0', 'planet0']
    #list_display = ('event_type', 'datetime0', 'datetime0', 'planet0', 'planet1')
    #date_hierarchy = 'datetime0'

admin.site.register(UserProfile, UserProfileAdmin)

class CountryAdmin(admin.ModelAdmin):
    pass
    #list_display = ('id', 'name')
    #fields = ['datetime0', 'planet0']
    #date_hierarchy = 'datetime0'

admin.site.register(Country, CountryAdmin)

class StateAdmin(admin.ModelAdmin):
    pass
    #list_display = ('id', 'country', 'name')
    #fields = ['datetime0', 'planet0']
    #date_hierarchy = 'datetime0'

admin.site.register(State, StateAdmin)
