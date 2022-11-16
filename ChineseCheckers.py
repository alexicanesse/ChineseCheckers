from tkinter import *
import tkinter.font as font
import numpy as np
import math
from ChineseCheckers_Players import *
import bin.libChineseCheckers as cc


import time #TEST


class Pion:
    
    def __init__(self, canvas, x : float, y : float, color = "white"):
        self._color = color
        self._canvas = canvas
        self._item = canvas.create_oval(x - self._canvas.pawn_radius,
                                         y - self._canvas.pawn_radius,
                                         x + self._canvas.pawn_radius,
                                         y + self._canvas.pawn_radius,
                                         fill =self._color,
                                         outline="black",
                                         width=2)
        self.x = x
        self.y = y
        self.case_x, self.case_y = self._canvas._canv2plat(x,y)
    
    def __str__(self):
        c = "blanc" if self._color == "white" else "noir"
        return(f"Pion {c} ({self.case_x},{self.case_y})")
    
    def __repr__(self):
        c = "b" if self._color =="white" else "n"
        return(f"P{c}{self.case_x}{self.case_y}")

    def set_xy(self, x : float, y : float):
        self.x = x
        self.y = y
    
    def redraw(self): 
        ''' deletes and redraws the pawn '''
        self._canvas.delete(self._item)
        self._item = self._canvas.create_oval(self.x - self._canvas.pawn_radius,
                                         self.y - self._canvas.pawn_radius,
                                         self.x + self._canvas.pawn_radius,
                                         self.y + self._canvas.pawn_radius,
                                         fill=self._color,
                                         outline="black",
                                         width=2)
        
    def move_on_ui(self, dx : float, dy : float): 
        ''' moves the pawn on the UI '''
        self._canvas.move(self._item, dx, dy)
        self.x += dx
        self.y += dy

        
    def is_in_case(self, i : int, j : int):
        return((i == self.case_x) and (j == self.case_y))
    
    def get_color(self):
        return(self._color)
    
    def move(self,x : float,y : float):
        ''' moves the pawn '''
        ncase_x, ncase_y = self._canvas._canv2plat(x, y)
        xf, yf = self._canvas._plat2canv(ncase_x, ncase_y)
        self.move_on_ui(xf - self.x, yf - self.y)
        self.x = xf
        self.y = yf
        self.case_x = ncase_x
        self.case_y = ncase_y
        
class Areas(Canvas):

    def get_color(self, s):
        ''' all colors '''
        if s == "dark": # color of the board
            return self.from_rgb((119, 149, 86))
        if s == "light": # color of the cases
            return self.from_rgb((235, 236, 208))
        if s == "white": # color of white pawns
            return self.from_rgb((249, 249, 249))
        if s == "black": # color of black pawns
            return self.from_rgb((87, 84, 82))
        if s == "highlighted": # color of highlighted cases
            return "coral"
        if s == "white arrows": # color of arrows for white player
            return self.from_rgb((137, 197, 246))
        if s == "black arrows": # color of arrows for black player
            return self.from_rgb((62, 123, 175))
        if s == "background": # color of window background
            return self.from_rgb((49, 46, 43))
        print(f"Unknown color {s}")
    
    def get_font(self):
        return font.Font(family='Helvetica', size=16)

    def from_rgb(self, rgb):
        '''translates an rgb tuple of int to a tkinter friendly color code'''
        return "#%02x%02x%02x" % rgb     

