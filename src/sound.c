#include "sound.h"
#include "raylib.h"
#include "raymath.h"
#include <unistd.h>

// Function to calculate stereo pan (-1 = full left, 1 = full right)
float CalculatePan(Vector2 sourcePos, Vector2 listenerPos) {
  float deltaX = sourcePos.x - listenerPos.x;
  float distance = fabsf(deltaX);
  float maxDistance = 10.0f; // Max distance for full panning effect
  return (distance > maxDistance) ? (deltaX > 0 ? 1.0f : -1.0f)
                                  : deltaX / maxDistance;
}

// Function to calculate volume based on distance
float CalculateVolume(Vector2 sourcePos, Vector2 listenerPos) {
  float distance = Vector2Distance(sourcePos, listenerPos);
  float maxDistance = 20.0f; // Beyond this, sound is almost inaudible
  return Clamp(1.0f - (distance / maxDistance), 0.0f, 1.0f);
}


#include "raylib.h"
#include <unistd.h> // For usleep


// Function to update sound parameters to create spatial audio effect
void UpdateSpatialAudio(Sound sound, float angle, float* pan, float* volume, float* pitch) 
{
    // Calculate pan (0 = left, 0.5 = center, 1 = right)
    *pan = (cosf(angle) + 1.0f) * 0.5f;
    
    // Calculate volume based on position (quieter when behind)
    *volume = 0.7f + 0.3f * (sinf(angle) + 1.0f) * 0.5f;
    
    // Calculate pitch effect
    *pitch = 0.5f + 0.1f * cosf(angle);
    
    // Apply to sound (converting to Raylib's expected ranges)
    SetSoundPan(sound, (*pan) * 2.0f - 1.0f);  // Convert 0-1 to -1 to 1
    SetSoundVolume(sound, *volume);
    SetSoundPitch(sound, (*pitch) * 2.0f);     // Convert 0-1 to 0-2 range
}



void p_play_sound(Sound sound, Vector2 sound_pos, Vector2 player_pos){

    Vector2 relativePosition = {sound_pos.x - player_pos.x, sound_pos.y - player_pos.y};

    float distance = sqrtf(relativePosition.x * relativePosition.x + relativePosition.y * relativePosition.y);
    float angle = atan2f(relativePosition.y, relativePosition.x);
    angle = -angle * 180 / PI; // Adjust angle for 2D space
    float pan = fabsf(angle) / 180.0f; // Map angle to pan

    float volume = 50.0f / distance; // Scale distance for louder volume
    volume *= 1.0f; // Increase overall volume (adjust this value as needed)
    if (volume > 1.0f) {
        volume = 1.0f;
    }

    float pitch = 1.0f;
    if (angle < -35 && angle > -145) {
        volume *= 0.5f; // Reduce volume
        pitch = 0.9f;  // Lower pitch
    }
    SetSoundPan(sound, pan);
    SetSoundVolume(sound, volume);
    SetSoundPitch(sound, pitch);

    PlaySound(sound);
    // Tentative audio 3D
    SetSoundVolume(sound, 1.0f);
    float pan;
    float volume;
    float pitch;
    // Smooth pan and volume adjustment from left to right
    for (float i = 0.0f; i <= 4*PI; i += 0.01) {
      UpdateSpatialAudio(sound, i, &pan, &volume, &pitch);
      usleep(1000*20);
    }
}





Sounds *p_init_sounds() {
  const char *sounds_path[] = {"resources/pilou.wav"};
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
