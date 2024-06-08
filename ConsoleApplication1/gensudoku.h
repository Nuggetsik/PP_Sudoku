/**
 * @file sudoku.h
 * @brief Biblioteka implementująca funkcje do generowania i rozwiązywania Sudoku.
 */
#include <time.h>
#include <stdbool.h>
#define N 9

#ifndef SUDOKU
#define SUDOKU
#define SWAP(a, b) { int temp = a; a = b; b = temp; }
//----------------------------------------------------------------------------
/**
 * @brief Struktura przechowująca pojedynczy element listy jednokierunkowej.
 */
struct glif {
    char data[3][3]; /**< Dwuwymiarowa tablica przechowująca wartości elementu. */
    struct glif* next; /**< Wskaźnik na następny element listy. */
};


/**
 * @brief Funkcja tworząca nowy element listy na podstawie przekazanych danych.
 *
 * @param data Dwuwymiarowa tablica danych do umieszczenia w elemencie.
 * @return Wskaźnik na utworzony element listy.
 */
static struct glif* create_list(char data[3][3]) {
    struct glif* front = (struct glif*) malloc(sizeof(struct glif));
    if (NULL != front) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                front->data[i][j] = data[i][j];

            }
        }
        front->next = NULL;
    }
    return front;
}


/**
 * @brief Funkcja dodająca nowy element na początek listy.
 *
 * @param front Wskaźnik na głowę listy.
 * @param data Dwuwymiarowa tablica danych do umieszczenia w elemencie.
 * @return Wskaźnik na głowę listy po dodaniu nowego elementu.
 */
static struct glif* insert_glif(struct glif* front, char data[3][3]) {
    if (NULL == front)
        return NULL;

    struct glif* new_glif = (struct glif*) malloc(sizeof(struct glif));
    if (NULL != new_glif) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                new_glif->data[i][j] = data[i][j];
            }
        }
        new_glif->next = front;
            return new_glif;


    }
    return front;
}

/**
 * @brief Funkcja zwracająca wskaźnik na element listy na podstawie numeru indeksu.
 *
 * @param head Wskaźnik na głowę listy.
 * @param n Numer indeksu elementu do znalezienia.
 * @return Wskaźnik na element listy o podanym indeksie lub NULL, jeśli nie istnieje.
 */
static struct glif* dostemp_struct(struct glif* head, int n) {
    if (head == NULL) {
        return NULL;
    }
    if (n <= 0) return head;

    struct glif* temp = head;
    for (int i = 1; i < n; i++) {
        if (temp == NULL) {
            return NULL;
        }
        temp = temp->next;
    }
    return temp;
}

/**
 * @brief Funkcja zwalniająca pamięć zajętą przez listę.
 *
 * @param head Wskaźnik na głowę listy do zwolnienia.
 */
static void free_list(struct glif* head) {
    struct glif* current = head;
    struct glif* next = NULL;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}


/*
void print_struct(struct glif* node, int index) {
    printf("glif %d:\n", index);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            printf("%d ", node->data[i][j]-'0');
        }
        printf("\n");
    }
    printf("\n");
}

void print_glifs(struct glif* head, int index ) {
    if (head == NULL) {
        return;
    }


    print_glifs(head->next, index - 1);
    print_struct(head, index);
}
*/

/**
 * @brief Modifies the content of a node in the glif structure based on the specified difficulty level.
 * 
 * This function randomly selects a specified number of fields in a 3x3 matrix in the node and sets them to '0', indicating no number in that field.
 * 
 * @param node A pointer to a node in the glif structure.
 * @param dif An array containing information about the difficulty level. dif[0] specifies how many fields will have their numbers removed from the 3x3 matrix, and dif[1] specifies the number of removed fields.
 */
static void dif1(struct glif* node,int dif[2]) {
    int i1=dif[1];

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j){

            if((rand()%dif[0]) && i1!=0){
            node->data[i][j]='0';
            i1--;
            }
        }
    }
}

/**
 * @brief Recursively modifies the content of the nodes in the glif structure based on the specified difficulty level.
 * 
 * This function traverses the glif structure recursively and calls dif1 to modify the content of each node.
 * 
 * @param head A pointer to the head node of the glif structure.
 * @param dif An array containing information about the difficulty level. dif[0] specifies how many fields will have their numbers removed from the 3x3 matrix, and dif[1] specifies the number of removed fields.
 */
