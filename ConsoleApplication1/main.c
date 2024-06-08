#define _CRT_SECURE_NO_WARNINGS
#include "menu.h"
#include "define.h"
#include "sound.h"
#include <allegro5/allegro5.h>

/**
 * \mainpage Sudoku
 *
 * \section intro_sec Wstęp
 *
 * Witaj w dokumentacji projektu Sudoku! Ten projekt to implementacja gry Sudoku w języku C przy użyciu biblioteki Allegro do interfejsu graficznego.
 *
 * \section features_sec Funkcje
 *
 * - Implementacja klasycznej gry Sudoku z dziewięcioma kwadratowymi obszarami.
 * - Graficzny interfejs użytkownika z możliwością interakcji z polem gry.
 * - Możliwość zapisywania i wczytywania gry, aby kontynuować ją później.
 * - Automatyczne obliczanie czasu gry i wyświetlanie go na ekranie.
 * - Wykorzystanie podpowiedzi, aby pomóc graczowi w rozwiązaniu łamigłówki.
 * - Automatyczne określanie wygranej lub przegranej gracza w zależności od poprawności wypełnienia pola gry.
 *
 * \section how_to_play Jak grać
 *
 * - Celem gry Sudoku jest wypełnienie pola gry 9x9 tak, aby w każdym wierszu, kolumnie i 3x3 kwadracie znajdowały się cyfry od 1 do 9, a każda cyfra pojawiała się tylko raz.
 * - Aby wypełnić komórkę, kliknij na nią, a następnie wybierz odpowiednią cyfrę z klawiatury numerycznej znajdującej się na dole ekranu.
 * - Użyj przycisku podpowiedzi, aby otrzymać podpowiedź w przypadku trudności. Pamiętaj, że liczba podpowiedzi jest ograniczona.
 * - Jeśli wypełnisz wszystkie komórki prawidłowo, wygrasz. Jeśli liczba serc spadnie do zera lub wykonasz niewłaściwy ruch, przegrasz.
 *
 * \section credits_sec Autorzy
 *
 * - Projekt ten został opracowany przez zespół składający się z:
 * - Mykyta Lytvyn
 * - Danylo Rushchak
 * - Maryna Kravchenko
 */




/**
 * @brief Główna funkcja programu.
 *
 * Funkcja ta inicjalizuje wszystkie niezbędne dodatki Allegro, tworzy okno wyświetlania,
 * ustawia kolejkę zdarzeń, inicjalizuje dźwięk i wyświetla menu. Obsługuje również
 * czyszczenie zasobów i zamykanie dodatków po zakończeniu programu.
 *
 * @return int Zwraca 0 po pomyślnym zakończeniu.
 */
int main(void) {
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
    // Inicjalizacja dodatków Allegro
    al_init();
    al_install_mouse(); // Instalacja dodatku do obsługi myszy
    al_init_font_addon(); // Inicjalizacja dodatku do obsługi czcionek
    al_init_ttf_addon(); // Inicjalizacja dodatku do obsługi czcionek TrueType
    al_init_image_addon(); // Inicjalizacja dodatku do obsługi obrazów
    al_init_primitives_addon(); // Inicjalizacja dodatku do obsługi prymitywów
    al_init_acodec_addon(); // Inicjalizacja dodatku do obsługi kodeków audio
    al_install_audio(); // Instalacja dodatku do obsługi dźwięku

    // Tworzenie okna wyświetlania
    ALLEGRO_DISPLAY* display = al_create_display(W, H);
    if (!display) exit(-1);

    // Ustawienie tytułu okna
    al_set_window_title(display, "Sudoku");

    // Utworzenie kolejki zdarzeń do obsługi zdarzeń
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    // Rejestracja źródeł zdarzeń
    al_register_event_source(event_queue, al_get_mouse_event_source()); // Rejestracja zdarzeń myszy
    al_register_event_source(event_queue, al_get_display_event_source(display)); // Rejestracja zdarzeń wyświetlania

    // Inicjalizacja dźwięku
    init_sound();

    // Odtwarzanie muzyki w tle
    al_play_sample_instance(songInstance);

    // Wyświetlenie menu
    menu(display, event_queue);

    // Zatrzymanie odtwarzania muzyki i zwolnienie zasobów
    if (songInstance) {
        al_destroy_sample_instance(songInstance);
    }
    if (song) {
        al_destroy_sample(song);
    }

    // Wyłączanie dodatków, zwalnianie zasobów
    al_shutdown_primitives_addon(); // Wyłączenie dodatku do obsługi prymitywów
    al_shutdown_image_addon(); // Wyłączenie dodatku do obsługi obrazów
    al_shutdown_ttf_addon(); // Wyłączenie dodatku do obsługi czcionek TrueType
    al_shutdown_font_addon(); // Wyłączenie dodatku do obsługi czcionek
    al_uninstall_mouse(); // Wyłączenie dodatku do obsługi myszy
    al_uninstall_audio(); // Wyłączenie dodatku do obsługi dźwięku

    // Zniszczenie kolejki zdarzeń i okna wyświetlania
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    return 0;
}
