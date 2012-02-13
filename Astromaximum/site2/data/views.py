import tempfile, os, shutil
from django.template import RequestContext
from django.shortcuts import render_to_response
from amax.datafile import DataFile
from django.db import transaction
from amax.models import Event
from django.conf import settings
from forms import UploadDataFileForm
from django.contrib.auth.decorators import permission_required

@permission_required('events.can_add')
def upload_datafile(request):
    result = 0
    message = ''
    if request.method == 'POST':
        form = UploadDataFileForm(request.POST, request.FILES)
        if form.is_valid():
            (result, message) = process_uploaded_datafile(request.FILES['datafile'])
    else:
        form = UploadDataFileForm()
    c = RequestContext(request, {'form': form, 'result': result, 'message': message})
    return render_to_response('data/upload_datafile.html', context_instance=c)

def process_uploaded_datafile(f):
    if f.name.split('.')[-1] == 'comm':
        dest_dir = settings.COMMON_DATAFILE_ROOT
        is_common = True
    else:
        dest_dir = settings.LOCATION_DATAFILE_ROOT
        is_common = False
    tmp = tempfile.NamedTemporaryFile(delete=False)
    for chunk in f.chunks():
        tmp.write(chunk)
    tmp.close()
    if is_common:
        (result, msg, year) = common_generate(tmp.name)
        filename = dest_dir + f.name
        shutil.copyfile(tmp.name, filename)
    else:
        (result, msg, year) = location_generate(tmp.name)
        try:
            os.mkdir(dest_dir + str(year))
        except OSError:
            pass
        filename = dest_dir + str(year) + '/' + f.name
        shutil.copyfile(tmp.name, filename)
    os.unlink(tmp.name)
    return (1, filename + ': ' + msg)

def common_generate(filename):
    "loads common datafile"
    df = DataFile(filename, True)
    year = df.year

    @transaction.commit_on_success
    def insert():
        return df.read_sub_data(df.process_event)
    
    try:
        Event.objects.filter(year__exact=year, city_id__exact=None)[0]
        result = 1
        message = 'Cannot insert: ' + str(year) + ' year data exist'
    except(IndexError):
        event_count = insert()
        result = 0
        message = str(year) + ' inserted, event_count ' + str(event_count)
    df.close()
    return (result, message, year)

def common_clean(request, year):
    "shows common events"

    @transaction.commit_on_success
    def delete(year):
        Event.objects.filter(year__exact=year).delete()
    
    delete(year)
    c = RequestContext(request, {'message': 'Year ' + str(year) + ' deleted'})
    return render_to_response('m/common.html', context_instance=c)

def location_generate(filename):
    "loads location datafile"
    df = DataFile(filename, False)
    year = df.year
    city_id = df.city_id
    
    @transaction.commit_on_success
    def insert():
        event_count = df.read_sub_data(df.process_event)
        #event_count += df.calc_planet_hours(df.process_event)
        return event_count

    @transaction.commit_on_success
    def insert_hours():
        event_count = df.calc_planet_hours(df.process_event)
        return event_count

    @transaction.commit_on_success
    def insert_mc_ic():
        event_count = df.calc_mc_ic(df.process_mc_ic)
        return event_count

    try:
        Event.objects.filter(year__exact=year, city_id__exact=city_id)[0]
        result = 1
        message = 'Cannot insert: ' + str(year) + ' year data exist'
    except(IndexError):
        event_count = insert()
        event_count += insert_hours()
        event_count += insert_mc_ic()
        result = 0
        message = str(year) + ' inserted, event_count ' + str(event_count)
    df.close()
    return (result, message, year)

