# yalul

Yet Another LibUi bindings for Lua. It is still **WIP**.

Yalul learns a lot from [zevv/libuilua](https://github.com/zevv/libuilua) and [mdombroski/libui-lua](https://github.com/mdombroski/libui-lua).

The difference is that, yalul splits source codes into multiple files, yalul consider Control as parent class, yalul does garbage collection and so on.

## Build

Edit CMakeLists and set the path of libui, path of lua if needed, and set the install path.

```
mkdir build; cd build
cmake ..
make
make install
```

## Notice

Yalul doesn't provide the destroy API for release the control resource. It does this when collecting the garbage. When destroying a control, it also destroys all its child controls. As a result, you should not use the child control if its parent control has been destroyed.

## Documents

Require the library as following:

```
local ui = require("libyalul")
```

### yalul

The yalul application is a looping:

```
ui.init()

-- initialize the UI
-- Add ui.quit() for exiting the loop

ui.main()
```

* ui.init(): initialize libui and yalul
* ui.main(): jump into the loop, only ui.quit can't finish the loop
* ui.quit(): finish the loop

There are some APIs for showing a standard dialog. These APIs all consumes a *Window* control as the first arguments. They may be moved into *Window* one day.

* ui.openFile(win): return nil or the file path.
* ui.saveFile(win): return nil or the file path.
* ui.msgBox(win, title, message)
* ui.msgBoxError(win, title, message)

### Control

Control is an abstract class, the parent class for all the other controls. It makes all the other controls suppor the following APIs.

* control:hide()
* control:show()
* control:visible(): returns true or false, represents whether the control is visible or not
* control:enable()
* control:disable()
* control:enabled(): returns true or false, represents whether the control is enable or not.

### Window

* ui.newWindow(title, width, height, hasMenuBar): return the *Window*, for now, *MenuBar* is not supported.
* window:setChild(control)
* window:title()
* window:setTitle(text)
* window:margined(): return an integer
* window:setMargined(int)
* window:onClosing(func): the parameter func is a function with no arguments, which returns true/false representing whether to close the window of not. Nil is also false.

An example:

```
local w = ui.newWindow("Hello", 400, 400, false)
            :setChild(ui.newLabel("Hello, World")
            :setMargined(10)
            :onClosing(
                function()
                    ui.quit()
                end)
w:show()
```

### Group

* ui.newGroup(title)
* group:title()
* group:setTitle(text)
* group:margined()
* group:setMargined(intValue)

### Box

Box is the container for controls, divided into HBox, the horizonal box, and VBox, the vertical box.

* ui.newHBox()
* ui.newVBox()
* box:append(control, stretchy): append the control to box, stretchy is a boolean, representing stretching it or not.
* box:delete(index): delete the indexed control
* box:delete(control): delete the control
* box:padded()
* box:setPadded(intValue)

### Label

* ui.newLabel(text): returns the *Label*
* label:text(): returns the text
* label:setText(text)

### Entry

Entry is an one-line editable textbox.

* ui.newEntry(): returns the *Entry*
* entry:text()
* entry:setText(text)
* entry:readOnly()
* entry:setReadOnly(bool)
* entry:onChanged(func): func consumes the text as the only arguments, no return results.

### Button

* ui.newButton(text)
* button:text()
* button:setText(text)
* button:onClicked(func): func, no arguments, no returns.

### Checkbox

* ui.newCheckbox(text)
* checkbox:text()
* checkbox:setText(text)
* checkbox:checked()
* checkbox:setChecked(bool)
* button:onToggled(func): func, has one argument, checked or not, no returns

### ProgressBar

* ui.newProgressBar()
* progressBar:setValue(): sets the progress

### Slider

* ui.newSlider(intMin, intMax)
* slider:value()
* slider:setValue(intValue)
* slider:onChanged(func): func, has one argument, the value, no returns.

### Spinbox

Spinbox is similar to Slider.

* ui.newSpinbox(intMin, intMax)
* spinbox:value()
* spinbox:setValue(intValue)
* spinbox:onChanged(func)

### RadioButtons

* ui.newRadioButtons()
* radioButtons:append(text1, text2, ...)
* radioButtons:selected(): return the index of the selected button.
* radioButtons:setSelected(index)
* radioButtons:onSelected(func): func, has one argument, the index of the selected button, no returns

### Combobox

Combobox is similar to RadioButtons.

* ui.newCombobox()
* combobox:append(text1, text2, ...)
* combobox:selected()
* combobox:setSelected(index)
* combobox:onSelected(func)

### EditableCombobox

EditableCombobox looks the almost the same as Combobox, except with the editable function. However, their APIs look not the same.

* ui.newEditableCombobox()
* eCombo:append(text1, text2, ...)
* eCombo:text()
* eCombo:setText(text)
* eCombo:onChanged(func): func, has one argument, the text, no returns.

### ColorButton

ColorButton is a button, and when you click it, it will show up a dialog for selecting a color.

* ui.newColorButton()
* colorButton:color(): returns r, g, b, a. All of them are double value between 0.0 ~ 1.0
* colorButton:setColor(r, g, b, a)
* colorButton:onChanged(func): func, has four arguments, r, g, b, a, no returns.

### Mores are coming.
