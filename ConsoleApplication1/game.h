/**
 * @file game.h
 * @brief Biblioteka zawierająca funkcje obsługi gry.
 */

/*!
 * @file game.h
 * @brief Wszystko, czego potrzebujesz do gry, znajduje się tutaj: ekran, interfejs, funkcjonalność. 
 * Użytkownik może wybrać pustą komórkę, do której chce przypisać liczbę (jest ona podświetlona ramką), 
 * wybrać liczbę, którą chce dodać, naciskając klawiaturę numeryczną, która znajduje się pod siatką sudoku. 
 * Implementuje również utratę gry, jeśli gracz straci wszystkie życia 
 * Użytkownik może zakończyć grę lub wyjść do menu, a gra zostanie automatycznie zapisana. 
 */
#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
#include "functions.h"
#include "define.h"
#include "setting.h"
#include "gensudoku.h"
#include "sound.h"


#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#define SAVE_FILE "save.dat"

#ifndef GAME
#define GAME

// Define the size of the sudoku matrix
/**
 * @brief Tablica przechowująca planszę Sudoku.
 */
int matrix_area[9][9];

/**
 * @brief Rozmiar tablicy przechowującej planszę Sudoku.
 */
int size = LEN; // Size of this array_zero

/**
 * @brief Struktura reprezentująca przyciski numeryczne na klawiaturze.
 */
// Array to store the numpad buttons
struct NUMPAD numpad[9];

/**
 * @brief Lista przechowująca puste komórki planszy Sudoku.
 */
// Array to store empty cells
struct CELL* array_zero = NULL;


/**
 * @brief Rozmiar listy przechowującej puste komórki planszy Sudoku.
 */
int size_array_zero = 0;


 /**
 * @brief Funkcja zapisująca stan gry do pliku.
 *
 * @param gameState Struktura przechowująca stan gry.
 * @return true, jeśli zapis powiódł się, w przeciwnym razie false.
 */
// Function of saving game state
bool save_game(const struct GameState* gameState) {
    FILE* file = fopen(SAVE_FILE, "wb");
    if (!file) {
        fprintf(stderr, "Unable to open save file for writing.\n");
        return false;
    }
    // Write data to file
    if (fwrite(gameState, sizeof(struct GameState), 1, file) != 1) {
        fprintf(stderr, "Error writing to save file.\n");
        fclose(file);
        return false;
    }
    fclose(file);
    return true;
}

/**
 * @brief Funkcja wczytująca stan gry z pliku.
 *
 * @param gameState Struktura przechowująca stan gry.
 * @return true, jeśli wczytywanie powiodło się, w przeciwnym razie false.
 */
// Function of loading the game state
bool load_game(struct GameState* gameState) {
    FILE* file = fopen(SAVE_FILE, "rb");
    if (!file) {
        fprintf(stderr, "No save file found.\n");
        return false;
    }
    // Read data from the file
    if (fread(gameState, sizeof(struct GameState), 1, file) != 1) {
        fprintf(stderr, "Error reading from save file.\n");
        fclose(file);
        return false;
    }
    fclose(file);
    return true;
}


/**
 * @brief Funkcja wczytująca stan gry z pliku podczas powrotu z menu.
 *
 * @param gameState Struktura przechowująca stan gry.
 * @return true, jeśli wczytywanie powiodło się, w przeciwnym razie false.
 */
// Example of the function of loading a game when returning from the menu
bool load_game_on_menu(struct GameState* gameState) {
    if (load_game(gameState)) {
        fprintf(stderr, "Game loaded successfully.\n");
        return true;
    }
    else {
        fprintf(stderr, "Failed to load the game.\n");
        return false;
    }
}

/**
 * @brief Funkcja zapisująca stan gry do pliku podczas wyjścia do menu.
 *
 * @param gameState Struktura przechowująca stan gry.
 */
