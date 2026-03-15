from datetime import datetime
import pandas as pd
from enum import Enum

class Priority(Enum):
    LOW = "LOW"
    MEDIUM = "MEDIUM"
    HIGH = "HIGH"



class Item() :
    def __init__(self, id: int, prio: Priority, tit: str, d: str, deadline: datetime, posted = datetime.now()) -> None:
        self.priority = prio
        self.title = tit
        self.desc = d
        self.posted = posted
        self.deadline = deadline
        self.id = id


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
            self.items = pd.read_csv(path, index_col = 0)
            print("read existing")
        except pd.errors.EmptyDataError: 
            print("creating...")
            self.items = pd.DataFrame(columns=["id", "priority", "title", "description", "deadline", "posted"])

        

    def addItem(self, item : Item) -> None:
        self.items.loc[len(self.items)] = [item.id, item.priority, item.title, item.desc, item.deadline, item.posted]
    
    def saveToCsv(self, path = None) -> None:    
        if path == None:
            self.items.to_csv(self.path)
        else:
            self.items.to_csv(path)
    
    def removeItem(self, item : Item) -> None:
        self.items.drop(self.items['id'] == item.id)