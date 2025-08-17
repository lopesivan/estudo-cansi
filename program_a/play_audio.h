#ifndef PLAY_AUDIO_H
#define PLAY_AUDIO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

int init_mixer();
void play_audio(char* file_name);
void clean_mixer();

#endif