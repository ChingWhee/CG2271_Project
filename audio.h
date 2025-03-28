#ifndef AUDIO_H
#define AUDIO_H

#include "MKL25Z4.h"
#include "cmsis_os2.h"

#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define REST 0

#define BEAT_LENGTH DEFAULT_SYSTEM_CLOCK
#define PRESCALAR_VALUE 128

#define BUZZER 1 // TPM2_CH0 - Port A Pin 1

void InitAudio(void);
void playNote(uint16_t frequency);
void audio_thread(void *argument);

#endif
