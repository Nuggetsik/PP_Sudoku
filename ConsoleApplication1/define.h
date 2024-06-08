#ifndef DEFINE
#define DEFINE
#include <stdbool.h>

/**
 * @brief Liczba klatek na sekundę (FPS).
 */
#define FPS 10                          //FPS 

/**
 * @brief Rozmiar czcionki.
 */
#define SIZE_FONT 60                    //Rozmiar czcionki

/**
 * @brief Długość array_zero.
 */
#define LEN 10

/**
 * @brief Szerokość okna.
 */
int W = 720;                           //Szerokość okna

/**
 * @brief Wysokość okna.
 */
int H = 720;                           //Wysokość okna

/**
 * @brief Tablica rozdzielczości okna.
 *
 * Tablica przechowująca różne rozdzielczości okna, które mogą być używane w programie.
 */
const int resolution[3][2] = { {720, 720}, {900, 900}, {1000, 1000} };	//Tablica rozdzielczości okna

/**
 * @brief Poziom trudności.
 *
 * Tablica przechowująca poziom trudności gry.
 */
int difficult[2] = { 0 , 0 };		//Poziom trudności

/**
 * @brief Poziom głośności.
 */
double volume = 1.0;	//Poziom głośności

/**
 * @brief Nazwa muzyki w tle.
 *
 * Tablica przechowująca nazwę pliku z muzyką w tle.
 */
char nameSong[250];		//Nazwa muzyki w tle

#endif
