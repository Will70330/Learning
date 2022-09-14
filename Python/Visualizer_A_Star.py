from itertools import count
import pygame
import math
from queue import PriorityQueue

####################################### SETTING UP WINDOW ############################################################################
WIDTH = 800                                                          # width will function as the length of a side of our square window
WIN = pygame.display.set_mode((WIDTH, WIDTH))                        # Creates a square window of our specified WIDTH (in this case, 800 by 800)
pygame.display.set_caption("A* Path Finding Algorithm Visualizer")   # caption for the display window
######################################################################################################################################

############## COLORS #################
RED = (255,0,0)
GREEN = (0,255,0)
BLUE = (0,0,255)
BLACK = (0,0,0)
WHITE = (255,255,255)

CRIMSON = (220,20,60)
YELLOW = (255,255,0)
PURPLE = (106,13,173)
ORANGE = (255, 140, 0)
GREY = (128,128,128)
TURQUOISE = (64, 224, 208)
NAVY = (0,0,128)
#######################################

class Node:
    def __init__(self, row, col, width, total_rows):
        self.row = row
        self.col = col
        # x, y = to its index in the row,col * its width so we can get an x, y pair for every square in the grid
        self.x = row * width 
        self.y = col * width
        self.color = WHITE # initialize the color to white for all nodes
        self.neighbors = [] #list of neighboring nodes
        self.width = width
        self.total_rows = total_rows

    def get_position(self):             # Returns position of node
        return self.row, self.col

     # returns whether or not the node is closed, open, a wall, the start, the end, etc
    def is_closed(self):
        return self.color == CRIMSON

    def is_open(self):
        return self.color == TURQUOISE

    def is_wall(self):
        return self.color == BLACK

    def is_start(self):
        return self.color == GREEN

    def is_end(self):
        return self.color == ORANGE

    def reset(self):
        self.color = WHITE
        
    # set the colors of the node to signify closed, open, wall, etc
    def make_closed(self):
        self.color = CRIMSON

    def make_open(self):
        self.color = TURQUOISE

    def make_wall(self):
        self.color = BLACK

    def make_start(self):
        self.color = GREEN

    def make_end(self):
        self.color = ORANGE

    def make_path(self):
        self.color = NAVY

    # function to draw the nodes:
    def draw(self, win):
        pygame.draw.rect(win, self.color, (self.x, self.y, self.width, self.width))

    # this will check up, down, left, right of the node to see if those neighboring nodes are walls or not (if not, we can add them as neighbors to the neighbors list)
    def update_neighbors(self, grid):
        self.neighbors = []
        # if the current node is not a node in the last row and the node above is not a barrier, then we can add that node to the neighbors list
        if self.row < self.total_rows - 1 and not grid[self.row + 1][self.col].is_wall():    # down
            self.neighbors.append(grid[self.row + 1][self.col])

        if self.row > 0 and not grid[self.row - 1][self.col].is_wall():    # up
            self.neighbors.append(grid[self.row - 1][self.col])

        if self.col < self.total_rows - 1 and not grid[self.row][self.col + 1].is_wall():    # right
            self.neighbors.append(grid[self.row][self.col + 1])

        if self.col > 0 and not grid[self.row][self.col - 1].is_wall():    # left
            self.neighbors.append(grid[self.row][self.col - 1])

    def __lt__(self, othr): # defines less than
        return False

def h(n1, n2): # heuristic function
    # bc/ n1 can be represented as a coordinate, we can initialize x1,y1 to n1 (3,0) or (0,0) or (5,6) etc
    x1, y1 = n1
    x2, y2 = n2
    return abs(x1-x2) + abs(y1-y2) # manhattan distance

def reconstruct_path(came_from, current, draw):
    # while we still have values in came_from list, we take the end node and connect it to the node that it came from and then draw that node
    # until we've reached the start node which is not in the came_from list
    while current in came_from:
        current = came_from[current]
        current.make_path()
        draw()

