#include "defines.h"
#include "log.h"

typedef struct {
    Sound* sounds;
    int n_sound;
} Sounds;

void p_play_sound(Sound sound, Vector2 sound_pos, Vector2 player_pos);

Sounds* p_init_sounds();
void test_spatial();
void p_free_sounds(Sounds* sounds);
