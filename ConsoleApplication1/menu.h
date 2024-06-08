/**
 * @file menu.h
 * @brief Funkcje do wyświetlania menu gry i zarządzania nim.
 */



/*!
 * @file
 *@brief Po naciśnięciu przycisku "Start" gracz zobaczy 3 poziomy trudności: Łatwy, Średni, Trudny. 
 *Po ich naciśnięciu gracz przejdzie do samej gry. 
 *Jeśli gracz już grał i gra została zapisana, w oknie pojawi się przycisk "Kontynuuj", 
 *po naciśnięciu którego tygrys będzie kontynuował grę z tym samym wynikiem.
 */
#include "structs.h"
#include "functions.h"
#include "define.h"
#include "game.h"
#include "sound.h"

#define SAVE_FILE "save.dat"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

#ifndef MENU
#define MENU

/**
 * @brief Wyświetla menu główne gry.
 *
 * Rysuje tło, przyciski start i kontynuacji oraz nazwę gry.
 *
 * @param background Wskaźnik do bitmapy tła.
 * @param b_start Struktura przycisku start.
 * @param b_continue Struktura przycisku kontynuacji.
 * @param continue_flag Flaga wskazująca, czy przycisk kontynuacji powinien być wyświetlony.
 * @param font Wskaźnik do czcionki.
 * @param text_width Szerokość tekstu nazwy gry.
 * @param text Tekst nazwy gry.
 */
void displayMenu(ALLEGRO_BITMAP* background, struct OBJECT b_start, struct OBJECT b_continue, bool continue_flag, ALLEGRO_FONT* font, int text_width, char *text)
{
    // Rysowanie tła
    al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 0, 0, W, H, 0);

    // Wyświetlanie przycisku start
    b_start.show(b_start.name_file, b_start.width, b_start.height, b_start.x, b_start.y);

    // Rysowanie tekstu nazwy gry
    al_draw_text(font, al_map_rgb(0, 0, 0), (W - text_width) / 2, H / 4, 0, text);

    // Wyświetlanie przycisku kontynuacji, jeśli continue_flag jest prawdziwe
    if (continue_flag)
    {
        b_continue.show(b_continue.name_file, b_continue.width, b_continue.height, b_continue.x, b_continue.y);
    }
    al_flip_display();
}

/**
 * @brief Wyświetla przyciski wyboru poziomu trudności.
 *
 * Rysuje tło, tekst nazwy gry oraz przyciski poziomów trudności.
 *
 * @param background Wskaźnik do bitmapy tła.
 * @param font Wskaźnik do czcionki.
 * @param text_width Szerokość tekstu nazwy gry.
 * @param text Tekst nazwy gry.
 * @param b_easy Struktura przycisku łatwego poziomu.
 * @param b_mid Struktura przycisku średniego poziomu.
 * @param b_hard Struktura przycisku trudnego poziomu.
 */
void displayDiffucult(ALLEGRO_BITMAP* background, ALLEGRO_FONT* font, int text_width, char* text, struct OBJECT b_easy, struct OBJECT b_mid, struct OBJECT b_hard)
{
    // Rysowanie tła
    al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 0, 0, W, H, 0);
    
    // Rysowanie tekstu nazwy gry
    al_draw_text(font, al_map_rgb(0, 0, 0), (W - text_width) / 2, H / 4, 0, text);

    // Poziomy trudności
    b_easy.show(b_easy.name_file, b_easy.width, b_easy.height, b_easy.x, b_easy.y);
    b_mid.show(b_mid.name_file, b_mid.width, b_mid.height, b_mid.x, b_mid.y);
    b_hard.show(b_hard.name_file, b_hard.width, b_hard.height, b_hard.x, b_hard.y);

    al_flip_display();
}

/**
 * @brief Sprawdza, czy istnieje zapis gry.
 *
 * Ustawia flagę continue_flag na true, jeśli istnieje zapis gry.
 *
 * @param continue_flag Wskaźnik na flagę kontynuacji.
 */
