import tkinter as tk
import tkinter.font as font
import numpy as np
import re 
import math

window_win = tk.Tk()
window_lose = tk.Toplevel(window_win)

# program constants
WINDOW_SIDE = 1100
CASE_RADIUS_HIT = WINDOW_SIDE // (1 + 7 * math.sqrt(3))
CASE_RADIUS_UI = CASE_RADIUS_HIT // 2
FONT = f"Helvetica {WINDOW_SIDE // 47} bold"
SCREEN_HEIGHT = window_win.winfo_screenheight()
SCREEN_WIDTH = window_win.winfo_screenwidth()

def plat2canv(i : int, j : int):
    '''converts the coordinates of the board to pixels'''
    r, s = CASE_RADIUS_HIT, WINDOW_SIDE
    x = (i - j) * r / 2 + s / 2
    y = (i + j) * (r - s) / 14 + s - r / 2

    return (x, y)

def canv2plat(x : float, y : float):
    '''the inverse function'''
    r, s = CASE_RADIUS_HIT, WINDOW_SIDE        
    x -= s / 2
    y += r / 2 - s
    A, B = 7 / (r - s), 1 / r
    i = round(A * y + B * x)
    j = round(A * y - B * x)
    return(i, j)

def from_rgb(rgb):
    '''translates an rgb list of int to a tkinter friendly color code'''
    r = int(rgb[0])
    g = int(rgb[1])
    b = int(rgb[2])
    return "#%02x%02x%02x" % (r, g, b)     

def to_color(r : float):
    ''' converts a number between 0 and 1 to a tk color following a linear gradient '''
    assert(0 <= r and r <= 1)
    min_color = np.array([0, 255, 0])
    max_color = np.array([255, 0, 120])
    return from_rgb(r * max_color + (1 - r) * min_color)


# set up windows
window_y = (SCREEN_HEIGHT - WINDOW_SIDE) // 2
window_win_x = (SCREEN_WIDTH - 2 * WINDOW_SIDE) // 2
window_win.geometry(f"{WINDOW_SIDE}x{WINDOW_SIDE}+{window_win_x}+{window_y}")
window_win.title("Win weights")
canvas_win = tk.Canvas(window_win, width=WINDOW_SIDE, height=WINDOW_SIDE, bg="white")
canvas_win.pack()

window_lose_x = (SCREEN_WIDTH) // 2
window_lose.geometry(f"{WINDOW_SIDE}x{WINDOW_SIDE}+{window_lose_x}+{window_y}")
window_lose.title("Lose weights")
canvas_lose = tk.Canvas(window_lose, width=WINDOW_SIDE, height=WINDOW_SIDE, bg="white")
canvas_lose.pack()

# extract matrices values
win_matrix = []
lose_matrix = []

with open("Black_best_players.txt")as file :
    file = file.readlines() 
    i = 0
    while file[i] != "Win data:\n":
        i += 1
    
    for _ in range(8):
        i += 1
        coeffs = re.split('[{,} \n]',file[i])
        coeffs = [float(c) for c in coeffs if c != '']
        win_matrix.append(coeffs)
        

    while file[i] != "lose data:\n":
        i += 1
    
    for _ in range(8):
        i += 1
        coeffs = re.split('[{,} \n]',file[i])
        coeffs = [float(c) for c in coeffs if c != '']
        lose_matrix.append(coeffs)
    
    print("Win matrix :", win_matrix)
    print("Lose matrix :", lose_matrix)


# first draw the cases and set their colors according to the matrices
for i in range(8):
    for j in range(8):
        x, y = plat2canv(i, j)
        color_lose = to_color(lose_matrix[i][j])
        canvas_lose.create_oval(x - CASE_RADIUS_UI, 
                                y - CASE_RADIUS_UI, 
                                x + CASE_RADIUS_UI,
                                y + CASE_RADIUS_UI,
                                fill=color_lose)
        canvas_lose.create_text(x, y, text=round(lose_matrix[i][j], 2), font=FONT)
        color_win = to_color(win_matrix[i][j])
        canvas_win.create_oval(x - CASE_RADIUS_UI, 
                                y - CASE_RADIUS_UI, 
                                x + CASE_RADIUS_UI,
                                y + CASE_RADIUS_UI,
                                fill=color_win)
        canvas_win.create_text(x, y, text=round(win_matrix[i][j], 2), font=FONT)

# then draw the gradient hint at bottom-right corner
x0, y0 = int(0.8 * WINDOW_SIDE), int(0.8 * WINDOW_SIDE)
x1, y1 = int(0.95 * WINDOW_SIDE), int(0.85 * WINDOW_SIDE)

for x in range(x0, x1 + 1):
    color = to_color((x - x0) / (x1 - x0))
    canvas_lose.create_line(x, y0, x, y1, fill=color)
    canvas_lose.create_text(x0, y1 + 0.01 * WINDOW_SIDE, text="0", font=FONT)
    canvas_lose.create_text(x1, y1 + 0.01 * WINDOW_SIDE, text="1", font=FONT)

    canvas_win.create_line(x, y0, x, y1, fill=color)
    canvas_win.create_text(x0, y1 + 0.01 * WINDOW_SIDE, text="0", font=FONT)
    canvas_win.create_text(x1, y1 + 0.01 * WINDOW_SIDE, text="1", font=FONT)

window_win.mainloop()