static void difrec(struct glif* head,int dif[2]) {
    srand(time(NULL));
    if (head == NULL) {
        return;
    }


    difrec(head->next,dif);
    dif1(head,dif);
}

//----------------------------------------------------------------------------------------------


static void set_rand_matrix(char array[3][3]) {
    char temp;
    int i, j, k, l;

    srand(time(NULL));

    for (int n = 0; n < 100; n++) {
        i = rand() % 3;
        j = rand() % 3;
        k = rand() % 3;
        l = rand() % 3;


        temp = array[i][j];
        array[i][j] = array[k][l];
        array[k][l] = temp;
    }
}


/**
 * @brief Sets the positions of elements in the specified array randomly.
 * 
 * This function shuffles the positions of elements in the array randomly using the Fisher-Yates shuffle algorithm.
 * 
 * @param array The array to shuffle.
 * @param size The size of the array.
 */
static void set_rand_poz(char *array, int size) {
    int temp, j;
    srand(time(NULL));
    for (int i = size - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

/**
 * @brief Swaps the elements at the specified positions in a 3x3 matrix horizontally.
 * 
 * This function swaps the elements at the specified positions in a 3x3 matrix horizontally.
 * 
 * @param matrix The 3x3 matrix.
 * @param i The index of the first row.
 * @param i2 The index of the second row.
 */
static void swap_pozX(char matrix[][3],int i,int i2)
{
    char temp;
    for (int j = 0; j < 3; j++) {
        temp = matrix[i][j];
        matrix[i][j] = matrix[i2][j];
        matrix[i2][j] = temp;
    }
}


/**
 * @brief Swaps the elements at the specified positions in a 3x3 matrix vertically.
 * 
 * This function swaps the elements at the specified positions in a 3x3 matrix vertically.
 * 
 * @param matrix The 3x3 matrix.
 * @param i The index of the first column.
 * @param i2 The index of the second column.
 */
static void swap_pozY(char matrix[][3], int i, int i2)
{
    char temp;
    for (int j = 0; j < 3; j++) {
        temp = matrix[j][i];
        matrix[j][i] = matrix[j][i2];
        matrix[j][i2] = temp;
    }
}

/**
 * @brief Fills the structure with permutations of a 3x3 matrix by swapping rows.
 * 
 * This function fills the structure with permutations of a 3x3 matrix by swapping rows.
 * 
 * @param first_array The original 3x3 matrix.
 * @param head A pointer to the head node of the glif structure.
 * @return A pointer to the head node of the modified glif structure.
 */
static struct glif* fill_structX(char first_array[][3],struct glif* head) {
     char (*temp)[3] = malloc(3 * sizeof(char[3]));
    if (temp == NULL) {
        printf("Memory allocation failed");
        return NULL ;
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            temp[i][j] = first_array[i][j];
        }
    }
    
    swap_pozX(temp,0,1);swap_pozX(temp,0,2);head = insert_glif(head,temp);
    swap_pozX(temp,0,1);swap_pozX(temp,0,2);head = insert_glif(head,temp);


    free(temp);
    return head;
}

/**
 * @brief Fills the structure with permutations of a 3x3 matrix by swapping columns.
 * 
 * This function fills the structure with permutations of a 3x3 matrix by swapping columns.
 * 
 * @param first_array The original 3x3 matrix.
 * @param head A pointer to the head node of the glif structure.
 * @param temp A temporary 3x3 matrix.
 * @return A pointer to the head node of the modified glif structure.
 */
static struct glif* fill_structY(char first_array[][3],struct glif* head,char temp[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            temp[i][j] = first_array[i][j];
        }
    }

    swap_pozY(temp,0,1);swap_pozY(temp,0,2);head = insert_glif(head,temp);



    return head;
}

/**
 * @brief Funkcja generująca Sudoku.
 *
 * @return Wskaźnik na głowę listy reprezentującej wygenerowane Sudoku.
 */
static struct glif* gen_sudoku()
{
    struct glif* head=NULL;

    char first_poz[3][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'}
        };
    char temp[3][3];
    set_rand_matrix(first_poz);
    head=create_list(first_poz);


    head = fill_structX(first_poz,head);


    head = fill_structY(first_poz,head,temp);
    head = fill_structX(temp,head);


    head = fill_structY(temp,head,temp);
    head = fill_structX(temp,head);

/*    head = diffecult(head,5);*/
    //difrec(head);
    return head;


}

