/**
 * @file functions.h
 * @brief Deklaracja funkcji używanych w grze Sudoku.
 */

#include "structs.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#ifndef FUNCTIONS
#define FUNCTIONS

/**
 * @brief Wyświetla obiekt na ekranie.
 *
 * Wczytuje określony plik obrazu i rysuje go na ekranie w podanej pozycji i rozmiarze.
 *
 * @param name_file Nazwa pliku obrazu do wyświetlenia.
 * @param width Szerokość obiektu.
 * @param height Wysokość obiektu.
 * @param x Współrzędna x lewego górnego rogu obiektu.
 * @param y Współrzędna y lewego górnego rogu obiektu.
 */
void show(char* name_file, float width, float height, int x, int y)
{
    ALLEGRO_BITMAP* obj = al_load_bitmap(name_file);
    if (!obj) exit(-3);

    al_draw_scaled_bitmap(obj, 0, 0, al_get_bitmap_width(obj), al_get_bitmap_height(obj), x, y, width, height, 0);
    al_destroy_bitmap(obj);
}

/**
 * @brief Zmienia rozdzielczość okna wyświetlacza.
 *
 * Zmienia rozdzielczość okna wyświetlacza na podaną szerokość i wysokość.
 *
 * @param display Wyświetlacz, który ma zostać zmieniony.
 * @param width Nowa szerokość okna wyświetlacza.
 * @param height Nowa wysokość okna wyświetlacza.
 */
void change_resolution(ALLEGRO_DISPLAY* display, int width, int height) {
    al_resize_display(display, width, height);
}

/**
 * @brief Wyświetla serca na ekranie.
 *
 * Pokazuje serca reprezentujące życia gracza na ekranie.
 *
 * @param hearts Tablica struktur OBJECT reprezentujących serca.
 * @param count Liczba serc do wyświetlenia.
 */
void show_heart(struct OBJECT hearts[3], int count)
{
    for (int i = 0; i < count; i++)
        hearts[i].show(hearts[i].name_file, hearts[i].width, hearts[i].height,
            hearts[i].x, hearts[i].y);
}

/**
 * @brief Rysuje klawiaturę numeryczną na ekranie.
 *
 * Rysuje klawiaturę numeryczną z cyframi 1-9 na ekranie.
 *
 * @param numpad Tablica struktur NUMPAD reprezentujących przyciski klawiatury numerycznej.
 */
void draw_numpad(struct NUMPAD* numpad) {
    for (int i = 0; i < 9; i++)
        show(numpad[i].name_file, numpad[i].width, numpad[i].height, numpad[i].x, numpad[i].y);
}

/**
 * @brief Usuwa element z tablicy.
 *
 * Usuwa element o określonym indeksie z tablicy.
 *
 * @param array Wskaźnik do tablicy, z której ma zostać usunięty element.
 * @param sizeOfArray Rozmiar tablicy.
 * @param indexToRemove Indeks elementu do usunięcia.
 */
void remove_element(struct CELL** array, int sizeOfArray, int indexToRemove) {
    if (indexToRemove < 0 || indexToRemove >= sizeOfArray) {
        // Nieprawidłowy indeks
        return;
    }

    struct CELL* temp = malloc((sizeOfArray - 1) * sizeof(struct CELL));
    if (temp == NULL) {
        // Nie powiodło się alokowanie pamięci
        return;
    }

    // Kopiuj wszystko PRZED indeksem
    memcpy(temp, *array, indexToRemove * sizeof(struct CELL));

    // Kopiuj wszystko PO indeksie
    memcpy(temp + indexToRemove, *array + indexToRemove + 1, (sizeOfArray - indexToRemove - 1) * sizeof(struct CELL));
    
    free(*array);
    *array = temp;
}

#endif
