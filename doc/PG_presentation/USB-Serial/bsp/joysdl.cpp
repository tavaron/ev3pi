// ------------------------------------------------------------------------
// 
// Datei: joysdl.cpp
// 
// Name:		Marius Christ
// Matr.-Nr.:	954402
// Datum:		16.01.2014 v0.1
// geaendert:	16.01.2014
// 
// Beschreibung: !!!
// 
// Eingabe: !!!
// 
// Ausgabe: !!!
// 
// ------------------------------------------------------------------------


#include <SDL/SDL.h>
#include <stdio.h>
 
int main()
{
    SDL_Joystick *joy;
 
    // Initialize the joystick subsystem
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
 
    // Check for joystick
    if(SDL_NumJoysticks()>0)
    {
        // Open joystick
        joy=SDL_JoystickOpen(0);
 
        if(joy)
        {
            printf("Opened Joystick 0\n");
            printf("Name: %s\n", SDL_JoystickName(0));
            printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
            printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
            printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
        }
        else
            printf("Couldn't open Joystick 0\n");
 
        // Close if opened
        if(SDL_JoystickOpened(0))
            SDL_JoystickClose(joy);
    }
    else
		printf("Keine Freudenstäbe!\n");
	
	return 0;
}

