from tkinter import *
import math
from ChineseCheckers_Areas import *
from ChineseCheckers_Board import *

class Checkboxes(Areas):
    # for the parameter checkboxes on the left
    def __init__(self, parent : Canvas, width : int, height : int, x : int, y : int, text, board, state = "off"): # /7 for thickness
        assert(state == "on" or state == "off")
        w = height / 14
        self.parent = parent
        self.box = parent.create_rectangle(x, y, x + height, y + height, fill="", width=w, outline=self.get_color("white"))
        self.cross = self.create_cross(parent, x + height // 2, y + height // 2, height // 4, 
                                        w, self.get_color("white") if state == "on" else "")
        self.text = parent.create_text(x + 3 * height // 2, y - height // 4, text=text,
                                        fill=self.get_color("white"), font=self.get_font(16), justify=LEFT, anchor="nw")
        self.hitbox = parent.create_rectangle(x, y, x + width, y + height, fill="", outline="")
        
        self.state = state # can be hoff, hon, off, on
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
    
    def bind(self, event, i):
        if event == "<Button-1>":
            self.parent.tag_bind(self.hitbox, "<Button-1>", lambda event : self.click_param(i, event))
        elif event == "<Enter>":
            self.parent.tag_bind(self.hitbox, "<Enter>", lambda event : self.set_state("h" + self.state))
        elif event == "<Leave>":
            self.parent.tag_bind(self.hitbox, "<Leave>", lambda event : self.set_state(self.state[1:]))
        else:
            print(f"Unknown event {event}")

    
    def click_param(self, i, event):
        old_state = self.state
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

    
    def set_state(self, state):
        '''change state & modify the appearance of the button to match its new state'''
        if self.state == state:
            return
        if state[0] == self.state[0]: # color does not change
            if state == "hoff" or state == "off":
                color = ""
            elif state == "hon":
                color = self.get_color("dark")
            else:
                color = self.get_color("white")
            self.parent.itemconfigure(self.cross, outline=color)
        else:
            color = self.get_color("white") if state[0] != "h" else self.get_color("dark")
            if state[-2:] == "on": # make the cross visible
                self.parent.itemconfigure(self.cross, outline=color)
            self.parent.itemconfigure(self.box, outline=color)
            self.parent.itemconfigure(self.text, fill=color)

        self.state = state
        self.parent.update_idletasks()


