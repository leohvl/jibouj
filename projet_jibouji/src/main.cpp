#include "../lib/read_wave/read_wave.h"

int main() {
    const char* wav_file = "../wav_files/son1.wav";
    int sample_rate, num_samples, num_channels;

    // Lire le fichier WAV
    int16_t* audio_data = read_wav(wav_file, &sample_rate, &num_samples, &num_channels);
    if (!audio_data) {
        return 1;  // Arrêter le programme si une erreur survient
    }

    printf("Lecture du fichier WAV à %d Hz, %d canaux, %d échantillons\n", sample_rate, num_channels, num_samples);

    // Jouer l'audio via ALSA
    play_audio(audio_data, num_samples, sample_rate, num_channels);

    free(audio_data);  // Libérer la mémoire
    return 0;
}