from tkinter import *
from ChineseCheckers_Areas import *
from ChineseCheckers_Players import *
import bin.libChineseCheckers as cc
import math

import time #TEST

class Pawn(Areas):
    
    def __init__(self, canvas, xy_pos : tuple, ij_pos : tuple, color : str):
        '''
        - ij_pos : board corodinates of the pawn
        - xy_pos : pixel coordinates of the pawn
        '''
        self._color = color
        self._canvas = canvas
        x, y = xy_pos
        self._item = canvas.create_oval(x - self._canvas.pawn_radius,
                                         y - self._canvas.pawn_radius,
                                         x + self._canvas.pawn_radius,
                                         y + self._canvas.pawn_radius,
                                         fill = self.get_color(color),
                                         outline="black",
                                         width=2)
        self.board_pos = ij_pos
    
    def __str__(self):
        c = "White" if self._color == "white" else "Black"
        return(f"{c} Pawn {self.board_pos}")
    
    def __repr__(self):
        c = "w" if self._color =="white" else "b"
        return(f"P{c}{self.board_pos}")
    
    def redraw(self): 
        ''' deletes and redraws the pawn '''
        self._canvas.delete(self._item)
        i, j = self.get_board_pos()
        x, y = self._canvas._plat2canv(i, j)
        self._item = self._canvas.create_oval(x - self._canvas.pawn_radius,
                                         y - self._canvas.pawn_radius,
                                         x + self._canvas.pawn_radius,
                                         y + self._canvas.pawn_radius,
                                         fill = self.get_color(self._color),
                                         outline="black",
                                         width=2)
        
    def move_on_ui(self, new_x : float, new_y : float): 
        ''' moves the pawn on the given pixel coordinates '''
        self._canvas.moveto(self._item, 
                            new_x - self._canvas.pawn_radius, 
                            new_y - self._canvas.pawn_radius)
        
    def is_in_case(self, i : int, j : int):
        return (i, j) == self.board_pos
    
    def get_pawn_color(self):
        return(self._color)

    def get_board_pos(self):
        return self.board_pos
    
    def move_to_case(self, new_i : float, new_j : float):
        ''' moves the pawn to case new_i, new_j '''
        new_x, new_y = self._canvas._plat2canv(new_i, new_j)

        self.move_on_ui(int(new_x), int(new_y))
        self.board_pos = (new_i, new_j)

