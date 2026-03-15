from textual.app import App, ComposeResult
from textual.widgets import Footer, Header, Button, Static, Label
from textual.containers import VerticalScroll, VerticalScroll
from Item import Item, ItemManager, Priority
from datetime import datetime

DB_PATH = "~/.todo.csv"

class ItemPreview(Label):
    def __init__(self, item: Item | None = None, **kwargs):
        super().__init__(**kwargs)
        self.item = item
        print("Initialization complete!")

    def compose(self) -> ComposeResult:
        if self.item:
            yield Static(self.item.title)
        else:
            yield Static("Placeholder")
    


# Urgent
class Urgent(VerticalScroll):
    BORDER_TITLE = "Due Today"
    def compose(self) -> ComposeResult:
        yield TimeSelection("")
        with VerticalScroll(): 
            

# Reminders
class Reminders(VerticalScroll):
    BORDER_TITLE = "Reminders"
    def compose(self) -> ComposeResult:
        yield SortButtons("")
        with VerticalScroll(): 
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()

# all items (left view)
class AllItems(VerticalScroll):
    BORDER_TITLE = "All Items"
    def compose(self) -> ComposeResult:
        yield SortButtons("")
        with VerticalScroll(): 
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
        
        
class SortButtons(Static):
    def compose(self) -> ComposeResult:
        yield Button("Oldest", flat = True)
        yield Button("Newest", flat = True)
        yield Button("Priority", flat = True)

class TimeSelection(Static):
    def compose(self) -> ComposeResult:
        yield Button("Today", flat = True)
        yield Button("Week", flat = True)
        yield Button("Month", flat = True)

# App is the base class for all textual apps
class TodoApp(App):
    CSS_PATH = "Terminal.tcss"
    BINDINGS = [("d", "toggle", "Toggle dark mode")]

    def compose(self) -> ComposeResult:
        yield Header()
        yield AllItems(classes="static", id="all-items")
        yield Urgent(classes="right-side")
        yield Reminders(classes="right-side")
        yield Footer()
        
    def action_toggle(self) -> None:
        self.theme = (
            "textual-dark" if self.theme == "textual-light" else "textual-light"
        )
    


if __name__ == "__main__":
    app = TodoApp()
    itemManager = ItemManager(DB_PATH)
    app.run()
    itemManager.saveToCsv()
    print("Done!")
