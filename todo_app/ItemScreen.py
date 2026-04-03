from textual.screen import Screen
from Item import Item
from config import formatTimestamp
from textual.widgets import Static, Button

class ItemScreen(Screen):
    BINDINGS = [("escape", "cancel", "Cancel")]
    def __init__(self, item : Item, **kwargs) -> None:
        super().__init__(**kwargs)
        self.item = item
    
    def compose(self) :
        yield Static(self.item.title)
        yield Static(f"Posted: {self.item.posted}")
        yield Static(self.item.desc)
        yield Static(f"Due by: {formatTimestamp(self.item.deadline, "deadline")}")
        yield Button("Mark Completed", flat = True, id="mark-completed-btn")
        

    def action_cancel(self) -> None:
        self.app.pop_screen()