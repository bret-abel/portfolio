import pygame as pg
import math
from astar_node import Node

from queue import PriorityQueue

WIDTH = 600
window = pg.display.set_mode((WIDTH, WIDTH))
pg.display.set_caption("A* Path Finding Algorithm")



def reconstruct_path(visited, current, draw):
    while current in visited:
        current = visited[current]
        current.make_path()
        draw()

def astar(draw, grid, start, end):
    count = 0
    open_set = PriorityQueue()
    open_set.put((0, count, start))
    visited = {} # use this to reconstruct path
    g_score = {node: float("inf") for row in grid for node in row}
    g_score[start] = 0
    f_score = {node: float("inf") for row in grid for node in row}
    f_score[start] = distance(start.get_pos(), end.get_pos())
    
    open_set_hash = {start}
    while not open_set.empty():
        for event in pg.event.get():
            if event.type == pg.QUIT:
                pg.quit()
        current = open_set.get()[2]
        open_set_hash.remove(current)

        if current == end:
            reconstruct_path(visited, end, draw)
            end.set_as_end()
            start.set_as_start()
            return True

        for neighbor in current.neighbors:
            temp_g_score = g_score[current] + 1 # this is where weights would come in to play
            if temp_g_score < g_score[neighbor]:
                visited[neighbor] = current
                g_score[neighbor] = temp_g_score
                f_score[neighbor] = temp_g_score + distance(neighbor.get_pos(), end.get_pos())

                if neighbor not in open_set_hash:
                    count += 1
                    open_set.put((f_score[neighbor], count, neighbor))
                    open_set_hash.add(neighbor)
                    neighbor.open()
        draw()

        if current != start:
            current.close()

    return False

def distance(p1, p2):
    x1, y1 = p1
    x2, y2 = p2
    return abs(x1 - x2) + abs(y1 - y2)

def make_grid(rows, width):
    grid = []
    size = width // rows
    for row in range(rows):
        grid.append([])
        for col in range(rows):
            node = Node(row, col, size, rows)
            grid[row].append(node)
    return grid

def draw_grid(win, rows, width):
    node_size = width // rows
    for i in range(rows):
        pg.draw.line(win, pg.Color("black"), (0, i * node_size), (width, i * node_size))
        for j in range(rows):
            pg.draw.line(win, pg.Color("black"), (j * node_size, 0), (j * node_size, width))
        
def draw(win, grid, rows, width):
    win.fill(pg.Color("white"))

    for row in grid:
        for node in row:
            node.draw(win)

    draw_grid(win, rows, width)
    pg.display.update()

def get_click_pos(pos, rows, width):
    node_size = width // rows
    y, x = pos
    row = y // node_size
    col = x // node_size
    return row, col

def main(win, width):
    ROWS = 30
    grid = make_grid(ROWS, width)

    start = None
    end = None

    run = True
    started = False

    while run:
        draw(win, grid, ROWS, width)
        for event in pg.event.get():
            if event.type == pg.QUIT:
                run = False

            # don't let user interrupt pathfinding
            if started:
                continue

            #left mouse button
            if pg.mouse.get_pressed()[0]:
                pos = pg.mouse.get_pos()
                row, col = get_click_pos(pos, ROWS, width)
                node = grid[row][col]

                # first two clicks set the start and end, the rest are blocked
                if not start and node != end:
                    start = node
                    start.set_as_start()

                elif not end and node != start:
                    end = node
                    end.set_as_end()

                elif node != end and node != start:
                    node.make_block()

            elif pg.mouse.get_pressed()[2]:
                pos = pg.mouse.get_pos()
                row, col = get_click_pos(pos, ROWS, width)
                node = grid[row][col]
                node.reset()
                if node == start:
                    start = None
                elif node == end:
                    end = None

            if event.type == pg.KEYDOWN:
                if event.key == pg.K_SPACE and not started:
                    started = True
                    for row in grid:
                        for node in row:
                            node.update_neighbors(grid)

                    astar(lambda: draw(win, grid, ROWS, width), grid, start, end)

                if event.key == pg.K_c:
                    start = None
                    end = None
                    grid = make_grid(ROWS, width)

    pg.quit()

main(window, WIDTH)