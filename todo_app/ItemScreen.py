from textual.screen import Screen
from Item import Item
from textual.widgets import Placeholder

class ItemScreen(Screen):
    BINDINGS = [("escape", "cancel", "Cancel")]
    def __init__(self, item : Item, **kwargs) -> None:
        super().__init__(**kwargs)
        self.item = item

    
    def compose(self) :
        yield Placeholder()

    def action_cancel(self) -> None:
        self.app.pop_screen()