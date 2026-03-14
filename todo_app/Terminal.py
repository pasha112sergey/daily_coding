from textual.app import App, ComposeResult
from textual.widgets import Footer, Header, Button, Placeholder
from textual.containers import VerticalScroll, VerticalScroll
from Item import Item, ItemManager

DB_PATH = "~/.todo.csv"

class ItemPreview(Placeholder):
    BASE_CSS = ""
    pass

# Urgent
class Urgent(VerticalScroll):
    BORDER_TITLE = "Due Today"
    def compose(self) -> ComposeResult:
        with VerticalScroll(): 
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview() 

# Reminders
class Reminders(VerticalScroll):
    BORDER_TITLE = "Reminders"
    def compose(self) -> ComposeResult:
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
class AllTodos(VerticalScroll):
    BORDER_TITLE = "All Items"
    def compose(self) -> ComposeResult:
        with VerticalScroll(): 
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()
            yield ItemPreview()


# App is the base class for all textual apps
class TodoApp(App):
    CSS_PATH = "Terminal.tcss"
    BINDINGS = [("d", "toggle", "Toggle dark mode")]

    def compose(self) -> ComposeResult:
        yield Header()
        yield AllTodos(classes="static", id="all-items")
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