// Example of the function of saving the game when exiting to the menu
void save_game_on_menu(const struct GameState* gameState) {
    if (save_game(gameState)) {
        fprintf(stderr, "Game saved successfully.\n");
    }
    else {
        fprintf(stderr, "Failed to save the game.\n");
    }
}

/**
 * @brief Funkcja rysująca siatkę planszy Sudoku.
 */
// Function to draw the grid
void draw_area() {
    ALLEGRO_COLOR gridColor = al_map_rgb(0, 0, 0); // Grid color
    ALLEGRO_COLOR sectionColor = al_map_rgb(128, 128, 128); // Section color

    // Draw thin gray lines first
    for (int i = 0; i <= 9; ++i) {
        int x = i * (W / 10.9);
        int y = i * (H / 11.5);
        if (i % 3 != 0) {
            al_draw_line(x, 0, x, H / 1.28, sectionColor, 3.0);
            al_draw_line(0, y, W / 1.2129, y, sectionColor, 3.0);
        }
    }

    // Draw thick black lines
    for (int i = 0; i <= 9; i += 3) {
        int x = i * (W / 10.9);
        int y = i * (H / 11.5);
        al_draw_line(x, 0, x, H / 1.28, gridColor, 6.0);
        al_draw_line(0, y, W / 1.2129, y, gridColor, 6.0);
    }
}

/**
 * @brief Funkcja generująca przyciski numeryczne na klawiaturze.
 */
// Generate numpad buttons
void generation_numpad() {
    float width = 0;
    float height = 0;
    int k_x = 0;
    int k_y = 0;

    // Adjust button size based on window size
    if (resolution[0][0] == W) {
        width = W / 13;
        height = H / 13;
        k_x = 150;
        k_y = 130;
        
    }
    if (resolution[1][0] == W) {
        width = W / 12.5;
        height = H / 12.5;
        k_x = 160;
        k_y = 160;
    }
    if (resolution[2][0] == W) {
        width = W / 12;
        height = H / 12;
        k_x = 170;
        k_y = 180;
    }
    int j = 0, h = 1;
    struct NUMPAD temp;
    
    for (int i = 0; i < 9; i++) {
        if (j > 4) { h = 2; j = 1; }
        char path[50];
       
        snprintf(path, sizeof(path), "image/numbers/%d.png", i + 1);
        temp = (struct NUMPAD){ "", width, height, k_x + j * (width + 20), H - k_y / h, show, i + 1};
        strcpy(temp.name_file, path);
        numpad[i] = temp;
        j++;
    }
}

/**
 * @brief Funkcja weryfikująca sukces operacji alokacji dynamicznej pamięci.
 *
 * @param success Sukces operacji alokacji dynamicznej pamięci.
 */
// Function to verify the success of an operation
void verification(bool success) {
    if (!success) {
        fprintf(stderr, "Cannot allocate dynamic variable 'array'\n");
        exit(1);
    }
}

/**
 * @brief Funkcja alokująca pamięć dla tablicy komórek CELL.
 *
 * @param size Rozmiar tablicy komórek CELL.
 * @param array Wskaźnik na tablicę komórek CELL.
 * @return true, jeśli alokacja pamięci powiodła się, w przeciwnym razie false.
 */
// Allocate memory for CELL array
bool allocate_array(int size, struct CELL** array) {
    *array = (struct CELL*)realloc(*array, size * sizeof(struct CELL));
    return (*array != NULL);
}

/**
 * @brief Funkcja zwalniająca pamięć zaalokowaną dla tablicy komórek CELL.
 *
 * @param ptr Wskaźnik na tablicę komórek CELL.
 */
// Free memory allocated for CELL array
void free_ptr(struct CELL* ptr) {
    free(ptr);
    ptr = NULL;
}

/**
 * @brief Funkcja dodająca element do tablicy komórek CELL.
 *
 * @param array Wskaźnik na wskaźnik tablicy komórek CELL.
 * @param frame Komórka do dodania.
 * @param i Indeks, pod którym ma być dodany element.
 * @param size Rozmiar tablicy.
 */
