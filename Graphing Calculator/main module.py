#imports
import pygame, math, sys
from pygame import *
from math import *
pygame.init()


##########################################
# NAME: graphpaper
# COURSE: ICS3U
# FILE: Stewart_Connor_Summitive-Assignment.py
# DESCRIPTION: this draws the graphpaper grid and clips it to the right hand of the window
# INPUTS: PPGrid - this is the pixles per grid, it is preset by default but can be changed by user input - from keyboard
# OUTPUTS:
# screen.FILL(screenColour) - makes the screen background the screnColour veriable
# pygame.DRAW.LINE(screen, gridcolor, (gridx,0),(gridx,height), 1) - draws a line based on [(gridx,0),(gridx,height)] coordinates, horizontal lines in graph paper
# pygame.DRAW.LINE(screen,gridcolor, (0,gridy), (width, gridy), 1) - draws a line based on [(0,gridy), (width, gridy)] coordinates, vertical lines in graph paper
# pygame.DRAW.LINE(screen,gridcolor, (width,0),(width,height),5) - draws a line based on [(width,0),(width,height)] coordinates, draws graph borders
# pygame.DRAW.LINE(screen,functionLineColour, (midx MULTIPLIED BY PPGrid, 0), (midx MULTIPLIED BY PPGrid,height),3) - draws a line based on [(midx MULTIPLIED BY PPGrid, 0), (midx MULTIPLIED BY PPGrid,height)] coordinates, draws x-axis
# pygame.DRAW.LINE(screen,functionLineColour, (0,midy MULTIPLIED BY PPGrid),(width, midy MULTIPLIED BY PPGrid), 3) - draws a line based on [(0,midy MULTIPLIED BY PPGrid),(width, midy MULTIPLIED BY PPGrid)] coordinates, draws the y-axis
# screen.SET_CLIP(None) - clip reset to all the window
# ALGORITHM:
# SET screen CLIP to right of page (0,0)
# FOR horizontalLineInGrid IN RANGE (width DEVIDED BY PPGrid) :
#   gridx EQUIALS PPGRID MULTIPLIED BY horizontalLineInGrid
#   gridy EQUIALS PPGrid MULTIPLIED BY horizontalLineInGrid
#   DRAW LINE on screen (screen, gridcolor, (gridx, 0), (gridx, height), 1)
#   DRAW LINE on screen (screen,gridcolor, (0,gridy), (width, gridy), 1)
# ENDFOR
# DRAW LINE on screen (screen,gridcolor, (width,0),(width,height),5)
# midx, midy EQUIALS width DEVIDED BY (2 MULTIPLIED BY PPGrid), height DEVIDED BY (2 MULTIPLIED BY PPGrid)
# DRAW LINE on screen (screen,functionLineColour, (midx MULTIPLIED BY PPGrid, 0), (midx MULTIPLIED BY PPGrid, height),3)
# DRAW LINE on screen pygame.DRAW.LINE(screen, functionLineColour, (0, midy MULTIPLIED BY PPGrid),(width, midy MULTIPLIED BY PPGrid), 3)
# SET screen CLIP to None to clip reset window
# DATE:
# Code added: 22/11/2014 - created a screen
# Code added: 26/12/2014 - put graph paper on screen
# Code updated: 8/1/2015 - added more in text commants and clerified some veriable names
# pseudocode added: 12/1/2015 - added pseudocode and in text quotation showing when if, def, for, and while commands are END'ed
##########################################
#graph paper generator
def graphpaper (PPGrid):
    screen.set_clip(0,0,width, height)
    screen.fill(screenColour)

    #draw graph paper
    for horizontalLineInGrid in range (width/PPGrid) :
        gridx = PPGrid * horizontalLineInGrid
        gridy = PPGrid * horizontalLineInGrid
        pygame.draw.line(screen, gridcolor, (gridx,0),(gridx,height), 1)
        pygame.draw.line(screen,gridcolor, (0,gridy), (width, gridy), 1)
    #ENDFOR

    pygame.draw.line(screen,gridcolor, (width,0),(width,height),5)

    #graph x and y axis
    midx, midy=width/(2*PPGrid),height/(2*PPGrid)
    pygame.draw.line(screen,functionLineColour, (midx*PPGrid, 0), (midx*PPGrid,height),3)
    pygame.draw.line(screen,functionLineColour, (0,midy*PPGrid),(width, midy*PPGrid), 3)

    #clip reset to all the window
    screen.set_clip(None)
#END graphpaper

