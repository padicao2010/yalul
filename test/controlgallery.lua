local ui = require("libyalul")

ui.init()

local w

ui.newMenu("File")
    :appendItem("Open"):onItemClicked(
        function()
            local s = ui.openFile(w)
            if s then
                ui.msgBox(w, "INFO", "Open File: " .. s)
            end 
        end)
    :appendItem("Save"):onItemClicked(
        function()
            local s = ui.saveFile(w)
            if s then   
                ui.msgBox(w, "INFO", "Save File: " .. s)
            end
        end)
    :appendSeparator()
    :appendItem("Quit"):onItemClicked(
        function()
            ui.quit()
        end)

local em
em = ui.newMenu("Edit")
    :appendItem("Nothing")
    :appendSeparator()
    :appendCheckItem("Enabled"):setItemChecked(true):onItemClicked(
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

local mt = "Even when Phillip was not at home, Emmeline had a great deal to busy herself with, and she was never bored or lonely. She would help her mother with her embroidery, or exchange idle gossip with the servants, or go for long walks about the countryside, eating blackberries from the hedgerows and snagging her dress on brambles."

w = ui.newWindow("yalul Control Gallery", 640, 480, true)
    :setMargined(1)
    :onClosing(
        function() 
            ui.quit() 
        end
    ):setChild(ui.newHBox():setPadded(1)
        :append(ui.newGroup("Basic Controls"):setMargined(1)
            :setChild(ui.newVBox():setPadded(1)
                :append(ui.newButton("Button"))
                :append(ui.newCheckbox("Checkbox"):setChecked(true))
                :append(ui.newEntry():setText("Entry"))
                :append(ui.newLabel("Label"))
                :append(ui.newColorButton():setColor(0, 0, 0, 1))
                :append(ui.newMultilineEntry():append(mt))
                :append(ui.newNWMultilineEntry():append(mt):setReadOnly(true))
            )
        ):append(ui.newVBox()
            :append(ui.newGroup("Numbers"):setMargined(1)
                :setChild(ui.newVBox():setPadded(1)
                    :append(ui.newSpinbox(0, 100):setValue(50))
                    :append(ui.newSlider(0, 100):setValue(50))
                    :append(ui.newProgressBar(0, 100):setValue(50))
                )
            ):append(ui.newGroup("Lists"):setMargined(1)
                :setChild(ui.newVBox():setPadded(1)
                    :append(ui.newCombobox()
                        :append("Combobox Item 1", "Combobox Item 2", "Combobox Item 3")
                        :setSelected(1))
                    :append(ui.newEditableCombobox()
                        :append("Editable Item 1", "Editable Item 2", "Editable Item 3")
                        :setText("Editable Item 2"))
                    :append(ui.newRadioButtons()
                        :append("Radio Button 1", "Radio Button 2", "Radio Button 3")
                        :setSelected(3))
                )
            ):append(ui.newTab()
                :append("Page 1", ui.newLabel("Welcome"))
                :append("Page 2", ui.newLabel("Wonderful"))
                :append("Page 3", ui.newLabel("Powerful"))
                :setMargined(1, 1)
                :setMargined(2, 1)
                :setMargined(3, 1)
            )
        , true)
    ):show()
    
ui.main()
