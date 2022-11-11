from tkinter import *
import tkinter.font as font
import numpy as np
import math
from ChineseCheckers_Players import *
import bin.libChineseCheckers as cc


import time #TEST


class Pion:
    
    def __init__(self, canvas, x, y, color = "white"):
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

    def set_xy(self, x, y):
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
        
    def move_on_ui(self, dx, dy): 
        ''' moves the pawn on the UI '''
        self._canvas.move(self._item, dx, dy)
        self.x += dx
        self.y += dy

        
    def is_in_case(self, i, j):
        return((i == self.case_x) and (j == self.case_y))
    
    def get_color(self):
        return(self._color)
    
    def move(self,x,y):
        ''' moves the pawn '''
        ncase_x, ncase_y = self._canvas._canv2plat(x, y)
        xf, yf = self._canvas._plat2canv(ncase_x, ncase_y)
        self.move_on_ui(xf - self.x, yf - self.y)
        self.x = xf
        self.y = yf
        self.case_x = ncase_x
        self.case_y = ncase_y
        
class Areas(Canvas):
    
    def from_rgb(self, rgb):
        '''translates an rgb tuple of int to a tkinter friendly color code'''
        return "#%02x%02x%02x" % rgb   

class BoardArea(Areas):
    
    def __init__(self, parent, width, height, show_black_ar, show_white_ar, show_moves, playerW = Human(), playerB = Human()):
        # initialization of the canvas
        assert(width == height)
        Canvas.__init__(self, parent, width=width, height=height, highlightthickness=0)
        self.bind("<Configure>", self.on_resize)
        self.width = width # width of the board area
        self.height = height # height of the board area
        self.case_width = width/13 # width of a case (for the hitbox)
        self.case_height = height/13 # height of a case
        self.case_radius = self.case_width/5 # radius of a case
        self.highlight_radius = int(self.case_radius / 1.5)
        self.pawn_radius = self.case_width/2.5 # radius of a pawn
        self.DARK = self.from_rgb((119, 149, 86)) # color of the board
        self.LIGHT = self.from_rgb((235, 236, 208)) # color of the cases
        self.WHITE = self.from_rgb((249, 249, 249)) # color of white pawns
        self.BLACK = self.from_rgb((87, 84, 82)) # color of black pawns
        self.HIGHLIGHT = "coral" # color of highlighted cases
        self.show_moves = show_moves # True if we show possible moves when clicking on pawn
        self.show_black_ar = show_black_ar # True if we indicates black movements using arrows
        self.show_white_ar = show_white_ar # True if we indicates white movements using arrows
        self.configure(bg=self.DARK)
        self.playerW = playerW
        self.playerB = playerB
        
        # drawing the board cases
        for i in range(8):
            for j in range(8):
                posx, posy = self._plat2canv(i,j)
                self.create_oval(posx - self.case_radius,
                                 posy - self.case_radius,
                                 posx + self.case_radius,
                                 posy + self.case_radius,
                                 fill = self.LIGHT,
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
                    self.wp.append(Pion(self, x, y, self.WHITE))
                    x,y = self._plat2canv(7 - i, 7 - j)
                    self.bp.append(Pion(self, x, y, self.BLACK))
                    




        #temporary data to use c++ olvers and test cod without changing the rest of the code
        self.playerW = AI_cpp(depth = 3)
        
        
        
        #initiating a Game
        self.whoistoplay = self.playerW# Allow us to check if a player
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
            etude_coup = self.coup_legal(self.__piece_courante.case_x,self.__piece_courante.case_y,ncase_x,ncase_y)
            if ncase_x == self.pospioninit[0] and ncase_y == self.pospioninit[1]:
                # dropped the pawn on the same case as before
                
                if self.show_moves: # Drawing of the reachable boxes
                    possible_moves = self.coups_possibles_a_supprimer([self.__piece_courante.case_x,
                                                                       self.__piece_courante.case_y],
                                                                       self.pbpn2pospion_a_supprimer(self.wp, self.bp))
                    self.highlight_cases(possible_moves)
                    self.show_arrows([], self.color) # erases drawn arrows
                
                self.__piece_courante.move(event.x,event.y) 
                self.reset_working_data()
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
            
            
    def reset_working_data(self):
        self.__piece_courante = ""
        self.joueurajouer = False
        self.pospioninit = (-1,-1)
        self.coup_precedent = ""
        self.coup_courant = []
    
    def swap_whoistoplay(self):
        if self.whoistoplay == self.playerW:
            self.whoistoplay = self.playerB
            self.movablePaws = self.bp
        else:
            self.whoistoplay = self.playerW
            self.movablePaws = self.wp
            
                        
    def  _plat2canv(self,i,j):
        '''converts the coordinates of the board to pixels'''
        
        x = (i * self.case_width - j * self.case_height) / 2 + self.height / 2
        y = (-i * self.case_width - j * self.case_height) * np.sqrt(3) / 2 + self.height-self.case_height / 2
        return(x,y)
    
    def _canv2plat(self,x,y):
        '''the inverse function'''
        
        X = x - self.height / 2
        Y = y + self.case_height / 2 - self.height
        i = round((X - Y / np.sqrt(3)) / self.case_width)
        j = round((-X - Y / np.sqrt(3)) / self.case_height)
        return(i,j)

    #All of the following functions are graphical functions
 
    def show_coordinates(self):
        '''show empty cases with their coordinates'''
        lfont = font.Font(family='Helvetica', size=20)
        for i in range(8):
            for j in range(8):
                x, y = self._plat2canv(i, j)
                self.create_text(x, y, text=f"{i},{j}", fill="red", font=lfont)
    
    def highlight_cases(self, l_moves):
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
    
    def show_arrows(self, l_moves, color):
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
                y += self.case_radius # TODO dans l'autre sens
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
        ''' called on resize events '''
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
            self.self.joueurajouer = self.board.move(intwhoistoplay,self.coup_courant)
            print( "Player White" if intwhoistoplay == 0 else"PLayer Black",self.coup_courant)
        else: # An AI is playing
            tic = time.time()
            self.coup_courant = self.whoistoplay.getMove()
            toc = time.time()
            self.joueurajouer = self.board.move(intwhoistoplay,self.coup_courant)
            print("AI White" if intwhoistoplay == 0 else "AI Black",self.joueurajouer,self.coup_courant,toc-tic)
            
            
        if self.joueurajouer:
            self.playerW.applyMove(intwhoistoplay,self.coup_courant)
            self.playerB.applyMove(intwhoistoplay,self.coup_courant)
            self.reset_working_data()
            self.swap_whoistoplay()
        
        for case in self.hc:
            self.delete(case)
        self.hc = []
        
        """
        assert(self.joueurajouer)
        tic = time.time()
        temp = self.ia.joueralphabeta(depth = 3)
        toc = time.time()
        print("IA:",temp[1],'en ',toc-tic,'s')
        self.appliquer_coup(temp[0],temp[1])
        if (self.ia.couleur == 'white' and self.show_white_ar) \
           or (self.ia.couleur == 'black' and self.show_black_ar):
            self.show_arrows(temp[1], self.ia.couleur)
        self.reset_working_data()
        # de-highlight precedently highlighted cases
        for case in self.hc:
            self.delete(case)
        self.hc = []
        """
            
    def coups_possibles_a_supprimer(self,p,pospions):#TODO mettre dans Human()
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
    
    def pbpn2pospion_a_supprimer(self,wp,bp):
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
       
    def piece_dans_case(self,i,j):
        for p in self.wp:
            if p.is_in_case(i,j) :
                return(True)
        for p in self.bp:
            if p.is_in_case(i,j) :
                return(True)
        return(False)
    
    def appliquer_coup(self,p,l):
        if p.case_x == l[0][0] and p.case_y == l[0][1]:
            for i in range(1,len(l)):
                a,b =l[i-1]
                c,d = l[i]
                
                test = self.coup_legal(a,b,c,d)
                if test != "ilegal":
                    x,y = self._plat2canv(c,d)
                    _ = p.move(x,y)
                    p.redraw()
                    if test == "non saut":
                        break
                else:
                    print('L\'IA a joué un coup jugé comme illégal pour faire le saut ',(a,b),(c,d))
                    break
                    
    def coup_legal(self,a,b,c,d):
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
    # TODO ajouter classe Human() pour regrouper les fonctions permettant de jouer manuellement


class Board(Tk,Areas):

    def __init__(self, width, height):

        Tk.__init__(self)
        assert(width >= height) # we want the window to be wider than tall to fit buttons on the right
        
        # Initializing the scene
        self.title('Chinese Checkers')
        screen_height = self.winfo_screenheight()
        screen_width = self.winfo_screenwidth()
        window_x = (screen_width - width) / 2
        window_y = (screen_height - height) / 2
        # put the window in the center of the screen
        print(f"screen is {screen_width}x{screen_height}, putting window at ({window_x},{window_y})")
        self.geometry(f"{width}x{height}+{int(window_x)}+{int(window_y)}")
        self.aspect(width, height, width, height) # Keep the aspect ratio fixed when user resizes

        # The scene is in 5 parts : on the left are parameters, in the center is the board, on the right is the "play" button
        # in the bottom & top are blank spaces
        board_side = height
        control_width = int((width -  board_side) / 2.3)
        parameters_width = width -  board_side - control_width
        N = 25

        # Initializing the parameters area
        self.__parametersArea = Canvas(self, width=parameters_width, height=height, highlightthickness=0)
        self.__parametersArea.configure(bg=self.from_rgb((49, 46, 43)))
        self.__parametersArea.pack(side=LEFT, padx=0, fill=BOTH, expand=YES)

        # Initializing the board
        self.__boardArea = BoardArea(self, board_side, board_side, False, False, False)
        self.__boardArea.addtag_all("all")
        self.__boardArea.pack(padx=0, side=LEFT, fill=BOTH)
        
        # Initializing the control area
        self.__controlArea = Canvas(self, width=control_width, height=height, highlightthickness=0)
        self.__controlArea.configure(bg=self.from_rgb((49, 46, 43)))
        self.__controlArea.pack(side=RIGHT, padx=0, fill=BOTH, expand=YES)

        # parameters images
        names = ["ba_on", "ba_off", "ba_hon", "ba_hoff", 
                    "wa_on", "wa_off", "wa_hon", "wa_hoff", 
                    "pmoves_on", "pmoves_off", "pmoves_hon", "pmoves_hoff"]
        for name in names:
            exec(f"{name} = PhotoImage(file=\"icons/{name}.png\")\n" + \
                f"scale = (parameters_width / 1.1) / {name}.width()\n" + \
                f"{name} = {name}.zoom(int(N * scale), int(N * scale))\n" + \
                f"self.{name}_icon = {name}.subsample(N, N)")
        
        # create buttons
        self.wa_button = self.__parametersArea.create_image(parameters_width / 2,
                                                        height / 2 - height / 8,
                                                        image=self.wa_off_icon)
        self.wa_button_state = "off" # can be "on", "off", "hon" or "hoff"
        self.ba_button = self.__parametersArea.create_image(parameters_width / 2,
                                                        height / 2,
                                                        image=self.ba_off_icon)
        self.ba_button_state = "off" # can be "on", "off", "hon" or "hoff"
        self.pmoves_button = self.__parametersArea.create_image(parameters_width / 2,
                                                        height / 2 + height / 8,
                                                        image=self.pmoves_off_icon)
        self.pmoves_button_state = "off" # can be "on", "off", "hon" or "hoff"
        
        # "PLAY" button
        icon = PhotoImage(file="icons/ai_button.png")
        icon_pressed = PhotoImage(file="icons/ai_button_pressed.png")
        icon_grayed = PhotoImage(file="icons/ai_button_grayed.png")

        scale = (control_width / (1.2)) / icon.width() # scales button
        icon = icon.zoom(int(N * scale), int(N * scale))
        self.play_ai_icon = icon.subsample(N, N)

        scale = (control_width / (1.2)) / icon_pressed.width() # scales pressed button
        icon_pressed = icon_pressed.zoom(int(N * scale), int(N * scale))
        self.play_ai_icon_pressed = icon_pressed.subsample(N, N)

        scale = (control_width / (1.2)) / icon_grayed.width() # scales grayed button
        icon_grayed = icon_grayed.zoom(int(N * scale), int(N * scale))
        self.play_ai_icon_grayed = icon_grayed.subsample(N, N)
        
        self.AI_button = self.__controlArea.create_image((control_width) / 2,
                                                        (height) / 2,
                                                        image=self.play_ai_icon)
        self.AI_button_state = "normal" # can be "grayed", "normal" or "pressed"
        
        # mouse events config
        self.__controlArea.tag_bind(self.AI_button, "<Button-1>", self.press_jouerIA)
        self.__controlArea.tag_bind(self.AI_button, "<ButtonRelease-1>", self.release_jouerIA)
        self.__controlArea.bind("<Configure>", self.on_resize)
        # arrows for white
        self.__parametersArea.tag_bind(self.wa_button, "<Button-1>", lambda event: self.click("wa", event))
        self.__parametersArea.tag_bind(self.wa_button, "<Enter>", lambda event: self.hover("wa", event))
        self.__parametersArea.tag_bind(self.wa_button, "<Leave>", lambda event: self.end_hover("wa", event))
        # arrows for black
        self.__parametersArea.tag_bind(self.ba_button, "<Button-1>", lambda event: self.click("ba", event))
        self.__parametersArea.tag_bind(self.ba_button, "<Enter>", lambda event: self.hover("ba", event))
        self.__parametersArea.tag_bind(self.ba_button, "<Leave>", lambda event: self.end_hover("ba", event))
        # show possible moves
        self.__parametersArea.tag_bind(self.pmoves_button, "<Button-1>", lambda event: self.click("pmoves", event))
        self.__parametersArea.tag_bind(self.pmoves_button, "<Enter>", lambda event: self.hover("pmoves", event))
        self.__parametersArea.tag_bind(self.pmoves_button, "<Leave>", lambda event: self.end_hover("pmoves", event))
        self.__parametersArea.bind("<Configure>", self.on_resize)

        self.__boardArea.bind("<ButtonRelease-1>", self.release_pawn)
        self.__boardArea.bind("<Button-1>", self.__boardArea.bouton1_appuye)
        self.__boardArea.bind("<B1-Motion>", self.__boardArea.bouton1_deplace)

    def press_jouerIA(self, event):
        ''' changes button appearance and makes the AI play on press '''
        if self.AI_button_state == "normal":
            # if the button is normal (i.e not grayed), change it to pressed
            self.__controlArea.itemconfigure(self.AI_button, image=self.play_ai_icon_pressed)
            self.__controlArea.update_idletasks()
            self.__boardArea.jouerIA()
            self.AI_button_state = "pressed"
    
    def release_jouerIA(self, event):
        ''' changes button appearance on release '''
        if self.AI_button_state == "pressed":
            # if the button is pressed, make the AI play and change the button to grayed
            self.__controlArea.itemconfigure(self.AI_button, image=self.play_ai_icon_grayed)
            self.__controlArea.update_idletasks()
            self.AI_button_state = "grayed"
    
    def release_pawn(self, event):
        self.__boardArea.bouton1_relache(event)
        if self.__boardArea.can_play():
            # if human has moved a pawn, change the button from grayed to normal
            self.__controlArea.itemconfigure(self.AI_button, image=self.play_ai_icon)
            self.__controlArea.update_idletasks()
            self.AI_button_state = "normal"
    
    def hover(self, button_name, event):
        ''' changes button appearance on hovering '''
        state = eval(f"self.{button_name}_button_state")
        if state not in ["on", "off"]: return # shouldn't reach

        self.__parametersArea.itemconfigure(eval(f"self.{button_name}_button"), 
                                            image = eval(f"self.{button_name}_h{state}_icon"))
        self.__parametersArea.update_idletasks()
        exec(f"self.{button_name}_button_state = \"h{state}\"")
    
    def end_hover(self, button_name, event):
        ''' changes button appearance on ending hover '''
        state = eval(f"self.{button_name}_button_state")
        if state not in ["hon", "hoff"]: return # shouldn't reach
        state = "on" if state == "hon" else "off"
        
        self.__parametersArea.itemconfigure(eval(f"self.{button_name}_button"), 
                                            image = eval(f"self.{button_name}_{state}_icon"))
        self.__parametersArea.update_idletasks()
        exec(f"self.{button_name}_button_state = \"{state}\"")
    
    def click(self, button_name, event):
        ''' changes button appearance on clicking '''
        state = eval(f"self.{button_name}_button_state")
        if state not in ["hon", "hoff"]: return # shouldn't reach
        state = "off" if state == "hon" else "on"
        
        self.__parametersArea.itemconfigure(eval(f"self.{button_name}_button"), 
                                            image = eval(f"self.{button_name}_{state}_icon"))
        self.__parametersArea.update_idletasks()
        exec(f"self.{button_name}_button_state = \"{state}\"")
        if button_name == "wa":
            self.__boardArea.show_white_ar = not self.__boardArea.show_white_ar
            if not self.__boardArea.show_white_ar:
                self.__boardArea.show_arrows([], "white") # removes existing arrows
        elif button_name == "ba":
            self.__boardArea.show_black_ar = not self.__boardArea.show_black_ar
            if not self.__boardArea.show_black_ar:
                self.__boardArea.show_arrows([], "black") # removes existing arrows
        elif button_name == "pmoves":
            self.__boardArea.show_moves = not self.__boardArea.show_moves
            if not self.__boardArea.show_moves:
                self.__boardArea.highlight_cases([]) # removes existing highlighted cases
    
    def on_resize(self, event):
        ''' called on resize events '''
        c_width, c_height = int(self.__controlArea['width']), int(self.__controlArea['height'])
        p_width, p_height = int(self.__parametersArea['width']), int(self.__parametersArea['height'])
        assert(p_height == c_height)
        # determine the ratio
        scale = float(event.height)/c_height
        # resize the two canvas 
        self.__controlArea.config(width=int(c_width * scale), height=int(c_height * scale))
        self.__parametersArea.config(width=int(p_width * scale), height=int(p_height * scale))
        # fixes the position of the buttons
        new_c_width, new_c_height = int(self.__controlArea['width']), int(self.__controlArea['height'])
        new_p_width, new_p_height = int(self.__parametersArea['width']), int(self.__parametersArea['height'])
        self.__controlArea.move(self.AI_button, (new_c_width - c_width) / 2, (new_c_height - c_height) / 2)
        self.__parametersArea.move(self.wa_button, (new_p_width - p_width) / 2, (new_p_height - p_height) / 2)
        self.__parametersArea.move(self.ba_button, (new_p_width - p_width) / 2, (new_p_height - p_height) / 2)
        self.__parametersArea.move(self.pmoves_button, (new_p_width - p_width) / 2, (new_p_height - p_height) / 2)


if __name__ == "__main__":
    
    fen = Board(1500, 900)
    fen.mainloop()
