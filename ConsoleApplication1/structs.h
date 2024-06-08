/**
 * @file structs.h
 * @brief Deklaracja struktur używanych w grze Sudoku.
 */

#ifndef STRUCTS
#define STRUCTS

/**
 * @struct OBJECT
 * @brief Globalna struktura dla obiektów.
 *
 * Struktura reprezentująca obiekt z obrazem, rozmiarem i pozycją oraz wskaźnikiem na funkcję wyświetlającą.
 */
struct OBJECT
{
    char name_file[50];     ///< Ścieżka do obrazu.
    float width;            ///< Szerokość przycisku.
    float height;           ///< Wysokość przycisku.
    float x;                ///< Współrzędna x obiektu.
    float y;                ///< Współrzędna y obiektu.
    void (*show)(char*, float, float, int, int);  ///< Wskaźnik na funkcję wyświetlającą.
};

/**
 * @struct CELL
 * @brief Struktura dla komórek.
 *
 * Struktura reprezentująca komórkę na planszy gry, dziedzicząca właściwości z OBJECT.
 */
struct CELL
{
    struct OBJECT;   ///< Dziedziczenie właściwości z OBJECT.
    int rows;        ///< Indeks wiersza.
    int column;      ///< Indeks kolumny.
};

/**
 * @struct NUMPAD
 * @brief Struktura dla przycisków numerycznych.
 *
 * Struktura reprezentująca przycisk numeryczny na klawiaturze numerycznej, dziedzicząca właściwości z OBJECT.
 */
struct NUMPAD
{
    struct OBJECT;  ///< Dziedziczenie właściwości z OBJECT.
    int num;        ///< Numer przycisku.
};

/**
 * @struct GameState
 * @brief Struktura do przechowywania stanu gry.
 *
 * Struktura przechowująca aktualny stan gry, w tym planszę gry, wskazówki, serca i czas gry.
 */
struct GameState {
    int matrix_area[9][9]; ///< Plansza gry.
    int hints;             ///< Liczba wskazówek.
    int hearts;            ///< Liczba serc.
    unsigned int minutes;  ///< Liczba minut gry.
    unsigned int seconds;  ///< Liczba sekund gry.
    unsigned int time_count; ///< Licznik czasu gry.
    // Dodaj inne zmienne do przechowywania
};

#endif
