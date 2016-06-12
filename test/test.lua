local ui = require "libyalul"

local function printTable(t)
	print(t)
	for k, v in pairs(t) do
		print(k, v)
	end
end

printTable(ui)

ui.init()

local d = 0
local prog = ui.newProgressBar():setValue(d)

local w

local box = ui.newVBox():setPadded(5)
	:append(ui.newLabel("UserName:", true))
	:append(ui.newEntry("APPLE"):setReadOnly(true))
	:append(ui.newLabel("Password:"))
	:append(ui.newEntry())
	:append(prog)
	:append(ui.newRadioButtons():append("BOY", "GIRL"):onSelected(
		function(index)
			print(index)
		end):setSelected(2))
	:append(ui.newColorButton():setColor(1, 0, 0, 1):onChanged(
		function(r, g, b, a)
			print(r, g, b, a)
		end))
	:append(ui.newCombobox():append("Student", "Teacher", "Stuff"):setSelected(2):onSelected(
		function(index)
			print(index)
		end))
	:append(ui.newEditableCombobox():append("Human", "Animal", "Plant"):onChanged(
		function(text)
			print(text)
		end))
	:append(ui.newButton("OpenFile"):onClicked(
		function() 
			local str = ui.openFile(w)
			ui.msgBox(w, "TITLE", str or "NIL")
			str = ui.saveFile(w)
			ui.msgBoxError(w, "TITLE", str or "NIL")
		end))
	:append(ui.newSlider(20, 100):setValue(30):onChanged(
		function(v)
			print(v)
		end))
	:append(ui.newSpinbox(20, 100):setValue(30):onChanged(
		function(v)
			print(v)
		end))
	:append(ui.newButton("Login"):onClicked(
		function()
			print("CLICK")
			d = d + 10
			prog:setValue(d)
		end
	))
w = ui.newWindow("Hello", 400, 400, false):setChild(
	box
):onClosing(
	function()
		ui.quit()
	end
):show()

collectgarbage()

ui:main()
