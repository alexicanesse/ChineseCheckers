# -*- coding: utf-8 -*-

import numpy as np
import random as random
import bin.solvers.AlphaBeta as ab
from typing import final


class Player():
    def __init__(self,Humanity):
        self.__isHuman = Humanity
    
    @final
    def getHumanity(self):
        return(self.__isHuman)
    
    def applyMove(self,intwhoistoplay,move):
        pass
    
    def getMove(self):
        return([])
    
    

class Human(Player):
    def __init__(self):
        super().__init__(True)

class AI_cpp(Player):
    def __init__(self,depth = 3):
        super().__init__(False)
        self.__solver = ab.Solver()
        self.depth = depth
        
    def applyMove(self,intwhoistoplay,move):
        self.__solver.move(intwhoistoplay,move)
    
    def getMove(self):
        return(self.__solver.getMove(self.depth,-100000,100000))
        
        
    


class AI_python(Player):
    # TEST for the whole class
    def __init__(self,canevas,couleur):
        super().__init__(False)
        self.couleur = couleur
        self._canevas = canevas
        self.pions = canevas.bp if couleur == "black" else canevas.wp
        self.pospions = np.empty((8,8),dtype = bool)
        self.maj_pospions()
        self.directions_possibles = [(1,0),(0,1),(-1,0),(0,-1),(1,-1),(-1,1)]
        self.isHuman = False
    
    
    def maj_pospions(self):
        self.pospions.fill(False)
        for p in self._canevas.wp:
            self.pospions[p.case_x,p.case_y] = True
        for p in self._canevas.bp:
            self.pospions[p.case_x,p.case_y] = True
    
    def coups_possibles(self,p):
        rep = []
        self.pospions[p.case_x,p.case_y] = False
        nb_non_sauts = 0
        for a,b in self.directions_possibles:
            if (0 <= p.case_x +a <= 7) and (0<= p.case_y +b <= 7) and (not self.pospions[p.case_x +a,p.case_y+b]):
                rep.append((p,[(p.case_x,p.case_y),(p.case_x+a,p.case_y +b)]))
                nb_non_sauts +=1
        
        #gestion des sauts
        vus = [(p.case_x,p.case_y)]
        a_voir = [[(p.case_x,p.case_y)]]
        while len(a_voir) != 0:
            l =  a_voir.pop(0)
            pp =  l[-1]
            for (a,b) in self.directions_possibles:
                ii = pp[0] + a
                jj = pp[1] + b
                saut = False
                while (0 <= ii <= 7) and (0<= jj <= 7):#on se déplace dans la direction donnée par a,b et on cherche le premier pion
                    if  self.pospions[ii,jj]:
                        saut = True
                        break
                    ii += a
                    jj += b
                if saut:
                    arrivex = 2*ii - pp[0]
                    arrivey = 2*jj - pp[1]
                    if (0<= arrivex <= 7) and (0<= arrivey <= 7) and (not self.pospions[arrivex,arrivey]) and (not (arrivex,arrivey) in vus):
                        saut_legal = True
                        ii += a
                        jj += b
                        while ii != arrivex or jj != arrivey:
                            if self.pospions[ii,jj]:
                                saut_legal = False
                                break
                            ii += a
                            jj += b
                        if saut_legal:
                            ll = l.copy()
                            ll.append((arrivex,arrivey))
                            a_voir.append(ll)
                            vus.append((arrivex,arrivey))
                            rep.append((p,ll))
                    

        _ = vus.pop(0)
        self.pospions[p.case_x,p.case_y] = True
        return(rep)
    
    
    def coups_possibles2(self,p,pospions):
        rep = []
        pospions[p] = False
        #gestion des sauts
        vus = [p]
        a_voir = [[p]]
        while len(a_voir) != 0:
            l =  a_voir.pop(0)
            pp =  l[-1]
            for (a,b) in self.directions_possibles:
                ii = pp[0] + a
                jj = pp[1] + b
                saut = False
                while (0 <= ii <= 7) and (0<= jj <= 7):#on se déplace dans la direction donnée par a,b et on cherche le premier pion
                    if pospions[ii,jj]:
                        saut = True
                        sauti = ii
                        sautj = jj
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
                            rep.append((ll))
                    
        for a,b in self.directions_possibles:
            if (0 <= p[0] +a <= 7) and (0<= p[1] +b <= 7) and (not pospions[p[0] +a,p[1]+b]):
                rep.append([(p),(p[0]+a,p[1] +b)])
                
        pospions[p] = True
        return(rep)
    
    def coups_possibles3(self,p,pospions):
        rep = []
        pospions[p[0],p[1]] = False
        
        #gestion des sauts
        vus = np.empty((8,8),dtype = 'bool')
        vus.fill(False)
        vus[p[0],p[1]] = True
        a_voir = [(p[0],p[1])]
        while len(a_voir) != 0:
            pp =  a_voir.pop(0)
            for (a,b) in self.directions_possibles:
                ii = pp[0] + a
                jj = pp[1] + b 
                saut = False
                while (0 <= ii <= 7) and (0<= jj <= 7):#on se déplace dans la direction donnée par a,b et on cherche le premier pion
                    if pospions[ii,jj]:
                        saut = True
                        break
                    ii += a
                    jj += b
                if saut:
                    arrivex = 2*ii - pp[0]
                    arrivey = 2*jj - pp[1]
                    if (0<= arrivex <= 7) and (0<= arrivey <= 7) and (not pospions[arrivex,arrivey]) and (not vus[arrivex,arrivey]):
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
                            a_voir.append((arrivex,arrivey))
                            vus[arrivex,arrivey] = True
                            rep.append(((p[0],p[1]),(arrivex,arrivey)))
        pospions[p[0],p[1]] = True
        for a,b in self.directions_possibles:
            arrivex = p[0] + a
            arrivey =  p[1] + b
            if (0 <= arrivex <= 7) and (0<= arrivey <= 7) and (not pospions[arrivex,arrivey]) and (not vus[arrivex,arrivey]):
                rep.append(((p[0],p[1]),(arrivex,arrivey)))
        return(rep)
    
    def pbpn2pospion(self,pb,pn):
        pospion = np.empty((8,8), dtype = 'bool')
        pospion.fill(False)
        for a,b in pb:
            pospion[a,b] = True
        for a,b in pn:
            pospion[a,b] = True
        return(pospion)
        
    def jouerRnd(self):
        temp = []
        self.maj_pospions()
        for p in self.pions:
            temp += self.coups_possibles(p)
        ind = random.randint(0,len(temp)-1)
        return(temp[ind])
    
    def changer(self,ll,x,y):
        for i,l in enumerate(ll):
            if l ==x:
                ll[i]= y
                break
        
    
    def jouerprofondeur1(self):
        pb = []
        for p in self._canevas.wp:
            pb.append((p.case_x,p.case_y))
        pn = []
        for p in self._canevas.bp:
            pn.append((p.case_x,p.case_y))
            

        temp = []
        self.maj_pospions()
        for p in self.pions:
            temp += self.coups_possibles(p)
        
        score = 1300
        coupf  = temp[0]
        for t in temp:
            piece,coup = t
            if self.couleur =="black":
                self.changer(pn,coup[0],coup[-1])
            else:
                self.changer(pb,coup[0],coup[-1])
            score_eval = self.evaluer(pb,pn)
            if score_eval <= score:
                score = score_eval
                coupf = t
                
            if self.couleur =="black":
                self.changer(pn,coup[-1],coup[0])
            else:
                self.changer(pb,coup[-1],coup[0])
        return(coupf)
            
        
    def joueralphabeta(self,depth):
        pb = []
        for p in self._canevas.wp:
            pb.append((p.case_x,p.case_y))
        pn = []
        for p in self._canevas.bp:
            pn.append((p.case_x,p.case_y))
        
        pospion = self.pbpn2pospion(pb, pn)
        
        coups_possibles = []
        for p in pn:#on joue les noirs
            coups_possibles += self.coups_possibles2(p,pospion)
        coups_possibles.sort(key = lambda x: sum(x[-1]) - sum(x[0]))
        alpha = -1e10
        beta = 1e10
        v = beta
        coup_a_jouer = coups_possibles[0]
        for iii,coup in enumerate(coups_possibles):
            print(100*(iii+1)/len(coups_possibles),'%')
            self.changer(pn,coup[0],coup[-1])
            ab = self.alphabeta(depth-1,pb,pn,alpha,beta,"max")
            self.changer(pn,coup[-1],coup[0])
            if ab <= v:
                v = ab
                coup_a_jouer = coup
            if v <= alpha:
                coup_a_jouer = coup
                break
            beta = min(beta,v)
        for p in self.pions:
            if (p.case_x,p.case_y) == coup_a_jouer[0]:
                return(p,coup_a_jouer)
    
    

    def alphabeta(self,depth,pb,pn,alpha,beta,t):
        if self.evaluerpb(pb) ==20:
            return(1e10)
        if self.evaluerpn(pn) == 20:
            return(-1e10)
        if depth ==0:
            return(self.evaluer(pb,pn))
        if  t == "min":
            v = 1e10
            # récupération des coups possibles
            pospion = self.pbpn2pospion(pb, pn)
            fils = []
            for p in pn:#les noirs jouent
                fils += self.coups_possibles3(p,pospion)

            fils.sort(key = lambda x: sum(x[-1]) - sum(x[0]))
            for fil in fils:
                self.changer(pn,fil[0],fil[-1])
                v = min(v,self.alphabeta(depth-1,pb,pn,alpha,beta,"max"))
                self.changer(pn,fil[-1],fil[0])
                
                if alpha >= v:
                    return(v)
                beta = min(beta,v)
        else:
            v = -1e10
            #récupération des fils
            pospion = self.pbpn2pospion(pb, pn)
            fils = []
            for p in pb:#leses blancs jouent
                fils += self.coups_possibles3(p,pospion)
            fils.sort(key = lambda x: sum(x[0]) - sum(x[-1]))
            for fil in fils:
                self.changer(pb,fil[0],fil[-1])
                v = max(v,self.alphabeta(depth-1,pb,pn,alpha,beta,"min"))
                self.changer(pb,fil[-1],fil[0])
                if v >= beta:
                    return(v)
                alpha = max(alpha,v)
                
        return(v)
            
    
    def evaluer(self,pb,pn):
        return(6*self.evaluerpn(pn) -self.evaluerpb(pb))
                
    
    def evaluerpn(self,pn):
        rep = 0
        for p in pn:
            rep += p[0] + p[1]
        return(rep)
    
    def evaluerpb(self,pb):
        rep = 0
        for p in pb:
            rep +=14- p[0] - p[1]
        return(rep)
    

if __name__ == "__main__":
    pass