from django.template import RequestContext
from django.http import HttpResponseRedirect
from django.shortcuts import render_to_response
from django.contrib.auth import authenticate, login, logout
from amax.apps import J2MEApp
from amax.models import Location, Country

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

def citylist_sunrise(request, id):
    c = RequestContext(request, {
                                 'user': request.user,
                                 'app': J2MEApp(),
                                 'id': id,
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