void checkEmptyFile(bool *continue_flag)
{
    FILE* fp = fopen("save.dat", "r");
    if (fp != NULL) // Jeśli plik nie jest pusty
    {
        fseek(fp, 0, SEEK_END); // Przesunięcie wskaźnika pliku na koniec
        int fsize = ftell(fp);  // Pobranie aktualnej pozycji wskaźnika (rozmiar pliku)
        if (fsize > 0)  // Sprawdzenie rozmiaru pliku
            *continue_flag = true;
        fclose(fp);
    }
    else
        *continue_flag = false;
}

/**
 * @brief Aktualizuje układ i rozmiar przycisków na podstawie rozmiaru ekranu.
 *
 * @param b_start Wskaźnik na strukturę przycisku start.
 * @param b_continue Wskaźnik na strukturę przycisku kontynuacji.
 * @param b_easy Wskaźnik na strukturę przycisku łatwego poziomu.
 * @param b_mid Wskaźnik na strukturę przycisku średniego poziomu.
 * @param b_hard Wskaźnik na strukturę przycisku trudnego poziomu.
 */
void resizing(struct OBJECT *b_start, struct OBJECT *b_continue, struct OBJECT *b_easy, struct OBJECT *b_mid, struct OBJECT* b_hard) {
    b_start->x = W / 2 - b_start->width / 2;
    b_start->y = H / 1.25 - b_start->height / 2 - 50;

    b_continue->x = b_start->x;
    b_continue->y = b_start->y + W / 9;

    b_easy->x = W / 2 - b_easy->width / 2;
    b_easy->y = H / 1.25 - b_easy->height / 2 - 100;

    b_mid->x = W / 2 - b_mid->width / 2;
    b_mid->y = b_easy->y + W / 9;

    b_hard->x = W / 2 - b_hard->width / 2;
    b_hard->y = b_mid->y + W / 9;

    b_start->width = b_continue->width = b_easy->width = b_mid->width = b_hard->width = W / 5;

    b_start->height = b_continue->height = b_easy->height = b_mid->height = b_hard->height = H / 11;
}

/**
 * @brief Funkcja menu gry.
 *
 * Wyświetla interfejs menu gry, obsługuje zdarzenia użytkownika i uruchamia grę.
 *
 * @param display Wskaźnik na wyświetlacz.
 * @param event_queue Wskaźnik na kolejkę zdarzeń.
 * @return 0 jeśli gra zakończyła się pomyślnie.
 */
