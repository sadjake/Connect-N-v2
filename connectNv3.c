#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_WHITE "\x1b[37m"
#define ANSI_COLOR_BLACK "\x1b[30m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_BG_CYAN "\x1b[46m"
#define ANSI_COLOR_BG_RESET "\x1b[49m"
#define MAXSIZE 25
#define MINSIZE 8
#define MINCONNECTNUM 4

int InitializeBoard(int **connectNBoard, int numRowsInBoard);
int MakeMove(int **connectNBoard, int numRowsInBoard, int playerID, int columnChosen);
int DisplayBoard(int **connectNBoard, int numRowsInBoard);
int CheckWinner(int **connectNBoard, int numRowsInBoard, int numConnect, int columnChosen, int playerID);

int main()
{
    int numRows;
    int numToConnect;
    int numPlayers; // Added variable for the number of players

    // get numRows from user
    while (1)
    {
        printf("Enter the number of squares along each edge of the board\n");
        if (scanf("%d", &numRows) != 1)
        {
            printf("ERROR: The value of numRows is not an integer \n");
            while (getchar() != '\n')
                ;
            continue;
        }
        else if (numRows > MAXSIZE)
        {
            printf("ERROR: Board size too large (>%d)\n", MAXSIZE);
            while (getchar() != '\n')
                ;
            continue;
        }
        else if (numRows < MINSIZE)
        {
            printf("ERROR: Board size too small (it should be >%d)\n", MINSIZE);
            while (getchar() != '\n')
                ;
            continue;
        }
        else
        {
            while (getchar() != '\n')
                ;
            break;
        }
    }

    // get numToConnect from user
    while (1)
    {
        printf("Enter the number of pieces that must form a line to win\n");
        if (scanf("%d", &numToConnect) != 1)
        {
            printf("ERROR: The value provided was not an integer \n");
            while (getchar() != '\n')
                ;
            continue;
        }
        else if (numToConnect > numRows - MINCONNECTNUM)
        {
            printf("ERROR: Number to connect is too large (>%d)\n", (numRows - MINCONNECTNUM));
            while (getchar() != '\n')
                ;
            continue;
        }
        else if (numToConnect < MINCONNECTNUM)
        {
            printf("ERROR: Number to connect is too small (<%d)\n", MINCONNECTNUM);
            while (getchar() != '\n')
                ;
            continue;
        }
        else
        {
            while (getchar() != '\n')
                ;
            break;
        }
    }

    // get numPlayers from user
    while (1)
    {
        printf("Enter the number of players (2 to 4): ");
        if (scanf("%d", &numPlayers) != 1 || (numPlayers < 2 || numPlayers > 4))
        {
            printf("ERROR: Please enter a valid number of players (2 to 4).\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        else
        {
            while (getchar() != '\n')
                ;
            break;
        }
    }

    // allocate game boards
    int **myConnectNBoard = (int **)malloc(numRows * sizeof(int *));
    for (int i = 0; i < numRows; i++)
    {
        myConnectNBoard[i] = (int *)malloc(numRows * sizeof(int));
    }

    if (!InitializeBoard(myConnectNBoard, numRows))
    {
        printf("ERROR: Could not initialize the game board ");
        return 0;
    }

    int playerID = 1; // Initialize playerID to 1
    int chosenColumn;
    int gameWon = 0; // Variable to track if the game is won

    do
    {
        printf("\n\n");
        if (!DisplayBoard(myConnectNBoard, numRows))
        {
            printf("ERROR: Could not display the game board ");
        }

        printf("Player %d moves\n", playerID);

        while (1)
        {
            printf("Enter the column number where you want to put your piece\n");
            if (scanf("%d", &chosenColumn) != 1)
            {
                printf("ERROR: The value provided was not an integer \n");
                while (getchar() != '\n')
                    ;
                continue;
            }

            // Check if the column chosen is within the valid range
            if (chosenColumn < 0 || chosenColumn >= numRows)
            {
                printf("ERROR: Column %d is not on the board: try again \n", chosenColumn);
                printf("ERROR: Column number should be >= 0 and <%d\n", numRows);
                while (getchar() != '\n')
                    ;
                continue;
            }

            // Make the move
            if (MakeMove(myConnectNBoard, numRows, playerID, chosenColumn))
            {
                break; // Move successful, break out of the loop
            }
            else
            {
                printf("Invalid move, try again\n");
            }
        }

        // Check if the current player has won
        if (CheckWinner(myConnectNBoard, numRows, numToConnect, chosenColumn, playerID))
        {
            gameWon = 1; // Set gameWon to true
            break;
        }

        // Switch to the next player
        playerID = (playerID % numPlayers) + 1;

    } while (!gameWon);

    // Display the final board
    printf("\n\n");
    if (!DisplayBoard(myConnectNBoard, numRows))
    {
        printf("ERROR: Could not display the game board ");
    }

    // Display the winner
    printf("Player %d has won!\n", playerID);

    return 0;
}

int InitializeBoard(int **connectNBoard, int numRowsInBoard)
{
    // Check if pointer to the board is NULL
    if (connectNBoard == NULL)
    {
        printf("ERROR: cannot initialize the board because the pointer to the board is NULL\n");
        return 0;
    }

    if (numRowsInBoard > MAXSIZE || numRowsInBoard < MINSIZE)
    {
        // check illegal rows
        printf("ERROR: illegal number of rows in board\n");
        return 0;
    }

    for (int i = 0; i < numRowsInBoard; i++)
    {
        for (int j = 0; j < numRowsInBoard; j++)
        {
            connectNBoard[i][j] = 0; // Initialize each element to 0
        }
    }

    return 1;
}

// DisplayBoard
int DisplayBoard(int **connectNBoard, int numRowsInBoard)
{
    // Check if the pointer to the board is NULL
    if (connectNBoard == NULL)
    {
        printf("ERROR: cannot display the game board because the pointer to the board is NULL\n");
        return 0;
    }

    // Check if the number of rows is within the legal range
    if (numRowsInBoard > MAXSIZE || numRowsInBoard < MINSIZE)
    {
        printf("ERROR: illegal number of rows in board\n");
        return 0;
    }

    // Print the column numbers at the top of the board
    printf("   ");
    for (int j = 0; j < numRowsInBoard; j++)
    {
        printf("%3d", j);
    }
    printf("\n");

    // Iterate over each row in the board
    for (int i = 0; i < numRowsInBoard; i++)
    {
        // Print the row number at the left edge of the board
        printf("%2d ", i);

        // Iterate over each column in the row
        for (int j = 0; j < numRowsInBoard; j++)
        {
            // Check the value in the current cell of the board
            if (connectNBoard[i][j] == 2)
            {
                printf(ANSI_COLOR_BLUE "%3c" ANSI_COLOR_RESET, '2'); // Print 'B' for black game piece
            }
            else if (connectNBoard[i][j] == 1)
            {
                printf(ANSI_COLOR_RED "%3c" ANSI_COLOR_RESET, '1'); // Print 'R' for white game piece
            }

            else if (connectNBoard[i][j] == 3)
            {
                printf(ANSI_COLOR_YELLOW "%3c" ANSI_COLOR_RESET, '3'); // Print 'R' for white game piece
            }

            else if (connectNBoard[i][j] == 4)
            {
                printf(ANSI_COLOR_GREEN "%3c" ANSI_COLOR_RESET, '4'); // Print 'R' for white game piece
            }

            else
            {
                printf("%3c", 'o'); // Print 'o' for empty square
            }
        }
        printf("\n"); // Move to the next line for the next row
    }

    return 1;
}

// MakeMove function
int MakeMove(int **connectNBoard, int numRowsInBoard, int playerID, int columnChosen)
{
    // Check if the board pointer is NULL
    if (connectNBoard == NULL)
    {
        printf("ERROR: cannot make a move because the pointer to the board is NULL\n");
        return 0;
    }

    // Check if the column chosen is within the valid range
    if (columnChosen < 0 || columnChosen > numRowsInBoard)
    {
        printf("Column chosen is out of range\n");
        return 0;
    }

    // Find the bottommost available row in the chosen column
    int row = numRowsInBoard - 1;
    while (row >= 0 && connectNBoard[row][columnChosen] != 0)
    {
        row--;
    }

    // Check if the column is full (no available row)
    if (row < 0)
    {
        printf("Column is already full\n");
        return 0;
    }

    // Place the player's game piece in the chosen column and row
    connectNBoard[row][columnChosen] = playerID;

    // Print the move information based on the player's ID
    if (playerID == 1)
    {
        printf("Player 1 has moved\nUpdated board:");
    }

    else if (playerID == 2)
    {
        printf("Player 2 has moved\nUpdated board:");
    }

    else if (playerID == 3)
    {
        printf("Player 3 has moved\nUpdated board:");
    }

    else
    {
        printf("Player 4 has moved\nUpdated board:");
    }

    return 1;
}

int CheckWinner(int **connectNBoard, int numRowsInBoard, int numConnect, int columnChosen, int playerID)
{
    int afterCount = 0;
    int beforeCount = 0;
    int i;

    // Check if the board pointer is NULL
    if (connectNBoard == NULL)
    {
        return 0;
    }

    // Check if the column chosen is within the valid range
    if (columnChosen < 0 || columnChosen >= numRowsInBoard)
    {
        return 0;
    }

    // Check if the pointer to the board is NULL
    if (*connectNBoard == NULL)
    {
        return 0;
    }

    // Find the row where the newest game piece has been placed
    // TOP DOWN
    int row = 0;
    while (row < numRowsInBoard && connectNBoard[row][columnChosen] == 0)
    {
        row++;
    }

    // If the row is not found, the move is not legal
    if (row < 0)
    {
        return 0;
    }

    // Check the row to the right of the columnChosen
    for (i = columnChosen + 1; i < numRowsInBoard; i++)
    {
        if (connectNBoard[row][i] == playerID)
        {
            afterCount++;
        }
        else
        {
            break;
        }
    }

    // Check the row to the left of the columnChosen
    for (i = columnChosen - 1; i >= 0; i--)
    {
        if (connectNBoard[row][i] == playerID)
        {
            beforeCount++;
        }
        else
        {
            break;
        }
    }

    // Check if the player has won horizontally
    if (afterCount + beforeCount + 1 >= numConnect)
    {
        return 1;
    }

    // re initialize afterCount and beforeCount to 0
    afterCount = 0;
    beforeCount = 0;

    // Check the column below the newest piece
    for (i = row + 1; i < numRowsInBoard; i++)
    {
        if (connectNBoard[i][columnChosen] == playerID)
        {
            afterCount++;
        }
        else
        {
            // printf("AFTER%d\n",afterCount);
            break;
        }
    }

    // Check if the player has won vertically
    if (afterCount + beforeCount + 1 >= numConnect)
    {
        return 1;
    }

    // re initialize afterCount and beforeCount to 0
    afterCount = 0;
    beforeCount = 0;

    // Check the diagonal row from upper left to lower right
    for (i = 1; row + i < numRowsInBoard && columnChosen + i < numRowsInBoard; i++)
    {
        if (connectNBoard[row + i][columnChosen + i] == playerID)
        {
            afterCount++;
        }
        else
        {
            break;
        }
    }

    // Check the diagonal row from upper left to lower right part 2
    for (i = 1; row - i >= 0 && columnChosen - i >= 0; i++)
    {
        if (connectNBoard[row - i][columnChosen - i] == playerID)
        {
            beforeCount++;
        }
        else
        {
            break;
        }
    }

    // Check if the player has won diagonally (upper left to lower right)
    if (afterCount + beforeCount + 1 >= numConnect)
    {
        return 1;
    }

    // re initialize afterCount and beforeCount to 0
    afterCount = 0;
    beforeCount = 0;

    // Check the diagonal row from upper right to lower left
    for (i = 1; row + i < numRowsInBoard && columnChosen - i >= 0; i++)
    {
        if (connectNBoard[row + i][columnChosen - i] == playerID)
        {
            afterCount++;
        }
        else
        {
            break;
        }
    }

    // Check the diagonal row from upper right to lower left part 2
    for (i = 1; row - i >= 0 && columnChosen + i < numRowsInBoard; i++)
    {
        if (connectNBoard[row - i][columnChosen + i] == playerID)
        {
            beforeCount++;
        }
        else
        {
            break;
        }
    }

    // Check if the player has won diagonally (upper right to lower left)
    if (afterCount + beforeCount + 1 >= numConnect)
    {
        return 1;
    }

    // No winning condition found
    return 0;
}
