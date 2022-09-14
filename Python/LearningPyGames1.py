import sys, pygame
from cv2 import flip
from turtle import width

pygame.init() #initializes all of the built in modules

size = width, height = 680, 960
speed = [1, 1]
black = 0, 0, 0

screen = pygame.display.set_mode(size) #makes the window

ball = pygame.image.load("/Users/willmuckelroyiii/Desktop/Self-Taught Journey/Python/images/intro_ball.gif")
ballrect = ball.get_rect() #position of the ball image's rectangle

while 1:
    for event in pygame.event.get(): # check for exit
        if event.type == pygame.QUIT: sys.exit()

    ballrect = ballrect.move(speed) #update position of ball
    if ballrect.left < 0 or ballrect.right > width:
        speed[0] = -speed[0]
    if ballrect.top < 0 or ballrect.bottom > height:
        speed[1] = -speed[1]

    screen.fill(black) # clear screen
    screen.blit(ball, ballrect) #takes in source image and destination for that image
    pygame.display.flip() #displays everything onto the screen