// Add an element to the CELL array
void add_element(struct CELL** array, struct CELL frame, int i, int* size) {
    if (i >= *size) {
        *size += 10;
        bool success = allocate_array(*size, array);
        verification(success);
    }
    (*array)[i] = frame;
}

/**
 * @brief Funkcja sprawdzająca, czy gra została zakończona.
 *
 * @param count_hearts Liczba pozostałych żyć.
 * @param WIN Wskaźnik na zmienną informującą o wygranej.
 * @param LOSE Wskaźnik na zmienną informującą o przegranej.
 * @return true, jeśli gra została zakończona, w przeciwnym razie false.
 */
// Check if the game is finished
bool check_finish_game(int count_hearts, bool *WIN, bool *LOSE) {
    if (count_hearts <= 0) {
        *LOSE = true;
        return true;
    }

    if (size_array_zero <= 0) {
        *WIN = true;
        return true;
    }

    return false;
}


/**
 * @brief Funkcja rysująca cyfry na ekranie
 *
 * Przeszukujemy elementy macierzy sudoki i jeśli jest ona równa liczbie od 1 do 9, podświetlamy tę liczbę w siatce
 */
//Displaying numbers on the screen 
void matrix_validation()
{
    //Uploading images
    ALLEGRO_BITMAP* num1 = al_load_bitmap("image/numbers/1.png");
    if (!num1) exit(-3);
    ALLEGRO_BITMAP* num2 = al_load_bitmap("image/numbers/2.png");
    if (!num2) exit(-3);
    ALLEGRO_BITMAP* num3 = al_load_bitmap("image/numbers/3.png");
    if (!num3) exit(-3);
    ALLEGRO_BITMAP* num4 = al_load_bitmap("image/numbers/4.png");
    if (!num4) exit(-3);
    ALLEGRO_BITMAP* num5 = al_load_bitmap("image/numbers/5.png");
    if (!num5) exit(-3);
    ALLEGRO_BITMAP* num6 = al_load_bitmap("image/numbers/6.png");
    if (!num6) exit(-3);
    ALLEGRO_BITMAP* num7 = al_load_bitmap("image/numbers/7.png");
    if (!num7) exit(-3);
    ALLEGRO_BITMAP* num8 = al_load_bitmap("image/numbers/8.png");
    if (!num8) exit(-3);
    ALLEGRO_BITMAP* num9 = al_load_bitmap("image/numbers/9.png");
    if (!num9) exit(-3);


    float width = 0;       //Size of digits
    float height = 0;
    int x = 0, y = 0;       //Coordinates
    int k_x = 0, k_y = 0;   //Coefficient

    //Sizing for different screen extensions
    if (resolution[0][0] == W)
    {
        width = W / 13;
        height = H / 13;
        k_x = 10;
        k_y = 7;
    }
    if (resolution[1][0] == W)
    {
        width = W / 12.5;
        height = H / 12.5;
        k_x = 10;
        k_y = 5;
    }
    if (resolution[2][0] == W)
    {
        width = W / 12;
        height = H / 12;
        k_x = 7;
        k_y = 3;
    }

    //Drawing of numbers
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            // Calculate coordinates for each cell
            x = (j * (width + k_x)) + (W / 90);
            y = (i * (height + k_y)) + (H / 100);

            if (matrix_area[i][j] == 1)
                al_draw_scaled_bitmap(num1, 0, 0, al_get_bitmap_width(num1), al_get_bitmap_height(num1), x, y, width, height, 0);
            else if (matrix_area[i][j] == 2)
                al_draw_scaled_bitmap(num2, 0, 0, al_get_bitmap_width(num2), al_get_bitmap_height(num2), x, y, width, height, 0);
            else if (matrix_area[i][j] == 3)
                al_draw_scaled_bitmap(num3, 0, 0, al_get_bitmap_width(num3), al_get_bitmap_height(num3), x, y, width, height, 0);
            else if (matrix_area[i][j] == 4)
                al_draw_scaled_bitmap(num4, 0, 0, al_get_bitmap_width(num4), al_get_bitmap_height(num4), x, y, width, height, 0);
            else if (matrix_area[i][j] == 5)
                al_draw_scaled_bitmap(num5, 0, 0, al_get_bitmap_width(num5), al_get_bitmap_height(num5), x, y, width, height, 0);
            else if (matrix_area[i][j] == 6)
                al_draw_scaled_bitmap(num6, 0, 0, al_get_bitmap_width(num6), al_get_bitmap_height(num6), x, y, width, height, 0);
            else if (matrix_area[i][j] == 7)
                al_draw_scaled_bitmap(num7, 0, 0, al_get_bitmap_width(num7), al_get_bitmap_height(num7), x, y, width, height, 0);
            else if (matrix_area[i][j] == 8)
                al_draw_scaled_bitmap(num8, 0, 0, al_get_bitmap_width(num8), al_get_bitmap_height(num8), x, y, width, height, 0);
            else if (matrix_area[i][j] == 9)
                al_draw_scaled_bitmap(num9, 0, 0, al_get_bitmap_width(num9), al_get_bitmap_height(num9), x, y, width, height, 0);
        }
    }
    al_destroy_bitmap(num1);
    al_destroy_bitmap(num2);
    al_destroy_bitmap(num3);
    al_destroy_bitmap(num4);
    al_destroy_bitmap(num5);
    al_destroy_bitmap(num6);
    al_destroy_bitmap(num7);
    al_destroy_bitmap(num8);
    al_destroy_bitmap(num9);
}