class BoardArea(Areas):
    
    def __init__(self, parent, width : int, height : int, show_black_ar : bool, show_white_ar : bool, show_moves : bool, playerW = Human(), playerB = Human()):
        # initialization of the canvas
        assert(width == height)
        Canvas.__init__(self, parent, width=width, height=height, highlightthickness=0)
        self.width = width # width of the board area
        self.height = height # height of the board area
        self.case_width = width // 13 # width of a case (for the hitbox)
        self.case_height = height // 13 # height of a case
        self.case_radius = self.case_width // 5 # radius of a case
        self.highlight_radius = int(self.case_radius // 1.5)
        self.pawn_radius = self.case_width // 2.5 # radius of a pawn
        self.show_moves = show_moves # True if we show possible moves when clicking on pawn
        self.show_black_ar = show_black_ar # True if we indicates black movements using arrows
        self.show_white_ar = show_white_ar # True if we indicates white movements using arrows
        self.configure(bg=self.get_color("dark"))
        
        # drawing the board cases
        for i in range(8):
            for j in range(8):
                posx, posy = self._plat2canv(i,j)
                self.create_oval(posx - self.case_radius,
                                 posy - self.case_radius,
                                 posx + self.case_radius,
                                 posy + self.case_radius,
                                 fill = self.get_color("light"),
                                 outline="")
      
        # adding pieces
        self.wp =[] # white pawns
        self.bp =[] # black pawns
        self.hc = [] # highlighted cases
        self.arrows_black = [] # stores arrows representing black movements
        self.arrows_white = [] # stores arrows representing white movements
        for i in range(4):
            for j in range(4):
                if i + j <= 3:
                    x,y = self._plat2canv(i, j)
                    self.wp.append(Pion(self, x, y, self.get_color("white")))
                    x,y = self._plat2canv(7 - i, 7 - j)
                    self.bp.append(Pion(self, x, y, self.get_color("black")))


        #temporary data to use c++ solvers and test code without changing the rest of the code
        self.playerW = AI_cpp(depth = 1)
        self.playerB = AI_cpp(depth = 1)        
        
        #initiating a Game
        self.whoistoplay = self.playerW # Allow us to check if a player
        self.movablePaws = self.wp if self.playerW.getHumanity() else []
        self.board= cc.Game()

        #working data 
        self.__piece_courante = ""
        self.joueurajouer = False
        self.pospioninit = (-1,-1)
        self.coup_precedent = ""
        self.coup_courant = []
        
        #temporary data 
        self.color = "white"
        
    def bouton1_appuye(self,event):
        # TEST
        if self.whoistoplay.getHumanity():
            eventi,eventj = self._canv2plat(event.x,event.y)
            for p in self.movablePaws:
                if p.is_in_case(eventi,eventj):
                        self.__piece_courante = p
                        break
            if self.__piece_courante != "":
                self.__piece_courante.redraw()
                if self.pospioninit == (-1,-1):
                    self.pospioninit = self.__piece_courante.case_x,self.__piece_courante.case_y
                    self.coup_courant = [(eventi,eventj)]
                
    def bouton1_deplace(self,event):
        # TEST
        if self.__piece_courante != "":
            dx = event.x - self.__piece_courante.x
            dy = event.y - self.__piece_courante.y
            self.__piece_courante.move_on_ui(dx,dy)

    def bouton1_relache(self,event):
        # TEST
        if self.__piece_courante != "" :
            ncase_x,ncase_y = self._canv2plat(event.x,event.y)
            etude_coup = self.elementaryMove(self.__piece_courante.case_x,self.__piece_courante.case_y,ncase_x,ncase_y)
            if ncase_x == self.pospioninit[0] and ncase_y == self.pospioninit[1]:
                # dropped the pawn on the same case as before
                
                if self.show_moves: # Drawing of the reachable boxes
                    possible_moves = self.possible_moves([self.__piece_courante.case_x,
                                                                       self.__piece_courante.case_y],
                                                                       self.__pbpn2pospion(self.wp, self.bp))
                    self.highlight_cases(possible_moves)
                    self.show_arrows([], self.color) # erases drawn arrows
                
                self.__piece_courante.move(event.x,event.y) 
                self.__reset_working_data()
            elif etude_coup != 'ilegal' and (self.coup_precedent =="" or (self.coup_precedent == "saut" and etude_coup == "saut")):
                # wants to play a legal move
                self.__piece_courante.move(event.x,event.y)
                self.coup_precedent = etude_coup
                self.joueurajouer = True
                self.coup_courant.append((ncase_x,ncase_y))
                if (self.color == 'white' and self.show_white_ar) or\
                   (self.color == 'black' and self.show_black_ar):
                    self.show_arrows(self.coup_courant, self.color)
    
            else:
                xf,yf = self._plat2canv(self.__piece_courante.case_x,self.__piece_courante.case_y)
                self.__piece_courante.move_on_ui(xf-self.__piece_courante.x,yf-self.__piece_courante.y)         
            
            
    def __reset_working_data(self):
        self.__piece_courante = ""
        self.joueurajouer = False
        self.pospioninit = (-1,-1)
        self.coup_precedent = ""
        self.coup_courant = []
    
    def __swap_whoistoplay(self):
        if self.whoistoplay == self.playerW:
            self.whoistoplay = self.playerB
            self.movablePaws = self.bp
        else:
            self.whoistoplay = self.playerW
            self.movablePaws = self.wp
            
                        
    def  _plat2canv(self,i : int,j : int):
        '''converts the coordinates of the board to pixels'''
        
        x = (i * self.case_width - j * self.case_height) / 2 + self.height / 2
        y = (-i * self.case_width - j * self.case_height) * np.sqrt(3) / 2 + self.height-self.case_height / 2
        return(x,y)
    
    def _canv2plat(self,x : float,y : float):
        '''the inverse function'''
        
        X = x - self.height / 2
        Y = y + self.case_height / 2 - self.height
        i = round((X - Y / np.sqrt(3)) / self.case_width)
        j = round((-X - Y / np.sqrt(3)) / self.case_height)
        return(i,j)
    
    def set_parameter(self, s, v):
        '''modify an UI parameter'''
        if s == "show_moves":
            self.show_moves = v
        elif s == "show_black_ar":
            self.show_black_ar = v
        elif s == "show_white_ar":
            self.show_white_ar = v
        else:
            print(f"Unknown parameter {s}")

    #All of the following functions are graphical functions
 
    def show_coordinates(self):
        '''show empty cases with their coordinates'''
        lfont = font.Font(family='Helvetica', size=20)
        for i in range(8):
            for j in range(8):
                x, y = self._plat2canv(i, j)
                self.create_text(x, y, text=f"{i},{j}", fill="red", font=lfont)
    
    def highlight_cases(self, l_moves : list):
        '''highlights the cases in the l_moves cases list'''
        
        for oval in self.hc:
            self.delete(oval)

        for move in l_moves: # l_moves is the list of intermediate cases
            i, j = move
            r = self.highlight_radius
            x, y = self._plat2canv(i, j)
            self.hc.append(self.create_oval(x - r,
                                 y - r,
                                 x + r,
                                 y + r,
                                 fill=self.HIGHLIGHT,
                                 outline=""))
    
    def show_arrows(self, l_moves : list, color : str):
        '''draw arrows between the cases in the l_moves cases list'''
        
        # deletes existing arrows
        if color == 'white':
            for arrow in self.arrows_white:
                self.delete(arrow)
            self.arrows_white = []
        else:
            for arrow in self.arrows_black:
                self.delete(arrow)
            self.arrows_black = []
        
        if(len(l_moves) <= 1):
            return
        
        last = l_moves[0]
        for new in l_moves[1:]:
            i, j = last
            new_i, new_j = new
            x, y = self._plat2canv(i, j)
            new_x, new_y = self._plat2canv(new_i, new_j)
            
            if x != new_x:
                p = (new_y - y) / (new_x - x) # slope of the arrow
                theta = math.atan(p) # angle of the arrow with horizontal axis
                x_offset = abs(self.case_radius * math.cos(theta))
                y_offset = abs(self.case_radius * math.sin(theta))
                x += x_offset if new_x > x else -x_offset
                y += y_offset if new_y > y else -y_offset
                new_x -= x_offset if new_x > x else -x_offset
                new_y -= y_offset if new_y > y else -y_offset
            else: # vertical arrow
                y += self.case_radius 
                new_y -= self.case_radius

            if color == 'white':  
                self.arrows_white.append(self.create_line(x, 
                                                  y, 
                                                  new_x, 
                                                  new_y, 
                                                  arrow=LAST, 
                                                  arrowshape='15 20 8',
                                                  fill=self.from_rgb((137, 197, 246)),
                                                  width=8))
            else:
                self.arrows_black.append(self.create_line(x, 
                                                  y, 
                                                  new_x, 
                                                  new_y, 
                                                  arrow=LAST, 
                                                  arrowshape='15 20 8',
                                                  fill=self.from_rgb((62, 123, 175)),
                                                  width=8))
            last = new
    
    def on_resize(self, event):
        ''' called on resize events for resizing boardArea '''
        # determine the ratio
        scale = float(event.height)/self.height
        self.width = event.height
        self.height = event.height      
        # resize the canvas 
        self.config(width=self.width, height=self.height)
        # rescale all the objects tagged with the "all" tag
        self.scale("all", 0, 0, scale, scale)  
        
        # update values
        self.case_width = self.width/13 # width of a case (for the hitbox)
        self.case_height = self.height/13 # height of a case
        self.case_radius = self.case_width/5 # radius of a case
        self.pawn_radius = self.case_width/2.5 # radius of a pawn
        for pawn in self.wp:
            pawn.set_xy(pawn.x * scale, pawn.y * scale)
    
    # All of the following functions in the class are TEST
  

    def can_play(self):
        return self.joueurajouer

    def jouerIA(self):
        # TEST
        intwhoistoplay = 0 if self.whoistoplay == self.playerW else 1
        if self.whoistoplay.getHumanity():# A human is playing
            self.joueurajouer = self.board.move(intwhoistoplay,self.coup_courant)
            print( "Player White" if intwhoistoplay == 0 else"PLayer Black",self.joueurajouer,self.coup_courant)
        else: # An AI is playing
            tic = time.time()
            self.coup_courant = self.whoistoplay.getMove()
            toc = time.time()
            self.joueurajouer = self.board.move(intwhoistoplay,self.coup_courant)
            print("AI White" if intwhoistoplay == 0 else "AI Black",self.joueurajouer,self.coup_courant,toc-tic)
            
            
        if self.joueurajouer:
            if not self.whoistoplay.getHumanity():
                self.apply_move_without_check(self.coup_courant)
            self.playerW.applyMove(intwhoistoplay,self.coup_courant)
            self.playerB.applyMove(intwhoistoplay,self.coup_courant)
            if intwhoistoplay == 0 and self.show_white_ar:
                self.show_arrows(self.coup_courant, "white")
            elif intwhoistoplay == 1 and self.show_black_ar: 
                self.show_arrows(self.coup_courant, "black")
            self.__reset_working_data()
            self.__swap_whoistoplay()

            
        else:
            print("Move considered as illegal")
        
        for case in self.hc:
            self.delete(case)
        self.hc = []

            
    def possible_moves(self,p : list,pospions):
        rep = []
        nb_non_sauts = 0
        for a,b in [(1,0),(0,1),(-1,0),(0,-1),(1,-1),(-1,1)]:
            arrivex = p[0] +a
            arrivey = p[1] +b
            if (0 <= arrivex <= 7) and (0<= arrivey <= 7) and (not pospions[arrivex,arrivey]):
                rep.append((arrivex,arrivey))
                nb_non_sauts +=1
        pospions[p[0],p[1]] = False
        #gestion des sauts
        vus = [(p[0],p[1])]
        a_voir = [[p]]
        while len(a_voir) != 0:
            l =  a_voir.pop(0)
            pp =  l[-1]
            for (a,b) in [(1,0),(0,1),(-1,0),(0,-1),(1,-1),(-1,1)]:
                ii = pp[0] + a
                jj = pp[1] + b
                saut = False
                while (0 <= ii <= 7) and (0<= jj <= 7):#on se déplace dans la direction donnée par a,b et on cherche le premier pion
                    if  pospions[ii,jj]:
                        saut = True
                        break
                    ii += a
                    jj += b
                if saut:
                    arrivex = 2*ii - pp[0]
                    arrivey = 2*jj - pp[1]
                    if (0<= arrivex <= 7) and (0<= arrivey <= 7) and (not pospions[arrivex,arrivey]) and (not (arrivex,arrivey) in vus):
                        saut_legal = True
                        ii += a
                        jj += b
                        while ii != arrivex or jj != arrivey:
                            if pospions[ii,jj]:
                                saut_legal = False
                                break
                            ii += a
                            jj += b
                        if saut_legal:
                            ll = l.copy()
                            ll.append((arrivex,arrivey))
                            a_voir.append(ll)
                            vus.append((arrivex,arrivey))
                            rep.append((arrivex,arrivey))
                    
        pospions[p[0],p[1]] = True
        return(rep)
    
    def __pbpn2pospion(self,wp : list,bp : list):
        pospion = np.empty((8,8), dtype = 'bool')
        pospion.fill(False)
        pb, pn = [], []
        for p in wp:
            pb.append([p.case_x, p.case_y])
        for p in bp:
            pn.append([p.case_x, p.case_y])
        
        for a,b in pb:
            pospion[a,b] = True
        for a,b in pn:
            pospion[a,b] = True
        return(pospion)
    

    def apply_move_without_check(self,move : list):
        ''' Apply move on screen'''
        for p in self.wp:
            if p.case_x == move[0][0] and p.case_y == move[0][1]:
                pp = p
                break
        for p in self.bp:
            if p.case_x == move[0][0] and p.case_y == move[0][1]:
                pp = p
                break
            
        x,y = self._plat2canv(move[-1][0],move[-1][1])
        _ = pp.move(x,y)
        pp.redraw()


                    
    def elementaryMove(self,a : int,b : int,c : int,d : int):
        ''' Check legality of an elementary move'''
        if (0<= a <= 7) and (0<= b <= 7) and (0<= c <= 7) and (0<= d <= 7):
            if ((c-a)*(d-b) == 0) or ((d-b)/(c-a)==-1):
                rep = (abs(a-c+b-d) <= 1) and (abs(a-c) +abs(b-d) <= 2)
                for p in self.bp:
                    if p.case_x == c and p.case_y ==d:
                        return('ilegal')
                    elif p.case_x != a or p.case_y != b :
                        if p.case_x == (a+c)/2 and p.case_y == (b+d)/2:
                            rep = True
                        elif (p.case_x- a)*(d-b) == (p.case_y-b)*(c-a) and (p.case_x -a)*(c-a) + (p.case_y-b)*(d-b) >= 0 and (p.case_x -c)*(a-c) + (p.case_y-d)*(b-d) >= 0:
                            return('ilegal')

                for p in self.wp:
                    if p.case_x == c and p.case_y ==d:
                        return('ilegal')
                    elif p.case_x != a or p.case_y !=b :
                        if p.case_x == (a+c)/2 and p.case_y == (b+d)/2:
                            rep = True
                        elif (p.case_x- a)*(d-b) == (p.case_y-b)*(c-a) and (p.case_x -a)*(c-a) + (p.case_y-b)*(d-b) >= 0 and (p.case_x -c)*(a-c) + (p.case_y-d)*(b-d) >= 0:
                            return('ilegal')
                if rep:
                    if (abs(a-c+b-d) <= 1) and abs(a-c) +abs(b-d) <= 2:
                        return("non saut")
                    else:
                        return("saut")
                else:   
                    return('ilegal')
            else:
                return('ilegal')
        else:
            return('ilegal')

class Board(Tk,Areas):

    def __init__(self, width, height):

        # TODO :
        # add player choice in the UI
        # re do "Play AI" button
        # split this file into several
        # ends of games
        # relaunch game

        # fix a bug where cancelling a move (human side) doesn't gray out the "Play AI" button
        # clean code
        # cancel current move as human

        Tk.__init__(self)
        assert(width >= height) # we want the window to be wider than tall to fit buttons on the right
        
        # Initializing the scene
        self.title('Chinese Checkers')
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
        self.__boardArea = BoardArea(self, board_side, board_side, False, False, False)
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
                                                "")
            new_text = self.__parametersArea.create_text(
                                                (parameters_width - self.BUTTONS_WIDTH + 3 * self.BUTTONS_HEIGHT) // 2,
                                                (height + (4 * i + start - 0.7) * self.BUTTONS_HEIGHT) // 2,
                                                text=texts[i],
                                                fill=self.get_color("white"),
                                                font=self.get_font(),
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
                                        "state": "off", # can be "on", "off", "hon" or "hoff"
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
        self.__boardArea.bind("<Button-1>", self.__boardArea.bouton1_appuye)
        self.__boardArea.bind("<B1-Motion>", self.__boardArea.bouton1_deplace)
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
        self.__boardArea.bouton1_relache(event)
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
