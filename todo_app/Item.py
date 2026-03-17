from datetime import datetime
import pandas as pd
from enum import Enum

class Priority(Enum):
    LOW = "LOW"
    MEDIUM = "MEDIUM"
    HIGH = "HIGH"

datetime.now()

class Item() :
    def __init__(self, id: int, prio: Priority, tit: str, d: str, deadline= datetime.now(), posted = datetime.now()) -> None:
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
            self.itemsDf = pd.read_csv(path, index_col = 0, parse_dates=['deadline', 'posted'])
            print(self.itemsDf)

        except pd.errors.EmptyDataError: 
            print("creating...")
            self.itemsDf = pd.DataFrame(columns=["id", "priority", "title", "description", "deadline", "posted"])

    def addItem(self, item : Item) -> None:
        self.itemsDf.loc[len(self.itemsDf)] = [item.id, item.priority, item.title, item.desc, item.deadline, item.posted]
    
    def saveToCsv(self, path = None) -> None:    
        self.itemsDf[['deadline', 'posted']] = self.itemsDf[['deadline', 'posted']].map(lambda x: x.to_isostring())
        self.itemsDf.to_csv(self.path) if path == None else self.itemsDf.to_csv(path)
    
    def removeItem(self, item : Item) -> None:
        self.itemsDf.drop(self.itemsDf['id'] == item.id)