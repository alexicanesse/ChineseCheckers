from tkinter import *
import tkinter.font as font
import numpy as np
import math
from ChineseCheckers_AI import *

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
                                         outline="")
        self.x = x
        self.y = y
        self.case_x, self.case_y = self._canvas._canv2plat(x,y)
    
    def __str__(self):
        c = "blanc" if self._color == "white" else "noir"
        return(f"Pion {c} ({self.case_x},{self.case_y})")
    
    def __repr__(self):
        c = "b" if self._color =="white" else "n"
        return(f"P{c}{self.case_x}{self.case_y}")
    
    def redraw(self): 
        ''' deletes and redraws the pawn '''
        self._canvas.delete(self._item)
        self._item = self._canvas.create_oval(self.x - self._canvas.pawn_radius,
                                         self.y - self._canvas.pawn_radius,
                                         self.x + self._canvas.pawn_radius,
                                         self.y + self._canvas.pawn_radius,
                                         fill =self._color,
                                         outline="")
        
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

class BoardArea(Canvas):
    
    def __init__(self, parent, width, height):
        # initialization of the canvas
        Canvas.__init__(self, parent, width=width, height=height, highlightthickness=0)
        self.width = width # width of the board area
        self.height = height # height of the board area
        self.case_width = width/13 # width of a case (for the hitbox)
        self.case_height = height/13 # height of a case
        self.case_radius = self.case_width/5 # radius of a case
        self.pawn_radius = self.case_width/3 # radius of a pawn
        # TODO couleurs rouge&vert à la place de noir&blanc ?
        # TODO ajuster couleurs (prendre les codes RGB sur lichess par ex ?)
        
        # drawing the board cases
        for i in range(8):
            for j in range(8):
                posx, posy = self._plat2canv(i,j)
                self.create_oval(posx - self.case_radius,
                                 posy - self.case_radius,
                                 posx + self.case_radius,
                                 posy + self.case_radius,
                                 fill = "light grey",
                                 outline="")
      
        # adding pieces
        self.wp =[] # white pawns
        self.bp =[] # black pawns
        self.hc = [] # highlighted cases
        self.arrows = []
        for i in range(4):
            for j in range(4):
                if i + j <= 3:
                    x,y = self._plat2canv(i, j)
                    self.wp.append(Pion(self, x, y, "white"))
                    x,y = self._plat2canv(7 - i, 7 - j)
                    self.bp.append(Pion(self, x, y, "black"))
                    
        # TEST
        self.ia = IA(self,'black')

        #working data pour tester les coups du joueur # TODO traduire
        self.__piece_courante = ""
        self.joueurajouer = False
        self.pospioninit = (-1,-1)
        self.coup_precedent = ""
        self.coup_courant = []
                
    def reset_working_data(self):
        self.__piece_courante = ""
        self.joueurajouer = False
        self.pospioninit = (-1,-1)
        self.coup_precedent = ""
        self.coup_courant = []
                        
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
            x, y = self._plat2canv(i, j)
            self.hc.append(self.create_oval(x - self.case_radius,
                                 y - self.case_radius,
                                 x + self.case_radius,
                                 y + self.case_radius,
                                 fill="spring green",
                                 outline=""))
    
    def show_arrows(self, l_moves):
        '''draw arrows between the cases in the l_moves cases list'''
        
        # deletes existing arrows
        for arrow in self.arrows:
            self.delete(arrow)
        self.arrow = []
        
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
                
            self.arrows.append(self.create_line(x, 
                                                  y, 
                                                  new_x, 
                                                  new_y, 
                                                  arrow=LAST, 
                                                  arrowshape='15 20 8',
                                                  fill="SteelBlue2",
                                                  width=6))
            last = new
    
    # All of the following functions in the class are TEST
    
    def jouerIA(self):
        # TEST
        if self.joueurajouer:
            print("J:",self.coup_courant)
            tic = time.time()
            temp = self.ia.joueralphabeta(depth = 3)
            toc = time.time()
            print("IA:",temp[1],'en ',toc-tic,'s')
            self.appliquer_coup(temp[0],temp[1])
            self.show_arrows(temp[1])
            self.reset_working_data()
        # de-highlight precedently highlighted cases
        for case in self.hc:
            self.delete(case)
        self.hc = []
            
    def bouton1_appuye(self,event):
        # TEST
        eventi,eventj = self._canv2plat(event.x,event.y)
        for p in self.wp:
            if p.is_in_case(eventi,eventj):
                if not self.joueurajouer:
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
    
    def bouton1_relache(self,event):
        # TEST
        if self.__piece_courante != "" :
            ncase_x,ncase_y = self._canv2plat(event.x,event.y)
            etude_coup = self.coup_legal(self.__piece_courante.case_x,self.__piece_courante.case_y,ncase_x,ncase_y)
            if ncase_x == self.pospioninit[0] and ncase_y == self.pospioninit[1]:
                # dropped the pawn on the same case as before
                # Drawing of the reachable boxes
                possible_moves = self.coups_possibles_a_supprimer([self.__piece_courante.case_x,
                                                                  self.__piece_courante.case_y],
                                                                 self.pbpn2pospion_a_supprimer(self.wp, self.bp))
                
                self.highlight_cases(possible_moves)
                
                self.__piece_courante.move(event.x,event.y) 
                self.reset_working_data()
            elif etude_coup != 'ilegal' and (self.coup_precedent =="" or (self.coup_precedent == "saut" and etude_coup == "saut")):
                # wants to play a legal move
                self.__piece_courante.move(event.x,event.y)
                self.coup_precedent = etude_coup
                self.joueurajouer = True
                self.coup_courant.append((ncase_x,ncase_y))
    
            else:
                xf,yf = self._plat2canv(self.__piece_courante.case_x,self.__piece_courante.case_y)
                self.__piece_courante.move_on_ui(xf-self.__piece_courante.x,yf-self.__piece_courante.y)         
        
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


class Board(Tk):

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
        print(f"screen is {screen_height}x{screen_width}, putting window at ({window_x},{window_y})")
        self.geometry(f"{width}x{height}+{int(window_x)}+{int(window_y)}")
        self.configure(bg = "white")

        # Initializing the board
        self.__boardArea = BoardArea(self, height, height)
        self.__boardArea.configure(bg='burlywood3')
        self.__boardArea.pack(padx = 0,side = LEFT)
        
        # Buttons and mouse events
        self.boutonIA= Button(self,text = "Jouer IA")
        self.boutonIA.pack(side=LEFT, padx  = 10)
        bfont = font.Font(family='Helvetica', size=40)
        self.boutonIA.config(command = self.__boardArea.jouerIA, bg = 'lightgrey', font = bfont)
        
        #Config des événements
        self.__boardArea.bind("<ButtonRelease-1>", self.__boardArea.bouton1_relache)
        self.__boardArea.bind("<Button-1>", self.__boardArea.bouton1_appuye)
        self.__boardArea.bind("<B1-Motion>", self.__boardArea.bouton1_deplace)
        
        # TEST
        '''
        self.__boardArea.show_coordinates()
        l = [(0,0), (3,4), (5,0)]
        self.__boardArea.highlight_cases(l)
        l = [(0,1), (7,4), (2,4)]
        self.__boardArea.highlight_cases(l)
        l = [(0,1), (4,1), (4,4), (0,3), (0,1)]
        self.__boardArea.show_arrows(l)
        '''

if __name__ == "__main__":
    
    fen = Board(1300, 1000)
    fen.mainloop()
