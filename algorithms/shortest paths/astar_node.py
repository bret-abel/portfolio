import pygame as pg

colors = {
    "pink" : pg.Color("#FFAEBC"),
    "tiffany blue" : pg.Color("#A0E7E5"),
    "mint" : pg.Color("#B4F8C8"),
    "citrus" : pg.Color("#D8CC34"),
    "brown" : pg.Color("#65463E"),
    "black" : pg.Color(0, 0, 0),
    "white" : pg.Color(255, 255, 255)
}

class Node:
    def __init__(self, row, col, width, total_rows):
        self.row = row
        self.col = col
        self.width = width
        self.total_rows = total_rows
        self.x = row * width
        self.y = col * width
        self.color = colors["white"]

    def get_pos(self):
        return self.row, self.col

    def is_closed(self):
        return self.color == colors["pink"]

    def is_open(self):
        return self.color == colors["tiffany blue"]

    def is_blocked(self):
        return self.color == colors["black"]

    def is_start(self):
        return self.color == colors["mint"]

    def is_end(self):
        return self.color == colors["citrus"]

    def close(self):
        self.color = colors["pink"]

    def open(self):
        self.color = colors["tiffany blue"]

    def make_block(self):
        self.color = colors["black"]

    def set_as_start(self):
        self.color = colors["mint"]

    def set_as_end(self):
        self.color = colors["citrus"]

    def reset(self):
        self.color = colors["white"]

    def make_path(self):
        self.color = colors["brown"]

    def draw(self, win):
        pg.draw.rect(win, self.color, (self.x, self.y, self.width, self.width))

    def update_neighbors(self, grid):
        self.neighbors = []
        if self.row < self.total_rows - 1: # down neighbor
            if not grid[self.row + 1][self.col].is_blocked():
                self.neighbors.append(grid[self.row + 1][self.col])
        
        if self.row > 0: # up neigbor
            if not grid[self.row - 1][self.col].is_blocked():
                self.neighbors.append(grid[self.row - 1][self.col])
        
        if self.col < self.total_rows - 1: # right neighbor
            if not grid[self.row][self.col + 1].is_blocked():
                self.neighbors.append(grid[self.row][self.col + 1])
        
        if self.col > 0: # left neighbor
            if not grid[self.row][self.col - 1].is_blocked():
                self.neighbors.append(grid[self.row][self.col - 1])
        
    def __lt__(self, other):
        return False