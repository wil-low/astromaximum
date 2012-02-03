from django import forms
from amax.models import UserProfile

class SettingsForm(forms.ModelForm):
    class Meta:
        model = UserProfile
        exclude = ('user',)
