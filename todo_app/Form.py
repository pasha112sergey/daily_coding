from textual import on
from textual.widget import Widget
from textual.widgets import Input, Button, Label
from textual.app import ComposeResult
from textual.containers import Vertical
from textual.validation import ValidationResult

class InputDescriptor():
    def __init__(self, name: str | None = None, **kwargs):
        self.name = name
        self.args = kwargs


class Form(Widget):
    def __init__(self, inputArgs: dict[str, InputDescriptor] = {}) -> None:
        super().__init__()
        self.inputs = inputArgs
        self.inputElements = []
        self.inputResult = {}

    def compose(self) -> ComposeResult:
        with Vertical():
            for inputName in self.inputs:
                self.inputElements.append(Input(name=inputName, **(self.inputs[inputName].args)))
                yield Label(inputName, id='{self.inputElements[-1].id}-label')
                yield self.inputElements[-1]
            yield Button(label="Submit", action="submitForm")

    def action_submitForm(self) -> None:
        for ele in self.inputElements:
            @on (Input.Submitted, ele.id)
            def handleActionId(self, submittedEvent):
                if submittedEvent.validation_result and not submittedEvent.validation_result.is_valid:
                    self.query_one(Label, '#{ele.id}-label').update(submittedEvent.validation_result.failure_descriptions[0])
                else: 
                    self.inputResult[ele.id] = submittedEvent.value
                    
        for ele in self.inputElements:
            ele.action_submit()

        
