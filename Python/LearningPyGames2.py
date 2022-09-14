import sys, pygame
from turtle import width
pygame.init()

clock = pygame.time.Clock()

size = width, height = 500, 500
black = 0,0,0
white = 255,255,255
screen = pygame.display.set_mode(size)

center = [width/2, height/2]
velocity = [0, 0]
acceleration = [0, 0.0098]
circle = pygame.draw.circle(screen, white, center, 15, 0)

while 1:
    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()

    velocity[1] += acceleration[1]*time
    center[1] += velocity[1]

    if center[1] > height or center[1] < 0: velocity[1] *= -1

    screen.fill(black)
    circle = pygame.draw.circle(screen, white, center, 15, 0)
    pygame.display.flip()
    clock.tick(60)

##### ADD PHYSICS FOR ACCELERATION

#Vf = Vi + AT