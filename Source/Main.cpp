/********************> Headers <*****/

#include "Game.h"
#include <SDL3/SDL_main.h>



/********************> Globals <*****/



Game game;



/********************> main() <*****/

int 	main( int argc, char *argv[] )

	{

	
	



	LocateGameData();

	game.InitGL();

	

	game.InitGame();

	

	game.EventLoop();

	

	game.Dispose();

	return 0;

	
	

}

