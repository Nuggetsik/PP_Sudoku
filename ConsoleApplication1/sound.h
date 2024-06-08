/**
 * @file sound.h
 * @brief Deklaracja funkcji i zmiennych używanych do obsługi dźwięku.
 */

#include <allegro5/allegro5.h>
#include <time.h>
#include "define.h"
#include <string.h>

#ifndef SOUND
#define SOUND
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

/**
 * @var ALLEGRO_SAMPLE* song
 * @brief Wskaźnik na aktualny próbkę dźwięku.
 */
ALLEGRO_SAMPLE* song = NULL;

/**
 * @var ALLEGRO_SAMPLE_INSTANCE* songInstance
 * @brief Wskaźnik na instancję próbki dźwięku.
 */
ALLEGRO_SAMPLE_INSTANCE* songInstance = NULL;

/**
 * @var char* songs[]
 * @brief Tablica ścieżek do plików dźwiękowych.
 */
char* songs[] = {
    "23.Upgrade_Station.ogg",
    "IGGY_WALK.ogg",
    "JoJo's Bizarre Adventure_ Diamond is Unbreakable OST - The Stardust Man Appears.ogg", 
    "JoJo's Bizarre Adventure_ Stardust Crusaders OST - Setting Off.ogg",
    "OST Stardust Crusaders [World] Track 19 - Travelers Who Rest.ogg", 
};

/**
 * @var int len_song
 * @brief Liczba plików dźwiękowych.
 */
int len_song = sizeof(songs) / sizeof(*songs);

/**
 * @brief Wybiera losowy utwór dźwiękowy i odtwarza go.
 *
 * Funkcja losowo wybiera jeden z dostępnych utworów dźwiękowych,
 * ładuje go i rozpoczyna odtwarzanie. Przy tym zwalniane są zasoby
 * poprzedniego utworu.
 */
void rand_sound() {
    // Wybór losowego utworu
    int numSong = rand() % len_song;

    // Kopiowanie ścieżki do wybranego utworu do nameSong
    strcpy(nameSong, songs[numSong]);
    int len = strlen(nameSong);
    if (len > 4)
        nameSong[len - 4] = '\0';
        
    // Tworzenie pełnej ścieżki do pliku
    char path[256];
    snprintf(path, sizeof(path), "sounds/songs/%s", songs[numSong]);

    // Ładowanie nowego pliku dźwiękowego
    ALLEGRO_SAMPLE* new_song = al_load_sample(path);
    if (!new_song) {
        fprintf(stderr, "Nie udało się załadować utworu: %s\n", path);
        exit(-5);
    }

    // Zwalnianie zasobów poprzedniego utworu
    if (songInstance) {
        al_destroy_sample_instance(songInstance);
    }
    if (song) {
        al_destroy_sample(song);
    }

    // Ustawienie nowego utworu
    song = new_song;
    songInstance = al_create_sample_instance(song);

    // Konfiguracja i odtwarzanie utworu
    al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_ONCE);
    al_set_sample_instance_gain(songInstance, volume);
    al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());
    al_play_sample_instance(songInstance);
}

/**
 * @brief Inicjalizacja obsługi dźwięku.
 *
 * Funkcja rezerwuje sloty dla próbek dźwiękowych, inicjalizuje generator liczb
 * losowych i losowo wybiera oraz odtwarza jeden z dostępnych utworów.
 */
void init_sound() {
    // Rezerwacja 4 slotów dla próbek dźwiękowych
    al_reserve_samples(4);

    // Inicjalizacja generatora liczb losowych
    srand(time(NULL));

    // Losowe odtwarzanie muzyki w tle
    rand_sound();
}

/**
 * @brief Sprawdza, czy aktualny utwór się zakończył i odtwarza nowy.
 *
 * Funkcja sprawdza, czy aktualny utwór dźwiękowy się zakończył. Jeśli utwór się zakończył,
 * wybierany jest i odtwarzany nowy losowy utwór.
 */
void check_song_finished() {
    // Sprawdzenie, czy aktualny utwór się zakończył
    if (!al_get_sample_instance_playing(songInstance)) {
        rand_sound();
    }
}

#endif