/**
 * @brief Generuje planszę Sudoku o określonym stopniu trudności.
 * 
 * Funkcja ta generuje planszę Sudoku o określonym stopniu trudności.
 * Plansza jest generowana na podstawie gotowej struktury danych `glif`,
 * która reprezentuje wzory plansz Sudoku.
 * 
 * @param arr Tablica dwuwymiarowa, do której zostanie zapisana wygenerowana plansza Sudoku.
 * @param difficult Tablica dwuelementowa zawierająca informacje o stopniu trudności generowanej planszy.
 * 
 * @return Brak.
 */
void sudmatrix(/*int (*arr2D)[9],*/int (*arr)[9], const int difficult[2])
{
    
    struct glif* temp = NULL;
    struct glif* head = gen_sudoku();
    /*
    for (int i = 0; i < 9; i++) {
        temp = dostemp_struct(head, i + 1);
        for (int j = 0; j < 9; j++) {
            if (j < 3) {
                arr2D[i][j] = temp->data[0][j] - '0';
            }
            else if (j < 6) {
                arr2D[i][j] = temp->data[1][j - 3] - '0';
            }
            else {
                arr2D[i][j] = temp->data[2][j - 6] - '0';
            }
        }
    }
*/
    
    difrec(head,difficult);
    int kosc[9] = { 1,2,3,4,5,6,7,8,9, };



    srand(time(NULL));
    /*
    for (int i = 8; i > 0; i--) {
        int j = rand() % (i + 1);
        SWAP(kosc[i], kosc[j]);
    }
    */

    
    for (int i = 0; i < 9; i++) {
        temp = dostemp_struct(head, kosc[i]);
        for (int j = 0; j < 9; j++) {
            if (j < 3) {
                arr[i][j] = temp->data[0][j] - '0';
            }
            else if (j < 6) {
                arr[i][j] = temp->data[1][j - 3] - '0';
            }
            else {
                arr[i][j] = temp->data[2][j - 6] - '0';
            }
        }
    }

    free_list(head);

}
/**
 * @brief Funkcja sprawdzająca, czy dana wartość jest unikalna w kolumnie Sudoku.
 *
 * @param arr Tablica dwuwymiarowa reprezentująca planszę Sudoku.
 * @param x Indeks wiersza.
 * @param y Indeks kolumny.
 * @return true, jeśli wartość jest unikalna w kolumnie, w przeciwnym razie false.
 */
static bool is_col(const int arr[9][9],const int x,const int y)
{
    int temp=arr[x][y];
    for(int i=0;i<9;i++)
    {
        if(i==y)
            continue;
        if (temp == arr[x][i])
            return false;
        
    }
    return true;
}

/**
 * @brief Funkcja sprawdzająca, czy dana wartość jest unikalna w wierszu Sudoku.
 *
 * @param arr Tablica dwuwymiarowa reprezentująca planszę Sudoku.
 * @param x Indeks wiersza.
 * @param y Indeks kolumny.
 * @return true, jeśli wartość jest unikalna w wierszu, w przeciwnym razie false.
 */
static bool is_row(const int arr[9][9],const int x,const int y)
{
    int temp=arr[x][y];
    for(int i=0;i<9;i++)
    {
        if(i==x)
            continue;
        if (temp == arr[i][y])
            return false;
        
    }
    
    return true;
}

/**
 * @brief Sprawdza, czy liczba w danej pozycji jest poprawna w bloku 3x3.
 *
 * Funkcja ta sprawdza, czy liczba znajdująca się w danej pozycji planszy Sudoku
 * jest poprawna w swoim bloku 3x3. Blok 3x3 jest definiowany przez współrzędne x i y,
 * które są podzielne przez 3.
 * 
 * @param arr Tablica dwuwymiarowa reprezentująca planszę Sudoku.
 * @param x Współrzędna x (numer wiersza) pozycji, którą należy sprawdzić.
 * @param y Współrzędna y (numer kolumny) pozycji, którą należy sprawdzić.
 * 
 * @return true, jeśli liczba jest poprawna w swoim bloku 3x3; false w przeciwnym razie.
 */
static bool is_poz(const int arr[9][9], const short int x, const short int y)
{
    int px, py;
    int temp = arr[x][y];
    px = x % 3;
    py = y % 3;
    for (int i = px; i < 9; i += 3)//x->3
    {
        for (int j = py; j < 9; j += 3)//y->3
        {
            if (x == i && y == j)
                continue;
            if (temp == arr[i][j]) 
                return false;
            
        }
    }
    return true;
}

