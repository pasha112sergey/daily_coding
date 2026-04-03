from textual import on
from textual.app import App, ComposeResult
from textual.widgets import Input, Placeholder, Label
from textual.containers import Vertical, Horizontal
from textual.screen import Screen
from textual.validation import Function, ValidationResult
from Item import Item, Priority, ItemManager
import pandas as pd
from Form import Form, InputDescriptor

    
class AddItemScreen(Screen):
    BINDINGS = [("escape", "cancelSubmission", "Cancel")]
    def __init__(self, itemManager : ItemManager, app : App, **kwargs) -> None:
        super().__init__(**kwargs)
        self.itemManager = itemManager
        self.appParent = app

    def prioValidator(self, x):
        try: 
            Priority(x)
            return True
        except ValueError:
            return False

    def compose(self) -> ComposeResult:
        textValidator = lambda x: 0 < len(x)
        titleDescriptor = InputDescriptor("Title", placeholder='Title (up to 20 chars)', type="text", max_length=20, validators=Function(textValidator, "Title of invalid length"))
        prioDescriptor = InputDescriptor("Priority", placeholder='Priority (LOW, MED, HIGH)', type="text", max_length=10, validators=Function(self.prioValidator, "Must be one of LOW, MED, HIGH"))
        descDescriptor = InputDescriptor("Description", placeholder='Description (up to 100 chars)', type="text", max_length=100, validators=Function(textValidator, "Description of invalid length"))
        deadlineDescriptor = InputDescriptor("Deadline", placeholder="Deadline (format: DD-MM-[YYYY] [00:00])", type="text", max_length=20, validators=Function(lambda x: self.checkDate(x), "Invalid deadline"))
        postedDescriptor = InputDescriptor("Posted", placeholder="Posted date (format: DD-MM-[YYYY] [00:00])", type="text", max_length=20, validators=Function(lambda x: self.checkDate(x), "Invalid Posted Date"))

        formArgs = {
            "title" : titleDescriptor,
            "prio" : prioDescriptor,
            "desc" : descDescriptor,
            "deadline": deadlineDescriptor,
            "posted": postedDescriptor
        }

        with Vertical():
            yield Form(self.appParent, self.itemManager, formArgs)
    
    def checkDate(self, date: str, optional = True) -> bool:
        if optional == True and len(date) == 0:
            return True
        try: 
            pd.to_datetime(date)
            return True
        except (pd.errors.ParserError, ValueError):
            return False
        
    def action_cancelSubmission(self) -> None:
        self.appParent.pop_screen()
    
    def on_form_resulting(self, message: Form.Resulting):
        self.itemManager.addItem(Item(**message.result))