##########################################
# NAME: graphEq
# COURSE: ICS3U
# FILE: Stewart_Connor_Summitive-Assignment.py
# DESCRIPTION: This set of procidures allowes for functions to be evaluated and plotted onto the graph paper
# INPUTS:
# equationOne - this is imported to the function and is used to evaluate and graph the first equation stored
# equationTwo - this is imported to the fucntion and is used to evaluate and graph the second equation stored
# PPGrid - pixle per grid, this controlled the amount of boxes on the graph paper, used for scalling in evaluations in order to make graph work
# xValue - the x value on the function to plot the coordanent of, from keyboard EVENT.KEY
# keyboard input through EVENT.KEY (stuff thats called but not stored by input), from keyboard:
#   EVENT.KEY RETURN - pressing this initiates the plotting of the x value based on xVAlue's contents, from keyboard
#   EVENT.KEY IS EQUIAL TO K_ESCAPE - system exits the application
#   EVENT.KEY IS EQUIAL TO K_y - plots the y-intercept
#   EVENT.KEY IS EQUIAL TO K_(HOME, PAGEUP, PAGEDOWN, INSERT, END) - used for changing the scale of the graph
#   EVENT.KEY IS EQUIAL TO K_q - resets the graph by CALL myMain
#   EVENT.KEY IS EQUIAL TO K_g - allows the user to plot another function by CALL myMain
# OUTPUTS:
# pygame.DRAW.LINE(screen, graphcolor,positionOne,positionTwo,3)
# pygame.DRAW.LINE(screen, graphcolor,positionOneFunctionTwo,positionTwoFunctionTwo,3)
# pygame.DRAW.CIRCLE(screen,interceptDotColour,(INTERGER(width DEVIDED BY 2),INTERGER(height DEVIDED BY 2 MINUS yint MULTIPLIED BY PPGrid)), INTERGER(4))
# pygame.DRAW.CIRCLE(screen, dotColour, (INTERGER(width DEVIDED BY 2 PLUS x MULTIPLIED BY PPGrid),INTERGER(height DEVIDED BY 2 MINUS yVal MULTIPLIED BY PPGrid)),INTERGER(4))
# screen.BLIT(ploty,(width PLUS 210,170))
# screen.BLIT(plotx, (width PLUS 10, 170))
# screen.BLIT(tital, (width PLUS 10, 20))
# screen.BLIT(instruct, (width  PLUS  10, 70))
# screen.BLIT(instruct, (width  PLUS  10, 100))
# screen.BLIT(instruct, (width  PLUS  20, 120))
# screen.BLIT(instruct, (width  PLUS  10, height MINUS 70))
# screen.BLIT(instruct, (width  PLUS  10, height MINUS 55))
# screen.BLIT(instruct, (width  PLUS  10, height MINUS 100))
# CALL myMain(1,' ')
# CALL graphpaper(PPGrid)
# CALL graphEq(equationOne,equationTwo,PPGrid)
# CALL graphpaper(PPGrid)
# CALL graphEq(equationOne,equationTwo,PPGrid)
# CALL graphpaper(PPGrid)
# CALL graphEq(equationOne,equationTwo,PPGrid)
# CALL graphpaper(PPGrid)
# CALL graphEq(equationOne,equationTwo,PPGrid)
# CALL graphpaper(PPGrid)
# CALL graphEq(equationOne,equationTwo,PPGrid)
# CALL myMain(2,equationOne)
# screen.SET_CLIP(width,0,width+exteraW,height-30)
# screen.SET_CLIP(None)
# creen.SET_CLIP(width+10, 150, width+exteraW, 200)
# screen.FILL(screenColour)
# ALGORITHM:
# FOR theRangeOfWidth IN RANGE(width) :
#     TRY:
#         x EQUIALS (width DEVIDED BY 2 MINUS theRangeOfWidth)/(float(PPGrid))
#         y EQUIALS EVAL(equationOne)
#         positionOne EQUIALS (width DEVIDED BY 2 PLUS x MULTIPLIED BY PPGrid,height DEVIDED BY 2 MINUS y MULTIPLIED BY PPGrid)
# 
#         pointThatXAxisIsDrawnAcross EQUIALS x EQUIALS (width DEVIDED BY 2  MINUS  (theRangeOfWidth PLUS 1))/(float(PPGrid))
#         pointThatYAxisIsDrawnAcross EQUIALS EVAL(equationOne)
#         positionTwo EQUIALS (width DEVIDED BY 2 PLUS pointThatXAxisIsDrawnAcross MULTIPLIED BY PPGrid,height DEVIDED BY 2 MINUS pointThatYAxisIsDrawnAcross MULTIPLIED BY PPGrid)
# 
#         pygame.DRAW.LINE(screen, graphcolor,positionOne,positionTwo,3)
#     EXCEPT:
#         PASS
#     ENDTRY
#     TRY:
#         x EQUIALS (width DEVIDED BY 2 MINUS theRangeOfWidth)/(float(PPGrid))
#         y2 EQUIALS EVAL(equationTwo)
#         positionOneFunctionTwo EQUIALS (width DEVIDED BY 2 PLUS x MULTIPLIED BY PPGrid,height DEVIDED BY 2 MINUS y2 MULTIPLIED BY PPGrid)
# 
#         pointThatXAxisIsDrawnAcross EQUIALS x EQUIALS (width DEVIDED BY 2  MINUS  (theRangeOfWidth PLUS 1))/(float(PPGrid))
#         pointThatYAxisIsDrawnAcross2 EQUIALS EVAL(equationTwo)
#         positionTwoFunctionTwo EQUIALS (width DEVIDED BY 2 PLUS pointThatXAxisIsDrawnAcross MULTIPLIED BY PPGrid,height DEVIDED BY 2 MINUS pointThatYAxisIsDrawnAcross2 MULTIPLIED BY PPGrid)
# 
#         pygame.DRAW.LINE(screen, graphcolor,positionOneFunctionTwo,positionTwoFunctionTwo,3)
#     EXCEPT:
#         PASS
#     ENDTRY
# ENDFOR
# screen.SET_CLIP(width,0,width PLUS exteraW,height MINUS 30)
# tital EQUIALS font2.RENDER("Connors Grapher", 1, titalcolour)
# screen.BLIT(tital, (width PLUS 10, 20))
# instruct EQUIALS font.RENDER("Select 'q' to restart", 1, textColour)
# screen.BLIT(instruct, (width  PLUS  10, 70))
# x EQUIALS 0
# TRY:
#     yint EQUIALS EVAL(equationOne)
#     yint EQUIALS ROUND(yint,2)
# EXCEPT:yint EQUIALS 'dne'
# ENDTRY
# instruct EQUIALS font.RENDER("The y MINUS intercept is at (0,"+STRING(yint)+")", 1, textColour)
# screen.BLIT(instruct, (width  PLUS  10, 100))
# IF yint DOES NOT EQUIAL 'dne':
#     instruct EQUIALS font.RENDER("Select 'y' to plot the intercept, ENTER number to plot y value", 1, textColour)
#     screen.BLIT(instruct, (width  PLUS  20, 120))
# ENDIF
# instruct EQUIALS font4.RENDER("Select PgUp and PgDn to toggle scale", 1, textColour)
# screen.BLIT(instruct, (width  PLUS  10, height MINUS 70))
# instruct EQUIALS font4.RENDER("Home = default END = largest INSERT = smallest", 1, textColour)
# screen.BLIT(instruct, (width  PLUS  10, height MINUS 55))
# instruct EQUIALS font.RENDER("Select 'g' to add another graph", 1, textColour)
# screen.BLIT(instruct, (width  PLUS  10, height MINUS 100))
# xValue EQUIALS []
# xVal ='?'
# yVal EQUIALS '?'
# screen.SET_CLIP(None)
# active EQUIALS True
# WHILE active:
#     screen.SET_CLIP(width PLUS 10, 150, width PLUS exteraW, 200)
#     screen.fill(screenColour)
#     xDisplay EQUIALS string.join(xValue)
#     xDisplay EQUIALS string.replace(xDisplay, " ", "")
#     plotx EQUIALS font.RENDER("x = "+STRING(xDisplay), 1, dotColour)
#     screen.BLIT(plotx, (width PLUS 10, 170))
#     ploty EQUIALS font.RENDER("("+STRING(xVal)+","+STRING(yVal)+")", 1, dotColour)
#     screen.BLIT(ploty,(width PLUS 210,170))
#     screen.SET_CLIP(None)
#     pygame.display.update()
#     FOR event IN pygame.event.GET():
#         IF event.type IS EQUIAL TO pygame.QUIT:
#             avtive EQUIALS False
#         ENDIF
#         ELIF event.type IS EQUIAL TO pygame.KEYDOWN:
#             IF event.key IS EQUIAL TO K_q
#                 CALL myMain(1,' ')
#             ELIF event.key IS EQUIAL TO K_y:
#                 TRY:
#                     pygame.DRAW.CIRCLE(screen,interceptDotColour,(INTERGER(width DEVIDED BY 2),INTERGER(height DEVIDED BY 2 MINUS yint MULTIPLIED BY PPGrid)), INTERGER(4))
#                 EXCEPT:
#                     PASS
#                 ENDTRY
#             ENDIF
#             ELIF event.key IS EQUIAL TO K_PAGEUP:
#                 IF (PPGrid MULTIPLIED BY 2) > 256 :
#                     PPGrid EQUIALS PPGrid
#                 else :
#                     PPGrid EQUIALS PPGrid MULTIPLIED BY 2
#                 ENDIF
#                 CALL graphpaper(PPGrid)
#                 CALL graphEq(equationOne,equationTwo,PPGrid)
#             ELIF event.key IS EQUIAL TO K_PAGEDOWN:
#                 IF (PPGrid DEVIDED BY 2) < 1 :
#                     PPGrid EQUIALS PPGrid
#                 else :
#                     PPGrid EQUIALS PPGrid DEVIDED BY 2
#                 ENDIF
#                 CALL graphpaper(PPGrid)
#                 CALL graphEq(equationOne,equationTwo,PPGrid)
#             ELIF event.key IS EQUIAL TO K_HOME:
#                 PPGrid EQUIALS 32
#                 CALL graphpaper(PPGrid)
#                 CALL graphEq(equationOne,equationTwo,PPGrid)
#             ELIF event.key IS EQUIAL TO K_END:
#                 PPGrid EQUIALS 256
#                 CALL graphpaper(PPGrid)
#                 CALL graphEq(equationOne,equationTwo,PPGrid)
#             ELIF event.key IS EQUIAL TO K_INSERT:
#                 PPGrid EQUIALS 1
#                 CALL graphpaper(PPGrid)
#                 CALL graphEq(equationOne,equationTwo,PPGrid)
#             ELIF event.key IS EQUIAL TO K_g:
#                 CALL myMain(2,equationOne)
# 
#             ELIF event.key IS EQUIAL TO K_RETURN:
#                 TRY:
#                     x EQUIALS xVal EQUIALS float(xDisplay)
#                     yVal EQUIALS EVAL(equationOne)
#                     pygame.DRAW.CIRCLE(screen, dotColour, (INTERGER(width DEVIDED BY 2 PLUS x MULTIPLIED BY PPGrid),INTERGER(height DEVIDED BY 2 MINUS yVal MULTIPLIED BY PPGrid)),INTERGER(4))
#                     xValue EQUIALS []
#                 EXCEPT:
#                     PASS
#                 ENDTRY
#             ELIF event.key IS EQUIAL TO K_1:
#                 xValue.APPEND("1")
#             ELIF event.key IS EQUIAL TO K_2:
#                 xValue.APPEND("2")
#             ELIF event.key IS EQUIAL TO K_3:
#                 xValue.APPEND("3")
#             ELIF event.key IS EQUIAL TO K_4:
#                 xValue.APPEND("4")
#             ELIF event.key IS EQUIAL TO K_5:
#                 xValue.APPEND("5")
#             ELIF event.key IS EQUIAL TO K_6:
#                 xValue.APPEND("6")
#             ELIF event.key IS EQUIAL TO K_7:
#                 xValue.APPEND("7")
#             ELIF event.key IS EQUIAL TO K_8:
#                 xValue.APPEND("8")
#             ELIF event.key IS EQUIAL TO K_9:
#                 xValue.APPEND("9")
#             ELIF event.key IS EQUIAL TO K_0:
#                 xValue.APPEND("0")
#             ELIF event.unicode IS EQUIAL TO u'.':
#                 xValue.APPEND(".")
#             ELIF event.unicode IS EQUIAL TO u'-':
#                 xValue.APPEND("-")
#             ELIF event.key IS EQUIAL TO K_DELETE:
#                 xValue EQUIALS []
#             ELIF event.key IS EQUIAL TO K_BACKSPACE and xValue !=[]:
#                 xValue.pop()
#             ELIF event.key IS EQUIAL TO K_ESCAPE :
#                 sys.exit()
#             ENDIF
#         ENDIF
#     ENDFOR
# ENDWHILE
# pygame.quit()
# sys.exit()
# DATE:
# Code added: 30/12/2014 - added the ability for various functions to be graphed, BUG fixes
# Code added/updated: 31/12/2014 - added the ability for y-int to be pointed, the graph to reset, and to plot on points on the function, BUG fixes, application rearanged for better optamization
# Code added/DEBUG: 1/1/2015 - added the ability for multiple functions to be displayed on graph, mother of all BUG fixes, verious attempts to polish code to imporve stability over long periods
# Code added: 2/1/2015 - added the ability for the user to change the scale of the graph, changed graph resolution to binary numbers to allow the graph to be scalled as much as possable
# Code DEBUG: 5/1/2015 - fixed bug whitch caused crashes on plotting asypmtotslocated on the coordenents (0,0), updated some of the user prompts to make more sense
# Code DEBUG : 6/1/2015 - fixed bug which prevented user from backspaces when plotting points in the x = label
# Code updated : 8/1/2015 - added more in text commants and clerified some veriable names
# Code added : 9/1/2015 - added the ability to exit the application when the ESCAPE key is pressed
# pseudocode added : 12/1/2015 - added pseudocode and in text quotation showing when if, def, for, and while commands are END'ed
# Code updated: 13/1/2015 - cleared user prompt, updated pseudocode 
##########################################
def graphEq(equationOne, equationTwo, PPGrid):

    #graphing of the equation
    for theRangeOfWidth in range(width) :
        #x = the domain of the function 
        #y = the range of the function
        try:
            x = (width/2-theRangeOfWidth)/(float(PPGrid))
            y = eval(equationOne)
            positionOne = (width/2+x*PPGrid,height/2-y*PPGrid)

            pointThatXAxisIsDrawnAcross = x = (width/2 - (theRangeOfWidth+1))/(float(PPGrid))
            pointThatYAxisIsDrawnAcross = eval(equationOne)
            positionTwo = (width/2+pointThatXAxisIsDrawnAcross*PPGrid,height/2-pointThatYAxisIsDrawnAcross*PPGrid)

            pygame.draw.line(screen, graphcolor,positionOne,positionTwo,3)
        except:
            pass
        #ENDTRY
        
        try:
            x = (width/2-theRangeOfWidth)/(float(PPGrid))
            y2 = eval(equationTwo)
            positionOneFunctionTwo = (width/2+x*PPGrid,height/2-y2*PPGrid)

            pointThatXAxisIsDrawnAcross = x = (width/2 - (theRangeOfWidth+1))/(float(PPGrid))
            pointThatYAxisIsDrawnAcross2 = eval(equationTwo)
            positionTwoFunctionTwo = (width/2+pointThatXAxisIsDrawnAcross*PPGrid,height/2-pointThatYAxisIsDrawnAcross2*PPGrid)

            pygame.draw.line(screen, graphcolor,positionOneFunctionTwo,positionTwoFunctionTwo,3)
        except:
            pass
        #ENDTRY
    #ENDFOR
        
    #clip the screen to keep text clean
    screen.set_clip(width,0,width+exteraW,height-30)

    #INSTRUCTIONS ANDD RESULTS
    tital = font2.render("Connors Grapher", 1, titalcolour)
    screen.blit(tital, (width+10, 20))
    instruct = font.render("Select 'q' to restart", 1, textColour)
    screen.blit(instruct, (width + 10, 70))

    #calculate and display y-intercept
    x = 0
    try:
        yint = eval(equationOne)
        yint = round(yint,2)
    except:yint = 'dne'
    #ENDTRY
    
    instruct = font.render("The y-intercept is at (0,"+str(yint)+")", 1, textColour)
    screen.blit(instruct, (width + 10, 100))

    if yint <> 'dne':
        instruct = font4.render("Select 'y' to plot the intercept, ENTER number to plot y value", 1, textColour)
        screen.blit(instruct, (width + 20, 120))
    #ENDIF

    #resizing of grid instructions
    instruct = font4.render("Select PgUp and PgDn to toggle scale", 1, textColour)
    screen.blit(instruct, (width + 10, height-70))

    instruct = font4.render("Home = default END = largest INSERT = smallest", 1, textColour)
    screen.blit(instruct, (width + 10, height-55))

    #instruction for new graph
    instruct = font.render("Select 'g' to add another graph", 1, textColour)
    screen.blit(instruct, (width + 10, height-100))



    #plotting of x values
    xValue = []
    xVal ='?'
    yVal = '?'

    
    #reset clip of screen
    screen.set_clip(None)
    
        
    #running a infinite loop to control window
    active = True
    while active:
        #clip the screen to show x and y value
        screen.set_clip(width+10, 150, width+exteraW, 200)
        screen.fill(screenColour)
        xDisplay = string.join(xValue)
        xDisplay = string.replace(xDisplay, " ", "")
        plotx = font.render("x = "+str(xDisplay), 1, dotColour)
        screen.blit(plotx, (width+10, 170))
        ploty = font.render("("+str(xVal)+","+str(yVal)+")", 1, dotColour)
        screen.blit(ploty,(width+210,170))
        screen.set_clip(None)
        
        
                    
        #update the screen
        pygame.display.update()

        #keyboard and mouse control
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                avtive = False
            #ENDIF
            #commands for starting over and other features
            elif event.type == pygame.KEYDOWN:
                if event.key == K_q:
                    myMain(1,' ')
                elif event.key == K_y:
                    try:
                        pygame.draw.circle(screen,interceptDotColour,(int(width/2),int(height/2-yint*PPGrid)), int(4))
                    except:
                        pass
                    #ENDTRY
                #ENDIF

                                       
                #resize commands
                elif event.key == K_PAGEUP:
                    if (PPGrid*2) > 256 :
                        PPGrid=PPGrid
                    else :
                        PPGrid=PPGrid*2
                    #ENDIF
                    graphpaper(PPGrid)
                    graphEq(equationOne,equationTwo,PPGrid)
                elif event.key == K_PAGEDOWN:
                    if (PPGrid/2) < 1 :
                        PPGrid=PPGrid
                    else :
                        PPGrid=PPGrid/2
                    #ENDIF
                    graphpaper(PPGrid)
                    graphEq(equationOne,equationTwo,PPGrid)
                elif event.key == K_HOME:
                    PPGrid=32
                    graphpaper(PPGrid)
                    graphEq(equationOne,equationTwo,PPGrid)
                elif event.key == K_END:
                    PPGrid=256
                    graphpaper(PPGrid)
                    graphEq(equationOne,equationTwo,PPGrid)
                elif event.key == K_INSERT:
                    PPGrid=1
                    graphpaper(PPGrid)
                    graphEq(equationOne,equationTwo,PPGrid)
                    

                #add new graph and plot points
                elif event.key == K_g:
                    myMain(2,equationOne)

                elif event.key == K_RETURN:
                    try:
                        x = xVal = float(xDisplay)
                        yVal = eval(equationOne)
                        pygame.draw.circle(screen, dotColour, (int(width/2+x*PPGrid),int(height/2-yVal*PPGrid)),int(4))
                        xValue = []
                    except:
                        pass
                    #ENDTRY
                
                
                #numbers typed in for equation and the x veriable
                elif event.key == K_1:
                    xValue.append("1")
                elif event.key == K_2:
                    xValue.append("2")
                elif event.key == K_3:
                    xValue.append("3")
                elif event.key == K_4:
                    xValue.append("4")
                elif event.key == K_5:
                    xValue.append("5")
                elif event.key == K_6:
                    xValue.append("6")
                elif event.key == K_7:
                    xValue.append("7")
                elif event.key == K_8:
                    xValue.append("8")
                elif event.key == K_9:
                    xValue.append("9")
                elif event.key == K_0:
                    xValue.append("0")

                #negitive and decimal values
                elif event.unicode == u'.':
                    xValue.append(".")
                elif event.unicode == u'-':
                    xValue.append("-")

                #delete and backspace
                elif event.key == K_DELETE:
                    xValue = []
                elif event.key == K_BACKSPACE and xValue !=[]:
                    xValue.pop()

                #allows user to exit
                elif event.key == K_ESCAPE :
                    sys.exit()
                #ENDIF
            #ENDIF
        #ENDFOR
    #ENDWHILE
    pygame.quit()
    sys.exit()
