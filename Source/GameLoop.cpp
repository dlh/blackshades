#include "Game.h"	



extern double multiplier;

extern int visions;

extern unsigned int gSourceID[100];

extern unsigned int gSampleSet[100];

extern Camera camera;

extern float rad2deg;

extern Fog fog;

extern int environment;

extern int slomo;

/********************> HandleKeyDown() <*****/

void	Game::HandleKeyDown( char theChar )

{

	ALint tempInt;

	XYZ facing;

	

	if(!mainmenu){

	switch( theChar )

	{

		case 'k':

			if(debug)timeremaining=0;

			break;

		

		case 'b':

			if(debug){

			alSourcePlay(gSourceID[soulinsound]);

			if(!slomo)slomo=1;

			else slomo=0;}

			if(slomo){

				alSourcef(gSourceID[knifesong], AL_PITCH, (ALfloat)(.5));

				alSourcef(gSourceID[shootsong], AL_PITCH, (ALfloat)(.5));

				alSourcef(gSourceID[zombiesong], AL_PITCH, (ALfloat)(.5));

			}

			if(!slomo){

				alSourcef(gSourceID[knifesong], AL_PITCH, (ALfloat)(1));

				alSourcef(gSourceID[shootsong], AL_PITCH, (ALfloat)(1));

				alSourcef(gSourceID[zombiesong], AL_PITCH, (ALfloat)(1));

			}

			break;

		case 'B':

			if(debug){

			alSourcePlay(gSourceID[soulinsound]);

			paused=1-paused;}

			break;

		case 'f':

			if(debug){

			alSourcePlay(gSourceID[souloutsound]);

			//Facing

			facing=0;

			facing.z=-1;

			

			facing=DoRotation(facing,-camera.rotation2,0,0);

			facing=DoRotation(facing,0,0-camera.rotation,0);

			for(int i=1;i<numpeople;i++){				

				if(person[i].skeleton.free!=1){

				if(findDistancefast(person[i].playercoords,person[0].playercoords)<1000){

				person[i].skeleton.free=1;

				person[i].longdead=1;

				for(int j=0;j<person[i].skeleton.num_joints;j++){

					person[i].skeleton.joints[j].position=DoRotation(person[i].skeleton.joints[j].position,0,person[i].playerrotation,0);

					person[i].skeleton.joints[j].position+=person[i].playercoords;

					person[i].skeleton.joints[j].realoldposition=person[i].skeleton.joints[j].position;

					person[i].skeleton.joints[j].velocity=DoRotation(person[i].skeleton.joints[j].velocity,0,person[i].playerrotation,0);

					person[i].skeleton.joints[j].velocity+=person[i].velocity;

					person[i].skeleton.joints[j].velocity+=facing*50;

					person[i].skeleton.joints[j].velocity.x+=abs(Random()%20)-10;

					person[i].skeleton.joints[j].velocity.y+=abs(Random()%20)-10;

					person[i].skeleton.joints[j].velocity.z+=abs(Random()%20)-10;

				}}}

			}}

			break;

		case 'X':

			if(debug){

			if(person[0].grenphase==0){

				person[0].ammo=-1;

				person[0].whichgun++;

				person[0].grenphase=0;

				person[0].reloads[person[0].whichgun]=3;

				if(person[0].whichgun>7)person[0].whichgun=0;

			}}

			break;

	}

	}

}



/********************> DoEvent() <*****/

void	Game::DoEvent( EventRecord *event )

{

	

	char	theChar;

	

	switch ( event->what )

	{

		case keyDown:

		case autoKey:

			theChar = event->message & charCodeMask;	// Get the letter of the key pressed from the event message

			HandleKeyDown( theChar );					// Only some key presses are handled here because it is slower and less responsive

			break;

	}

	

	

}

/********************> EventLoop() <*****/

void	Game::EventLoop( void )

{

	EventRecord		event;

	unsigned char	theKeyMap[16];

	int colaccuracy,i;

	GLfloat oldmult;

	gQuit = false;

	while ( gQuit == false )

	{

		if ( GetNextEvent( everyEvent, &event ) )

			DoEvent( &event );

		

		start=TimerGetTime(&theTimer);

		

		colaccuracy=sps/framespersecond+1;

		if(colaccuracy>sps){colaccuracy=sps;}

		

		oldmult=multiplier;

		multiplier/=colaccuracy;

		for(i=0;i<(int)(colaccuracy+.5);i++){

			Tick();

		}

		multiplier=oldmult;

		

		if ( DrawGLScene())

			aglSwapBuffers( gOpenGLContext );

		else

			gQuit = true;

		oldmult=multiplier;	

		

		end=TimerGetTime(&theTimer);

		timetaken=end-start;

		framespersecond=600000000/timetaken;

		while(framespersecond>maxfps){

			end=TimerGetTime(&theTimer);

			timetaken=end-start;

			framespersecond=600000000/timetaken;

		}

		multiplier5=multiplier4;

		multiplier4=multiplier3;

		multiplier3=multiplier2;

		multiplier2=1/framespersecond;

		multiplier=(multiplier2+multiplier3+multiplier4+multiplier5)/4;

		if(multiplier>1)multiplier=1;

		if(multiplier<.00001)multiplier=.00001;

		if(visions==1&&mainmenu==0)multiplier/=3;

		if(slomo)multiplier*=.2;

		if(paused)multiplier=0;

		GetKeys( ( unsigned long * )theKeyMap );

		if ( IsKeyDown( theKeyMap, MAC_COMMAND_KEY )&&IsKeyDown( theKeyMap, MAC_Q_KEY )){

			gQuit = true;

			if(score>highscore){

				highscore=score;

				 ofstream opstream(":Data:Highscore"); 

		        opstream << highscore;

				opstream << "\n";

	        	opstream << beatgame;

				opstream.close();

			}

		}

		if ( IsKeyDown( theKeyMap, MAC_ESCAPE_KEY )){

			alSourcePause(gSourceID[rainsound]);

		

			mainmenu=1;

			alSourcePlay(gSourceID[souloutsound]);

			flashamount=1;

			flashr=1;flashg=1;flashb=1;

			alSourceStop(gSourceID[visionsound]);

			whichsong=mainmenusong;

			alSourceStop(gSourceID[knifesong]);

			alSourceStop(gSourceID[shootsong]);

			alSourceStop(gSourceID[zombiesong]);

			alSourceStop(gSourceID[mainmenusong]);

			alSourcef(gSourceID[knifesong], AL_MIN_GAIN, 0);

			alSourcef(gSourceID[shootsong], AL_MIN_GAIN, 0);

			alSourcef(gSourceID[zombiesong], AL_MIN_GAIN, 0);

			alSourcef(gSourceID[mainmenusong], AL_MIN_GAIN, 0);

			alSourcePlay(gSourceID[whichsong]);

			alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 1);

		}

	}

}