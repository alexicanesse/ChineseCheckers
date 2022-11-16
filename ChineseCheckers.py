from tkinter import *
import math
from ChineseCheckers_Areas import *
from ChineseCheckers_Board import *

class Board(Tk,Areas):

    def __init__(self, width, height):

        # TODO :
        # [DONE] activate arrows by default
        # [DONE] fix arrow color 
        # add player choice in the UI
        # re do "Play AI" button
        # [DONE] split this file into several
        # ends of games
        # relaunch game

        # fix a bug where cancelling a move (human side) doesn't gray out the "Play AI" button
        # clean code
        # cancel current move as human

        Tk.__init__(self)
        assert(width >= height) # we want the window to be wider than tall to fit buttons on the right
        
        # Initializing the scene
        self.title("ACABette")
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
        MIN_WIDTH = 1300 
        self.minsize(MIN_WIDTH, MIN_WIDTH * int(height/width))

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
        init_states = [True, True, False]
        self.__boardArea = BoardArea(self, board_side, board_side, init_states, "Human", "Human")
        self.__boardArea.addtag_all("all")
        self.__boardArea.pack(padx=0, side=LEFT, fill=BOTH)
        
        # Initializing the control area
        self.__controlArea = Canvas(self, width=control_width, height=height, highlightthickness=0)
        self.__controlArea.configure(bg=self.get_color("background"))
        self.__controlArea.pack(side=LEFT, padx=0, fill=BOTH, expand=YES)
        
        # create buttons
        self.parameters_buttons = [] # will store the parameters buttons
        self.BUTTONS_WIDTH = parameters_width / 1.1
        self.BUTTONS_HEIGHT = height / 25

        texts = ["Show arrows for\nwhite player moves",
                "Show arrows for\nblack player moves",
                "Show possible moves\nwhen clicking on a pawn"]
        N_buttons = len(texts)
        start = - 2 * N_buttons + 1 / 2
        self.RECT_THICKNESS_RATIO = 350
        w = height / self.RECT_THICKNESS_RATIO # width of rectangle lines
        for i in range(N_buttons):
            new_button = self.__parametersArea.create_rectangle(
                                                (parameters_width - self.BUTTONS_WIDTH) // 2,
                                                (height + (4 * i + start) * self.BUTTONS_HEIGHT) // 2,
                                                (parameters_width - self.BUTTONS_WIDTH + 2 * self.BUTTONS_HEIGHT) // 2,
                                                (height + (4 * i + start + 2) * self.BUTTONS_HEIGHT) // 2,
                                                fill="",
                                                width=w,
                                                outline=self.get_color("white"))
            new_cross = self.create_cross(self.__parametersArea,
                                                (parameters_width - self.BUTTONS_WIDTH + self.BUTTONS_HEIGHT) // 2,
                                                (height + (4 * i + start + 1) * self.BUTTONS_HEIGHT) // 2,
                                                self.BUTTONS_HEIGHT // 4,
                                                w,
                                                self.get_color("white") if init_states[i] else "")
            new_text = self.__parametersArea.create_text(
                                                (parameters_width - self.BUTTONS_WIDTH + 3 * self.BUTTONS_HEIGHT) // 2,
                                                (height + (4 * i + start - 0.7) * self.BUTTONS_HEIGHT) // 2,
                                                text=texts[i],
                                                fill=self.get_color("white"),
                                                font=self.get_font(16),
                                                justify=LEFT,
                                                anchor="nw")
            new_hitbox = self.__parametersArea.create_rectangle(
                                                (parameters_width - self.BUTTONS_WIDTH) // 2,
                                                (height + (4 * i + start) * self.BUTTONS_HEIGHT) // 2,
                                                (parameters_width + self.BUTTONS_WIDTH) // 2,
                                                (height + (4 * i + start + 2) * self.BUTTONS_HEIGHT) // 2,
                                                fill="", # make the hitbox invisible
                                                outline="")
            # first coordinate : the button. Second : its state. Third : its associated text. Fourth : hitbox
            self.parameters_buttons.append({
                                        "button": new_button,
                                        "state": "on" if init_states[i] else "off", # can be "on", "off", "hon" or "hoff"
                                        "text": new_text,
                                        "hitbox": new_hitbox,
                                        "cross": new_cross
                                    })
        
        
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
            self.__parametersArea.tag_bind(self.parameters_buttons[i]["hitbox"], "<Button-1>",
                                        self.get_click_f(i))
            self.__parametersArea.tag_bind(self.parameters_buttons[i]["hitbox"], "<Enter>",
                                        self.get_enter_f(i))
            self.__parametersArea.tag_bind(self.parameters_buttons[i]["hitbox"], "<Leave>",
                                        self.get_leave_f(i))
                                        
        self.__boardArea.bind("<ButtonRelease-1>", self.release_pawn)
        self.__boardArea.bind("<Button-1>", self.__boardArea.pawn_pressed)
        self.__boardArea.bind("<B1-Motion>", self.__boardArea.pawn_moved)
        self.bind("<Configure>", self.on_resize)
    
    def get_click_f(self, i): # returns the function that will be called when clicking on button i 
        return lambda event: self.click_param(i, event)

    def get_enter_f(self, i):
        return lambda event: self.hover_param(i, event)

    def get_leave_f(self, i):
        return lambda event: self.end_hover_param(i, event)

    def scale_img(self, img, scale):
        N = 25
        return (img.zoom(int(N * scale), int(N * scale))).subsample(N, N)
    
    def create_rounded_rect(self, canvas, x1, y1, x2, y2, r, w, c):
        
        points = (x1+r, y1, x1+r, y1, x2-r, y1, x2-r, y1, x2, y1, x2, y1+r, x2, y1+r, x2, y2-r, x2, \
            y2-r, x2, y2, x2-r, y2, x2-r, y2, x1+r, y2, x1+r, y2, x1, y2, x1, y2-r, x1, y2-r, x1, y1+r, x1, y1+r, x1, y1)
        return canvas.create_polygon(points, width=w, outline=c, fill="", smooth=True)
    
    def create_cross(self, canvas, x, y, s, w, c):
        
        offset = math.sqrt(2) * s
        points = (x - offset, y - offset, x, y, \
                x + offset, y - offset, x, y, \
                x + offset, y + offset, x, y, \
                x - offset, y + offset, x, y)
        return canvas.create_polygon(points, width=w, outline=c, fill="")


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
    
    def click_param(self, i, event):
        old_state = self.parameters_buttons[i]["state"]
        assert(old_state == "hoff" or old_state == "hon") # the button must be highlighted

        self.parameters_buttons[i]["state"] = "hoff" if old_state == "hon" else "hon"
        if i == 0: # show white arrows button
            self.__boardArea.set_parameter("show_white_ar", not self.__boardArea.show_white_ar)
            if not self.__boardArea.show_white_ar:
                self.__boardArea.show_arrows([], "white") # removes existing arrows
        if i == 1: # show black arrows button
            self.__boardArea.set_parameter("show_black_ar", not self.__boardArea.show_black_ar)
            if not self.__boardArea.show_black_ar:
                self.__boardArea.show_arrows([], "black") # removes existing arrows
        if i == 2: # show moves button
            self.__boardArea.set_parameter("show_moves", not self.__boardArea.show_moves)
            if not self.__boardArea.show_moves:
                self.__boardArea.highlight_cases([]) # removes existing highlighted cases

        if old_state == "hoff": # button now activated : show the cross
            self.__parametersArea.itemconfigure(self.parameters_buttons[i]["cross"], outline=self.get_color("dark"))
        else: # button now deactivated : hide the cross
            self.__parametersArea.itemconfigure(self.parameters_buttons[i]["cross"], outline="")
        self.__parametersArea.update_idletasks()
        
    
    def hover_param(self, i, event):
        old_state = self.parameters_buttons[i]["state"]
        assert(old_state == "off" or old_state == "on") # the button must be highlighted

        self.parameters_buttons[i]["state"] = "hoff" if old_state == "off" else "hon"

        # change color of buttons
        self.__parametersArea.itemconfigure(self.parameters_buttons[i]["button"], outline=self.get_color("dark"))
        self.__parametersArea.itemconfigure(self.parameters_buttons[i]["text"], fill=self.get_color("dark"))
        if old_state == "on": # change the color of the cross
            self.__parametersArea.itemconfigure(self.parameters_buttons[i]["cross"], outline=self.get_color("dark"))
    
    def end_hover_param(self, i, event):
        old_state = self.parameters_buttons[i]["state"]
        assert(old_state == "hoff" or old_state == "hon") # the button must be highlighted

        self.parameters_buttons[i]["state"] = "off" if old_state == "hoff" else "on"

        # change color of buttons
        self.__parametersArea.itemconfigure(self.parameters_buttons[i]["button"], outline=self.get_color("white"))
        self.__parametersArea.itemconfigure(self.parameters_buttons[i]["text"], fill=self.get_color("white"))
        if old_state == "hon": # change the color of the cross
            self.__parametersArea.itemconfigure(self.parameters_buttons[i]["cross"], outline=self.get_color("white"))
    
    def release_pawn(self, event):
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
            N_buttons = len(self.parameters_buttons)
            start = - 2 * N_buttons + 1 / 2
            for i in range(N_buttons):
                self.__parametersArea.moveto(self.parameters_buttons[i]["button"],
                                            (parameters_width - self.BUTTONS_WIDTH) // 2,
                                            (event.height + (4 * i + start) * self.BUTTONS_HEIGHT) // 2)
                self.__parametersArea.moveto(self.parameters_buttons[i]["text"],
                                                (parameters_width - self.BUTTONS_WIDTH + 3 * self.BUTTONS_HEIGHT) // 2,
                                                (event.height + (4 * i + start - 0.7) * self.BUTTONS_HEIGHT) // 2)
                self.__parametersArea.moveto(self.parameters_buttons[i]["hitbox"],
                                            (parameters_width - self.BUTTONS_WIDTH) // 2,
                                            (event.height + (4 * i + start) * self.BUTTONS_HEIGHT) // 2)
                offset = (self.BUTTONS_HEIGHT // 2 - int(self.BUTTONS_HEIGHT / (2 * math.sqrt(2)))) // 2
                self.__parametersArea.moveto(self.parameters_buttons[i]["cross"],
                                            (parameters_width - self.BUTTONS_WIDTH) // 2 + offset,
                                            (event.height + (4 * i + start) * self.BUTTONS_HEIGHT) // 2 + offset)
            self.__parametersArea.update_idletasks()
            self.__controlArea.update_idletasks()



if __name__ == "__main__":
    
    fen = Board(1300, 700)
    fen.mainloop()
