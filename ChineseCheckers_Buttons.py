from tkinter import *
import math
import numpy as np
from ChineseCheckers_Areas import *

class ClassicButton(Areas):
    # for the "Go" and "End Turn" button
    def __init__(self, parent : Canvas, width : int, height : int, x : int, y : int, text, state):
        assert(state in {"normal", "grayed"})
        c = self.get_color("darkgreen") if state == "normal" else self.get_color("gray")
        r = height // 4
        self.parent = parent
        self.box = self.create_rounded_rect(parent, x, y, x + width, y + height, r, c)
        self.text = parent.create_text(x + width // 2, y + height // 2, text=text,
                                        fill=self.get_color("white"), font=self.get_font(16), justify=CENTER)
        self.hitbox = self.create_rounded_rect(parent, x, y, x + width, y + height, r, "") # invisible hitbox
        self.state = state
        self.width = width
        self.height = height
                                        
        self.parent.tag_bind(self.hitbox, "<Enter>", lambda event : self.set_state("hovered") if self.state != "grayed" else None)
        def f(event):
            if self.state == "hovered":
                self.set_state("normal")
            elif self.state == "pressed":
                self.set_state("grayed")

        self.parent.tag_bind(self.hitbox, "<Leave>", f)

    
    def create_rounded_rect(self, canvas, x1, y1, x2, y2, r, c):
        # r is corner radius, c is color (fill)
        points = (x1+r, y1, x1+r, y1, x2-r, y1, x2-r, y1, x2, y1, x2, y1+r, x2, y1+r, x2, y2-r, x2, \
            y2-r, x2, y2, x2-r, y2, x2-r, y2, x1+r, y2, x1+r, y2, x1, y2, x1, y2-r, x1, y2-r, x1, y1+r, x1, y1+r, x1, y1)
        return canvas.create_polygon(points, outline="", fill=c, smooth=True)

    
    def moveto(self, x, y):
        self.parent.moveto(self.box, x, y)
        self.parent.moveto(self.hitbox, x, y)
        a, b, c, d = self.parent.bbox(self.text)
        text_width, text_height = c - a, d - b
        self.parent.moveto(self.text, x + (self.width - text_width) // 2, y + (self.height - text_height) // 2)


    def set_state(self, state):
        if state == "pressed":
            c = self.get_color("pressed")
        elif state == "normal":
            c = self.get_color("darkgreen")
        elif state == "grayed":
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
    def __init__(self, parent : Canvas, width : int, height : int, x : int, y : int, text, board, state = "off"):
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
            if not self.board.show_white_ar:
                self.board.show_arrows([], "white") # removes existing arrows
        elif i == 1: # show black arrows button
            self.board.set_parameter("show_black_ar", not self.board.show_black_ar)
            if not self.board.show_black_ar:
                self.board.show_arrows([], "black") # removes existing arrows
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

class ChoiceMenu(Areas):
    # for the player choices on the left
    def __init__(self, parent : Canvas, width : int, height : int, x : int, y : int, labels : list[str],selected = -1):# no choice is selected by default
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
    

    def disable(self):
        for button in self.choice_buttons:
            button.set_state(self.parent, "gsel" if button.get_state() == "sel" else "gnsel")

    
    def moveto(self, x, y):
        '''moves the button on the ui'''
        for i in range(len(self.choice_buttons)):
            self.choice_buttons[i].moveto(self.parent, x, y + i * self.height)
    

    def get_selected(self):
        '''returns the text of the selected button if it exists. otherwise return "" '''
        if self.selected == -1:
            return ""
        else:
            return self.choice_buttons[self.selected].get_text()
    

    def get_n_items(self):
        return len(self.choice_buttons)

    
    def click_param(self, i, event):
        old_state = self.choice_buttons[i].get_state()
        if old_state[0] == "g" or old_state == "sel": # cannot click on grayed or selected button
            return
        assert(old_state == "hnsel") # the button must be highlighted to be clicked

        self.choice_buttons[i].set_state(self.parent, "sel") # select button i
        if self.selected != -1:
            self.choice_buttons[self.selected].set_state(self.parent, "nsel") # de-select button
        self.selected = i
        

