// http://de.wikibooks.org/wiki/SDL:_Joystick
// modified by Joachim Schiele
// feel free to use this code for whatever you want to do....

//  example2.cpp can be compiled with:
// 	g++ example2.cpp -o example2 `sdl-config --libs` `sdl-config --cflags`

#include <stdio.h>
#include "SDL.h"

int main ( int argc, char *argv[] )
{
	if ( SDL_InitSubSystem ( SDL_INIT_JOYSTICK ) < 0 )
	{
		fprintf ( stderr, "Unable to initialize Joystick: %s\n", SDL_GetError() );
		return -1;
	}
	//while (1) {
	//	continue;
	//}
	printf ( "%i joysticks found\n", SDL_NumJoysticks () );
	
	// TODO add check
	SDL_Joystick* joy1 = SDL_JoystickOpen ( 0 );

	if ( joy1 == NULL )
		printf ( "could not open joystick\n" );

	printf ( "%i achsen\n", SDL_JoystickNumAxes ( joy1 ) );
	printf ( "%i rollbaelle\n", SDL_JoystickNumBalls ( joy1 ) );
	printf ( "%i heads\n", SDL_JoystickNumHats ( joy1 ) );
	printf ( "%i koepfe\n", SDL_JoystickNumButtons ( joy1 ) );

	//SDL_JoystickEventState (SDL_ENABLE);
	// this will alter the behaviour of the event queue of the sdl system
	SDL_JoystickEventState ( SDL_QUERY );

	while ( 1 )
	{
		// This is needed in the even queue
		SDL_JoystickUpdate ();

		// now we query for some input
		for ( int i=0; i < SDL_JoystickNumButtons ( joy1 ); ++i )
		{
			unsigned int n = SDL_JoystickGetButton ( joy1, i );
			if ( n != 0 )
				printf ( "found you pressed button %i\n", i );
		}

		for ( int i=0; i < SDL_JoystickNumAxes ( joy1 ); ++i )
		{
			signed short a = SDL_JoystickGetAxis ( joy1, i );
			if ( a != 0 )
				printf ( "axis %i is %d\n", i,a );
		}
	}
	return 0;
}
