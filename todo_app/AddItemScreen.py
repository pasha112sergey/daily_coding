from textual import on
from textual.app import App, ComposeResult
from textual.widgets import Input, Placeholder, Label
from textual.containers import Vertical, Horizontal
from textual.screen import Screen
from textual.validation import Function, ValidationResult
from Item import Item, ItemManager
import config
import pandas as pd
import time
from textual.binding import Binding, BindingType
from typing import TYPE_CHECKING, ClassVar, Iterable, NamedTuple


def checkDate(date: str, optional = True) -> bool:
    if optional == True and len(date) == 0:
        return True
    try: 
        pd.to_datetime(date)
        return True
    except (pd.errors.ParserError, ValueError):
        return False
        
        

class AddItemScreen(Screen):

    BINDINGS = [("escape", "cancelSubmission", "Cancel")]

    def compose(self) -> ComposeResult:
        with Vertical():
            # text values
            yield Label("Create new Item")
            yield FormInput(placeholder="Title (up to 20 chars)", type="text", max_length=20, validators=[Function(lambda x: 0 < len(x), "Title of invalid length")])
            yield Input(placeholder="Description (up to 100 chars)", type="text", max_length=100, validators=[Function(lambda x: 0 < len(x), "Description of invalid length")])
            
            # time values
            yield Input(placeholder="Deadline", type="text", validators=[Function(lambda x: checkDate(x, optional=True), "Invalid date format for deadline")])
            yield Input(placeholder="Posted on", type="text", validators=[Function(lambda x: checkDate(x, optional=True), "Invalid date format for posted on")])
    
    def action_cancelSubmission(self) -> None:
        config.app.pop_screen()

    @on(Input.Submitted)
    def createItem(self, event: Input.Submitted):
        if event.validation_result and not event.validation_result.is_valid:
            # self.query_one(Label).update(event.validation_result.failure_descriptions[0])
            self.query_one(Label).update(event.value)
        else:
            self.query_one(Label).update(event.value)
    
    """
        idea: 
        change the submit method to listen for the event emitted by enter key.

        Then, everything gets submitted together!
    """