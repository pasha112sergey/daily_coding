from textual import on
from textual.widget import Widget
from textual.app import App
from textual.widgets import Input, Button, Label
from textual.app import ComposeResult
from textual.containers import Vertical
from textual.validation import ValidationResult
from textual.message import Message
from Item import Item, ItemManager, Priority

class InputDescriptor():
    def __init__(self, name: str | None = None, **kwargs):
        self.name = name
        self.args = kwargs


class Form(Widget):
    def matchPrio(self, prio):
        if prio == "LOW":
            return Priority.LOW
        if prio == "MED":
            return Priority.MEDIUM
        if prio == "HIGH":
            return Priority.HIGH
        else:
            return None

    def __init__(self, app: App, itemManager : ItemManager, inputArgs: dict[str, InputDescriptor] = {}) -> None:
        super().__init__()
        self.itemManager = itemManager
        self.appParent = app

        self.inputs = inputArgs
        print(self.inputs)
        self.inputElements: list[Input] = []
        self.inputResult = {}
        self.title = Label("Create a new todo item!")

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
        
        if not submitting:
            return
        
        prio = self.matchPrio(submission["prio"])
        item = Item(submission["prio"], submission["title"], submission["desc"], submission["deadline"])
        self.itemManager.addItem(item)
        self.appParent.pop_screen()

