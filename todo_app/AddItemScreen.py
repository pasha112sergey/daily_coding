from textual import on
from textual.app import App, ComposeResult
from textual.widgets import Input, Placeholder, Label
from textual.containers import Vertical, Horizontal
from textual.screen import Screen
from textual.validation import Function, ValidationResult
from Item import Item, ItemManager
import config
import pandas as pd
import config
from textual.binding import Binding, BindingType
from typing import TYPE_CHECKING, ClassVar, Iterable, NamedTuple
from Form import Form, InputDescriptor
from Item import Priority, ItemManager
        

class AddItemScreen(Screen):

    BINDINGS = [("escape", "cancelSubmission", "Cancel")]
    def prioValidator(self, x):
        try: 
            Priority(x)
            return True
        except ValueError:
            return False

    def compose(self) -> ComposeResult:
        textValidator = lambda x: 0 < len(x)
        titleDescriptor = InputDescriptor("Title", placeholder='Title (up to 20 chars)', type="text", max_length=20, validators=Function(textValidator, "Title of invalid length"))
        prioDescriptor = InputDescriptor("Priority", placeholder='Priority (LOW, MED, HIGH)', type="text", max_length=5, validators=Function(self.prioValidator, "Must be one of LOW, MED, HIGH"))
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
            yield Form(formArgs)
    
    def checkDate(self, date: str, optional = True) -> bool:
        if optional == True and len(date) == 0:
            return True
        try: 
            pd.to_datetime(date)
            return True
        except (pd.errors.ParserError, ValueError):
            return False
        


    def action_cancelSubmission(self) -> None:
        config.app.pop_screen()
    
    def on_form_resulting(self, message: Form.Resulting):
        config.itemManager.addItem(Item(**message.result))
        