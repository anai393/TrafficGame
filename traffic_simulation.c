#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NUM_ROWS 8
#define NUM_COLS 8

#define SPACE 0
#define WALL -1
#define EXIT -2

/***********************************************************/
/***********************************************************/
/******* ADD THE REQUIRED FUNCTIONS BELOW THIS POINT *******/
/***********************************************************/
/***********************************************************/

// Your function definitions should go here...
void InitialiseRoad(int road[NUM_ROWS][NUM_COLS], char side, int pos)
{
    //intialise variables
    int rowPos, colPos, i, j;
    //set start values for exit postion so they aren't unreferenced if NESW is not used
    rowPos = 0;
    colPos = 0;
    if (side == 'N')
    {
        //exit at top row
        rowPos = 0;
        colPos = pos;
    }
    else if (side == 'E')
    {
        //exit at right-most column
        colPos = NUM_COLS - 1;
        rowPos = pos;
    }
    else if (side == 'S')
    {
        //exit at bottom row
        rowPos = NUM_ROWS - 1;
        colPos = pos;
    }
    else if (side == 'W')
    {
        //exit at left most column
        colPos = 0;
        rowPos = pos;
    }

    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLS; j++)
        {
            //nested loop to set values to corresponding points
            if (i == 0 || j == 0 || i == (NUM_ROWS - 1) || j == (NUM_COLS - 1))
            {
                road[i][j] = WALL;
            }
            else
            {
                road[i][j] = SPACE;
            }
            if (i == rowPos && j == colPos)
            {
                road[i][j] = EXIT;
            }
        }
    }
}

void PrintRoad(int road[NUM_ROWS][NUM_COLS])
{
    //prints characters for corresponding points
    int i, j;
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLS; j++)
        {
            if (road[i][j] == WALL)
            {
                //prints wall
                printf("#");
            }
            else if (road[i][j] == EXIT)
            {
                //sets to letter for exit
                printf("O");
            }
            else if (road[i][j] == SPACE)
            {
                //prints emptyspace
                printf(" ");
            }
            else if (road[i][j] >= 'A' && road[i][j] <= 'Z')
            {
                //prints letter
                printf("%c", road[i][j]);
            }
        }
        printf("\n");
    }
}

double PercentUsed(int road[NUM_ROWS][NUM_COLS])
{
    //initialise counter as double to save time converting later
    double percent;
    int i, j, size, counter;
    //intitialise counter
    counter = 0;
    size = ((NUM_ROWS - 2) * (NUM_COLS - 2)); //gets area of car space
    for (i = 1; i < NUM_ROWS - 1; i++)
    {
        for (j = 1; j < NUM_COLS - 1; j++)
        {
            //checks for spaces
            if ((road[i][j] == SPACE))
            {
                counter++;
            }
        }
    }
    percent = (1 - ((double)counter / size)) * 100; //given the number of spaces, returns the percentage that is not spaces
    return percent;
}

void AddCar(int road[NUM_ROWS][NUM_COLS], int row, int col, int size)
{
    //intialise variables
    int horiz, i, j, counter;
    char letter;

    if (row >= NUM_ROWS || col >= NUM_COLS || col <= 0 || row <= 0)
    {
        return;
    }
    //set starting letter and counter
    letter = 'A';
    counter = 0;

    //check if horizontal
    if (size > 0)
    {
        horiz = 1;
    }
    else
    {
        horiz = 0;
        size = size * -1;
    }

    //nested loop to see what letter the car will be set to
    for (i = 1; i < NUM_ROWS - 1; i++)
    {
        for (j = 1; j < NUM_COLS - 1; j++)
        {
            if ((road[i][j] == letter))
            {
                letter++;
                j = 1;
                i = 1;
            }
        }
    }

    // horizontal car
    if (horiz == 1)
    {
        for (j = col; counter < size; j++)
        {
            if (road[row][j] != SPACE)
            {
                //check if any potential corresponding position is not a space
                return;
            }
            counter++;
        }
        counter = 0;
        for (j = col; counter < size; j++)
        {
            //replace spaces with letters
            road[row][j] = letter;
            counter++;
        }
    }
    //vertical car
    else
    {
        for (i = row; counter < size; i++)
        {
            if (road[i][col] != SPACE)
            {
                //check if any potential corresponding position in not a space
                return;
            }
            counter++;
        }
        counter = 0;
        for (i = row; counter < size; i++)
        {
            //replace spaces with letters
            road[i][col] = letter;
            counter++;
        }
    }
}

