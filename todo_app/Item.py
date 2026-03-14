from datetime import datetime
import pandas as pd
from enum import Enum

class Priority(Enum):
    LOW = "LOW"
    MEDIUM = "MEDIUM"
    HIGH = "HIGH"



class Item() :
    def __init__(self, prio: Priority, tit: str, d: str, deadline: datetime) -> None:
        self.priority = prio
        self.title = tit
        self.desc = d
        self.posted = datetime.now()
        self.deadline = deadline


    @property
    def title(self) -> str:
        return self._title

    @title.setter
    def title(self, value: str) -> None:
        self._title = value

    @property
    def desc(self) -> str:
        return self._desc
    
    @desc.setter
    def desc(self, new : str) -> None:
        self._desc = new

    @property
    def deadline(self) -> datetime:
        return self._deadline
    
    @deadline.setter
    def deadline(self, newDate: datetime) -> None:
        self._deadline = newDate
    
    def __str__(self) -> str:
        return f"Title: {self.title}\nDescription: {self._desc}\nDeadline: {self.deadline}\nPosted: {self.posted}"

class ItemManager():
    def __init__(self, path: str):
        self.path = path
        try :
            self.items = pd.read_csv(path)
            print("read exisiting")
        except pd.errors.EmptyDataError: 
            print("creating...")
            self.items = pd.DataFrame(columns=["priority", "title", "description", "deadline", "posted"])

        print(self.items)

    def addItem(self, item : Item) -> None:
        self.items.loc[len(self.items)] = [item.priority, item.title, item.desc, item.deadline, item.posted]
    
    def saveToCsv(self, path = None) -> None:    
        if path == None:
            print("path == none")
            self.items.to_csv(self.path, header = False)
        else:
            self.items.to_csv(path, header = False)
        