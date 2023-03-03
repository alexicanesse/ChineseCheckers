from tkinter import *
import math
import numpy as np
from ChineseCheckers_Areas import *

class ClassicButton(Areas):
    # for the "New Game" and "Next Turn" button
    def __init__(self, parent : Canvas, width : int, height : int, x : int, y : int, text : str, state : str, grayed_text : str):
        # states are : pressed, normal, hovered, grayed
        '''
        - text : string that will appear on the button
        - grayed_text : string that will appear below the button when it is grayed
        '''
        assert(state in {"normal", "grayed"})
        c = self.get_color("darkgreen") if state == "normal" else self.get_color("gray")
        r = height // 4
        self.parent = parent
        self.box = self.create_rounded_rect(parent, x, y, x + width, y + height, r, c)
        self.text = parent.create_text(x + width // 2, y + height // 2, text=text,
                                        fill=self.get_color("white"), font=self.get_font(16), justify=CENTER)
        self.grayed_text = parent.create_text(x + width // 2, y + 3 * height // 2, text=grayed_text,
                                        fill=self.get_color("white"), font=self.get_font(12), justify=CENTER) 
        self.hitbox = self.create_rounded_rect(parent, x, y, x + width, y + height, r, "") # invisible hitbox
        self.state = state
        self.width = width
        self.height = height

          
        if (self.state != "grayed"):
            self.disable_grayedtext()

        def f_enter_hitbox(event):
            if self.state != "grayed":
                self.set_state("hovered")

        self.parent.tag_bind(self.hitbox, "<Enter>", f_enter_hitbox)

        def f_leave_hitbox(event):
            if self.state != "grayed":
                self.set_state("normal")

        self.parent.tag_bind(self.hitbox, "<Leave>", f_leave_hitbox)
    
        def f_release_hitbox(event):
            if self.state == "pressed":
                self.set_state("hovered")

        self.parent.tag_bind(self.hitbox, "<ButtonRelease-1>", f_release_hitbox)
    

    def enable_grayedtext(self):
        # show the text below the button
        self.parent.itemconfig(self.grayed_text, state = 'normal')
    

    def disable_grayedtext(self):
        # show the text below the button
        self.parent.itemconfig(self.grayed_text, state = 'hidden')

    
    def create_rounded_rect(self, canvas, x1, y1, x2, y2, r, c):
        # r is corner radius, c is color (fill), thanks Stackoverflow for the code
        points = (x1+r, y1, x1+r, y1, x2-r, y1, x2-r, y1, x2, y1, x2, y1+r, x2, y1+r, x2, y2-r, x2, \
            y2-r, x2, y2, x2-r, y2, x2-r, y2, x1+r, y2, x1+r, y2, x1, y2, x1, y2-r, x1, y2-r, x1, y1+r, x1, y1+r, x1, y1)
        return canvas.create_polygon(points, outline="", fill=c, smooth=True)

    
    def moveto(self, x, y):
        self.parent.moveto(self.box, x, y)
        self.parent.moveto(self.hitbox, x, y)
        a, b, c, d = self.parent.bbox(self.text)
        text_width, text_height = c - a, d - b
        self.parent.moveto(self.text, x + (self.width - text_width) // 2, y + (self.height - text_height) // 2)

        try:
            a, b, c, d = self.parent.bbox(self.grayed_text)
            text_width, text_height = c - a, d - b
            self.parent.moveto(self.grayed_text, x + (self.width - text_width) // 2, y + self.height + (self.height - text_height) // 2)
        except TypeError:
            
            self.enable_grayedtext()
            a, b, c, d = self.parent.bbox(self.grayed_text)
            text_width, text_height = c - a, d - b
            self.parent.moveto(self.grayed_text, x + (self.width - text_width) // 2, y + self.height + (self.height - text_height) // 2)
            self.disable_grayedtext()


    def set_state(self, state):
        self.disable_grayedtext()
        if state == "pressed":
            c = self.get_color("pressed")
        elif state == "normal":
            c = self.get_color("darkgreen")
        elif state == "grayed":
            self.enable_grayedtext()
            c = self.get_color("gray")
        elif state == "hovered":
            c = self.get_color("hovered")
        else:
            print(f"Unknown state {state}")
            return
        self.parent.itemconfigure(self.box, fill=c)
        self.parent.update_idletasks()
        self.state = state
        
    def get_state(self):
        return self.state


class Checkboxes(Areas):
    # for the parameter checkboxes on the left
    def __init__(self, parent : Canvas, width : int, height : int, x : int, y : int, text, board : Canvas, state = "off"):
        assert(state[0] != "h") # a button is not highlighted by default
        w = height / 14
        self.parent = parent
        self.box = parent.create_rectangle(x, y, x + height, y + height, fill="", width=w, outline=self.get_color("white"))
        self.cross = self.create_cross(parent, x + height // 2, y + height // 2, height // 4, 
                                        w, self.get_color("white") if state == "on" else "")
        self.text = parent.create_text(x + 3 * height // 2, y - height // 4, text=text,
                                        fill=self.get_color("white"), font=self.get_font(16), justify=LEFT, anchor="nw")
        self.hitbox = parent.create_rectangle(x, y, x + width, y + height, fill="", outline="")
        
        self.state = "" # can be hoff, hon, off, on, gon, goff
        self.set_state(state)
        self.width = width
        self.height = height
        self.board = board
        self.cross_offset = int(self.parent.coords(self.cross)[0] - x) - w // 2
        
    
    def create_cross(self, canvas : Canvas, x : int, y : int, side : int, w : int, color):
        '''draws a cross centered in x, y, with thickness w'''
        offset = math.sqrt(2) * side
        points = (x - offset, y - offset, x, y, \
                x + offset, y - offset, x, y, \
                x + offset, y + offset, x, y, \
                x - offset, y + offset, x, y)
        return canvas.create_polygon(points, width=w, outline=color, fill="")

    
    def moveto(self, x, y):
        '''moves the button on the ui'''
        self.parent.moveto(self.box, x, y)
        self.parent.moveto(self.text, x + 3 * self.height // 2, y - self.height // 4)
        self.parent.moveto(self.hitbox, x, y)
        self.parent.moveto(self.cross, x + self.cross_offset, y + self.cross_offset)
    
    def bind(self, i):
        self.parent.tag_bind(self.hitbox, "<Button-1>", lambda event : self.click_param(i, event))
        self.parent.tag_bind(self.hitbox, "<Enter>", lambda event : self.set_state("h" + self.state) if self.state[0] != "g" else None)
        self.parent.tag_bind(self.hitbox, "<Leave>", lambda event : self.set_state(self.state[1:]) if self.state[0] != "g" else None)

    
    def click_param(self, i, event):
        old_state = self.state
        if old_state[0] == "g": # cannot click grayed button
            return
        assert(old_state == "hoff" or old_state == "hon") # the button must be highlighted to be clicked

        self.set_state("hoff" if old_state == "hon" else "hon")
        if i == 0: # show white arrows button
            self.board.set_parameter("show_white_ar", not self.board.show_white_ar)
            
            if not self.board.show_white_ar and self.board.arrows_white:

                # reset previously saved arrows
                self.board.reset_saved_arrows("white")
                self.board.save_arrows("white")
                self.board.show_arrows([], "white") # removes existing arrows
            else:

                self.board.show_arrows(self.board.previous_arrows_white, "white")
        elif i == 1: # show black arrows button
            self.board.set_parameter("show_black_ar", not self.board.show_black_ar)
            if not self.board.show_black_ar and self.board.arrows_black:
                
                # reset previously saved arrows
                self.board.reset_saved_arrows("black")
                self.board.save_arrows("black")
                self.board.show_arrows([], "black") # removes existing arrows
            else:

                self.board.show_arrows(self.board.previous_arrows_black, "black")
        elif i == 2: # show moves button
            self.board.set_parameter("show_moves", not self.board.show_moves)
            if not self.board.show_moves:
                self.board.highlight_cases([]) # removes existing highlighted cases

    
    def get_state(self):
        return self.state

    
    def set_state(self, state):
        '''change state & modify the appearance of the button to match its new state'''
        if self.state == state:
            return
        if state[0] == "h": # state is hon or hoff
            color = self.get_color("darkgreen")
        elif state[0] == "g": # state is gon or goff
            color = self.get_color("gray")
        else:
            color = self.get_color("white")

        self.parent.itemconfigure(self.box, outline=color)
        self.parent.itemconfigure(self.text, fill=color)

        if state[-2:] == "on": # make the cross visible
            self.parent.itemconfigure(self.cross, outline=color)
        else:
            self.parent.itemconfigure(self.cross, outline="")

        self.state = state
        self.parent.update_idletasks()
    
class ChoiceButton(Areas):
    def __init__(self, canvas : Canvas, width : int, height : int, x : int, y : int, text : str, state : str, i : int):
        self.height = height
        self.width = width
        w = height / 14
        self.contour = canvas.create_rectangle(x, y, x + width, y + height, fill="", width=1, outline=self.get_color("white")) # contour
        self.box = canvas.create_rectangle(x, y, x + width, y + height, fill="", width=w, outline="")
        self.ui_text = canvas.create_text(x + width // 2, y + height // 2, text=text,
                                        fill=self.get_color("white"), font=self.get_font(16), justify=CENTER)
        self.hitbox = canvas.create_rectangle(x, y, x + width, y + height, fill="", width=w, outline="") # invisible hitbox
        self.text = text
        self.number = i
        self.state = "" # can be sel, gsel, nsel, hnsel, gnsel. sel for selected, h for highlighted, g for grayed
        self.set_state(canvas, state)
    
    def get_text(self):
        return self.text
    
    def moveto(self, canvas, x, y):
        '''moves the button on the ui'''
        canvas.moveto(self.box, x, y)
        canvas.moveto(self.hitbox, x, y)
        canvas.moveto(self.contour, x, y)
        a, b, c, d = canvas.bbox(self.ui_text)
        text_width, text_height = c - a, d - b
        canvas.moveto(self.ui_text, x + (self.width - text_width) // 2, y + (self.height - text_height) // 2)
    
    def get_state(self):
        return self.state
    
    def set_state(self, canvas : Canvas, state : str):
        if state == "sel":
            c_fill, c_outline, t_fill = self.get_color("darkgreen"), self.get_color("darkgreen"), self.get_color("white")
        elif state == "gsel":
            c_fill, c_outline, t_fill = self.get_color("gray"), self.get_color("gray"), self.get_color("white")
        elif state == "nsel":
            c_fill, c_outline, t_fill = "", "", self.get_color("white")
        elif state == "hnsel":
            c_fill, c_outline, t_fill = "", self.get_color("darkgreen"), self.get_color("darkgreen")
        elif state == "gnsel":
            c_fill, c_outline, t_fill = "", "", self.get_color("gray")
            canvas.itemconfigure(self.contour, outline=self.get_color("gray"))
        else:
            print(f"Unknown state {state}")
            return
        
        canvas.itemconfigure(self.box, fill=c_fill, outline=c_outline)
        canvas.itemconfigure(self.ui_text, fill=t_fill)
        canvas.update_idletasks()
        self.state = state
        

class ChoiceDepth(Areas):
    # for the input fields that allow user to enter depth
    def __init__(self, parent : Canvas, width : int, x : int, y : int, defaultText : str):
        ''' x, y is the coordinate of the leftmost top corner '''
        self.parent = parent
        self.defaultText = defaultText

        self.entry = Entry(self.parent, font=self.get_font(15), fg=self.get_color("gray"), justify=CENTER)
        self.entry.insert(0, self.defaultText)
        self.input_field = self.parent.create_window(x, y, window=self.entry, anchor=NW, width=width)

        # bind to make default text disappear when focusing widget and re-appear afterwards
        self.entry.bind('<FocusIn>', self.onFocusIn)
        self.entry.bind('<FocusOut>', self.onFocusOut)
    
    def onFocusIn(self, event):
        if self.entry.get() == self.defaultText:
            # delete placeholder on focus in
            self.entry.delete(0, 'end')
            self.entry.configure({"fg": "black"})

    def onFocusOut(self, event):
        if self.entry.get() == "":
            # puts placeholder again if nothing is entered on focus out
            self.entry.insert(0, self.defaultText)
            self.entry.configure({"fg": self.get_color("gray")})
    
    def disable(self):
        '''make the text field disappear'''
        self.parent.itemconfig(self.input_field, state='hidden')
        self.parent.update_idletasks()

    def enable(self):
        '''make the text field appear'''
        self.parent.itemconfig(self.input_field, state='normal')
        self.parent.update_idletasks()

    def moveto(self, x, y):
        '''moves the text field on the ui'''
        self.parent.moveto(self.input_field, x, y)
    
    def getDepth(self):
        '''returns entered depth if it is valid, -1 otherwise'''
        inp_text = self.entry.get()
        try:
            return int(inp_text)
        except:
            return -1


class ChoiceMenu(Areas):
    # for the player choices on the left
    def __init__(self, parent : Canvas, width : int, height : int, x : int, y : int, labels : list[str], gap, selected = -1):# no choice is selected by default
        self.choice_buttons = []

        N_choices = len(labels)
        for i in range(N_choices):
            self.choice_buttons.append(ChoiceButton(parent, width, height, x, y + i * height, labels[i], "sel" if i == selected else "nsel", i))
        
        self.selected = selected # no choice is selected by default
        self.height = height
        
        for i in range(N_choices):
            button = self.choice_buttons[i]
            def function(event, b, j): # to force python to STOP LAZY EVALUATION
                if event == "<Button-1>":
                    return lambda ev : self.click_param(j, ev)
                elif event == "<Enter>":
                    return lambda ev : b.set_state(parent, "hnsel") if b.get_state() == "nsel" else None
                elif event == "<Leave>":
                    return lambda ev : b.set_state(parent, "nsel") if b.get_state() == "hnsel" else None
                else: pass

            parent.tag_bind(button.hitbox, "<Button-1>", function("<Button-1>", button, i))
            parent.tag_bind(button.hitbox, "<Enter>", function("<Enter>", button, i))
            parent.tag_bind(button.hitbox, "<Leave>", function("<Leave>", button, i))
        self.parent = parent

        # create depth input field
        self.text_entry = ChoiceDepth(self.parent, width, x, y + len(self.choice_buttons) * self.height + gap, "Depth")
        # hide it if C++ AI is not selectef
        if self.get_selected()[0] != "C++ AI":
            self.text_entry.disable()
        self.GAP = gap
    

    def disable(self):
        for button in self.choice_buttons:
            button.set_state(self.parent, "gsel" if button.get_state() == "sel" else "gnsel")

    
    def moveto(self, x, y):
        '''moves the button on the ui'''
        for i in range(len(self.choice_buttons)):
            self.choice_buttons[i].moveto(self.parent, x, y + i * self.height)
        self.text_entry.moveto(x, y + len(self.choice_buttons) * self.height + self.GAP)
    

    def get_selected(self):
        '''returns selected configuration if it is valid. otherwise return (text, None) '''
        selected_text = self.choice_buttons[self.selected].get_text()
        if self.selected == -1:
            return selected_text, None
        else:
            if selected_text == "C++ AI":
                # depth must be >= 0
                selected_depth = self.text_entry.getDepth()
                return (selected_text, selected_depth) if selected_depth >= 0 else (selected_text, None)
            else:
                # the selected player is human so depth doesn't matter
                return selected_text, 0
    
    def has_valid_configuration(self):
        return self.get_selected()[1] != None
    

    def get_n_items(self):
        return len(self.choice_buttons)

    
    def click_param(self, i, event):
        '''event when clicking on an item in the list'''
        old_state = self.choice_buttons[i].get_state()
        if old_state[0] == "g" or old_state == "sel": # cannot click on grayed or selected button
            return
        assert(old_state == "hnsel") # the button must be highlighted to be clicked

        self.choice_buttons[i].set_state(self.parent, "sel") # select button i
        if self.selected != -1:
            self.choice_buttons[self.selected].set_state(self.parent, "nsel") # de-select button
        self.selected = i

        # activates or de-activates text field according to selected item
        if self.choice_buttons[i].get_text() == "C++ AI":
            self.text_entry.enable()
        else:
            self.text_entry.disable()