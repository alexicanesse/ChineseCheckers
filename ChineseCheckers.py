from tkinter import *
import tkinter.font as font
import numpy as np

class Pion:
    
    def __init__(self, canvas, x, y, color = "white"):
        self._color = color
        self._canvas = canvas
        self._item = canvas.create_oval(x-2*self._canvas.pawn_radius,
                                         y-2*self._canvas.pawn_radius,
                                         x+2*self._canvas.pawn_radius,
                                         y+2*self._canvas.pawn_radius,
                                         fill =self._color)
        self.x = x
        self.y = y
        self.casex, self.casey = self._canvas._canv2plat(x,y)
    
    def __str__(self):
        c = "blanc" if self._color == "white" else "noir"
        return(f"Pion {c} ({self.casex},{self.casey})")
    
    def __repr__(self):
        c = "b" if self._color =="white" else "n"
        return(f"P{c}{self.casex}{self.casey}")
    
    def redraw(self): # deletes and redraws the pawn
        self._canvas.delete(self._item)
        self._item = self._canvas.create_oval(self.x-2*self._canvas.pawn_radius,
                                               self.y-2*self._canvas.pawn_radius,
                                               self.x+2*self._canvas.pawn_radius,
                                               self.y+2*self._canvas.pawn_radius,
                                               fill=self._color)
        
    def move_on_ui(self, dx, dy): # moves the pawn
        self._canvas.move(self._item, dx, dy)
        self.x += dx
        self.y += dy

        
    def is_in_case(self, i, j):
        return((i == self.casex) and (j == self.casey))
    
    def get_color(self):
        return(self._color)
    
    def move(self,x,y):
        ncasex,ncasey = self._canvas._canv2plat(x,y)
        xf,yf = self._canvas._plat2canv(ncasex,ncasey)
        self.move_on_ui(xf-self.x,yf-self.y)
        self.x = xf
        self.y = yf
        self.casex = ncasex
        self.casey = ncasey

class BoardArea(Canvas):
    
    def __init__(self, parent, width, height):
        # initialization of the canvas
        Canvas.__init__(self, parent, width=width, height=height, highlightthickness=0)
        self.width = width # width of the board area
        self.height = height # height of the board area
        self.case_width = width/13 # width of a case (for the hitbox)
        self.case_height = height/13 # height of a case
        self.case_radius = self.case_width/8 # radius of a case
        self.pawn_radius = self.case_width/6 # radius of a pawn
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
                                 fill = "light grey")

        # adding pieces
        self.wp =[] # white pawns
        self.bp =[] # black pawns
        self.hc = [] # highlighted cases
        for i in range(4):
            for j in range(4):
                if i+j <= 3:
                    x,y = self._plat2canv(i,j)
                    self.wp.append(Pion(self, x, y, "white"))
                    x,y = self._plat2canv(7-i,7-j)
                    self.bp.append(Pion(self, x, y, "black"))
                    
        # TODO fonction qui affiche toutes les coordonnées des cases

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
    
    def jouerIA(self): 
        # TODO pendant le branchement
        pass
    
    def show_coordinates(self):
        '''show empty cases with their coordinates'''
        for i in range(8):
            for j in range(8):
                x, y = self._plat2canv(i, j)
                self.create_text(x, y, text=f"{i},{j}", fill="red")
    
    def show_moves(self, l_moves):
        '''highlights the cases in the l_moves cases list'''
        
        # de-highlight precedently highlighted cases
        for case in self.hc:
            self.delete(case)
        self.hc = []
        for move in l_moves: # l_moves is the list of intermediate cases
            i, j = move
            x, y = self._plat2canv(i, j)
            print(f"drawing {i},{j} at {x},{y}")
            self.hc.append(self.create_oval(x - self.case_radius,
                                 y - self.case_radius,
                                 x + self.case_radius,
                                 y + self.case_radius,
                                 fill = "spring green"))
    

class Plateau(Tk):

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
        self.__boardArea.configure(bg='burlywood2')
        self.__boardArea.pack(padx = 0,side = LEFT)
        
        # TEST
        self.__boardArea.show_coordinates()
        l = [(0,0), (3,4), (5,0)]
        self.__boardArea.show_moves(l)
        l = [(0,1), (7,4), (2,4)]
        self.__boardArea.show_moves(l)

if __name__ == "__main__":
    
    fen = Plateau(1300, 1000)
    fen.mainloop()
