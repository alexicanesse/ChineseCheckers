from tkinter import *
from ChineseCheckers_Areas import *
from ChineseCheckers_Players import *
import bin.libChineseCheckers as cc
import math

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

class BoardArea(Areas):
    
    def __init__(self, parent, side : int, init_states : list[bool], playerW : str, playerB : str):
        # initialization of the canvas
        Canvas.__init__(self, parent, width=side, height=side, highlightthickness=0)
        self.side = side # side of the board area
        self.case_radius_hit = side // (1 + 7 * math.sqrt(3)) # radius of a case (for the hitbox)
        self.case_radius = self.case_radius_hit // 5 # radius of a case
        self.highlight_radius = int(self.case_radius // 1.5)
        self.pawn_radius = self.case_radius_hit // 2.5 # radius of a pawn
        self.show_white_ar = init_states[0] # True if we indicates white movements using arrows
        self.show_black_ar = init_states[1] # True if we indicates black movements using arrows
        self.show_moves = init_states[2] # True if we show possible moves when clicking on pawn
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
        if playerW == "Human":
            self.playerW = Human()
        elif playerW == "C++ AI":
            self.playerW = AI_cpp(depth = 3)
        else:
            print(f"Unknown player type {playerW}")
        if playerB == "Human":
            self.playerB = Human()
        elif playerB == "C++ AI":
            self.playerB = AI_cpp(depth = 3)
        else:
            print(f"Unknown player type {playerB}")     
        
        #initiating a Game
        self.whoistoplay = self.playerW # Allow us to check if a player
        self.movablePaws = self.wp if self.playerW.getHumanity() else []
        self.board = cc.Game()

        #working data 
        self.__piece_courante = ""
        self.joueurajouer = False
        self.pospioninit = (-1,-1)
        self.coup_precedent = ""
        self.coup_courant = []
        
    def pawn_pressed(self,event):
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
                
    def pawn_moved(self,event):
        if self.__piece_courante != "":
            dx = event.x - self.__piece_courante.x
            dy = event.y - self.__piece_courante.y
            self.__piece_courante.move_on_ui(dx,dy)

    def pawn_released(self,event):
        color_playing = "black" if self.whoistoplay == self.playerB else "white"
        if self.__piece_courante != "" :
            ncase_x,ncase_y = self._canv2plat(event.x,event.y)
            etude_coup = self.elementaryMove(self.__piece_courante.case_x,self.__piece_courante.case_y,ncase_x,ncase_y)
            if ncase_x == self.pospioninit[0] and ncase_y == self.pospioninit[1]:
                # dropped the pawn on the same case as before
                
                if self.show_moves: # Drawing of the reachable cases
                    possible_moves = self.possible_moves([self.__piece_courante.case_x,
                                                                       self.__piece_courante.case_y],
                                                                       self.__pbpn2pospion(self.wp, self.bp))
                    self.highlight_cases(possible_moves)
                    self.show_arrows([], color_playing) # erases drawn arrows
                
                self.__piece_courante.move(event.x,event.y) 
                self.__reset_working_data()
            elif etude_coup != 'ilegal' and (self.coup_precedent =="" or (self.coup_precedent == "saut" and etude_coup == "saut")):
                # wants to play a legal move
                self.__piece_courante.move(event.x,event.y)
                self.coup_precedent = etude_coup
                self.joueurajouer = True
                self.coup_courant.append((ncase_x,ncase_y))
                if (color_playing == 'white' and self.show_white_ar) or\
                   (color_playing == 'black' and self.show_black_ar):
                    self.show_arrows(self.coup_courant, color_playing)
    
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
            
                        
    def _plat2canv(self,i : int,j : int):
        '''converts the coordinates of the board to pixels'''
        r, s = self.case_radius_hit, self.side
        x = (i - j) * r / 2 + s / 2
        y = (i + j) * (r - s) / 14 + s - r / 2

        return (x, y)
    
    def _canv2plat(self,x : float,y : float):
        '''the inverse function'''
        r, s = self.case_radius_hit, self.side        
        x -= s / 2
        y += r / 2 - s
        A, B = 7 / (r - s), 1 / r
        i = round(A * y + B * x)
        j = round(A * y - B * x)
        return(i, j)
    
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
                t = 2
                self.create_oval(x - t, y - t, x + t, y + t, fill="green") # mark the center of the cases
                self.create_oval(x - self.case_radius_hit,
                                 y - self.case_radius_hit,
                                 x + self.case_radius_hit,
                                 y + self.case_radius_hit,
                                 fill = "",
                                 outline=self.get_color("black"))
    
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
                                 fill=self.get_color("highlighted"),
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
                                                  arrowshape='15 15 8', # length, width, arc
                                                  fill=self.get_color("white arrows"),
                                                  width=8))
            else:
                self.arrows_black.append(self.create_line(x, 
                                                  y, 
                                                  new_x, 
                                                  new_y, 
                                                  arrow=LAST, 
                                                  arrowshape='15 20 8',
                                                  fill=self.get_color("black arrows"),
                                                  width=8))
            last = new
    
    def on_resize(self, event):
        ''' called on resize events for resizing boardArea '''
        # determine the ratio
        scale = float(event.height)/self.side
        self.side = event.height   
        # resize the canvas 
        self.config(width=self.side, height=self.side)
        # rescale all the objects tagged with the "all" tag
        self.scale("all", 0, 0, scale, scale)  
        
        # update values
        self.case_radius_hit = self.side // (1 + 7 * math.sqrt(3)) # width of a case (for the hitbox)
        self.case_radius = self.case_radius_hit //5 # radius of a case
        self.pawn_radius = self.case_radius_hit / 2.5 # radius of a pawn
        for pawn in self.wp:
            pawn.set_xy(pawn.x * scale, pawn.y * scale)
        for pawn in self.bp:
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