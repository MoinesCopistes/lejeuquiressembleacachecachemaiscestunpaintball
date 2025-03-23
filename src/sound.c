#include "sound.h"



void p_play_sound(Sound sound, Vector2 sound_pos, Vector2 player_pos){
    PlaySound(sound);
}

Sounds* p_init_sounds(){
    const char* sounds_path[] = {"resources/walking-sound.wav"};
    int num_sounds = sizeof(sounds_path) / sizeof(sounds_path[0]);
    log_debug("num sounds : %i", num_sounds);
    Sounds* sounds = (Sounds*) malloc(sizeof(Sounds));
    Sound* sound = (Sound *) malloc(sizeof(Sound) * num_sounds);
    for (int i = 0; i < num_sounds; i++){
        sound[i] = LoadSound(sounds_path[i]);
    }
    sounds->n_sound = num_sounds;
    sounds->sounds = sound;
    return sounds;
}

void p_free_sounds(Sounds* sounds){
    for (int i = 0; i < sounds->n_sound; i++){
        UnloadSound(sounds->sounds[i]);
    }
    free(sounds);
    sounds = NULL;
}