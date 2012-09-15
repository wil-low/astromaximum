from django.template import RequestContext
from django.http import HttpResponseRedirect, HttpResponse
from django.db.models import Q
from django.shortcuts import render_to_response
from django.contrib.auth import authenticate, login, logout
import datetime
import simplejson as json
from amax.apps import J2MEApp
from amax.models import Location, Country, State, Event
from forms import DemoForm

def home_redirect(request):
    return HttpResponseRedirect('home')

def main(request, page):
    #request.LANGUAGE_CODE = lang
    year = 2012
    c = RequestContext(request, {
                                 'user': request.user,
                                 'app': J2MEApp(),
                                 })
    return render_to_response('desktop/' + page + '.html', context_instance=c)

def citylist(request):
    country_list = Country.objects.all()
    country_letters = []
    letter = ' '
    last_letter_list = []
    country_locations = []
    for country in country_list:
        country_locations.append((country.id, country.name, Location.objects.filter(country=country)))
        if letter == country.name[0]:
            last_letter_list.append (country)
        else:
            letter = country.name[0]
            country_letters.append(last_letter_list)
            last_letter_list = [country]
    if len(last_letter_list) > 1:
        country_letters.append(last_letter_list)

    #import pdb; pdb.set_trace()
    c = RequestContext(request, {
                                 'user': request.user,
                                 'app': J2MEApp(),
                                 'country_letters': country_letters,
                                 'country_locations': country_locations,
                                 })
    return render_to_response('desktop/citylist.html', context_instance=c)

def citylist_sunrise(request, location_id, year=None):
    app = J2MEApp()
    if year is None:
        year = app.year
    else:
        year = int(year)
    location = Location.objects.select_related().get(id=int(location_id))
    #Event.set_tzinfo(location)
    #now = Event.fromutc(datetime.datetime.utcnow())
    now = datetime.datetime.utcnow().replace(year=year)
    sunrise_list = Event.objects.filter(year__exact=year, event_type__exact=Event.EV_RISE, planet0__exact=Event.SE_SUN, datetime0__lt=now).\
        order_by('-datetime0')
    sunrise = None
    if sunrise_list:
        sunrise = sunrise_list[0]
    year_navigation = [2013, 2012, 2011, 2010]
    c = RequestContext(request, {
                                 'year_navigation': year_navigation,
                                 'user': request.user,
                                 'app': app,
                                 'location': location,
                                 'year': year,
                                 'sunrise': sunrise,
                                 'now': now,
                                 })
    return render_to_response('desktop/citylist_sunrise.html', context_instance=c)

def login_view(request):
    username = request.POST['login']
    password = request.POST['pass']
    user = authenticate(username=username, password=password)
    if user is not None:
        if user.is_active:
            login(request, user)
            # Redirect to a success page.
        else:
            pass
            # Return a 'disabled account' error message
    else:
        pass
        # Return an 'invalid login' error message.    
    return HttpResponseRedirect('home')

def logout_view(request):
    logout(request)
    return HttpResponseRedirect('home')

def dl(request, mode=None, country_id=0, state_id=0, year=0):
    if mode is None:
        c = RequestContext(request, {
                                     'user': request.user,
                                     'app': J2MEApp(),
                                     })
        return render_to_response('desktop/dl.html', context_instance=c)
    else:
        #import pdb; pdb.set_trace()
        obj = []
        obj = None
        if mode == '0':
            obj = list(Country.objects.all().values('id', 'name'))
        elif mode == '1':
            obj = [{'id': 0, 'name': 'All states'}]
            obj.extend(list(State.objects.filter(country__id__exact=country_id).values('id', 'name')))
        elif mode == '2':
            q = Q(country__id=country_id)
            if state_id != '0':
                q = q & Q(state__id=state_id)
            obj = list(Location.objects.filter(q).values('id', 'name'))
        response = HttpResponse()
        response['Content-Type'] = 'application/json;charset=UTF-8'
        response['Cache-Control'] = 'no-cache'
        response.write(json.dumps({'content': obj}, separators=(',',':')))
        return response

def geo(request):
    if request.POST['Action']:
        pass
    c = RequestContext(request, {
                                 'user': request.user,
                                 'app': J2MEApp(),
                                 })
    return render_to_response('desktop/geo.html', context_instance=c)

def demo(request):
    if request.POST:
        form = DemoForm(request.POST)
        # Validate the form: the captcha field will automatically
        # check the input
        human = False
        if form.is_valid():
            human = True
    else:
        form = DemoForm()

    c = RequestContext(request, {
                                 'form': form,
                                 'user': request.user,
                                 'app': J2MEApp(),
                                 })
    return render_to_response('desktop/demo.html', context_instance=c)
