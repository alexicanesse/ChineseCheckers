from tkinter import *
from ChineseCheckers_Areas import *
from ChineseCheckers_Board import *
from ChineseCheckers_Buttons import *

class Board(Tk,Areas):

    def __init__(self, width, height):

        # TODO :
        # [DONE] fix cross centering when resizing
        # [DONE] fix black pawn position when resized
        # [DONE] fix plot2canv
        # [DONE] activate arrows by default
        # [DONE] fix arrow color 
        # [DONE] gray buttons to choose players during game
        # [DONE] add player choice in the UI
        # [DONE] re do "Play AI" button
        # [DONE] split this file into several
        # [MOSTLY DONE] ends of games
        # [DONE] relaunch game
        # allow user to specify depth for AI
        
        # new window that shows weights
        # when disabling then enabling "show arrows" checkbox, show arrows that were previously shown before disabling
        # fix classicbuttons colors when pressed
        # fix a bug where cancelling a move (human side) doesn't gray out the "Play AI" button
        # clean code
        # cancel current move as human
        # change player when playing ?
        # gray buttons when "new game" hasn't been pressed at all
        # make in-depth tests of "new game" button
        # fix moments when buttons become grayed
        # show possible moves when clicking on a pawn even if it is AI
        # bug where u can drag black pawns even if black player is not human
        # diverse bugs with new_game function

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

        # The scene is in 5 parts : on the left are parameters, in the center is the board, on the right is the "play" button
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
        self.__boardArea = BoardArea(self, board_side, init_states,default_playerW,default_playerB)
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
        
        # "New Game" button
        self.NEW_GAME_WIDTH = control_width // 2
        self.NEW_GAME_HEIGHT = self.ITEM_HEIGHT
        self.new_game_button = ClassicButton(self.__controlArea, 
                                        self.NEW_GAME_WIDTH,
                                        self.NEW_GAME_HEIGHT,
                                        (control_width - self.NEW_GAME_WIDTH) // 2,
                                        height // 3 - self.NEW_GAME_HEIGHT // 2,
                                        "New game",
                                        "normal")
        
        # "Next turn" button
        self.TURN_WIDTH = control_width // 2.2
        self.TURN_HEIGHT = height // 12
        self.nextturn_b = ClassicButton(self.__controlArea, 
                                        self.TURN_WIDTH,
                                        self.TURN_HEIGHT,
                                        (control_width - self.TURN_WIDTH) // 2,
                                        2 * height // 3 - self.TURN_HEIGHT // 2,
                                        "Next turn",
                                        "normal")
        
        # mouse events config
        self.__controlArea.tag_bind(self.nextturn_b.hitbox, "<Button-1>", self.press_NextTurn)
        self.__controlArea.tag_bind(self.new_game_button.hitbox, "<Button-1>", self.press_NewGame)

        for i in range(N_buttons):
            self.p_buttons[i].bind(i)
                                        
        self.__boardArea.bind("<ButtonRelease-1>", self.release_pawn)
        self.__boardArea.bind("<Button-1>", self.__boardArea.pawn_pressed)
        self.__boardArea.bind("<B1-Motion>", self.__boardArea.pawn_moved)
        self.bind("<Configure>", self.on_resize)

    def press_NextTurn(self, event):
        ''' next turn '''

        if self.nextturn_b.get_state() != "grayed":
            self.__boardArea.jouerIA()

    def game_is_over(self,type_of_end : int):#triggered when game is over
        print("Game's over !",type_of_end)
    
    def press_NewGame(self, event):
        ''' begins a new game '''

        if self.new_game_button.get_state() == "grayed":
            return

        choice_playerB = self.playerB_menu.get_selected()
        choice_playerW = self.playerW_menu.get_selected()
        depthB = self.playerB_menu.getInputDepth()
        depthW = self.playerW_menu.getInputDepth()
        if choice_playerB != "" and choice_playerW != "": # if human has chosen both
            self.nextturn_b.set_state("normal") # enable next turn button

            playerW = AI_cpp(depthW) if choice_playerW == "C++ AI" else Human() # TODO ckoicebordel
            playerB = AI_cpp(depthB) if choice_playerB == "C++ AI" else Human()
            self.__boardArea.reset(playerW,playerB)
        else:
            print("No choice of players has been made")
            
    
    def release_pawn(self, event):
        '''wrapper function to update the status of the Play AI button when pawn in released'''
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

            # fix buttons positions
            self.new_game_button.moveto((control_width - self.NEW_GAME_WIDTH) // 2, event.height // 3 - self.NEW_GAME_HEIGHT // 2) 
            self.nextturn_b.moveto((control_width - self.TURN_WIDTH) // 2, 2 * event.height // 3 - self.TURN_HEIGHT // 2)
                                                 
            self.__parametersArea.update_idletasks()
            self.__controlArea.update_idletasks()



if __name__ == "__main__":
    
    fen = Board(1300, 700)
    fen.mainloop()
