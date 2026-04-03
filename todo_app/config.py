from datetime import datetime
import pandas as pd

def formatTimestamp(time : datetime, option: str = "") -> str:
    def convertWeekday(s):
        if s == 0:
            return "Monday"
        if s == 1:
            return "Tuesday"
        if s == 2:
            return "Wednesday"
        if s == 3:
            return "Thursday"
        if s == 4:
            return "Friday"
        if s == 5:
            return "Saturday"
        if s == 6:
            return "Sunday"
    
    if pd.isna(time):
        return ""
    if option == "deadline":
        return f"Due: [{convertWeekday(time.weekday())} - {time.month}/{time.day} @ {time.hour}:{time.minute}]"
    else:
        return f"[{time.weekday()} - {time.day} @ {time.hour}:{time.minute}]"
