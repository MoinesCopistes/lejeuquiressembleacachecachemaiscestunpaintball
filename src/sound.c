#include "raylib.h"
#include "raymath.h"
#include "sound.h"
#include <unistd.h>

void p_play_sound(Sound sound, Vector2 sound_pos, Vector2 player_pos) {

  Vector2 relativePosition = {sound_pos.x - player_pos.x,
                              sound_pos.y - player_pos.y};

  float distance = sqrtf(relativePosition.x * relativePosition.x +
                         relativePosition.y * relativePosition.y);

  float angle = atan2f(relativePosition.y, relativePosition.x);
  angle = -angle * 180 / PI; // Adjust angle for 2D space

  // Map angle to pan
  float pan = fabsf(angle) / 180.0f; // Map angle to pan

  // Adjust volume based on distance
  float volume = 50.0f / distance; // Scale distance for louder volume
  volume *= 1.0f; // Increase overall volume (adjust this value as needed)

  // Clamp volume to a maximum of 1.0
  if (volume > 1.0f) {
    volume = 1.0f;
  }

  float pitch = 1.0f;
  if (angle < -35 && angle > -145) {
    volume *= 0.5f; // Reduce volume
    pitch = 0.9f;   // Lower pitch
  }

  SetSoundPan(sound, pan);

  SetSoundVolume(sound, volume);
  SetSoundPitch(sound, pitch);

  PlaySound(sound);
}

Sounds *p_init_sounds() {
  const char *sounds_path[] = {"resources/walking_sound.wav"};
  int num_sounds = sizeof(sounds_path);
  Sounds *sounds = (Sounds *)malloc(sizeof(Sounds));
  Sound *sound = (Sound *)malloc(sizeof(Sound) * num_sounds);
  for (int i = 0; i < num_sounds; i++) {
    sound[i] = LoadSound(sounds_path[i]);
  }
  sounds->n_sound = num_sounds;
  sounds->sounds = sound;
  return sounds;
}

void p_free_sounds(Sounds *sounds) {
  for (int i = 0; i < sounds->n_sound; i++) {
    UnloadSound(sounds->sounds[i]);
  }
  free(sounds);
  sounds = NULL;
}