/**
 * @brief Wypełnia tablicę array_zero pustymi komórkami planszy Sudoku.
 */
void fillArrayZero()
{
    float width = 0.0; // Cell width
    float height = 0.0; // Cell height
    float x = 0, y = 0; // Coordinates
    int idx = 0; // Element index in array_zero

    // Determine the cell size depending on the screen resolution
    if (resolution[0][0] == W)
    {
        width = W / 10.74;
        height = H / 11.61;
    }
    if (resolution[1][0] == W)
    {
        width = W / 10.9756;
        height = H / 11.53846;
    }
    if (resolution[2][0] == W)
    {
        width = W / 10.752688;
        height = H / 11.6279;
    }

    // Draw a frame for the selected cell
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            x = j * width + (W / 90);
            y = i * height + (H / 100);
            if (matrix_area[i][j] == 0) {
                // Define the coordinates and dimensions of the frame around the selected cell
                float frame_x = j * (W / 10.9);
                float frame_y = i * (H / 11.5);
                float frame_width = width;
                float frame_height = height;
                // Draw a frame for the selected cell
                struct CELL frame = (struct CELL){ "image/numbers/frame.png", frame_width, frame_height, frame_x, frame_y, show, i, j };
                add_element(&array_zero, frame, idx, &size);
                idx++;
                size_array_zero++;
               
;           }
        }
    }
}

/**
 * @brief Zapisuje stan gry.
 *
 * @param count_clue Liczba dostępnych wskazówek.
 * @param count_heart Liczba dostępnych żyć.
 * @param minutes Liczba minut na zegarze gry.
 * @param seconds Liczba sekund na zegarze gry.
 * @param time_count Całkowity czas gry.
 */
//Function for saving the game
void saveGame(int count_clue, int count_heart, unsigned int minutes, unsigned int seconds, unsigned int time_count){
    struct GameState gameState;
    memcpy(gameState.matrix_area, matrix_area, sizeof(matrix_area));
    gameState.hints = count_clue;
    gameState.hearts = count_heart;
    gameState.minutes = minutes;
    gameState.seconds = seconds;
    gameState.time_count = time_count;

    if (save_game(&gameState)) {
        fprintf(stderr, "Game saved successfully.\n");
    }
    else {
        fprintf(stderr, "Failed to save the game.\n");
    }
}