#END graphEq

##########################################
# NAME: myMain
# COURSE: ICS3U
# FILE: Stewart_Connor_Summitive-Assignment.py
# DESCRIPTION: this module controlls user input and calls other functions
# INPUTS:
# graphs - this  number is inported when the function is called, it indicates weather or not there is a second equation, if the value is 1 then there are 2 equations
# equationOne - this is the user equation input formated to be evaluated and graph in the next function, it is imported so that another function can be stored without deleteing this one
# equation - this is an entry that contains the users keystrocks, numbers are appended to this equation via the event command, from keyboard EVENT.KEY
# keyboard input through EVENT.KEY (stuff thats called but not stored by input), from keyboard:
#    EVENT.KEY IS EQUIAL TO K_RETURN - enteres graphEq function
#    EVENT.KEY IS EQUIAL TO K_DELETE - resets equation veriable
#    EVENT.KEY IS EQUIAL TO K_BACKSPACE and equation !=[] - deletes one object from equation tupple
#    EVENT.KEY IS EQUIAL TO K_q - CALL myMain
#    EVENT.KEY IS EQUIAL TO K_ESCAPE  - system exits the EVENT.KEY
# OUTPUTS:
# screen.fill(screenColour)
# BLIT "Connors Grapher" to screen 
# BLIT "Type in equation. Ex - 2*x^2 + 8*x + 2" to screen
# BLIT "Select ENTER key when done or 'q' to restart" to screen
# BLIT "Select BACKSPACE to back once, or DELETE to clear" to screen
# BLIT "s=sin(), c=cos(), t=tan(), r = sqrt(), a=||, l=log10(), n=log(), e=e, p=pi" to screen
# BLIT "for decimal values enter real numbers for coefficients" to screen
# CALL graphpaper(PPGrid)
# CALL graphEq(equationOne, equationTwo, PPGrid) 
# ALGORITHM:
# PPGrid EQUIALS 32 - always use a k that is a factor of width and hight
# IF graphs IS EQUIAL TO 1 :
#   screen.FILL(screenColour)
#   CALL graphpaper(PPGrid)
#   equation EQUIALS []
#   equationTwo EQUIALS ' '
# ELSE :
#   screen.SET_CLIP(width, 0, width PLUS exteraW, height)
#   screen.FILL(screenColour)
#   screen.SET_CLIP(None)
#   equationTwo EQUIALS equationOne
#   equation EQUIALS []
# tital EQUIALS font2.render("Connors Grapher", 1, titalcolour)
# screen.BLIT(tital, (width PLUS 10, 20))
# instruct EQUIALS font.RENDER("Type in equation. Ex - 2*x^2 + 8*x + 2", 1, textColour)
# screen.BLIT(instruct, (width PLUS 10, 70))
# instruct EQUIALS font.RENDER("Select ENTER key when done or 'q' to restart", 1, textColour)
# screen.BLIT(instruct, (width PLUS 10, 100))
# instruct EQUIALS font.RENDER("Select BACKSPACE to back once, or DELETE to clear", 1, textColour)
# screen.BLIT(instruct, (width PLUS 10, 130))
# instruct EQUIALS font3.RENDER("s=sin(), c=cos(), t=tan(), r = sqrt(), a=||, l=log10(), n=log(), e=e, p=pi", 1, textColour)
# screen.BLIT(instruct, (width PLUS 10, 160))
# instruct EQUIALS font3.RENDER("for decimal values enter real numbers for coefficients", 1, textColour)
# screen.BLIT(instruct, (width PLUS 10, 180))
# done EQUIALS false
# active EQUIALS True
# WHILE active:
#   screen.SET_CLIP(width PLUS 10, height MINUS 30,width PLUS exteraW, height)
#   screen.FILL(screenColour)
#   equationOne EQUIALS string.JOIN(equation)
#   equationOne EQUIALS string.REPLACE(equationOne, " ", "") 
#   eqshow EQUIALS font.render("Function: y= " PLUS equationOne, 1, titalcolour)
#   screen.BLIT(eqshow, (width PLUS 10,height MINUS 30))  
#   FOR event in pygame.event.GET():
#       IF event.TYPE IS EQUIAL TO pygame.QUIT:
#           avtive EQUIALS False
#           done EQUIALS True     
#       ELIF event.TYPE IS EQUIAL TO pygame.KEYDOWN:
#         APPEND to equation string based on outcome of if...elif conditional determining which key has been pressed
#         ENDIF
#       ENDIF
#   ENDFOR
# ENDWHILE
# IF done:
#   pygame.QUIT()
# ELSE:
#   screen.SET_CLIP(width,0,width+exteraW, height-30)
#   screen.FILL(screenColour)
#   screen.SET_CLIP(None)
#   CALL graphEq(equationOne, equationTwo, PPGrid)
# ENDIF
# sys.exit()
# DATE:
# Code added: 28/12/2014 - added Entry tupple (equation) and instructions
# Code added: 29/12/2014 - added the ability for keyboard keys to be recognized by the application, BUG fixes
# Code added: 30/12/2014 - added the ability for various functions to be graphed, BUG fixes
# Code DEBUG: 31/12/2014 - BUG fixes, application rearanged for better optamization
# Code added: 1/1/2015 - added the ability for multiple functions to be displayed on grap (in this function added an else statment and the ability to import graphs and equationOne)h, mother of all BUG fixes,
#    verious attempts to polish code to imporve stability over long periods
# Code updated: 5/1/2015 updated some of the user prompts to make more sense
# Code updated: 8/1/2015 - added more in text commants and clerified some veriable names
# Code added : 9/1/2015 - added the ability to exit the application when the ESCAPE key is pressed
# pseudocode added: 12/1/2015 - added pseudocode and in text quotation showing when if, def, for, and while commands are END'ed
##########################################

