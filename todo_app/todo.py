from datetime import datetime
import tkinter as tk

class Item() :
    def __init__(self, tit: str, d: str, dedline: datetime, flrs: list[str]) -> None:
        self.title = tit
        self.desc = d
        self.posted = datetime.now()
        self.deadline = dedline
        self.flairs = flrs

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
    
    @property
    def flairs(self) -> list[str]:
        return self._flairs
    
    @flairs.setter
    def flairs(self, new: list[str]) -> None:
        self._flairs = new

    def addFlair(self, ele: str) -> None:
        self.flairs.append(ele)
    
    def __str__(self) -> str:
        return f"Title: {self.title}\nDescription: {self._desc}\nDeadline: {self.deadline}\nFlairs: {self.flairs}\nPosted: {self.posted}"

def main():
    root = tk.Tk()
    root.title("TO DO")


    root.mainloop()

if __name__ == "__main__":
    main()