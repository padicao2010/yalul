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

```lua
ui.init()

-- initialize the UI
-- Add ui.quit() for exiting the loop

ui.main()
```

* ui.init(): initialize libui and yalul
* ui.main(): jump into the loop, only ui.quit can finish the loop
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

```lua
local w = ui.newWindow("Hello", 400, 400, false)
            :setChild(ui.newLabel("Hello, World")
            :setMargined(10)
            :onClosing(
                function()
                    ui.quit()
                end)
w:show()
```

### Menu

Due to the difference of design between Mac and Windows, Linux, libui deploys the Mac desgin, because it can be implemented easily by limiting the usage in Windows and Linux.

* ui.newMenu(title): Adds a new menu in menubar, for now, menu can only be added.
* menu:addItem(title): Adds a submenu to menu
* menu:addCheckItem(title): Adds a check-style submenu to menu
* menu:addPreferencesItem(): Adds a preferences-style submenu to menu
* menu:addAboutItem(): Adds a about-style submenu to menu
* menu:addQuitItem(): Adds a quit-style submenu to menu

After orginazing all menus, we can deal with it. These four functions all have a optional argument as the first argument. If it's omitted, all comes to the last submenu of the menu. Or not, all comes to the index submenu.

* menu:setItemChecked([index, ]boolValue)
* menu:enableItem([index])
* menu:disableItem([index])
* menu:onItemClicked([index, ]func): func, has one argument, checked or not, always false when it is not a check-style submenu, no returns

We can also get the submenu status.

* menu:itemChecked(index): return intValue. Returns false when it is not a check-style submenu.

Here's an example:

```lua
ui.newMenu("File")
    :appendItem("Open")
    :appendItem("Save")
    :appendSeparator()
    :appendItem("Quit"):onItemClicked(
        function()
            ui.quit()
        end)
local em
em = ui.newMenu("Edit")
    :appendItem("Nothing")
    :appendCheckItem("Enable"):setItemChecked(true):onItemClicked(
        function(c)
            if c then
                em:enableItem(1)
            else
                em:disableItem(1)
            end
        end)
ui.newMenu("Help")
    :appendPreferencesItem()
    :appendAboutItem()
    :appendQuitItem()
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

### Tab

Tab is a multi-pages container, and every page has a title.

* ui.newTab()
* tab:append(title, control): add a new page, with the title, containing the control.
* tab:insertAt(title, index, control): add a new page at a specific position.
* tab:delete(index): remove the page at specific position
* tab:delete(control): remove the specific control
* tab:margined(index): return the margin of the specific page
* tab:setMargined(index, intValue): set the margin of the specific page

## TODO

* support *DateTimePicker*, the current state of libui is you choose a date or a time, and then it's over.
* support *FontButton*, the current state of libui is you choose a font, and the it's over.
* support *MultilineEntry*, the current state of libui is the height of the control is zero in Linux.
* more test