class BoardArea(Areas):
    
    def __init__(self, parent, side : int, init_states : list[bool],playerW = Human(), playerB = Human()):
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
        self.configure(bg=self.get_color("darkgreen"))
        self.parent = parent
        
        # drawing the board cases
        for i in range(8):
            for j in range(8):
                posx, posy = self._plat2canv(i,j)
                self.create_oval(posx - self.case_radius,
                                 posy - self.case_radius,
                                 posx + self.case_radius,
                                 posy + self.case_radius,
                                 fill = self.get_color("cases"),
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
                    self.wp.append(Pawn(self, (x, y), (i, j), "white"))
                    x,y = self._plat2canv(7 - i, 7 - j)
                    self.bp.append(Pawn(self, (x, y), (7 - i, 7 - j), "black"))

        #initiating a Game
        
        self.playerB = playerB
        self.playerW = playerW  
        self.whoistoplay = self.playerW 
        self.board = cc.Game()
        self.game_is_on = True

        # working data 
        self.__clicked_piece = ""
        self.joueurajouer = False
        self.pospioninit = (-1,-1)
        self.coup_precedent = ""
        self.coup_courant = []
        
        # list of all pawns
        self.allPawns = self.bp + self.wp


    def reset(self,playerW : Player,playerB : Player):
        self.playerW = playerW
        self.playerB = playerB
        self.whoistoplay = self.playerW
        self.__reset_working_data()
        count = 0
        for i in range(4):
            for j in range(4):
                if i + j <= 3:
                    self.wp[count].move_to_case(i, j)
                    self.bp[count].move_to_case(7 - i, 7 - j)
                    count += 1
        self.highlight_cases([])
        self.show_arrows([], "black")
        self.show_arrows([], "white")
        self.board.new_game()
        self.game_is_on = True
        
    #unused
    def set_players(self, playerW : str, playerB : str):
        if playerW == "Human":
            self.playerW = Human()
        elif playerW == "C++ AI":
            self.playerW = AI_cpp(depth = 3)
        else:
            print(f"Unknown player type {playerW}")

        if playerB == "Human":
            self.playerB = Human()
        elif playerB == "C++ AI":
            self.playerB = AI_cpp(depth = 3) # TODO ça modifie pas
        else:
            print(f"Unknown player type {playerB}")  

        self.whoistoplay = self.playerW
        print("RAAAAH")
        
    def pawn_pressed(self,event):
        ''' run when starting to click on a pawn '''
        assert(self.__clicked_piece == "")
        eventi, eventj = self._canv2plat(event.x, event.y)
        
        # find clicked piece
        for p in self.allPawns:
            if p.is_in_case(eventi,eventj):
                    self.__clicked_piece = p
                    break
            
        if self.__clicked_piece == "": # no piece is clicked
            return

        if self.whoistoplay != None and self.whoistoplay.getHumanity():
            # if it's human turn
            movablePawns = self.wp if self.whoistoplay == self.playerW else self.bp
            if self.__clicked_piece in movablePawns:
                self.__clicked_piece.redraw()
                if self.pospioninit == (-1,-1):
                    self.pospioninit = self.__clicked_piece.get_board_pos()
                    self.coup_courant = [(eventi, eventj)]
                
    def pawn_moved(self,event):
        '''' run when dragging a pawn across the board '''
        if self.__clicked_piece != "" and self.whoistoplay.getHumanity():
            # can drag a pawn only if human plays
            self.__clicked_piece.move_on_ui(event.x, event.y)

    def pawn_released(self,event):
        ''' run when releasing a pawn on the board '''
        if self.__clicked_piece == "":
            return
        
        color_playing = "black" if self.whoistoplay == self.playerB else "white"

        # first highlight cases
        if self.__clicked_piece in self.allPawns:
            if self.show_moves: # Drawing of the reachable cases from clicked pawn
                i, j = self.__clicked_piece.get_board_pos()
                possible_moves = self.possible_moves([i, j], self.__pbpn2pospion(self.wp, self.bp))
                self.highlight_cases(possible_moves)   

        movablePawns = self.wp if self.whoistoplay == self.playerW else self.bp
        if self.whoistoplay.getHumanity() and self.__clicked_piece in movablePawns:
            new_i, new_j = self._canv2plat(event.x,event.y)
            old_i, old_j = self.coup_courant[-1]
            etude_coup = self.elementaryMove(old_i, old_j, new_i, new_j)

            if new_i == old_i and new_j == old_j:
                # dropped the pawn on the same case as before
                self.__clicked_piece.move_to_case(new_i, new_j) 
                self.__reset_working_data()
                self.show_arrows([], color_playing)
            elif etude_coup != 'illegal' and (self.coup_precedent == "" or (self.coup_precedent == "jump" and etude_coup == "jump")):
                # wants to play a legal move
                self.__clicked_piece.move_to_case(new_i, new_j) 
                self.coup_precedent = etude_coup
                self.joueurajouer = True
                self.coup_courant.append((new_i,new_j))
                if (color_playing == 'white' and self.show_white_ar) or\
                   (color_playing == 'black' and self.show_black_ar):
                    self.show_arrows(self.coup_courant, color_playing)
                    self.highlight_cases([])   # remove highlighted cases
            else:
                # reset clicked pawn position
                i, j = self.__clicked_piece.get_board_pos()
                self.__clicked_piece.redraw()
                if self.coup_courant == [self.pospioninit]:
                    self.coup_courant = []
    
        else:
            # reset clicked pawn position
            i, j = self.__clicked_piece.get_board_pos()
            self.__clicked_piece.redraw()
        self.__clicked_piece = ""

            
            
    def __reset_working_data(self):
        self.__clicked_piece = ""
        self.joueurajouer = False
        self.pospioninit = (-1,-1)
        self.coup_precedent = ""
        self.coup_courant = []
    
    def __swap_whoistoplay(self):
        if self.whoistoplay == self.playerW:
            self.whoistoplay = self.playerB
        else:
            self.whoistoplay = self.playerW
            
                        
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
            print(f"Unknown UI parameter {s}")

    #All of the following functions are graphical functions
 
    def show_coordinates(self):
        '''show empty cases with their coordinates'''
        lfont = self.get_font(20)
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
    
    def show_hitboxes(self):
        '''show empty cases with their coordinates'''
        self.img = PhotoImage(width=self.side, height=self.side)
        N = self.side
        for x in range(N):
            print(x)
            for y in range(N):
                i, j = self._canv2plat(x, y)
                c_x, c_y = self._plat2canv(i, j)
                c_x, c_y = int(max(c_x, 0)), int(max(c_y, 0))
                c_x, c_y = min(c_x, self.side), min(c_y, self.side)
                r = int(c_x / self.side * 255)
                g = int(c_y / self.side * 255)
                self.img.put(f"{self.from_rgb((r, g, 150))}", (x, y))
        self.create_image(self.side // 2, self.side // 2, image=self.img)
    
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
    
    # All of the following functions in the class are TEST
  

    def can_play(self):
        return self.joueurajouer

    def jouerIA(self):
        if not self.game_is_on:
            return
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
            #check if the game is over
            type_end_of_game =self.board.state_of_game()
            if type_end_of_game !=0:
                self.parent.game_is_over(type_end_of_game)
                self.game_is_on = False
            
            if self.whoistoplay.getHumanity():
                self.__swap_whoistoplay()
                if  not self.whoistoplay.getHumanity():
                    self.jouerIA()
            else:
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
            p_i, p_j = p.get_board_pos()
            pb.append([p_i, p_j])
        for p in bp:
            p_i, p_j = p.get_board_pos()
            pn.append([p_i, p_j])
        
        for a,b in pb:
            pospion[a,b] = True
        for a,b in pn:
            pospion[a,b] = True
        return(pospion)
    

    def apply_move_without_check(self,move : list):
        ''' Apply move on screen'''
        for p in self.wp:
            p_i, p_j = p.get_board_pos()
            if p_i == move[0][0] and p_j == move[0][1]:
                pp = p
                break
        for p in self.bp:
            p_i, p_j = p.get_board_pos()
            if p_i == move[0][0] and p_j == move[0][1]:
                pp = p
                break
            
        pp.move_to_case(move[-1][0],move[-1][1])
        pp.redraw()
                    
    def elementaryMove(self,a : int,b : int,c : int,d : int):
        ''' Check legality of an elementary move'''
        if (0<= a <= 7) and (0<= b <= 7) and (0<= c <= 7) and (0<= d <= 7):
            if ((c-a)*(d-b) == 0) or ((d-b)/(c-a)==-1):
                rep = (abs(a-c+b-d) <= 1) and (abs(a-c) +abs(b-d) <= 2)
                for p in self.bp:
                    p_i, p_j = p.get_board_pos()
                    if p_i == c and p_j ==d:
                        return('illegal')
                    elif p_i != a or p_j != b :
                        if p_i == (a+c)/2 and p_j == (b+d)/2:
                            rep = True
                        elif (p_i- a)*(d-b) == (p_j-b)*(c-a) and (p_i -a)*(c-a) + (p_j-b)*(d-b) >= 0 and (p_i -c)*(a-c) + (p_j-d)*(b-d) >= 0:
                            return('illegal')

                for p in self.wp:
                    p_i, p_j = p.get_board_pos()
                    if p_i == c and p_j ==d:
                        return('illegal')
                    elif p_i != a or p_j !=b :
                        if p_i == (a+c)/2 and p_j == (b+d)/2:
                            rep = True
                        elif (p_i- a)*(d-b) == (p_j-b)*(c-a) and (p_i -a)*(c-a) + (p_j-b)*(d-b) >= 0 and (p_i -c)*(a-c) + (p_j-d)*(b-d) >= 0:
                            return('illegal')
                if rep:
                    if (abs(a-c+b-d) <= 1) and abs(a-c) +abs(b-d) <= 2:
                        return("non jump")
                    else:
                        return("jump")
                else:   
                    return('illegal')
            else:
                return('illegal')
        else:
            return('illegal')