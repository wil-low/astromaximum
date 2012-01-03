class Event:
    def __init__(self, type, datetime):
        self.datetime = datetime
        self.type = type
 
    @staticmethod
    def date_to_string(date):
        "converts datetime to YYYY-MM-DD string"
        return "%04d-%02d-%02d" % (date.year, date.month, date.day)

    def __str__(self):
        "simple stub"
        return "I'm event %s (%s)" % (self.datetime, self.type)

