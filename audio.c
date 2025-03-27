#include "audio.h"

extern volatile int moving_flag;

int melody1[] = {
  NOTE_A4, REST, NOTE_B4, REST, NOTE_C5, REST, NOTE_A4, REST,
  NOTE_D5, REST, NOTE_E5, REST, NOTE_D5, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, REST,
  NOTE_D5, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_D5, NOTE_D5, REST,
  NOTE_C5, REST, NOTE_B4, NOTE_A4, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_D5, REST,
  NOTE_B4, NOTE_A4, NOTE_G4, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_C5, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, REST,
  NOTE_D5, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_G5, NOTE_B4, REST,
  NOTE_C5, REST, NOTE_B4, NOTE_A4, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_D5, REST,
  NOTE_B4, NOTE_A4, NOTE_G4, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_C5, REST,

  NOTE_C5, REST, NOTE_D5, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_E5, REST,
  NOTE_G5, NOTE_F5, NOTE_E5, REST
};

int noteDurations1[] = {
  8, 8, 8, 8, 8, 8, 8, 4,
  8, 8, 8, 8, 2, 2,

  8, 8, 8, 8, 2, 8, 8,
  2, 8,

  8, 8, 8, 8, 2, 8, 8,
  4, 8, 8, 8, 8,

  8, 8, 8, 8, 2, 8, 8,
  2, 8, 4, 8, 8, 8, 8, 8, 1, 4,

  8, 8, 8, 8, 2, 8, 8,
  2, 8,

  8, 8, 8, 8, 2, 8, 8,
  2, 8, 8, 8, 8,

  8, 8, 8, 8, 2, 8, 8,
  4, 8, 3, 8, 8, 8, 8, 8, 1, 4,

  2, 6, 2, 6, 4, 4, 2, 6, 2, 3,
  8, 8, 8, 8
};

int melody2[] = {
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_D4,
  NOTE_E4, NOTE_F4, NOTE_D5, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, NOTE_F4, 
  NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_E5, 
  NOTE_D5, NOTE_G4, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_C5, REST, NOTE_C5, REST, NOTE_C5, NOTE_E5, 
  NOTE_C5, NOTE_D5, REST, NOTE_D5, NOTE_D5, NOTE_D5, REST, NOTE_D5, NOTE_F5, NOTE_D5, NOTE_E5, REST, NOTE_E5, 
  NOTE_E5, NOTE_E5, REST, NOTE_E5, NOTE_G5, NOTE_E5, NOTE_F5, REST, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_G4, 
  NOTE_B4, NOTE_C5, NOTE_C5, REST
};

int noteDurations2[] = {
  8, 8, 8, 8, 8, 8, 8, 4,
  8, 8, 8, 8, 2, 2,

  8, 8, 8, 8, 2, 8, 8,
  2, 8,

  8, 8, 8, 8, 2, 8, 8,
  4, 8, 8, 8, 8,

  8, 8, 8, 8, 2, 8, 8,
  2, 8, 4, 8, 8, 8, 8, 8, 1, 4,

  8, 8, 8, 8, 2, 8, 8,
  2, 8,

  8, 8, 8, 8, 2, 8, 8,
  2, 8, 8, 8, 8,

  8, 8, 8, 8, 2, 8, 8,
  4, 8, 3, 8, 8, 8, 8, 8, 1, 4,

  2, 6, 2, 6, 4, 4, 2, 6, 2, 3,
  8, 8, 8, 8
};

void InitAudio(void) {
  // Enable Clock Gating for PORTA
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;

  // Configure Mode 3 for the PWN pin operation
  PORTA->PCR[BUZZER] &= ~PORT_PCR_MUX_MASK;
  PORTA->PCR[BUZZER] |= PORT_PCR_MUX(3);

  // Enable Clock Gating for Timer2
  SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;

  // Select clock for TPM module
  SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // MCGFLLCLK or MCGPLLCLK/2

  // Initialzie Modulo Value 
  TPM2->MOD = 0;
	TPM2_C0V = 0;
	
  /* Edge-Aligned PWM */
  // Update Sns register: CMOD = 01, PS = 111 (128)
  TPM2->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
  TPM2->SC |= (TPM_SC_CMOD(1)) | (TPM_SC_PS(7));
  TPM2->SC &= ~(TPM_SC_CPWMS_MASK);

  // Enable PWM on TPM2 Channel 0 -> PTA1
  TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
  TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

void playNote(uint16_t frequency)
{
	if (frequency == 0) {
    TPM2_C0V = 0;
    return;
  }

  TPM2->MOD = (DEFAULT_SYSTEM_CLOCK / (PRESCALAR_VALUE * 2 * frequency));  // Set MOD value for the given frequency
  TPM2_C0V = TPM2->MOD * 0.5; // Set 90% duty cycle
}

void audio_thread(void *argument) {
	// Melody 1 - Moving; Melody 2 - Stationary
	int melody1_note = 0;
	int melody2_note = 0;
	
	// Length - 1 to consider zero indexing of arrays
	int melody1_length = sizeof(melody1)/sizeof(melody1[0]) - 1;
	int melody2_length = sizeof(melody2)/sizeof(melody2[0]) - 1;
	
	int prev_moving_flag = 0;
	
  for (;;) {
		if (moving_flag) {			
			// Reset song to beginning if flag changes or end of song reached
			if (prev_moving_flag != moving_flag || melody1_note == melody1_length) {
				melody1_note = 0;
			}
			
			int noteDuration = BEAT_LENGTH * noteDurations1[melody1_note];   // Calculate note duration
			playNote(melody1[melody1_note]);
			osDelay(noteDuration);
			
			playNote(0);
			osDelay(noteDuration * 1.3); // Delay between notes  
			
			melody1_note++;		
		} else {
			// Reset song to beginning if flag changes or end of song reached
			if (prev_moving_flag != moving_flag || melody2_note == melody2_length) {
				melody2_note = 0;
			}
			
			int noteDuration = BEAT_LENGTH * noteDurations1[melody1_note];   // Calculate note duration
			playNote(melody1[melody1_note]);
			osDelay(noteDuration);
			
			playNote(0);
			osDelay(noteDuration * 1.3); // Delay between notes  
			
			melody2_note++;			
		}
		
		prev_moving_flag = moving_flag;
	}
}
