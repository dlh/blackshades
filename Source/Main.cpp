/********************> Headers <*****/

#include "Game.h"



/********************> Globals <*****/



Game game;



/********************> main() <*****/

int 	main( int argc, char *argv[] )

	{

	
#ifdef MAC
	ToolboxInit();

	

	if ( HasAppearance() )

		RegisterAppearanceClient();
#endif
	



	game.InitGL();

	

	game.InitGame();

	

	game.EventLoop();

	

	game.Dispose();

	
#ifdef MAC
	if ( HasAppearance() )

		UnregisterAppearanceClient();

	

	FlushEvents( everyEvent, 0 );

	ExitToShell();
#endif
	

}

