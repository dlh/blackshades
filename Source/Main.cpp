/********************> Headers <*****/

#include "Game.h"



/********************> Globals <*****/



Game game;



/********************> main() <*****/

void	main( void )

	{

	

	ToolboxInit();

	

	if ( HasAppearance() )

		RegisterAppearanceClient();

	



	game.InitGL();

	

	game.InitGame();

	

	game.EventLoop();

	

	game.Dispose();

	

	if ( HasAppearance() )

		UnregisterAppearanceClient();

	

	FlushEvents( everyEvent, 0 );

	ExitToShell();

	

}

