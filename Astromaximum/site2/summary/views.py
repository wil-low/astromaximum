from django.template import RequestContext
from django.shortcuts import render_to_response
import datetime

def main(request, year, month, day):
    "prints home page"
    date = datetime.datetime(int(year), int(month), int(day))
    current_date = date.strftime("%Y-%m-%d")
    prev_date = (date + datetime.timedelta(days=-1)).strftime("%Y-%m-%d")
    next_date = (date + datetime.timedelta(days=1)).strftime("%Y-%m-%d")
    params = {
              'current_date': current_date,
              'prev_date': prev_date,
              'next_date': next_date,
              }
    c = RequestContext(request, params)
    #raise Exception(c)
    return render_to_response('summary.html', context_instance=c)
