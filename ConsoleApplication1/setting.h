/**
 * @file setting.h
 * @brief Deklaracje funkcji związanych z ustawieniami gry.
 */

#ifndef SETTING
#define SETTING

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "structs.h"
#include "functions.h"
#include "define.h"
#include "sound.h"

/**
 * @brief Funkcja odpowiedzialna za ustawienia gry.
 *
 * Funkcja wyświetla menu ustawień, pozwalając graczowi dostosować rozdzielczość ekranu i głośność muzyki.
 *
 * @param display Wskaźnik na obiekt reprezentujący wyświetlacz Allegro.
 * @param event_queue Wskaźnik na kolejkę zdarzeń Allegro.
 * @param timer Wskaźnik na timer Allegro.
 * @param finish Wskaźnik na flagę końca gry.
 * @return Wartość logiczna true, jeśli ustawienia zostały zaakceptowane, w przeciwnym razie false.
 */
// Funkcja do ustawień
bool setting(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_TIMER* timer, bool* finish)
{
    if (!display || !event_queue || !timer) {
        printf("Błąd inicjalizacji wyświetlacza, kolejki zdarzeń lub timera.\n");
        return false;
    }

    // Ładowanie tła menu ustawień
    ALLEGRO_BITMAP* setting_menu = al_load_bitmap("image/setting.png");
    if (!setting_menu) exit(-3);

    // Ustawianie rozmiaru okna
    int width = W / 1.5;  
    int height = H / 2.5;

    // Ustawianie pozycji okna ustawień
    float x_setting = W / 2 - width / 2;    
    float y_setting = H / 2 - height / 2;

    // Ładowanie czcionki
    int sizeFont, sizeFontName = 0;
    if (W == resolution[0][0]){
        sizeFont = 25;
        sizeFontName = 17;
    }
    else if (W == resolution[1][0]) {
        sizeFont = 29;
        sizeFontName = 21;
    }
    else if (W == resolution[2][0]) {
        sizeFont = 32;
        sizeFontName = 24;
    }

    ALLEGRO_FONT* font = al_load_ttf_font("Tenada.ttf", sizeFont, 0);
    if (!font) exit(- 2);

    ALLEGRO_FONT* fontNameSong = al_load_ttf_font("Tenada.ttf", sizeFontName, 0);
    if (!fontNameSong) exit(-2);

    char text_setting[] = "Setting";  // Tekst wyświetlany na ekranie
    int text_setting_width = al_get_text_width(font, text_setting);
    
    char text_resolution[] = "Resolution window ";
    char text_resolution_num[10];

    char *text_volume = "Volume ";

    static int variant_size = 0;  // Do zmiany rozmiaru okna

    // Przycisk akceptacji ustawień
    struct OBJECT b_accept = (struct OBJECT){ "image/accept.png", width / 5, height / 7, 0, 0, show };
    b_accept.x = x_setting + width - b_accept.width - 20;
    b_accept.y = y_setting + height - b_accept.height - 20;

    // Przycisk anulowania ustawień
    struct OBJECT b_cancel = (struct OBJECT){ "image/cancel.png", width / 5, height / 7, 0, 0, show };
    b_cancel.x = x_setting + 20;
    b_cancel.y = b_accept.y = y_setting + height - b_cancel.height - 20;

    // Przycisk lewa strzałka
    struct OBJECT b_left = (struct OBJECT){ "image/left_arrow.png", width / 20, height / 18, 0, 0, show };
    b_left.x = (x_setting + width) / 2 + 90;
    b_left.y = y_setting + W / 12;

    // Przycisk prawa strzałka
    struct OBJECT b_right = (struct OBJECT){ "image/right_arrow.png", width / 20, height / 18, 0, 0, show };
    b_right.x = b_left.x + 160;
    b_right.y = y_setting + W / 12;

    // Tło suwaka głośności
    struct OBJECT sliderBackground = (struct OBJECT){ "image/SliderBackground.png", width / 3, height / 50, 0, 0, show };
    sliderBackground.x = (x_setting + width) / 2 + 100;
    sliderBackground.y = b_left.y + 60;

    int sliderLenght = sliderBackground.width;  // Rozmiar tła suwaka

    // Suwak głośności
    struct OBJECT slider = (struct OBJECT){ "image/Slider.png", width / 30, height / 22, 0, 0, show };
    slider.x = volume * sliderLenght + sliderBackground.x - 5;
    slider.y = sliderBackground.y - 3;

    bool isDragging = false; // Flaga wskazująca, czy suwak jest aktualnie przeciągany

    ALLEGRO_EVENT event;  // Zmienna do przechowywania zdarzeń

    bool open_setting = true;
    while (open_setting)
    {
        al_wait_for_event(event_queue, &event);

        // Sprawdzenie zdarzenia zamknięcia okna
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            al_destroy_font(font);
            al_destroy_font(fontNameSong);
            al_destroy_bitmap(setting_menu);
            open_setting = false;
            *finish = true;
        }

        // Sprawdzenie zdarzenia naciśnięcia przycisku myszy
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (event.mouse.button & 1)
            {
                int x = event.mouse.x;
                int y = event.mouse.y;

                // Sprawdzenie, czy przycisk akceptacji został kliknięty
                if (x >= b_accept.x && x < (b_accept.x + b_accept.width) && y >= b_accept.y && y < (b_accept.y + b_accept.height))
                {
                    W = resolution[variant_size][0];
                    H = resolution[variant_size][1];

                    // Ustawienie pozycji okna na środku ekranu
                    al_set_window_position(display, (1920-W)/2, (1080-H)/2);

                    // Zmiana rozdzielczości ekranu
                    change_resolution(display, resolution[variant_size][0], resolution[variant_size][1]);
                    al_destroy_font(font);
                    al_destroy_font(fontNameSong);
                    al_destroy_bitmap(setting_menu);
                    *finish = false;
                    return true;
                }
                // Sprawdzenie, czy przycisk anulowania został kliknięty
                if (x >= b_cancel.x && x < (b_cancel.x + b_cancel.width) && y >= b_cancel.y && y < (b_cancel.y + b_cancel.height))
                {
                    al_destroy_font(font);
                    al_destroy_font(fontNameSong);
                    al_destroy_bitmap(setting_menu);
                    *finish = false;
                    return false;
                }
                // Sprawdzenie, czy przycisk prawa strzałka został kliknięty
                if (x >= b_right.x && x < (b_right.x + b_right.width) && y >= b_right.y && y < (b_right.y + b_right.height))
                {
                    if (variant_size >= 0 && variant_size < 2)
                    {
                        variant_size += 1;
                    }
                }

                // Sprawdzenie, czy przycisk lewa strzałka został kliknięty
                if (x >= b_left.x && x < (b_left.x + b_left.width) && y >= b_left.y && y < (b_left.y + b_left.height))
                {
                    if (variant_size > 0 && variant_size <= 2)
                    {
                        variant_size -= 1;
                    }
                }

                // Sprawdzenie, czy suwak został naciśnięty
                if (x >= slider.x && x < (slider.x + slider.width) && y >= slider.y && y < (slider.y + slider.height))
                {
                    isDragging = true; // Rozpoczęcie przeciągania
                }

            }
        }
        // Obsługa zdarzeń zmiany pozycji myszy
        else if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            if (isDragging)
            {
                int x = event.mouse.x;

                // Sprawdzenie, czy przeciąganie odbywa się w obrębie tła suwaka
                if (x >= sliderBackground.x && x <= (sliderBackground.x + sliderBackground.width - slider.width/2))
                {
                    slider.x = x; // Aktualizacja pozycji suwaka
                    volume = (double)(slider.x - sliderBackground.x) / sliderLenght;
                    
                    al_set_sample_instance_gain(songInstance, volume);
                }
            }
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            if (event.mouse.button & 1) // Sprawdzenie, czy lewy przycisk myszy został zwolniony
            {
                isDragging = false; // Zakończenie przeciągania
            }
        }
        
        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            // Formatowanie tekstu rozdzielczości
            sprintf(text_resolution_num, "%d:%d", resolution[variant_size][0], resolution[variant_size][1]);
            int len_res_num = al_get_text_width(font, text_resolution_num);
            
            // Rysowanie menu ustawień
            al_draw_scaled_bitmap(setting_menu, 0, 0, al_get_bitmap_width(setting_menu), al_get_bitmap_height(setting_menu), x_setting, y_setting, width, height, 0);
            al_draw_text(font, al_map_rgb(0, 0, 0), (x_setting + width) / 2, y_setting + 25, 0, text_setting);

            // Rysowanie tekstu rozdzielczości
            al_draw_text(font, al_map_rgb(0, 0, 0), x_setting + 25, b_right.y, 0, text_resolution);
            al_draw_text(font, al_map_rgb(0, 0, 0), b_left.x + 30, b_right.y, 0, text_resolution_num);

            // Rysowanie tekstu głośności
            al_draw_text(font, al_map_rgb(0, 0, 0), x_setting + 25, sliderBackground.y-8, 0, text_volume);
           
            // Rysowanie nazwy utworu
            int len_nameSong = al_get_text_width(font, nameSong);
            if (len_nameSong >= width) {
                // Znajdowanie środka tekstu
                int middle_index = strlen(nameSong) / 2;

                // Szukanie najbliższej spacji do środka tekstu, aby podzielić go na dwie części
                while (nameSong[middle_index] != ' ') {
                    middle_index--;
                }

                // Podział tekstu na dwie części
                char first_half[100]; // Zakładamy maksymalną długość pierwszej połowy tekstu
                char second_half[100]; // Zakładamy maksymalną długość drugiej połowy tekstu
                strncpy(first_half, nameSong, middle_index);
                first_half[middle_index] = '\0'; // Dodanie zakończenia zero
                strcpy(second_half, nameSong + middle_index + 1);

                // Rysowanie pierwszej połowy tekstu
                al_draw_text(fontNameSong, al_map_rgb(0, 0, 0), x_setting + 25, y_setting + W/4, 0, first_half);

                // Rysowanie drugiej połowy tekstu
                al_draw_text(fontNameSong, al_map_rgb(0, 0, 0), x_setting + 25, y_setting + W/4+20, 0, second_half);
            }
            else {
                // Jeśli tekst mieści się na ekranie, po prostu go rysujemy.
                al_draw_text(fontNameSong, al_map_rgb(0, 0, 0), x_setting + 25, y_setting + W/4, 0, nameSong);
            }

            // Aktualizacja pozycji przycisku prawej strzałki
            b_right.x = len_res_num + b_left.x + 35;

            // Wyświetlanie przycisków
            b_accept.show(b_accept.name_file, b_accept.width, b_accept.height, b_accept.x, b_accept.y);
            b_cancel.show(b_cancel.name_file, b_cancel.width, b_cancel.height, b_cancel.x, b_cancel.y);
            b_right.show(b_right.name_file, b_right.width, b_right.height, b_right.x, b_right.y);
            b_left.show(b_left.name_file, b_left.width, b_left.height, b_left.x, b_left.y);

            sliderBackground.show(sliderBackground.name_file, sliderBackground.width, sliderBackground.height, sliderBackground.x, sliderBackground.y);
            slider.show(slider.name_file, slider.width, slider.height, slider.x, slider.y);
            al_flip_display();  // Odświeżenie wyświetlacza
        }
    }

    return false;
}

#endif