void FindCar(int road[NUM_ROWS][NUM_COLS], char move, int *rowStart, int *colStart, int *rowEnd, int *colEnd)
{
    int i, j, numPlaces, vert;
    //set starts and ends to zeros as they will be changed at the end
    *rowStart = 0;
    *colStart = 0;
    *rowEnd = 0;
    *colEnd = 0;
    //counter for the number of places this letter appears
    numPlaces = 0;
    vert = 0;
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLS; j++)
        {
            if (road[i][j] == move)
            {
                numPlaces++;
                //the first time we see a letter it will be a the top left postion
                if (*rowStart == 0 && *colStart == 0)
                {
                    *rowStart = i;
                    *colStart = j;
                    if (road[i + 1][j] == move)
                    {
                        //checks if letter is vertical
                        vert = 1;
                    }
                }
            }
        }
    }
    if (vert == 0)
    {
        //if it is horizontal it will be going across columns for the number of times it was counted
        *colEnd = *colStart + numPlaces - 1;
        *rowEnd = *rowStart;
    }
    else if (vert == 1)
    {
        //if it is vertical it will be going down rows for the number of times it was counted
        *rowEnd = *rowStart + numPlaces - 1;
        *colEnd = *colStart;
    }
}

int MoveCar(int road[NUM_ROWS][NUM_COLS], int r0, int c0, int r1, int c1)
{
    int vert = 0;
    int pos, i, wallNext;
    //checks if vertical
    if (c0 == c1)
    {
        vert = 1;
    }
    if (vert == 1)
    {
        //checks if row before is a SPACE, if yes it moves UP, if no it moves DOWN
        if (road[r0 - 1][c0] == SPACE)
        {
            if(road[r1+1][c0] == EXIT){
                return 1;
            }
            else{
                pos = r0 - 1;
                //checks for spaces going up
                while ((road[pos][c0] == SPACE))
                {
                    pos--;
                }
                //sets for value where wall is
                wallNext = pos;
                pos++;
                for (i = r0; i <= r1; i++)
                {
                    //replaces spaces with new car positions
                    road[pos][c0] = road[i][c0];
                    road[i][c0] = SPACE;
                    pos++;
                }
                //checks if exit is next to car
                if (road[wallNext][c0] == EXIT)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }                
            }
        }
        else{
            if(road[r1+1][c0] == EXIT){
                return 1;
            }
            else if(road[r0-1][c0] == EXIT){
                return 1;
            }
            else {
                 //checks for spaces going down
                pos = r1 + 1;
                while ((road[pos][c0] == SPACE))
                {
                    pos++;
                }
                //sets value for where the wall is
                wallNext = pos;
                pos--;
                for (i = r1; i >= r0; i--)
                {
                    //replaces spaces with new car positions
                    road[pos][c0] = road[i][c0];
                    road[i][c0] = SPACE;
                    pos--;
                }
                //checks if wall is next to car
                if (road[wallNext][c0] == EXIT)
                {
                    return 1;
                }
                else{
                    return 0;
                }
                
            }
        }
           
    }

    else
    {
        //checks if column before is a SPACE, if yes it moves LEFT, if no it moves RIGHT
        if (road[r0][c0 - 1] == SPACE)
        {
            if(road[r0][c1+1] == EXIT){
                return 1;
            }
            else{
                pos = c0 - 1;
                while ((road[r0][pos] == SPACE))
                {
                    pos--;
                }
                wallNext = pos;
                pos++;
                for (i = c0; i <= c1; i++)
                {
                    //replaces spaces with new car positions
                    road[r0][pos] = road[r0][i];
                    road[r0][i] = SPACE;
                    pos++;
                }
                //checks if wall is next to car
                if (road[r0][wallNext] == EXIT)
                {
                    return 1;
                }
                else{
                    return 0;
                }

            }
        }
        else{
            if(road[r0][c1+1] == EXIT){
                return 1;
            }
            else if(road[r0][c0-1] == EXIT){
                return 1;
            }
            else{
                pos = c1 + 1;
                //checks for spaces going right
                while ((road[r0][pos] == SPACE))
                {
                    pos++;
                }
                //sets to where wall next to the car is
                wallNext = pos;
                pos--;
                for (i = c1; i >= c0; i--)
                {
                    //replaces spaces with new car positions
                    road[r0][pos] = road[r0][i];
                    road[r0][i] = SPACE;
                    pos--;
                }
                //checks if wall is next to car
                if (road[r0][wallNext] == EXIT)
                {
                    return 1;
                }
                else{
                    return 0;
                }
            }
        }
    }
}

