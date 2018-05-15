//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400
#define W 50
#define H 10

// number of rows of bricks
#define ROWS 8

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    double velocity1 = 5 * drand48();
    double velocity2 = 5 * drand48();
     
    waitForClick();
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // move circle along y-axis
        move(ball, velocity1, velocity2);
     
        GObject object = detectCollision(window, ball);
        // bounce off the paddle and edges of the window 
        if(object == paddle)
        {
            velocity2 = - velocity2;
            
        } 
        else if (getY(ball) <= 0)
        {
            velocity2 = - velocity2;
            
        }
        
        else if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            velocity1 = - velocity1;
            
        }
        
        else if (getX(ball) <= 0)
        {
            velocity1 = - velocity1;
            
        }
        else if(object == label)
        {
            velocity1 = 1 * velocity1;
            velocity2 = 1 * velocity2;
        } 
        else if (object != NULL)
        {
            velocity2 = - velocity2;
            removeGWindow(window, object);
            points = points + 1;
            bricks = bricks - 1;
            updateScoreboard(window, label, points);
            if( bricks == 0)
            {
                GLabel result = newGLabel(" YOU WON ");
                setFont(result,"SansSerif-42");
                double x = (getWidth(window) - getWidth(result)) / 2;
                double y = 100;
                setLocation(result, x, y);
                setColor(result, "cyan");
                add(window, result);
            }
        }
        
        pause(10);
        // TODO to give movement to the paddle
        GEvent event = getNextEvent(MOUSE_EVENT);
        if(event != NULL)
        {
            if(getEventType(event) == MOUSE_MOVED)
            {
                double x = getX(event) - 25;
                setLocation(paddle, x, 550);
            }
        }
        
        //lose one life if the ball touches the bottom of the window
        if (getY(ball) + getHeight(ball) >= getHeight(window))
        {
           
            pause(20);
            lives = lives - 1;
            if( lives != 0)
            {
                waitForClick();
                setLocation(ball, WIDTH/2 - 10, HEIGHT/2 - 10);
            }
            else if (lives == 0)
            {
                GLabel result = newGLabel(" YOU LOST ");
                setFont(result,"SansSerif-42");
                double x = (getWidth(window) - getWidth(result)) / 2;
                double y = 100;
                setLocation(result, x, y);
                setColor(result, "cyan");
                add(window, result);
            }
        }
        
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // TODO to form a grid of bricks
    for(int i = 0; i < ROWS; i++)
    {   
        int h = 10; 
        int w = 0;
        for( int j = 0; j < COLS; j++)
        {
            GRect brick = newGRect(j + (j * w) + 4 + ( 4 * j ), i + (i * h) + 4 + (10 * h) + ( 4 * i ), 39, 10);
            add(window, brick);
            w = 39;
            if(i == 0)
            {
                setFilled(brick, true);
                setColor(brick,"GREEN" );
            }   
            if(i == 1)
            {
                setFilled(brick, true);
                setColor(brick,"YELLOW" );
            }        
            if(i == 2)
            {
                setFilled(brick, true);
                setColor(brick,"#ffa5d2" );
            }        
            if(i == 3)
            {
                setFilled(brick, true);
                setColor(brick,"BLUE" );
            }        
            if(i == 4)
            {
                setFilled(brick, true);
                setColor(brick,"CYAN" );
            }
            if(i == 5)
            {
                setFilled(brick, true);
                setColor(brick,"red" );
            }      
             if(i == 6)
            {
                setFilled(brick, true);
                setColor(brick,"orange" );
            }      
             if(i == 7)
            {
                setFilled(brick, true);
                setColor(brick,"magenta" );
            }             
        }
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // TODO to create a ball in the window's center
    GOval ball = newGOval(WIDTH/2 - 10 , HEIGHT/2 - 10, 20, 20);
    setFilled(ball, true);
    setColor(ball,"BLACK" );
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // TODO to create a paddle in the window's center
    int x = (WIDTH/2) - (W/2);
    GRect paddle = newGRect(x, 550, W, H);
    setFilled(paddle, true);
    setColor(paddle, "BLACK");
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // TODO to creat a label for the score
    GLabel label = newGLabel("0");
    setFont(label,"SansSerif-36");
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
    setColor(label, "orange");
    add(window, label);
    return label; 
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {    
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {  
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {   
        return object;
    }

    // no collision
    return NULL;
}
