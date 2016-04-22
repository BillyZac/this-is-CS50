/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// Selected title indicies
int tileRow;
int tileCol;

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
bool getTileIndex(int tile);
bool inBounds(int row, int col);
bool isBlank(int row, int col);
void swap(int targetRow, int targetCol, int tileRow, int tileCol, int tileValue);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
        }

        // sleep thread for animation's sake
        usleep(50000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // Fill all spaces with zeroes, including the ones we won't use
    for (int i = 0; i < DIM_MAX; i++)
    {
        for (int j = 0; j < DIM_MAX; j++)
        {
            board[i][j] = 0;
        }
    }
    
    
    int counter = d * d - 1;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board[i][j] = counter;
            counter--;
        }
    }
    
    // If there are an odd number of tiles, swap the last two tiles
    if (d%2 == 0)
    {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }
    
    // Make the last tile a blank space.
    board[d-1][d-1] = 0;
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for (int j = 0; j < d; j++)
        {
            printf("-----");
        }
    printf("\n");   
        
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            int currentSymbol = board[i][j];
            if (currentSymbol == 0)
                printf(" __");
            else
                printf("%3d", board[i][j]);
            if (j != d - 1)
                printf(" |");
        }
        
        printf("\n");   
        
        for (int j = 0; j < d; j++)
        {
            printf("-----");
        }
        printf("\n");   
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    bool tileFound = getTileIndex(tile);
    if (tileFound)
    {
        int targetRow;
        int targetCol;
        
        // CHECK RIGHT
        targetRow = tileRow;
        targetCol = tileCol + 1;
        if (inBounds(targetRow, targetCol) && isBlank(targetRow, targetCol))
        {
            swap(targetRow, targetCol, tileRow, tileCol, tile);
            return true;    
        }
        
        // CHECK LEFT
        targetRow = tileRow;
        targetCol = tileCol - 1;
        if (inBounds(targetRow, targetCol) && isBlank(targetRow, targetCol))
        {
            swap(targetRow, targetCol, tileRow, tileCol, tile);
            return true;    
        }
        
        // CHECK ABOVE
        targetRow = tileRow - 1;
        targetCol = tileCol;
        if (inBounds(targetRow, targetCol) && isBlank(targetRow, targetCol))
        {
            swap(targetRow, targetCol, tileRow, tileCol, tile);
            return true;    
        }
        
        // CHECK BELOW
        targetRow = tileRow + 1;
        targetCol = tileCol;
        if (inBounds(targetRow, targetCol) && isBlank(targetRow, targetCol))
        {
            swap(targetRow, targetCol, tileRow, tileCol, tile);
            return true;    
        }
        
        // Tile was found, but move is illegal.
        return false;
    }
    return false;
}

bool getTileIndex(int tile)
{
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
            // Save tile index
            tileRow = i;
            tileCol = j;
            return true;
            }
        }
    } 
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            // If tile is the blank space, skip it
            if (board[i][j] != 0)
            {
                // if at edge, check next row
                if (j == d-1)
                {
                    if (board[i][j] > board[i+1][0])
                        return false;
                }
                
                // check the one to the right
                else if (board[i][j] > board[i][j+1])
                {
                    if (board[i][j+1] != 0)
                        return false;   
                }    
            }
        }   
    }
    // If you made it past those tests, you won!
    return true;
}

bool inBounds(int row, int col)
{
    if (row < 0)
        return false;
    if (row >= d)
        return false;    
    if (col < 0)
        return false;
    if (col >= d)
    {
        
        return false;    
    }
        
    return true;
}

bool isBlank(int row, int col)
{
    if (board[row][col] == 0)
        return true;
    return false;
}

void swap(int targetRow, int targetCol, int tileRow, int tileCol, int tileValue)
{
    board[targetRow][targetCol] = tileValue;
    board[tileRow][tileCol] = 0;
}