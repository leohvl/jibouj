#ifndef READWAVE_H
#define READWAVE_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sndfile.h>
#include <alsa/asoundlib.h>
#include <unistd.h>
#define GPIO_PIN 26 // Broche GPIO utilisée pour le PWM
#define GPIO_PIN5 5 // Broche GPIO utilisée pour le PWM

int16_t* read_wav(const char* filename, int* sample_rate, int* num_samples, int* num_channels);

// Fonction pour envoyer des données audio sur GPIO
void play_audio(int16_t* audio_data, int num_samples, int sample_rate, int num_channels);


#endif