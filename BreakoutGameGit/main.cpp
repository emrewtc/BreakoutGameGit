//
//  main.cpp
//  BreakoutGame
//
//  Created by Emre Cakirlar on 28/10/13.
//  Copyright (c) 2013 abc. All rights reserved.
//

#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2/SDL_opengl.h"

//  checking ball collision with the pad...
bool checkCollision(float Ax, float Ay, float Aw, float Ah, float Bx, float By, float Bw, float Bh)
{
    if(Ay + Ah < By)        return false; // if the ball is under the pad
    else if (Ay > By + Bh)  return false; // if the ball is above the pad
    else if (Ax > Bx + Bw)  return false; // if the ball is in left side of the pad
    else if (Ax + Aw < Bx)  return false; // if the ball is in right side of the pad
    return true; // collision
}

struct brick // structure of bricks
{
    float axisX_brick;
    float axisY_brick;
    float width_brick = 60;
    float height_brick = 20;
    bool isBrickAlive = true;
};

int main(int argc, const char * argv[])
{
    
    //*** initializing SDL parameters ***
    
    SDL_Window *screen;
    SDL_GLContext glcontext;
    
    SDL_Init(SDL_INIT_EVERYTHING);
    
    // setting up OpenGl's memory usage
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    screen = SDL_CreateWindow("My Game Window",
                              0,
                              0,
                              600, 400,
                              SDL_WINDOW_RESIZABLE| SDL_WINDOW_OPENGL); // creating a game screen...
    
    glcontext = SDL_GL_CreateContext(screen); // its necessary to use gl functions like below...
    
    
    glClearColor(1, 1, 1, 1); // R, G, B, A
    glViewport(0, 0, 600, 400); // screen portion
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_PROJECTION); // 2D rendering...
    glLoadIdentity(); // Save...
    glDisable(GL_DEPTH_TEST);
    
    //*** end initializing SDL parameters ***
    
    std:: cout << "Running...\n";
    
    
    //*** Main Loop ***
    
    bool isRunning = true;
    SDL_Event event;
    
    
    //  Pad variables
    float axisX_pad = 300; // starting x position of the pad
    float axisY_pad = 370; // starting y position of the pad
    float width_pad = 80; // width of the pad
    float height_pad = 20; // height of the pad
    
    //  Ball variables
    float axisX_ball = 50;
    float axisY_ball = 350;
    float WH_ball = 15;
    
    float velocityX = 3; // axis x velocity of the ball(its necessary for moving ball itself)
    float velocityY = -3; // axis y velocity of the ball(its necessary for moving ball itself)
    
    bool left = false, right = false ; // boolean variables for left and right arrow keys
    
    // Bricks variables
    const static int BRICKS = 10; // number of bricks as a global variable;
    
    brick bricks[BRICKS];
    
    // creating bricks...
    for (int i = 0, x = 4, y = 10; i < BRICKS; i++, x+=66)
    {
        if (x > 570)
        {
            x = 4;
            y += 25;
        }
        bricks[i].axisX_brick = x;
        bricks[i].axisY_brick = y;
        bricks[i].isBrickAlive = true;
        
    }
    
    
    
    while (isRunning)
    {
        
        // EVENTS
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } // it closes the game when the 'x' clicked...
            
            if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            } // it closes the game when the escape button pressed...
            
            if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_c) {
                glClearColor(1, 0, 0, 1);
            } // it changes the screen color black to red when the 'c' button pressed...
            
            if (event.type == SDL_KEYDOWN) // when the key pressed, set the keys pressed
            {
                if (event.key.keysym.sym == SDLK_LEFT) {
                    left = true;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    right = true;
                }
            }
            
            else if (event.type == SDL_KEYUP) // when the key released, set back the keys not pressed
            {
                if (event.key.keysym.sym == SDLK_LEFT) {
                    
                    left = false;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    right = false;
                }
                
            }
        }
        
        
        // LOGIC
        
        //  Pad's logic
        
        if (left == true) {
            axisX_pad -= 5;
        } // move 5 pxls to the left when the 'left' button pressed
        
        if (right == true) {
            axisX_pad += 5;
        } // move 5 pxls to the right when the 'right' button pressed
        
        if (axisX_pad < 0) {
            axisX_pad = 0;
        } // dont allow to pass through left border
        
        if (axisX_pad + width_pad > 600) {
            axisX_pad = 600 - width_pad;
        } // dont allow to pass through right border
        
        
        // Ball's logic
        
        // ball movement
        axisX_ball += velocityX;
        
        // checking collision with the bricks while the ball is moving on the X axis
        for (int i = 0; i < BRICKS; i++)
        {
            if (bricks[i].isBrickAlive)
            {
                if (checkCollision(axisX_ball, axisY_ball, WH_ball, WH_ball, bricks[i].axisX_brick, bricks[i].axisY_brick, bricks[i].width_brick, bricks[i].height_brick))
                {
                    velocityX = - velocityX;
                    bricks[i].isBrickAlive = false;
                }
            }
        }
        
        axisY_ball += velocityY;
        
        // checking collision with the bricks while the ball is moving on the Y axis
        for (int i = 0; i < BRICKS; i++)
        {
            if (bricks[i].isBrickAlive)
            {
                if (checkCollision(axisX_ball, axisY_ball, WH_ball, WH_ball, bricks[i].axisX_brick, bricks[i].axisY_brick, bricks[i].width_brick, bricks[i].height_brick))
                {
                    velocityY = - velocityY;
                    bricks[i].isBrickAlive = false;
                }
                
            }
        }
        
        // ball collision with the borders
        if (axisX_ball < 0)                     velocityX = - velocityX;
        else if (axisX_ball + WH_ball > 600)    velocityX = - velocityX;
        if (axisY_ball < 0)                     velocityY = - velocityY;
        else if (axisY_ball + WH_ball > 400)
        {
            // Set the pad and ball their initial positions, bring the bricks to life and restart the game
            
            axisX_pad = 300;
            axisY_pad = 370;
            
            axisX_ball = 50;
            axisY_ball = 350;
            velocityX = 3;
            velocityY = -3;
            
            for (int i = 0; i < BRICKS; i++)
            {
                bricks[i].isBrickAlive = true;
            }
        }
        
        // ball collision with the pad
        if (checkCollision(axisX_pad, axisY_pad, width_pad, height_pad, axisX_ball, axisY_ball, WH_ball, WH_ball))
        {
            velocityY = - velocityY;
        }
        
        
        
        
        
        
        
        
        
        // RENDERING the screen
        glClear(GL_COLOR_BUFFER_BIT);
        
        glPushMatrix(); // start the phase...
        
        glOrtho(0, 600, 400, 0, -1, 1); // set the matrix...
        
        /* temporary basic drawings
         glColor4ub(255, 0, 0, 255); // red color
         
         glBegin(GL_QUADS); // begin drawing... || GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_QUADS, GL_TRIANGLES, GL_POLIGON
         
         glVertex2f(5,5);
         glVertex2f(595, 5);
         
         glColor4ub(0, 255, 0, 255); // green color
         
         glVertex2f(595, 395);
         glVertex2f(5, 395);
         
         glEnd(); // end drawing...
         glColor4ub(0, 0, 0, 255); // black color
         
         glBegin(GL_LINES); // begin another drawing...
         
         glVertex2f(-600, -400); // top left corner
         glVertex2f(600, 400); // bottom right corner
         
         glEnd(); // end drawing...
         */
        
        glColor4ub(0, 0, 0, 255); // black color (the pad)
        
        glBegin(GL_QUADS); // start drawing a rectangle (the pad)
        
        glVertex2f(axisX_pad, axisY_pad);
        glVertex2f(axisX_pad + width_pad, axisY_pad);
        glVertex2f(axisX_pad + width_pad, axisY_pad + height_pad);
        glVertex2f(axisX_pad, axisY_pad + height_pad);
        
        glEnd(); // end drawing the pad
        
        glColor4ub(255, 0, 0, 255); // red color (the ball)
        
        glBegin(GL_QUADS); // start drawing a rectangle (the ball)
        
        glVertex2f(axisX_ball, axisY_ball);
        glVertex2f(axisX_ball + WH_ball, axisY_ball);
        glVertex2f(axisX_ball + WH_ball, axisY_ball + WH_ball);
        glVertex2f(axisX_ball, axisY_ball + WH_ball);
        
        glEnd(); // end drawing the ball
        
        glColor4ub(0, 0, 255, 255); // blue color (the bricks)
        
        glBegin(GL_QUADS); // start drawing a rectangle (the bricks)
        
        for (int i = 0; i< BRICKS; i++)
        {
            if(bricks[i].isBrickAlive)
            {
                glVertex2f(bricks[i].axisX_brick, bricks[i].axisY_brick); // upper left
                glVertex2f(bricks[i].axisX_brick + bricks[i].width_brick, bricks[i].axisY_brick); // upper right
                glVertex2f(bricks[i].axisX_brick + bricks[i].width_brick, bricks[i].axisY_brick + bricks[i].height_brick); // bottom right
                glVertex2f(bricks[i].axisX_brick, bricks[i].axisY_brick + bricks[i].height_brick); // bottom left
            }
        }
        
        glEnd(); // end drawing the bricks
        
        
        glPopMatrix(); // end the phase...
        
        SDL_GL_SwapWindow(screen);
        
    }
    
    
    
    //*** Main Loop ***
    
    SDL_Quit();
    
    return 0;
}

