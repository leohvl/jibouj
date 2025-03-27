#include "read_wave.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sndfile.h>
#include <alsa/asoundlib.h>
#include <unistd.h>


#define PCM_DEVICE "default"  // Périphérique de sortie audio par défaut

// Fonction pour lire un fichier WAV
int16_t* read_wav(const char* filename, int* sample_rate, int* num_samples, int* num_channels) {
    SNDFILE* file;
    SF_INFO sfinfo;

    file = sf_open(filename, SFM_READ, &sfinfo);
    if (!file) {
        fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier WAV (%s)\n", sf_strerror(NULL));
        return NULL;
    }

    if (sfinfo.format != (SF_FORMAT_WAV | SF_FORMAT_PCM_16)) {
        fprintf(stderr, "Erreur: Le fichier n'est pas au format PCM 16 bits\n");
        sf_close(file);
        return NULL;
    }

    *sample_rate = sfinfo.samplerate;
    *num_samples = sfinfo.frames;
    *num_channels = sfinfo.channels;

    int16_t* samples = (int16_t*)malloc(sfinfo.frames * sfinfo.channels * sizeof(int16_t));
    if (!samples) {
        perror("Erreur d'allocation mémoire");
        sf_close(file);
        return NULL;
    }

    sf_read_short(file, samples, sfinfo.frames * sfinfo.channels);
    sf_close(file);

    return samples;
}

// Fonction pour jouer l'audio avec ALSA
void play_audio(int16_t* audio_data, int num_samples, int sample_rate, int num_channels) {
    snd_pcm_t* handle;
    snd_pcm_hw_params_t* params;
    snd_pcm_uframes_t frames;
    int rc;

    // Ouvrir le périphérique de lecture PCM
    rc = snd_pcm_open(&handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        fprintf(stderr, "Impossible d'ouvrir le périphérique PCM: %s\n", snd_strerror(rc));
        return;
    }

    // Allouer la structure de paramètres
    snd_pcm_hw_params_malloc(&params);
    snd_pcm_hw_params_any(handle, params);
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(handle, params, num_channels);
    snd_pcm_hw_params_set_rate_near(handle, params, (unsigned int*)&sample_rate, 0);
    snd_pcm_hw_params(handle, params);
    snd_pcm_hw_params_free(params);

    frames = 32; // Nombre d'échantillons à envoyer par période
    snd_pcm_prepare(handle);

    // Lecture de l'audio
    int sample_index = 0;
    while (sample_index < num_samples * num_channels) {
        rc = snd_pcm_writei(handle, &audio_data[sample_index], frames);
        if (rc == -EPIPE) {
            fprintf(stderr, "Underrun détecté!\n");
            snd_pcm_prepare(handle);
        } else if (rc < 0) {
            fprintf(stderr, "Erreur d'écriture sur PCM: %s\n", snd_strerror(rc));
            break;
        } else {
            sample_index += frames * num_channels;
        }
    }

    snd_pcm_drain(handle);
    snd_pcm_close(handle);
}

