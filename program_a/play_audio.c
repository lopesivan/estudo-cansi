#include "play_audio.h"

int init_mixer(){
    // Initialize SDL and SDL_mixer
    if (SDL_Init(SDL_INIT_AUDIO) < 0){
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3){
        printf("Mix_Init failed: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0){
        printf("Mix_OpenAudio failed: %s\n", Mix_GetError());
        Mix_Quit();
        SDL_Quit();
        return 1;
    }
    return 0;
}

void clean_mixer(){
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

void play_audio(char *file_name){
    // Load the MP3 file
    Mix_Music *music = Mix_LoadMUS(file_name);
    if (!music){
        printf("Mix_LoadMUS failed: %s\n", Mix_GetError());
        return;
    }
    // Play the sound
    if (Mix_PlayMusic(music, 1) == -1){
        printf("Mix_PlayMusic failed: %s\n", Mix_GetError());
    }
    // Wait for the sound to finish playing
    while (Mix_PlayingMusic()){
        SDL_Delay(100); // Delay to allow the music to finish
    }
    // Free the audio resources
    Mix_FreeMusic(music);
}