def algorithm(draw, grid, start, end):
    count = 0
    open_set = PriorityQueue()
    open_set.put((0, count, start))
    came_from = {}
    g_score = {node: float("inf") for row in grid for node in row} # syntax for list/dictionary comprehension
    g_score[start] = 0
    f_score = {node: float("inf") for row in grid for node in row} # syntax for list/dictionary comprehension
    f_score[start] = h(start.get_position(), end.get_position())

    open_set_hash = {start}

    while not open_set.empty():
        for event in pygame.event.get(): # allows user to quit
            if event.type == pygame.QUIT:
                pygame.quit()
        
        current = open_set.get()[2] #current node we're looking at
        open_set_hash.remove(current)

        if current == end:
            reconstruct_path(came_from, end, draw)
            end.make_end()
            start.make_start()
            return True #make the path

        for neighbor in current.neighbors: 
            temp_g_score = g_score[current] + 1

            if temp_g_score < g_score[neighbor]:
                came_from[neighbor] = current
                g_score[neighbor] = temp_g_score
                f_score[neighbor] = temp_g_score + h(neighbor.get_position(), end.get_position())
                if neighbor not in open_set_hash:
                    count += 1
                    open_set.put((f_score[neighbor], count, neighbor))
                    open_set_hash.add(neighbor)
                    neighbor.make_open()
        draw()

        if current != start:
            current.make_closed()
    
    return False

# when making a grid of nodes, we need to define how big the nodes in the grid will be by taking the width of the window and dividing by the total num of rows (or cols, doesn't matter bc Square)
def make_grid(rows, width):
    grid = []
    sideLength = width // rows # // represents integer division (so no floating left over)
    for i in range(rows):
        grid.append([])
        for j in range(rows):
            block = Node(i, j, sideLength, rows)
            grid[i].append(block)
    return grid

# draws the grid lines 
def draw_grid(win, rows, width):
    sideLength = width // rows
    for i in range(rows):
        pygame.draw.line(win, GREY, (0, i * sideLength), (width, i * sideLength))
        for j in range(rows):
            pygame.draw.line(win, GREY, (j * sideLength, 0), (j * sideLength, width))

# draws and updates frames for the entire window
def draw(win, grid, rows, width):
    win.fill(WHITE)             # clears frame

    for row in grid:
        for node in row:
            node.draw(win)      # draws all the nodes

    draw_grid(win, rows, width) # draws the gridlines 
    pygame.display.update()     # updates the display of the window

# accepts mouse input and corresponds position to the node currently at a specific row, col in the grid
def get_clicked_pos(pos, rows, width):
    gap = width // rows
    y,x = pos

    row = y // gap
    col = x // gap
    return row, col

# clears the entire board to reset
def reset_Board(win, grid, rows, width):
    win.fill(WHITE)

    for row in grid:
        for node in row:
            node.reset()
            node.draw(win)

    draw_grid(win, rows, width)
    pygame.display.update()

def main(win, width):
    ROWS = 50
    grid = make_grid(ROWS, width)

    start = None
    end = None

    run = True
    started = False
    while run:
        draw(win, grid, ROWS, width)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:   # stops running the visualizer
                run = False
            if started:                     # once algorithm begins, won't let user interrupt (prevents user from causing errors during runtime)
                continue
            if pygame.mouse.get_pressed()[0]: # index 0 is the left mouse button
                pos = pygame.mouse.get_pos()
                row, col = get_clicked_pos(pos, ROWS, width) # gives us the specific node that we clicked on
                node = grid[row][col]

                if not start and node != end:
                    start = node
                    start.make_start()

                elif not end and node != start:
                    end = node
                    end.make_end()

                elif node != end and node != start:
                    node.make_wall()

            elif pygame.mouse.get_pressed()[2]: # index 2 is right mouse  button
                pos = pygame.mouse.get_pos()
                row, col = get_clicked_pos(pos, ROWS, width) # gives us the specific node that we clicked on
                node = grid[row][col]
                node.reset() 
                if node  == start:
                    start = None
                elif node == end:
                    end = None

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE and not started:
                    for row in grid:
                        for node in row:
                            node.update_neighbors(grid)
                    algorithm(lambda: draw(win, grid, ROWS, width), grid, start, end)
                if event.key == pygame.K_r and not started:
                    reset_Board(win, grid, ROWS, width)
                    start = None
                    end = None


        
    pygame.quit()


main(WIN, WIDTH)