/***********************************************************/
/***********************************************************/
/********* DO NOT MODIFY ANY CODE BELOW THIS POINT *********/
/***********************************************************/
/***********************************************************/

/* Function to obtain capital letter as input */
char GetMove(void)
{
    char move;
    printf("\nMove car: ");
    scanf("%c", &move);
    // Ignore non-capital letter inputs
    while ((move < 'A') || (move > 'Z'))
    {
        scanf("%c", &move);
    }
    return move;
}

/* The main Traffic Jam simulation */
int main(void)
{
    int gameOver = 0;
    int road[NUM_ROWS][NUM_COLS];
    int rowStart, colStart, rowEnd, colEnd;
    char input;

    /* Print banner */
    printf(" _____           __  __ _            _                                        \n");
    printf("|_   _| __ __ _ / _|/ _(_) ___      | | __ _ _ __ ___           ____          \n");
    printf("  | || '__/ _` | |_| |_| |/ __|  _  | |/ _` | '_ ` _ \\  --   __/  |_\\_      \n");
    printf("  | || | | (_| |  _|  _| | (__  | |_| | (_| | | | | | | --- |  _     _``-.    \n");
    printf("  |_||_|  \\__,_|_| |_| |_|\\___|  \\___/ \\__,_|_| |_| |_| --  '-(_)---(_)--'\n\n");

    /* Initialise the road and add cars */
    InitialiseRoad(road, 'E', 3);
    AddCar(road, 3, 3, 2);
    PrintRoad(road);
    AddCar(road, 1, 1, 2);
    PrintRoad(road);
    AddCar(road, 2, 1, 3);
    PrintRoad(road);
    AddCar(road, 3, 2, -2);
    PrintRoad(road);
    AddCar(road, 5, 2, -2);
    PrintRoad(road);
    AddCar(road, 4, 4, -2);
    PrintRoad(road);
    AddCar(road, 6, 3, 3);
    PrintRoad(road);
    AddCar(road, 1, 5, -3);
    PrintRoad(road);
    AddCar(road, 2, 6, -2);
    PrintRoad(road);
    /* Print status */
    printf("ENGGEN131 2020 - C Project\nTraffic Jam!  There is a lot of traffic on the road!\n");
    printf("In fact, %.2f%% of the road is cars!\n\n", PercentUsed(road));

    /* Main simulation loop */
    while (!gameOver)
    {
        PrintRoad(road);
        input = GetMove();
        FindCar(road, input, &rowStart, &colStart, &rowEnd, &colEnd);
        gameOver = MoveCar(road, rowStart, colStart, rowEnd, colEnd);
    }

    /* A car has exited - the simulation is over */
    PrintRoad(road);
    printf("\nCongratulations, you're on your way again!");

    return 0;
}
