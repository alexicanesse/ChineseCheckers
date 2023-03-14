from tkinter import *
from ChineseCheckers_Areas import *
from ChineseCheckers_Board import *
from ChineseCheckers_Buttons import *


class Board(Tk,Areas):

    def __init__(self, width, height):
        
        # new window that shows weights
        # clean code

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
        self.CONTROL_RATIO = 2.2

        # The scene is in 3 parts : on the left are parameters, in the center is the board, on the right is the "play" button
        # in the bottom & top are blank spaces
        board_side = height
        control_width = int((width -  board_side) / self.CONTROL_RATIO)
        parameters_width = width -  board_side - control_width

        # Initializing the parameters area
        self.__parametersArea = Canvas(self, width=parameters_width, height=height, highlightthickness=0)
        self.__parametersArea.configure(bg=self.get_color("background"))
        self.__parametersArea.pack(side=LEFT, padx=0, fill=BOTH, expand=YES)
        
        #default players
        default_selectedW = 1
        default_depthW = 30
        default_selectedB = 0
        default_depthB = 30
        
        default_playerW = Human() if default_selectedW == 1 else AI_cpp(depth = default_depthW)
        default_playerB = Human() if default_selectedB == 1 else AI_cpp(depth = default_depthB)

        # Initializing the board
        init_states = [True, True,False]
        self.__boardArea = BoardArea(self, board_side, init_states, default_playerW, default_playerB)
        self.__boardArea.addtag_all("all")
        self.__boardArea.pack(padx=0, side=LEFT, fill=BOTH)
        

        
        # Initializing the control area
        self.__controlArea = Canvas(self, width=control_width, height=height, highlightthickness=0)
        self.__controlArea.configure(bg=self.get_color("background"))
        self.__controlArea.pack(side=LEFT, padx=0, fill=BOTH, expand=YES)
        
        # create checkboxes
        self.p_buttons = [] # will store the parameters buttons
        self.BUTTONS_WIDTH = parameters_width / 1.1
        self.BUTTONS_HEIGHT = height / 25

        texts = ["Show arrows for\nwhite player moves",
                "Show arrows for\nblack player moves",
                "Show possible moves\nwhen clicking on a pawn"]
        N_buttons = len(texts)
        assert(N_buttons == len(init_states))
        start = + N_buttons / 2
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
    
        # create player choice menus and associated decoration elements
        self.ITEM_WIDTH = parameters_width / 3.2
        self.ITEM_HEIGHT = 1.5 * self.BUTTONS_HEIGHT
        choices = ["Human", "C++ AI"]
        menu_height = len(choices) * self.ITEM_HEIGHT
        x = (parameters_width - 2 * self.ITEM_WIDTH) // 3
        y = (height - self.ITEM_HEIGHT) // 6
        self.deco_elts = []
        self.DECO_WIDTH = 2 * x // 5
        self.deco_elts.append(self.__parametersArea.create_text(parameters_width // 2,
                                                                y + menu_height // 2,
                                                                text="VS", 
                                                                font=self.get_font(15), 
                                                                fill=self.get_color("darkgreen")))
        self.deco_elts.append(self.__parametersArea.create_rectangle(x - 2 * self.DECO_WIDTH,
                                                                    y, 
                                                                    x - self.DECO_WIDTH, 
                                                                    y + menu_height, 
                                                                    fill=self.get_color("white"), 
                                                                    outline="black"))
        self.deco_elts.append(self.__parametersArea.create_rectangle(2 * x + 2 * self.ITEM_WIDTH + self.DECO_WIDTH, 
                                                                    y, 
                                                                    2 * x + 2 * self.ITEM_WIDTH + 2 * self.DECO_WIDTH, 
                                                                    y + menu_height, 
                                                                    fill=self.get_color("black"), 
                                                                    outline="black"))
        self.playerW_menu = ChoiceMenu(self.__parametersArea, 
                                        self.ITEM_WIDTH,
                                        self.ITEM_HEIGHT, 
                                        x, 
                                        y, 
                                        choices, 
                                        self.DECO_WIDTH,
                                        default_selectedW)
        self.playerB_menu = ChoiceMenu(self.__parametersArea, 
                                        self.ITEM_WIDTH, 
                                        self.ITEM_HEIGHT, 
                                        self.ITEM_WIDTH + 2 * x, 
                                        y, 
                                        choices, 
                                        self.DECO_WIDTH,
                                        default_selectedB)
        # visual indicator that shows game infos (whose turn..)
        self.text_info_players = self.__parametersArea.create_text(parameters_width // 2,
                                                                menu_height // 2,
                                                                text="Choose the \nparameters", 
                                                                font=self.get_font(13), 
                                                                justify=CENTER,
                                                                fill=self.get_color("white"))


        
        # "New Game" button
        self.NEW_GAME_WIDTH = control_width // 2
        self.NEW_GAME_HEIGHT = height // 15
        self.new_game_button = ClassicButton(self.__controlArea, 
                                        self.NEW_GAME_WIDTH,
                                        self.NEW_GAME_HEIGHT,
                                        (control_width - self.NEW_GAME_WIDTH) // 2,
                                        height // 4 - self.NEW_GAME_HEIGHT // 2,
                                        "New game",
                                        "grayed",
                                        "First choose a valid\nconfiguration on the left")
        
        # "Next turn" button
        self.TURN_WIDTH = control_width // 2.2
        self.TURN_HEIGHT = height // 15
        self.nextturn_b = ClassicButton(self.__controlArea, 
                                        self.TURN_WIDTH,
                                        self.TURN_HEIGHT,
                                        (control_width - self.TURN_WIDTH) // 2,
                                        2 * height // 4 - self.TURN_HEIGHT // 2,
                                        "Next turn",
                                        "grayed",
                                        "First start a new\ngame and play a move")
        
        # "Cancel move" button
        self.CANCEL_WIDTH = control_width // 1.7
        self.CANCEL_HEIGHT = height // 15
        self.cancelmove_b = ClassicButton(self.__controlArea, 
                                        self.CANCEL_WIDTH,
                                        self.CANCEL_HEIGHT,
                                        (control_width - self.CANCEL_WIDTH) // 2,
                                        3 * height // 4 - self.CANCEL_HEIGHT // 2,
                                        "Cancel move",
                                        "grayed",
                                        "First make a move\nas a human player")
        
        # mouse events config
        self.__controlArea.tag_bind(self.nextturn_b.hitbox, "<Button-1>", self.press_NextTurn)
        self.__controlArea.tag_bind(self.new_game_button.hitbox, "<Button-1>", self.press_NewGame)
        self.__controlArea.tag_bind(self.cancelmove_b.hitbox, "<Button-1>", self.press_CancelMove)

        for i in range(N_buttons):
            self.p_buttons[i].bind(i)
                                        
        self.__boardArea.bind("<ButtonRelease-1>", self.release_pawn)
        self.__boardArea.bind("<Button-1>", self.__boardArea.pawn_pressed)
        self.__boardArea.bind("<B1-Motion>", self.__boardArea.pawn_moved)
        self.bind("<Configure>", self.on_resize)

        # create a parametersArea hitbox to trigger update events
        self.bind("<ButtonRelease-1>", lambda event : self.updateClassicButtons(event))
        self.bind("<KeyRelease>", lambda event : self.updateClassicButtons(event))

        # save current game parameters to show them on UI
        self.current_params = {"playerW" : None,
                               "playerB" : None,
                               "turn" : "White"}
    

    def updateClassicButtons(self, event):
        ''' this function is run at each click in the window or key press '''

        # First update New Game button
        if self.playerB_menu.has_valid_configuration() and self.playerW_menu.has_valid_configuration():
            if self.new_game_button.get_state() == "grayed":
                # new game must be not grayed
                self.new_game_button.set_state("normal")
        else:
            # no valid configuration is selected, should be grayed
            self.new_game_button.set_state("grayed")
        
        # Then update Next Turn button
        # it must be grayed if human has no current move
        if self.__boardArea.whoistoplay != None and \
            self.__boardArea.whoistoplay.getHumanity() and \
            self.__boardArea.coup_courant == []:
            self.nextturn_b.set_state("grayed")
        elif self.nextturn_b.get_state() == "grayed":
            self.nextturn_b.set_state("normal")
        
        # Then update Cancel Move button
        # it must be grayed if human has no current move
        if self.__boardArea.whoistoplay != None and \
            self.__boardArea.whoistoplay.getHumanity() and \
            self.__boardArea.coup_courant != []:
            self.cancelmove_b.set_state("normal")
        else:
            self.cancelmove_b.set_state("grayed")
        
    def updateGameInfos(self):
        ''' update the visual indications that show who plays what, and whose turn it is '''

        self.current_params["turn"] = self.__boardArea.get_color_playing()
        turn = self.current_params["turn"]
        playerW = self.current_params["playerW"]
        playerB = self.current_params["playerB"]

        self.__parametersArea.itemconfig(self.text_info_players, text=f"White {playerW} VS Black {playerB}\nIt's {turn}'s turn")


    def press_NextTurn(self, event):
        ''' next turn '''

        if self.nextturn_b.get_state() != "grayed":
            self.__boardArea.jouerIA()
            self.nextturn_b.set_state("pressed")
            self.updateGameInfos()

    def press_CancelMove(self, event):
        ''' next turn '''

        if self.cancelmove_b.get_state() != "grayed":
            self.__boardArea.cancel_current_move()
            self.nextturn_b.set_state("pressed")

    def game_is_over(self,type_of_end : int):#triggered when game is over
        print("Game's over !",type_of_end)
    
    def press_NewGame(self, event):
        ''' begins a new game '''

        if self.new_game_button.get_state() == "grayed":
            return
        else:
            self.new_game_button.set_state("pressed")

        choice_playerB, depthB = self.playerB_menu.get_selected()
        choice_playerW, depthW = self.playerW_menu.get_selected()
        if self.playerB_menu.has_valid_configuration() and self.playerW_menu.has_valid_configuration(): # if human has chosen both
            self.nextturn_b.set_state("normal") # enable next turn button

            playerW = AI_cpp(depthW) if choice_playerW == "C++ AI" else Human()
            playerB = AI_cpp(depthB) if choice_playerB == "C++ AI" else Human()
            self.__boardArea.reset(playerW,playerB)

            self.current_params["playerW"] = choice_playerW
            self.current_params["playerB"] = choice_playerB
            self.current_params["turn"] = "White"
        else:
            print("No choice of players has been made")
        
        self.updateGameInfos()
            
    
    def release_pawn(self, event):
        '''wrapper function to update the status of the Next Turn button when pawn in released'''
        self.__boardArea.pawn_released(event)
    
    def on_resize(self, event):
        ''' called on resize events. controls all three canvas '''

        # filter out calls so that the function is called only once overall at each resize, instead of once per widget
        if event.widget == self and self.height != event.height:
            self.height = event.height

            self.__boardArea.on_resize(event) # resize boardArea

            # fixes checkboxes positions
            control_width = (event.width - event.height) / self.CONTROL_RATIO
            parameters_width = event.width - control_width - event.height
            N_buttons = len(self.p_buttons)
            start = + N_buttons / 2
            for i in range(N_buttons):
                self.p_buttons[i].moveto((parameters_width - self.BUTTONS_WIDTH) // 2,
                                                 (event.height + (4 * i + start) * self.BUTTONS_HEIGHT) // 2)
            x = (parameters_width - 2 * self.ITEM_WIDTH) // 3
            y = (event.height - self.ITEM_HEIGHT) // 6
            self.playerW_menu.moveto(x, y)
            self.playerB_menu.moveto(2 * x + self.ITEM_WIDTH, y)

            # fixes deco position
            menu_height = self.ITEM_HEIGHT * self.playerB_menu.get_n_items()
            a, b, c, d = self.__parametersArea.bbox(self.deco_elts[0])
            text_width, text_height = c - a, d - b
            self.__parametersArea.moveto(self.deco_elts[0], (parameters_width - text_width) // 2, y + (menu_height - text_height) // 2)
            self.__parametersArea.moveto(self.deco_elts[1], x - 2 * self.DECO_WIDTH, y)
            self.__parametersArea.moveto(self.deco_elts[2], 2 * x + 2 * self.ITEM_WIDTH + self.DECO_WIDTH, y) 
            a, b, c, d = self.__parametersArea.bbox(self.text_info_players)
            text_width, text_height = c - a, d - b
            self.__parametersArea.moveto(self.text_info_players, (parameters_width - text_width) // 2, menu_height // 2 - text_height // 2)

            # fix buttons positions
            self.new_game_button.moveto((control_width - self.NEW_GAME_WIDTH) // 2, event.height // 4 - self.NEW_GAME_HEIGHT // 2) 
            self.nextturn_b.moveto((control_width - self.TURN_WIDTH) // 2, 2 * event.height // 4 - self.TURN_HEIGHT // 2)
            self.cancelmove_b.moveto((control_width - self.CANCEL_WIDTH) // 2, 3 * event.height // 4 - self.NEW_GAME_HEIGHT // 2) 
                                                 
            self.__parametersArea.update_idletasks()
            self.__controlArea.update_idletasks()



if __name__ == "__main__":
    
    fen = Board(1800, 900)
    fen.mainloop()