/**
 * @brief Funkcja sprawdzająca, czy dana wartość jest unikalna w kwadracie Sudoku.
 *
 * @param arr Tablica dwuwymiarowa reprezentująca planszę Sudoku.
 * @param x Indeks wiersza.
 * @param y Indeks kolumny.
 * @return true, jeśli wartość jest unikalna w kwadracie, w przeciwnym razie false.
 */
static bool is_square(const int arr[9][9], const short int x, const short int y)
{

    int px, py;
    int temp = arr[x][y];
    px = x - x % 3;
    py = y - y % 3;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            
            if ((px + i) == x && (py + j) == y) break;
            if (temp == arr[px + i][py + j]) 
                return false;
            
        }
    }
    return true;

}

/**
 * @brief Funkcja sprawdzająca, czy dana wartość jest unikalna w podanej komórce Sudoku.
 *
 * @param arr Tablica dwuwymiarowa reprezentująca planszę Sudoku.
 * @param x Indeks wiersza.
 * @param y Indeks kolumny.
 * @return true, jeśli wartość jest unikalna w podanej komórce, w przeciwnym razie false.
 */
bool is_true(const int arr[9][9],const short int x,const short int y)//dla wykorzystania wpisujesz tablice i nachodzenia o takich indeksach w tablice wybranej komurki
{
    if (is_row(arr, x, y) && is_col(arr, x, y) && is_square(arr, x, y) && is_poz(arr, x, y))
        return true;
    return false;

}

/**
 * @brief Znajduje pierwszą pustą komórkę na planszy Sudoku.
 *
 * Ta funkcja przeszukuje planszę Sudoku w poszukiwaniu pierwszej pustej komórki.
 * Jeśli zostanie znaleziona pusta komórka, funkcja zwraca true i aktualizuje wartości
 * wskaźników row i col na numer wiersza i kolumny znalezionej pustej komórki.
 * 
 * @param grid Tablica dwuwymiarowa reprezentująca planszę Sudoku.
 * @param row Wskaźnik na zmienną przechowującą numer wiersza znalezionej pustej komórki.
 * @param col Wskaźnik na zmienną przechowującą numer kolumny znalezionej pustej komórki.
 * 
 * @return true, jeśli znaleziono pustą komórkę; false w przeciwnym razie.
 */
bool findEmptyCell(int grid[N][N], int* row, int* col) {
    for (*row = 0; *row < N; (*row)++) {
        for (*col = 0; *col < N; (*col)++) {
            if (grid[*row][*col] == 0) {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Funkcja sprawdzająca, czy plansza Sudoku została rozwiązana.
 *
 * @param grid Tablica dwuwymiarowa reprezentująca planszę Sudoku.
 * @return true, jeśli plansza została rozwiązana, w przeciwnym razie false.
 */
bool solveSudoku(int grid[N][N]) {
    int row, col;


    if (!findEmptyCell(grid, &row, &col)) {
        return true;
    }

    
    for (int num = 1; num <= N; num++) {
        grid[row][col] = num;
        if (is_true(grid, row, col)) {
          
            if (solveSudoku(grid)) {
                return true;
            }
        }
       
        grid[row][col] = 0;
    }
    return false;
}

/**
 * @brief Funkcja sprawdzająca możliwe wskazówki dla planszy Sudoku.
 *
 * @param grid Tablica dwuwymiarowa reprezentująca planszę Sudoku.
 * @param rowCell Indeks wiersza komórki do sprawdzenia.
 * @param colCell Indeks kolumny komórki do sprawdzenia.
 */
void checkSudokuHints(int grid[N][N], int rowCell, int  colCell) {
    int gridMask[N][N];
    for(int i=0;i<9;i++)
        for (int j = 0; j < 9; j++)
        gridMask[i][j] = grid[i][j];
    solveSudoku(gridMask);
    grid[rowCell][colCell] = gridMask[rowCell][colCell];
}

/**
 * @brief Funkcja sprawdzająca, czy plansza Sudoku może być rozwiązana.
 *
 * @param grid Tablica dwuwymiarowa reprezentująca planszę Sudoku.
 * @param rowCell Indeks wiersza komórki do sprawdzenia.
 * @param colCell Indeks kolumny komórki do sprawdzenia.
 * @return true, jeśli plansza może być rozwiązana, w przeciwnym razie false.
 */
bool checkSudoku(int grid[N][N], int rowCell, int  colCell) {
    int gridMask[N][N];
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            gridMask[i][j] = grid[i][j];
    return solveSudoku(gridMask);
}


#endif


