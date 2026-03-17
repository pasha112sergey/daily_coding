from textual.app import App, ComposeResult
from textual.widgets import Placeholder
from textual.screen import Screen
import config

class AddItemScreen(Screen):

    BINDINGS = [("escape", "cancelSubmission", "Cancel")]
    def compose(self) -> ComposeResult:
        yield Placeholder()
    
    def action_cancelSubmission(self) -> None:
        config.app.pop_screen()
        