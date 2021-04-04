#include <stdlib.h>
#include <fcntl.h>
void play_sound(){
	system("cvlc sounds/lighting.mp3 vlc://quit& 2>>&1");
}