#main procidure
def myMain(graphs, equationOne):
    #pixle per grid (SELF NOTE: always use a k that is a factor of width and hight, else the graph breaks)
    PPGrid = 32

    #start an array that will hold equation
    if graphs == 1:
        #clear screen
        screen.fill(screenColour)
        graphpaper(PPGrid)
        equation=[]
        equationTwo = ' '
    else:
        screen.set_clip(width, 0, width+exteraW, height)
        screen.fill(screenColour)
        screen.set_clip(None)
        equationTwo = equationOne
        equation = []
    #ENDIF
    


    #INSTRUCTIONS ANDD RESULTS
    tital = font2.render("Connors Grapher", 1, titalcolour)
    screen.blit(tital, (width+10, 20))

    instruct = font.render("Type in equation. Ex - 2*x^2 + 8*x + 2", 1, textColour)
    screen.blit(instruct, (width + 10, 70))

    instruct = font.render("Select ENTER key when done or 'q' to restart", 1, textColour)
    screen.blit(instruct, (width + 10, 100))

    instruct = font.render("Select BACKSPACE to back once, or DELETE to clear", 1, textColour)
    screen.blit(instruct, (width + 10, 130))

    instruct = font3.render("s=sin(), c=cos(), t=tan(), r = sqrt(), a=||, l=log10(), n=log(), e=e, p=pi", 1, textColour)
    screen.blit(instruct, (width + 10, 160))

    instruct = font3.render("for decimal values enter real numbers for coefficients", 1, textColour)
    screen.blit(instruct, (width + 10, 180))


    done = False
    
    #main loop
    active = True
    while active:
        #updating the screen
        screen.set_clip(width+10, height - 30,width+exteraW, height)
        screen.fill(screenColour)

        #joining the equation array without commas
        equationOne = string.join(equation)
        equationOne = string.replace(equationOne, " ", "") #replaces spaces with no spaces

        #render and blit equaation
        eqshow = font.render("Function: y= "+equationOne,1,titalcolour)
        screen.blit(eqshow, (width+10,height-30))
        
        pygame.display.update()

        #keyboard and mouse control
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                avtive = False
                done = True
            
            elif event.type == pygame.KEYDOWN:
                #APPEND to equation string based on outcome of if...elif conditional determining which key has been pressed
                #math operations and symboles for the equation
                if event.unicode == u'*':
                    equation.append("*")
                elif event.unicode == u'+':
                    equation.append('+')
                elif event.unicode == u'/':
                    equation.append('/')
                elif event.unicode == u'-':
                    equation.append('-')
                elif event.unicode == u'.':
                    equation.append('.')
                elif event.unicode == u'(':
                    equation.append('(')
                elif event.unicode == u')':
                    equation.append(')')
                elif event.unicode == u'^':
                    equation.append('**') #two astrixes is the power in python for some reason
                    

                
                #numbers typed in for equation and the x veriable
                elif event.key == K_1:
                    equation.append("1")
                elif event.key == K_2:
                    equation.append("2")
                elif event.key == K_3:
                    equation.append("3")
                elif event.key == K_4:
                    equation.append("4")
                elif event.key == K_5:
                    equation.append("5")
                elif event.key == K_6:
                    equation.append("6")
                elif event.key == K_7:
                    equation.append("7")
                elif event.key == K_8:
                    equation.append("8")
                elif event.key == K_9:
                    equation.append("9")
                elif event.key == K_0:
                    equation.append("0")


                #math function commands
                elif event.key == K_s:
                    equation.append("sin(")
                elif event.key == K_c:
                    equation.append("cos(")
                elif event.key == K_t:
                    equation.append("tan(")
                elif event.key == K_r:
                    equation.append("sqrt(")
                elif event.key == K_a:
                    equation.append("abs(")
                elif event.key == K_l:
                    equation.append("log10(")
                elif event.key == K_n:
                    equation.append("log(")
                elif event.key == K_e:
                    equation.append("e")
                elif event.key == K_p:
                    equation.append("pi")
                
                #x is the domain of the function
                elif event.key == K_x:
                    equation.append("x")

                elif event.key == K_RETURN:
                    active = False
                elif event.key == K_DELETE:
                    equation = []
                elif event.key == K_BACKSPACE and equation !=[]:
                    equation.pop() 
                elif event.key == K_q:
                    myMain(1,' ')
                #allows user to exit
                elif event.key == K_ESCAPE :
                    sys.exit()
                #ENDIF
            #ENDIF
        #ENDFOR
    #ENDWHILE                   

    #an option to quit at tentering the equation or to graph
    if done:
        #quit at end loop
        pygame.quit()
    else:
        #clip right side of the screen (except equation)
        screen.set_clip(width,0,width+exteraW, height-30)
        screen.fill(screenColour)
        screen.set_clip(None)

        #graph equation function called
        graphEq(equationOne, equationTwo, PPGrid)
    #ENDIF
    sys.exit()