class Board(Tk,Areas):

    def __init__(self, width, height):

        # TODO :
        # [DONE] fix cross centering when resizing
        # [DONE] fix black pawn position when resized
        # [DONE] fix plot2canv bizarre behavior when resizing
        # [DONE] activate arrows by default
        # [DONE] fix arrow color 
        # gray buttons to choose players during game
        # add player choice in the UI
        # re do "Play AI" button
        # [DONE] split this file into several
        # ends of games
        # relaunch game

        # fix a bug where cancelling a move (human side) doesn't gray out the "Play AI" button
        # clean code
        # cancel current move as human
        # change player when playing ?

        Tk.__init__(self)
        assert(width >= height) # we want the window to be wider than tall to fit buttons on the right
        
        # Initializing the scene
        self.title("Chinese Checkers")
        screen_height = self.winfo_screenheight()
        screen_width = self.winfo_screenwidth()
        window_x = (screen_width - width) / 2
        window_y = (screen_height - height) / 2
        self.width = width
        self.height = height
        # put the window in the center of the screen
        print(f"screen is {screen_width}x{screen_height}, putting window at ({window_x},{window_y})")
        self.geometry(f"{width}x{height}+{int(window_x)}+{int(window_y)}")
        self.aspect(width, height, width, height) # Keep the aspect ratio fixed when user resizes

        # constants for the sizes of buttons and canvas
        self.AI_BUTTON_RATIO = 1.1
        self.CONTROL_RATIO = 2.2

        # The scene is in 5 parts : on the left are parameters, in the center is the board, on the right is the "play" button
        # in the bottom & top are blank spaces
        board_side = height
        control_width = int((width -  board_side) / self.CONTROL_RATIO)
        parameters_width = width -  board_side - control_width
        ai_button_width = control_width / self.AI_BUTTON_RATIO

        # Initializing the parameters area
        self.__parametersArea = Canvas(self, width=parameters_width, height=height, highlightthickness=0)
        self.__parametersArea.configure(bg=self.get_color("background"))
        self.__parametersArea.pack(side=LEFT, padx=0, fill=BOTH, expand=YES)

        # Initializing the board
        init_states = [True, True,False]
        self.__boardArea = BoardArea(self, board_side, init_states, "C++ AI", "Human")
        self.__boardArea.addtag_all("all")
        self.__boardArea.pack(padx=0, side=LEFT, fill=BOTH)
        
        # Initializing the control area
        self.__controlArea = Canvas(self, width=control_width, height=height, highlightthickness=0)
        self.__controlArea.configure(bg=self.get_color("background"))
        self.__controlArea.pack(side=LEFT, padx=0, fill=BOTH, expand=YES)
        
        # create buttons
        self.p_buttons = [] # will store the parameters buttons
        self.BUTTONS_WIDTH = parameters_width / 1.1
        self.BUTTONS_HEIGHT = height / 25

        texts = ["Show arrows for\nwhite player moves",
                "Show arrows for\nblack player moves",
                "Show possible moves\nwhen clicking on a pawn"]
        N_buttons = len(texts)
        assert(N_buttons == len(init_states))
        start = - 2 * N_buttons + 1 / 2
        for i in range(N_buttons):
            self.p_buttons.append(Checkboxes(self.__parametersArea,
                                            self.BUTTONS_WIDTH,
                                            self.BUTTONS_HEIGHT,
                                            (parameters_width - self.BUTTONS_WIDTH) // 2,
                                            (height + (4 * i + start) * self.BUTTONS_HEIGHT) // 2,
                                            texts[i],
                                            self.__boardArea))
        self.p_buttons[0].set_state("on")
        self.p_buttons[1].set_state("on")
        
        
        # "PLAY" button
        icon = PhotoImage(file="icons/ai_button.png")
        icon_pressed = PhotoImage(file="icons/ai_button_pressed.png")
        icon_grayed = PhotoImage(file="icons/ai_button_grayed.png")

        scale = ai_button_width / icon.width() # scales button
        self.play_ai_icon = self.scale_img(icon, scale)

        scale = ai_button_width / icon_pressed.width() # scales pressed button
        self.play_ai_icon_pressed = self.scale_img(icon_pressed, scale)

        scale = ai_button_width / icon_grayed.width() # scales grayed button
        self.play_ai_icon_grayed = self.scale_img(icon_grayed, scale)
        
        self.AI_button = self.__controlArea.create_image(control_width / 2,
                                                        height / 2,
                                                        image=self.play_ai_icon)
        self.AI_button_state = "normal" # can be "grayed", "normal" or "pressed"
        
        # mouse events config
        self.__controlArea.tag_bind(self.AI_button, "<Button-1>", self.press_jouerIA)
        self.__controlArea.tag_bind(self.AI_button, "<ButtonRelease-1>", self.release_jouerIA)

        for i in range(N_buttons):
            self.p_buttons[i].bind("<Button-1>", i)
            self.p_buttons[i].bind("<Enter>", i)
            self.p_buttons[i].bind("<Leave>", i)
                                        
        self.__boardArea.bind("<ButtonRelease-1>", self.release_pawn)
        self.__boardArea.bind("<Button-1>", self.__boardArea.pawn_pressed)
        self.__boardArea.bind("<B1-Motion>", self.__boardArea.pawn_moved)
        self.bind("<Configure>", self.on_resize)

    def scale_img(self, img, scale):
        N = 25
        return (img.zoom(int(N * scale), int(N * scale))).subsample(N, N)
    
    def create_rounded_rect(self, canvas, x1, y1, x2, y2, r, w, c):
        
        points = (x1+r, y1, x1+r, y1, x2-r, y1, x2-r, y1, x2, y1, x2, y1+r, x2, y1+r, x2, y2-r, x2, \
            y2-r, x2, y2, x2-r, y2, x2-r, y2, x1+r, y2, x1+r, y2, x1, y2, x1, y2-r, x1, y2-r, x1, y1+r, x1, y1+r, x1, y1)
        return canvas.create_polygon(points, width=w, outline=c, fill="", smooth=True)

    def press_jouerIA(self, event):
        ''' changes button appearance and makes the AI play on press '''
        if self.AI_button_state == "normal":
            # if the button is normal (i.e not grayed), change it to pressed
            self.__controlArea.itemconfigure(self.AI_button, image=self.play_ai_icon_pressed)
            self.__controlArea.update_idletasks()
            self.__boardArea.jouerIA()
            self.AI_button_state = "normal"
    
    def release_jouerIA(self, event):
        ''' changes button appearance on release '''
        if self.AI_button_state == "pressed":
            # if the button is pressed, make the AI play and change the button to grayed
            self.__controlArea.itemconfigure(self.AI_button, image=self.play_ai_icon_grayed)
            self.__controlArea.update_idletasks()
            self.AI_button_state = "grayed"
    
    def release_pawn(self, event):
        '''wrapper function to update the status of the Play AI button when pawn in released'''
        self.__boardArea.pawn_released(event)
        if self.__boardArea.can_play():
            # if human has moved a pawn, change the button from grayed to normal
            self.__controlArea.itemconfigure(self.AI_button, image=self.play_ai_icon)
            self.__controlArea.update_idletasks()
            self.AI_button_state = "normal"
    
    def on_resize(self, event):
        ''' called on resize events. controls all three canvas '''

        # filter out calls so that the function is called only once overall at each resize, instead of once per widget
        if event.widget == self and self.height != event.height:
            self.height = event.height

            self.__boardArea.on_resize(event) # resize boardArea

            # fixes buttons positions
            control_width = (event.width - event.height) / self.CONTROL_RATIO
            w = self.play_ai_icon.width()
            h = self.play_ai_icon.height()
            self.__controlArea.moveto(self.AI_button, (control_width - w) // 2, (event.height - h) // 2)

            parameters_width = event.width - control_width - event.height
            N_buttons = len(self.p_buttons)
            start = - 2 * N_buttons + 1 / 2
            for i in range(N_buttons):
                self.p_buttons[i].moveto((parameters_width - self.BUTTONS_WIDTH) // 2,
                                                 (event.height + (4 * i + start) * self.BUTTONS_HEIGHT) // 2)
                                                 
            self.__parametersArea.update_idletasks()
            self.__controlArea.update_idletasks()



if __name__ == "__main__":
    
    fen = Board(1300, 700)
    fen.mainloop()
