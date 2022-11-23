from tkinter import *
import tkinter.font as font
        
class Areas(Canvas):

    def get_color(self, s):
        ''' all colors '''
        if s == "darkgreen": # color of the board
            return self.from_rgb((119, 149, 86))
        if s == "hovered": # color of the hovered buttons
            return self.from_rgb((149, 187, 74))
        if s == "pressed": # color of the pressed buttons
            return self.from_rgb((139, 173, 71))
        if s == "cases": # color of the cases
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
        if s == "gray": # color of grayed buttons
            return self.from_rgb((139, 139, 139))
        print(f"Unknown color {s}")
    
    def get_font(self, size):
        return font.Font(family='Helvetica', size=size)

    def from_rgb(self, rgb):
        '''translates an rgb tuple of int to a tkinter friendly color code'''
        return "#%02x%02x%02x" % rgb     