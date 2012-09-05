from django import forms

class UploadDataFileForm(forms.Form):
    datafile  = forms.FileField(max_length=200)