/**
 * @brief Funkcja obsługująca proces gry, czyli Sudoku.
 * Posiada pętlę gry, rysuje tło, przyciski, scenę gry, numpad, sprawdza zdarzenia takie jak: 
 * zamknięcie okna oraz naciśnięcie przycisku. Jeśli ustawienia zostaną
 * zaakceptowane, zmiana rozmiaru okna oraz aktualizowanie pozycji i rozmiarów przycisków.

 * @param display Wskaźnik na ekran Allegro.
 * @param event_queue Kolejka zdarzeń Allegro.
 * @param contGame Flaga kontynuacji gry.
 * @return Wartość logiczna określająca, czy gra została zakończona.
 */
//Game process (Sudoku itself) parameters (screen, events)
bool game(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* event_queue, bool contGame)
{
    // Variable to store events
    ALLEGRO_EVENT event;            

    // Load font
    int fontSize = 0;
    if (W == resolution[0][0])
        fontSize = 25;
    else if (W == resolution[1][0])
        fontSize = 30;
    else if (W == resolution[2][0])
        fontSize = 34;

    ALLEGRO_FONT* font = al_load_ttf_font("Tenada.ttf", fontSize, 0);
    if (!font) exit(-2);

    // Load background
    ALLEGRO_BITMAP* background = al_load_bitmap("image/background.png");
    if (!background) exit(-3);

    //Load game result frame
    ALLEGRO_BITMAP* box_result = al_load_bitmap("image/result_box.png");

    // Create an ALLEGRO_TIMER object with the desired frame rate
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

    //Create a game timer
    ALLEGRO_TIMER* timeGame = al_create_timer(1);

    // Register the timer's event source with the event queue
    // This allows the event queue to receive timer-related events
    al_register_event_source(event_queue, al_get_timer_event_source(timer));    
    al_register_event_source(event_queue, al_get_timer_event_source(timeGame)); //for the stopwatch

    //Creating a hint button
    struct OBJECT clue = (struct OBJECT){ "image/clue.png", W / 8, H / 8, 0,0, show };
    clue.x = W - clue.width - 10;
    clue.y = H - clue.height - 25;

    //Creating a customize button
    struct OBJECT b_setting = (struct OBJECT){ "image/icon_setting.png", W / 20, H / 20, 0, 0, show };
    b_setting.x = W - b_setting.width - 5;
    b_setting.y = 5;

    //Creating an exit button
    struct OBJECT b_exit = (struct OBJECT){ "image/exit.png", W / 20, H / 20, 0, 0, show };
    b_exit.x = b_setting.x - b_exit.width - 20;
    b_exit.y = b_setting.y;

    //Creating an exit button in result box
    struct OBJECT b_exitBox = (struct OBJECT){ "image/exit.png", W / 18, H / 18, 0, 0, show };
    b_exitBox.x = 3 * W / 4.55;
    b_exitBox.y = H / 2.1;

    //Creating an restart button in result box
    struct OBJECT b_restart = (struct OBJECT){ "image/restart.png", W / 12, H / 12, 0, 0, show };
    b_restart.x = W / 3.5;
    b_restart.y = H / 2.15;

    const int total_clue = 3;           //Total hints (static value)
    int count_clue = total_clue;        //Number of hints
    char count_clue_str[10];            //For conversion from int to char (string)


    int count_heart = 3;                //All hearts
    int heart_width = W / 9;            //The size of the hearts
    int heart_height = H / 9;

    //Hearts
    struct OBJECT hearts[3];
    for (int i = 0; i < count_heart; i++)
    {
        hearts[i] = (struct OBJECT){ "image/heart.png", heart_width, heart_height, W - heart_width - 20, 70 + (i * heart_height + 25), show };  //Сердца в массив закидываем
    }

    // Generate numpad buttons
    generation_numpad();

    int idx = 0;                    //Index of the selected empty cell
    bool show_frame = false;        //Whether to display a frame
    bool change_set = false;        //Change window extension

    bool gameEnd = false;       //End-of-game flag
    bool gameWIN = false;       //Victory flag
    bool gameLOSE = false;      //Lose flag

    char timeResult[50];            //Stopwatch Text
    unsigned int time_count = 0;    //Time
    unsigned int minutes=0;
    unsigned int seconds=0;

    //If the continue button in the menu has been pressed
    if (contGame) {
        // Using the download function
        struct GameState loadedGameState;
        if (load_game(&loadedGameState)) {
            fprintf(stderr, "Game loaded successfully.\n");
            // Use loadedGameState to restore the game state
        }
        else {
            fprintf(stderr, "Failed to load the game.\n");
        }

        memcpy(matrix_area, loadedGameState.matrix_area, sizeof(matrix_area));
        count_clue = loadedGameState.hints;
        count_heart = loadedGameState.hearts;
        minutes = loadedGameState.minutes;
        seconds = loadedGameState.seconds;
        time_count = loadedGameState.time_count;
    }

    if (!contGame) {
        sudmatrix(matrix_area, difficult);
    }
    array_zero = (struct CELL*)malloc(size * sizeof(struct CELL));
    size_array_zero = 0;
    fillArrayZero();

    bool finish = false;
    al_start_timer(timer);
    al_start_timer(timeGame);
    while (!finish)
    {
        
        al_wait_for_event(event_queue, &event);
        gameEnd = check_finish_game(count_heart, &gameWIN, &gameLOSE);  //Checking the result of the game
        check_song_finished();                                          //Check that the background music is over

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)                  //Check if the window is closed
        {
            finish = true;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)              //Check for mouse button release
        {
            if (event.mouse.button & 1)                                  // pressing the LBM (1 - LBM, 2 - RBM, 3 - Wheel)
            {
                int x = event.mouse.x;                              //Get mouse coordinate
                int y = event.mouse.y;

                //Checking that the game isn't over yet
                if (gameEnd == false) {
                    //Press the tooltip button
                    if (count_clue > 0 && count_clue <= 3)
                    {
                        if (x >= clue.x && x < (clue.x + clue.width) && y >= clue.y && y < (clue.y + clue.height))    //Check if the mouse coordinates coincide with the button (if the button is pressed).
                        {
                            int idx_clue = rand() % size_array_zero;
                            int row = array_zero[idx_clue].rows;
                            int column = array_zero[idx_clue].column;

                            checkSudokuHints(matrix_area, row, column);
                            remove_element(&array_zero, size_array_zero, idx_clue);
                            size_array_zero -= 1;
                            count_clue--;
                        }
                    }

                    //Press the settings button
                    if (x >= b_setting.x && x < (b_setting.x + b_setting.width) && y >= b_setting.y && y < (b_setting.y + b_setting.height))
                    {
                        change_set = setting(display, event_queue, timer, &finish);
                    }

                    //Press the exit button
                    if (x >= b_exit.x && x < (b_exit.x + b_exit.width) && y >= b_exit.y && y < (b_exit.y + b_exit.height))
                    {
                        saveGame(count_clue, count_heart, minutes, seconds, time_count);
                        if (array_zero != NULL) {
                            free_ptr(array_zero);
                        }
                        al_stop_timer(timer);
                        al_stop_timer(timeGame);
                        al_destroy_timer(timer);
                        al_destroy_timer(timeGame);
                        al_destroy_font(font);
                        al_destroy_bitmap(box_result);
                        al_destroy_bitmap(background);
                        return false;
                    }

                    //Add a number to an empty cell
                    for (int i = 0; i < 9; i++)
                    {
                        if (x >= numpad[i].x && x < (numpad[i].x + numpad[i].width) && y >= numpad[i].y && y < (numpad[i].y + numpad[i].height))
                        {
                            if (show_frame)
                            {
                                //Get the indices of the empty cell in the matrix_area
                                int row = array_zero[idx].rows;
                                int column = array_zero[idx].column;
                                int num = numpad[i].num;        //Number pressed on numpad
                                matrix_area[row][column] = num;
                                if (checkSudoku(matrix_area, row, column)) {
                                    remove_element(&array_zero, size_array_zero, idx); //Delete empty cell from array_zero
                                    size_array_zero -= 1;
                                }
                                else {
                                    count_heart--;
                                    matrix_area[row][column] = 0;
                                }

                                show_frame = false;
                            }
                        }
                    }

                    //Check if an empty cell has been selected
                    for (int i = 0; i < size_array_zero; i++)
                    {
                        if (x >= array_zero[i].x && x < (array_zero[i].x + array_zero[i].width) && y >= array_zero[i].y && y < (array_zero[i].y + array_zero[i].height))
                        {
                            idx = i;
                            show_frame = true;
                        }
                    }
                }
                else
                {
                    //Check if the restart button is pressed
                    if (x >= b_restart.x && x < (b_restart.x + b_restart.width) && y >= b_restart.y && y < (b_restart.y + b_restart.height))
                    {
                        //Resetting everything
                        count_clue = 3;
                        count_heart = 3;
                        for (int i = 0; i < count_heart; i++)
                        {
                            hearts[i] = (struct OBJECT){ "image/heart.png", heart_width, heart_height, W - heart_width - 20, 70 + (i * heart_height + 25), show };
                        }
                        idx = 0;
                        size_array_zero = 0;
                        sudmatrix(matrix_area, difficult);
                        if (array_zero != NULL) {
                            free_ptr(array_zero);
                        }
                        array_zero = (struct CELL*)malloc(size * sizeof(struct CELL));
                        fillArrayZero();
                        gameEnd = false;
                        gameWIN = false;
                        gameLOSE = false;
                    }

                    //Press the exit button
                    if (x >= b_exitBox.x && x < (b_exitBox.x + b_exitBox.width) && y >= b_exitBox.y && y < (b_exitBox.y + b_exitBox.height))
                    {
                        if (array_zero != NULL) {
                            free_ptr(array_zero);
                        }
                        al_stop_timer(timer);
                        al_stop_timer(timeGame);
                        al_destroy_timer(timer);
                        al_destroy_timer(timeGame);
                        al_destroy_font(font);
                        al_destroy_bitmap(box_result);
                        al_destroy_bitmap(background);
                        if(remove("save.dat") == 0)
                            fprintf(stderr, "Deleted successfully");
                        else
                            fprintf(stderr, "Unable to delete the file");

                        return false;
                    }
                } 
            }
        }

        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            if (event.timer.source == timer) {
                sprintf(count_clue_str, "%d/%d", count_clue, total_clue);          //Convert from int to char (string)                                                              
                al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 0, 0, W, H, 0);      //Draw background
                al_draw_text(font, al_map_rgb(0, 0, 0), clue.x + 20, clue.y - 50, 0, count_clue_str);                                           //Draw text (number of prompts)
                clue.show(clue.name_file, clue.width, clue.height, clue.x, clue.y);                                                             //Show tooltip button
                b_setting.show(b_setting.name_file, b_setting.width, b_setting.height, b_setting.x, b_setting.y);                               //Display the settings button
                b_exit.show(b_exit.name_file, b_exit.width, b_exit.height, b_exit.x, b_exit.y);                                                 //Show exit button
                //Show exit button in result box

                //Drawing of stopwatch
                sprintf(timeResult, "%02d:%02d", minutes, seconds);
                al_draw_text(font, al_map_rgb(0, 0, 0), (W - al_get_text_width(font, timeResult)) - 20, hearts[2].y + 150, 0, timeResult);

                matrix_validation();                                        //Drawing numbers in the Sudoku 
                draw_area();                                                //Mesh drawing
                show_heart(hearts, count_heart);                            //Show hearts
                draw_numpad(numpad);                                        //Display numpad

                //If an empty cell is selected, display a frame
                if (show_frame)
                    array_zero[idx].show(array_zero[idx].name_file, array_zero[idx].width, array_zero[idx].height, array_zero[idx].x, array_zero[idx].y);

                //If the game is over
                if (gameEnd) {
                    //Drawing the result window
                    al_draw_scaled_bitmap(box_result, 0, 0, al_get_bitmap_width(box_result), al_get_bitmap_height(box_result), W / 4, H / 4, W / 2, H / 3, 0);

                    //Displaying the exit and restart button
                    b_restart.show(b_restart.name_file, b_restart.width, b_restart.height, b_restart.x, b_restart.y);
                    b_exitBox.show(b_exitBox.name_file, b_exitBox.width, b_exitBox.height, b_exitBox.x, b_exitBox.y);
                    if (gameWIN) {
                        //Let's write out that the player won
                        al_draw_text(font, al_map_rgb(0, 0, 0), (W - al_get_text_width(font, "You Win")) / 2, 3 * H / 10, 0, "You Win");
                    }
                    if (gameLOSE) {
                        //Write out that the player lost
                        al_draw_text(font, al_map_rgb(0, 0, 0), (W - al_get_text_width(font, "You Lose")) / 2, 3 * H / 10, 0, "You Lose");
                    }
                    sprintf(timeResult, "Your time: %02d:%02d", minutes, seconds );
                    al_draw_text(font, al_map_rgb(0, 0, 0), (W - al_get_text_width(font, timeResult)) / 2, H / 2.5, 0, timeResult); //Drawing of stopwatch in the box result

                }
                al_flip_display();
            }
            if (event.timer.source == timeGame) {
                if (!gameEnd) {
                    //Time calculation
                    time_count++;
                    minutes = time_count / 60;
                    seconds = time_count % 60;
                    
                }
            }
        }

        //If the settings are accepted, adjust everything to fit the window size
        if (change_set == true)
        {
            clue.width = W / 8;
            clue.height = H / 8;
            clue.x = W - clue.width - 10;
            clue.y = H - clue.height - 25;

            b_setting.width = W / 20;
            b_setting.height = H / 20;
            b_setting.x = W - b_setting.width - 5;
            b_setting.y = 5;

            b_exit.width = W / 20;
            b_exit.height = H / 20;
            b_exit.x = b_setting.x - b_exit.width - 20;
            b_exit.y = b_setting.y;

            int heart_width = W / 9;
            int heart_height = H / 9;
            for (int i = 0; i < count_heart; i++)
            {
                hearts[i].width = heart_width;
                hearts[i].height = heart_width;
                hearts[i].x = W - heart_width - 20;
                hearts[i].y = 70 + (i * heart_height + 25);
            }

            b_restart.width = W / 12;
            b_restart.height = H / 12;
            b_restart.x = W / 3.5;
            b_restart.y = H / 2.15;

            b_exitBox.width = W / 18; 
            b_exitBox.height = H / 18;
            b_exitBox.x = 3 * W / 4.55;
            b_exitBox.y = H / 2.1;

            if (W == resolution[0][0])
                fontSize = 25;
            else if (W == resolution[1][0])
                fontSize = 30;
            else if (W == resolution[2][0])
                fontSize = 34;
            al_destroy_font(font);
            font = al_load_ttf_font("Tenada.ttf", fontSize, 0);

            generation_numpad();
            size_array_zero = 0;
            if (array_zero != NULL) {
                free_ptr(array_zero);
            }
            array_zero = (struct CELL*)malloc(size * sizeof(struct CELL));
            fillArrayZero();
            change_set = false;

        }

    }
    saveGame(count_clue, count_heart, minutes, seconds, time_count);
    if (array_zero != NULL) {
        free_ptr(array_zero);
    }
    al_stop_timer(timer);
    al_stop_timer(timeGame);
    al_destroy_timer(timer);
    al_destroy_timer(timeGame);
    al_destroy_font(font);
    al_destroy_bitmap(box_result);
    al_destroy_bitmap(background);

    return true;
}

#endif
