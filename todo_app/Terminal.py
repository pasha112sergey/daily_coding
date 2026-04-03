from textual.app import App, ComposeResult
from textual.widgets import Footer, Header, Button, Static, Label
from textual.widget import Widget
from textual.containers import VerticalScroll
from Item import Item, ItemManager, Priority
from datetime import datetime
from AddItemScreen import AddItemScreen
from textual import on
import pandas as pd
from textual.screen import Screen
from datetime import timedelta
from ItemScreen import ItemScreen

# class ItemPanel():
#     def __init__(self, screen : Screen | None) -> None:
#         self.screen = screen

def formatTimestamp(time : datetime, option: str) -> str:
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

class ItemPreview(Label):
    def __init__(self, item: Item | None = None, **kwargs):
        super().__init__(**kwargs)
        self.item = item
        print("Initialization complete!")

    def compose(self) -> ComposeResult:
        if self.item:
            title = f"{self.item.title} {formatTimestamp(self.item.deadline, "deadline")}" 
            yield Static(classes=f"prio-color-{self.item.priority.lower()} prio")
            yield Static(f"{title}", classes="item-text")
        else:
            yield Static("placeholder")
    
    def on_click(self) :
        self.app.push_screen(ItemScreen(item = self.item))

    
# Urgent
class Urgent(VerticalScroll):
    BORDER_TITLE = "Due Today"
    def __init__(self, scr : Screen | None = None, **kwargs) -> None:
        VerticalScroll.__init__(self, **kwargs)
        # ItemPanel.__init__(self, screen=scr)

    def urgentFilter(self, items):
        func = lambda x : x.deadline is not None and datetime.now() < x.deadline < datetime.now() + timedelta(days = 7)
        return list(filter(func, items))
    
    def compose(self) -> ComposeResult:
        items = [Item(**row) for row in itemManager.itemsDf.to_dict(orient="records")]
        items = self.urgentFilter(items)

        with VerticalScroll(): 
            for item in items:
                yield ItemPreview(item = item)

# Reminders
class Reminders(VerticalScroll):

    BORDER_TITLE = "Reminders"

    def __init__(self, scr : Screen | None = None, **kwargs):
        VerticalScroll.__init__(self, **kwargs)
        # ItemPanel.__init__(self, screen=super())

    def compose(self) -> ComposeResult:
        yield SortButtons("")
        with VerticalScroll(): 
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()

# all items (left view)
class AllItems(VerticalScroll):
    BORDER_TITLE = "All Items"
    def __init__(self, scr : Screen | None = None, **kwargs):
        VerticalScroll.__init__(self, **kwargs)
        # ItemPanel.__init__(self, screen=scr)

    def compose(self) -> ComposeResult:
        global itemManager
        items = [Item(**row) for row in itemManager.itemsDf.to_dict(orient="records")]

        yield SortButtons("")
        with VerticalScroll(): 
            for item in items:
                yield ItemPreview(item)
        

class SortButtons(Static):
    def compose(self) -> ComposeResult:
        yield Button("Oldest", flat = True)
        yield Button("Newest", flat = True)
        yield Button("Priority", flat = True)


# App is the base class for all textual apps
class TodoApp(App):
    CSS_PATH = "Terminal.tcss"
    BINDINGS = [("a", "addItem", "Add Item")]

    def __init__(self, itemManager, **kwargs):
        super().__init__(**kwargs)
        self.itemManager = itemManager

    def compose(self) -> ComposeResult:
        yield Header()
        yield AllItems(classes="static", id="all-items")
        yield Urgent(classes="right-side")
        yield Reminders(classes="right-side")
        yield Footer()
        
    """
    Pulls up add Item screen
    """
    def action_addItem(self) -> None:
        self.push_screen(AddItemScreen(self.itemManager, self))
    

if __name__ == "__main__":
    DB_PATH = "~/.todo.csv"
    global itemManager 

    itemManager = ItemManager(DB_PATH)
    app = TodoApp(itemManager)
    app.run()
    itemManager.saveToCsv()
    print("Done!")


