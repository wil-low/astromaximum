from django.template import RequestContext
from django.shortcuts import render_to_response
from datetime import *
from utils.amax import *

def main(request, datestr=None):
    "prints home page"
    if datestr is None:
        datestr = Event.date_to_string(datetime.today())
    c = RequestContext(request, {'date': datestr})
    #raise Exception(c)
    return render_to_response('summary.html', context_instance=c)