int menu(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* event_queue)
{
    ALLEGRO_EVENT event;

    // Ładowanie czcionki
    ALLEGRO_FONT* font = al_load_ttf_font("Tenada.ttf", SIZE_FONT, 0);
    if (!font) exit(-2);

    // Ładowanie obrazu tła
    ALLEGRO_BITMAP* background = al_load_bitmap("image/menu_bg.png");
    if (!background) exit(-3);

    // Definiowanie przycisku start
    struct OBJECT b_start = (struct OBJECT){ "image/start.png", W / 5, H / 11, 0, 0, show };
    b_start.x = W / 2 - b_start.width / 2;
    b_start.y = H / 1.25 - b_start.height / 2 - 50;

    // Definiowanie przycisku kontynuacji
    struct OBJECT b_continue = (struct OBJECT){ "image/continue.png", W / 5, H / 11, 0, 0, show };
    b_continue.x = b_start.x;
    b_continue.y = b_start.y + 80;

    // Przyciski poziomu trudności
    struct OBJECT b_easy = (struct OBJECT){ "image/easy.png", W / 5, H / 11, 0, 0, show };
    b_easy.x = W / 2 - b_easy.width / 2;
    b_easy.y = H / 1.25 - b_easy.height / 2 - 100;

    struct OBJECT b_mid = (struct OBJECT){ "image/mid.png", W / 5, H / 11, 0, 0, show };
    b_mid.x = W / 2 - b_mid.width / 2;
    b_mid.y = b_easy.y + 80;

    struct OBJECT b_hard = (struct OBJECT){ "image/hard.png", W / 5, H / 11, 0, 0, show };
    b_hard.x = W / 2 - b_hard.width / 2;
    b_hard.y = b_mid.y + 80;

    char text[] = "Sudoku"; // Tekst wyświetlany na ekranie (nazwa gry)
    int text_width = al_get_text_width(font, text);

    bool finish = false;    // Flaga końca gry

    static bool continue_flag = false;      // Flaga dla przycisku "Kontynuuj"
    checkEmptyFile(&continue_flag);         // Sprawdzenie, czy istnieje zapis gry do kontynuacji
    bool start_game = false;                // Flaga rozpoczęcia gry
    
    bool displayDifficult = false;          // Flaga wyświetlania przycisków poziomu trudności
    bool chosenDifficult = false;           // Flaga sygnalizująca wybór poziomu trudności

    displayMenu(background, b_start, b_continue, continue_flag, font, text_width, text);        // Wyświetlanie interfejsu menu
    
    while (!finish)
    {
        al_wait_for_event(event_queue, &event);

        // Zdarzenie zamknięcia
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            finish = true;
        }

        // Zdarzenie kliknięcia przyciskiem myszy
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (event.mouse.button & 1)
            {
                int x = event.mouse.x;
                int y = event.mouse.y;

                if (!displayDifficult) {
                    // Sprawdzenie, czy przycisk start został kliknięty
                    if (x >= b_start.x && x < (b_start.x + b_start.width) && y >= b_start.y && y < (b_start.y + b_start.height))
                    {
                        continue_flag = true;
                        displayDiffucult(background, font, text_width, text, b_easy, b_mid, b_hard);
                        displayDifficult = true;
                    }

                    if (continue_flag)
                    {
                        // Sprawdzenie, czy przycisk kontynuacji został kliknięty
                        if (x >= b_continue.x && x < (b_continue.x + b_continue.width) && y >= b_continue.y && y < (b_continue.y + b_continue.height))
                        {
                            bool exit_game = game(display, event_queue, true);
                            if (exit_game) {
                                finish = true;
                            }
                            else
                            {
                                checkEmptyFile(&continue_flag);
                                displayMenu(background, b_start, b_continue, continue_flag, font, text_width, text);
                            }
                        }
                    }
                }
                else {
                    // Sprawdzenie, czy wybrano poziom trudności
                    if (x >= b_easy.x && x < (b_easy.x + b_easy.width) && y >= b_easy.y && y < (b_easy.y + b_easy.height))
                    {
                        difficult[0] = 3; // Szansa
                        difficult[1] = 4; // Liczba pustych komórek
                        
                        chosenDifficult = true; // Ustawienie flagi sygnalizującej wybór poziomu trudności
                    }

                    if (x >= b_mid.x && x < (b_mid.x + b_mid.width) && y >= b_mid.y && y < (b_mid.y + b_mid.height))
                    {
                        difficult[0] = 3;  
                        difficult[1] = 5;
                        chosenDifficult = true; // Ustawienie flagi sygnalizującej wybór poziomu trudności
                    }

                    if (x >= b_hard.x && x < (b_hard.x + b_hard.width) && y >= b_hard.y && y < (b_hard.y + b_hard.height))
                    {
                        difficult[0] = 4;  
                        difficult[1] = 6;
                        chosenDifficult = true; // Ustawienie flagi sygnalizującej wybór poziomu trudności
                    }

                    if (chosenDifficult) {
                        bool exit_game = game(display, event_queue, false); // Rozpoczęcie gry z wybranym poziomem trudności
                        // Jeśli gra zostanie zamknięta
                        if (exit_game) {
                            finish = true;
                        }
                        else {
                            // Powrót do menu
                            resizing(&b_start, &b_continue, &b_easy, &b_mid, &b_hard);
                            checkEmptyFile(&continue_flag); // Sprawdzenie, czy istnieje zapis
                            displayMenu(background, b_start, b_continue, continue_flag, font, text_width, text);
                            displayDifficult = false; // Resetowanie flagi wyświetlania poziomów trudności
                            chosenDifficult = false; // Resetowanie flagi wybranego poziomu trudności
                        }
                    }
                }
            }
        }
    }
    al_destroy_font(font);
    al_destroy_bitmap(background);
    return 0;
}

#endif