#END myMain

##########################################
# NAME: main
# COURSE: ICS3U
# FILE: Stewart_Connor_Summitive-Assignment.py
# DESCRIPTION: this module pulgs in veribles and settings, as well as calles the functions to run the program
# INPUTS: None
# OUTPUTS:
# CALL myMain(1, ' ') - calls the myMain function with the following data in order to start the graphing calculator
# pygame.display.SET_CAPTION("Connor's Graphing Calculator") - sets the caption on the window as "Connor's Graphing Calculator"
# ALGORITHM:
# width, height = 512, 512 - a binary grid is good for addition and multiplication when changing scale, it prevents image destortion
# screen EQUIALS pygame.display.set_mode((width PLUS exteraW, height))
# IF __name__ IS EQUIAL TO '__main__' :
#   CALL myMain(1, ' ')
# ENDIF
# DATE:
# Code added: 7/11/2014 - made run window and veriables
# Code added: 2/1/2015 - added more veriables involving colour options
# pseudocode added: 12/1/2015 - added pseudocode and in text quotation showing when if, def, for, and while commands are END'ed
##########################################

#---------Veriables---------#
#font veriables
font = pygame.font.SysFont('Arial',16, 1)
font2 = pygame.font.SysFont('Arial_Black',24)
font3 = pygame.font.SysFont('Arial', 14)
font4 = pygame.font.SysFont('Times_New_Roman', 12)
black = (0,0,0) #black
red = (200,0,0) #red
blue = (0,0,200) #blue
yellow = (0,200,0) #yellow
textColour = black 
functionLineColour = black
interceptDotColour = black 
screenColour = (255,255,255)#white
gridcolor = (200,200,200)#light black
graphcolor = (200,0,200)#light blue
dotColour = (0,0,200)#dark blue
titalcolour = (200, 0, 150) #dark purple

#screen veriables
width, height = 512, 512 #a binary grid is good for addition and multiplication when changing scale
exteraW = 400 #short for extera width
screen = pygame.display.set_mode((width+exteraW,height))
userInMenue = True
pygame.display.set_caption("Connor's Graphing Calculator")



#tell python to run program
if __name__=='__main__':
    myMain(1,' ')
#ENDIF
