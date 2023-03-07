# -*- coding: utf-8 -*-

import numpy as np
import random as random
import bin.AlphaBeta as ab
from typing import final


class Player():
    def __init__(self,Humanity : bool):
        self.__isHuman = Humanity
    
    @final
    def getHumanity(self):
        return(self.__isHuman)
    
    def applyMove(self,intwhoistoplay : int,move : list):
        pass
    
    def getMove(self):
        return([])
    
    

class Human(Player):
    def __init__(self):
        super().__init__(True)

class AI_cpp(Player):
    def __init__(self,depth = 3):
        assert(depth >= 0)
        super().__init__(False)
        self.__solver = ab.Solver()
        self.depth = depth
        
    def applyMove(self,intwhoistoplay : int,move : list):
        self.__solver.move(intwhoistoplay,move)
    
    def getMove(self):
        return(self.__solver.getMove(self.depth,-100000,100000))