from textual import on
from textual.widget import Widget
from textual.widgets import Input, Button, Label
from textual.app import ComposeResult
from textual.containers import Vertical
from textual.validation import ValidationResult
from textual.message import Message

class InputDescriptor():
    def __init__(self, name: str | None = None, **kwargs):
        self.name = name
        self.args = kwargs


class Form(Widget):
    def __init__(self, inputArgs: dict[str, InputDescriptor] = {}) -> None:
        super().__init__()
        self.inputs = inputArgs
        print(self.inputs)
        self.inputElements: list[Input] = []
        self.inputResult = {}
        self.title = Label("Title!")

        for inputName in self.inputs:
            inp = Input(name=inputName, **(self.inputs[inputName].args))
            self.inputElements.append(inp)
        
    def compose(self) -> ComposeResult:
        with Vertical():
            yield self.title
            for ele in self.inputElements:
                yield Label(ele.name, id=f'{ele.name}-label')
                yield ele
            yield Button(label="Submit")
        
    class Submitted(Message):
        pass

    class Resulting(Message):
        def __init__(self, result):
            super().__init__()
            self.result = result

    def on_button_pressed(self, event: Button.Pressed) -> None:    
        self.post_message(self.Submitted())
    
    def on_form_submitted(self, message: Submitted) -> None:
        submitting = True
        submission = {}
        for ele in self.inputElements:
            validityCheck = ele.validate(ele.value)
            if not validityCheck.is_valid:
                ele.styles.background = "red"
                self.query_one(f"#{ele.name}-label").content = validityCheck.failure_descriptions[0]
                submitting = False
            else:
                submission[ele.name] = ele.value
                self.query_one(f"#{ele.name}-label").content = f"{ele.name} = {submission[ele.name]}"
        
        if submitting:
            self.post_message(self.Resulting(submission))
        

