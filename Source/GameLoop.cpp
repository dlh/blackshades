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

	XYZ facing;

	

	if(!mainmenu){

	switch( theChar )

	{

		case 'l': 
		        if(!lasersight==1){lasersight=1;}else{lasersight=0;} 
			
			break;

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


static unsigned char ourkeys[16];

static bool mousegrabbed = true;
static bool relativemouse = false;

static int MacKeyFromSDL(SDL_Keycode key)
{
	switch (key) {
		case SDLK_1: return MAC_1_KEY;
		case SDLK_2: return MAC_2_KEY;
		case SDLK_3: return MAC_3_KEY;
		case SDLK_4: return MAC_4_KEY;
		case SDLK_5: return MAC_5_KEY;
		case SDLK_6: return MAC_6_KEY;
		case SDLK_7: return MAC_7_KEY;
		case SDLK_8: return MAC_8_KEY;
		case SDLK_9: return MAC_9_KEY;
		case SDLK_0: return MAC_0_KEY;
		case SDLK_KP_1: return MAC_NUMPAD_1_KEY;
		case SDLK_KP_2: return MAC_NUMPAD_2_KEY;
		case SDLK_KP_3: return MAC_NUMPAD_3_KEY;
		case SDLK_KP_4: return MAC_NUMPAD_4_KEY;
		case SDLK_KP_5: return MAC_NUMPAD_5_KEY;
		case SDLK_KP_6: return MAC_NUMPAD_6_KEY;
		case SDLK_KP_7: return MAC_NUMPAD_7_KEY;
		case SDLK_KP_8: return MAC_NUMPAD_8_KEY;
		case SDLK_KP_9: return MAC_NUMPAD_9_KEY;
		case SDLK_KP_0: return MAC_NUMPAD_0_KEY;
		case SDLK_A: return MAC_A_KEY;
		case SDLK_B: return MAC_B_KEY;
		case SDLK_C: return MAC_C_KEY;
		case SDLK_D: return MAC_D_KEY;
		case SDLK_E: return MAC_E_KEY;
		case SDLK_F: return MAC_F_KEY;
		case SDLK_G: return MAC_G_KEY;
		case SDLK_H: return MAC_H_KEY;
		case SDLK_I: return MAC_I_KEY;
		case SDLK_J: return MAC_J_KEY;
		case SDLK_K: return MAC_K_KEY;
		case SDLK_L: return MAC_L_KEY;
		case SDLK_M: return MAC_M_KEY;
		case SDLK_N: return MAC_N_KEY;
		case SDLK_O: return MAC_O_KEY;
		case SDLK_P: return MAC_P_KEY;
		case SDLK_Q: return MAC_Q_KEY;
		case SDLK_R: return MAC_R_KEY;
		case SDLK_S: return MAC_S_KEY;
		case SDLK_T: return MAC_T_KEY;
		case SDLK_U: return MAC_U_KEY;
		case SDLK_V: return MAC_V_KEY;
		case SDLK_W: return MAC_W_KEY;
		case SDLK_X: return MAC_X_KEY;
		case SDLK_Y: return MAC_Y_KEY;
		case SDLK_Z: return MAC_Z_KEY;
		case SDLK_F1: return MAC_F1_KEY;
		case SDLK_F2: return MAC_F2_KEY;
		case SDLK_F3: return MAC_F3_KEY;
		case SDLK_F4: return MAC_F4_KEY;
		case SDLK_F5: return MAC_F5_KEY;
		case SDLK_F6: return MAC_F6_KEY;
		case SDLK_F7: return MAC_F7_KEY;
		case SDLK_F8: return MAC_F8_KEY;
		case SDLK_F9: return MAC_F9_KEY;
		case SDLK_F10: return MAC_F10_KEY;
		case SDLK_F11: return MAC_F11_KEY;
		case SDLK_F12: return MAC_F12_KEY;
		case SDLK_RETURN: return MAC_RETURN_KEY;
		case SDLK_KP_ENTER: return MAC_ENTER_KEY;
		case SDLK_TAB: return MAC_TAB_KEY;
		case SDLK_SPACE: return MAC_SPACE_KEY;
		case SDLK_BACKSPACE: return MAC_DELETE_KEY;
		case SDLK_ESCAPE: return MAC_ESCAPE_KEY;
		case SDLK_LCTRL: return MAC_CONTROL_KEY;
		case SDLK_RCTRL: return MAC_CONTROL_KEY;
		case SDLK_LSHIFT: return MAC_SHIFT_KEY;
		case SDLK_RSHIFT: return MAC_SHIFT_KEY;
		case SDLK_CAPSLOCK: return MAC_CAPS_LOCK_KEY;
		case SDLK_LALT: return MAC_OPTION_KEY;
		case SDLK_RALT: return MAC_OPTION_KEY;
		case SDLK_PAGEUP: return MAC_PAGE_UP_KEY;
		case SDLK_PAGEDOWN: return MAC_PAGE_DOWN_KEY;
		case SDLK_INSERT: return MAC_INSERT_KEY;
		case SDLK_DELETE: return MAC_DEL_KEY;
		case SDLK_HOME: return MAC_HOME_KEY;
		case SDLK_END: return MAC_END_KEY;
		case SDLK_LEFTBRACKET: return MAC_LEFT_BRACKET_KEY;
		case SDLK_RIGHTBRACKET: return MAC_RIGHT_BRACKET_KEY;
		case SDLK_UP: return MAC_ARROW_UP_KEY;
		case SDLK_DOWN: return MAC_ARROW_DOWN_KEY;
		case SDLK_LEFT: return MAC_ARROW_LEFT_KEY;
		case SDLK_RIGHT: return MAC_ARROW_RIGHT_KEY;
		case SDLK_LGUI: return MAC_COMMAND_KEY;
		case SDLK_RGUI: return MAC_COMMAND_KEY;
		default: return -1;
	}
}

void GetKeys(unsigned char *keys)
{
	memcpy(keys, ourkeys, sizeof(ourkeys));
}

static void DoSDLKey(Game *g, SDL_Event *event)
{
	int press = event->key.down ? 1 : 0;
	int mackey;
	int index;
	int mask;
	
	/* the game polls key state, so repeats would only retrigger HandleKeyDown */
	if (event->key.repeat) {
		return;
	}
	
	mackey = MacKeyFromSDL(event->key.key);
	
	if (mackey != -1) {
		index = mackey / 8;
		mask = 1 << (mackey % 8);
		
		if (press) {
			ourkeys[index] |= mask;
		} else {
			ourkeys[index] &= ~mask;
		}
	}
	
	if (press) {
		/* not SDL text input, which pops up the macOS accent menu on held keys */
		SDL_Keycode typed = SDL_GetKeyFromScancode(event->key.scancode, event->key.mod, false);
		
		if (typed != SDLK_UNKNOWN && typed < 0x80) {
			/* hey, at least it was aleady public */
			g->HandleKeyDown((char)typed);
		}
	}
}

/* Mouselook during play; the menu draws its own cursor from absolute position. */
static void UpdateMouseMode(Game *g)
{
	bool relative = mousegrabbed && !g->mainmenu;
	
	if (relative != relativemouse) {
		relativemouse = relative;
		SDL_SetWindowRelativeMouseMode(gWindow, relative);
		SDL_GetRelativeMouseState(NULL, NULL); /* drop motion from the menu */
	}
	
	if (mousegrabbed) {
		SDL_HideCursor();
	} else {
		SDL_ShowCursor();
	}
}

static void ProcessSDLEvents(Game *g)
{
	SDL_Event event;
	
	if (SDL_PollEvent(&event)) {
		do {
			switch(event.type) {
				case SDL_EVENT_KEY_DOWN:
				if (event.key.key == SDLK_RETURN &&
					event.key.mod & SDL_KMOD_ALT)
				{
					if (!event.key.repeat) {
						bool fullscreen = (SDL_GetWindowFlags(gWindow) & SDL_WINDOW_FULLSCREEN) != 0;
						SDL_SetWindowFullscreen(gWindow, !fullscreen);
					}
					break;
				}
				if (event.key.key == SDLK_G &&
					event.key.mod & SDL_KMOD_CTRL)
				{
					if (!event.key.repeat) {
						mousegrabbed = !mousegrabbed;
					}
					break;
				}
				case SDL_EVENT_KEY_UP:
					DoSDLKey(g, &event);
					break;
				case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
					g->screenwidth = event.window.data1;
					g->screenheight = event.window.data2;
					break;
				case SDL_EVENT_WINDOW_FOCUS_LOST:
					/* the key-up will go to whichever app took focus */
					memset(ourkeys, 0, sizeof(ourkeys));
					break;
				case SDL_EVENT_QUIT:
					g->RequestQuit();
					break;
			}
		} while (SDL_PollEvent(&event));
	}
	
	UpdateMouseMode(g);
}	

/********************> RequestQuit() <*****/

void	Game::RequestQuit()

{

	gQuit = true;

	if(score>highscore){

		highscore=score;

		ofstream opstream("Data/Highscore");

		opstream << highscore;

		opstream << "\n";

		opstream << beatgame;

		opstream.close();

	}

}



/********************> EventLoop() <*****/

void	Game::EventLoop( void )

{


	unsigned char	theKeyMap[16];

	int colaccuracy,i;

	GLfloat oldmult;

	gQuit = false;

	while ( gQuit == false )

	{

		ProcessSDLEvents(this);
		

		start=TimerGetTime(&theTimer);

		

		colaccuracy=framespersecond>0?sps/framespersecond+1:sps;

		if(colaccuracy>sps){colaccuracy=sps;}

		

		oldmult=multiplier;

		multiplier/=colaccuracy;

		for(i=0;i<(int)(colaccuracy+.5);i++){

			Tick();

		}

		multiplier=oldmult;

		

		if ( DrawGLScene())

                        SDL_GL_SwapWindow(gWindow);

		else

			gQuit = true;

		oldmult=multiplier;	

		

		end=TimerGetTime(&theTimer);

		timetaken=end-start;

		/* timetaken is in nanoseconds, so this is 0.6 times the real frame rate */
		framespersecond=600000000/timetaken;

		if(framespersecond>maxfps){

			SDL_DelayPrecise((Uint64)(600000000.0/maxfps-timetaken));

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

		GetKeys( theKeyMap );

		if ( IsKeyDown( theKeyMap, MAC_COMMAND_KEY )&&IsKeyDown( theKeyMap, MAC_Q_KEY )){

			RequestQuit();

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
