// Maaz Khalid (22i-2736), Muhammad Bilal (22i-1533), Usman Asif (22i-8802)

#pragma once
#include<iostream>
#include <iomanip>
#include "playerAndToken.h"
#include "playerAndToken.h"
using namespace std;


// ANSI color codes for console text
#define RESET "\033[0m"
#define WHITE "\033[47m"
#define GRAY "\033[100m"
#define BLACK "\033[30m"
#define BLUE "\033[44m"
#define RED "\033[41m"
#define GREEN "\033[42m"
#define YELLOW "\033[43m"
#define CYAN "\033[46m"

void initializeGrid(int grid[15][15])
{
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            grid[i][j] = -9;
        }
    }

    // starting places(safe) after token is unlocked. (indicated by -3)
    grid[6][1] = -3;
    grid[13][6] = -3;
    grid[8][13] = -3;
    grid[1][8] = -3;

    // safe places other than starting posiitons. (indicated by -4)
    grid[8][2] = -4;
    grid[2][6] = -4;
    grid[6][12] = -4;
    grid[12][8] = -4;

    // places for valid movements of tokens. (indicated by 0)
    grid[6][0] = 0;
    grid[6][2] = 0;
    grid[6][3] = 0;
    grid[6][4] = 0;
    grid[6][5] = 0;

    grid[7][0] = 0;
    grid[7][14] = 0;
    grid[0][7] = 0;
    grid[14][7] = 0;

    grid[8][0] = 0;
    grid[8][1] = 0;
    grid[8][3] = 0;
    grid[8][4] = 0;
    grid[8][5] = 0;

    grid[6][14] = 0;
    grid[6][13] = 0;
    grid[6][11] = 0;
    grid[6][10] = 0;
    grid[6][9] = 0;

    grid[8][14] = 0;
    grid[8][12] = 0;
    grid[8][11] = 0;
    grid[8][10] = 0;
    grid[8][9] = 0;

    grid[0][6] = 0;
    grid[1][6] = 0;
    grid[3][6] = 0;
    grid[4][6] = 0;
    grid[5][6] = 0;

    grid[0][8] = 0;
    grid[2][8] = 0;
    grid[3][8] = 0;
    grid[4][8] = 0;
    grid[5][8] = 0;

    grid[9][6] = 0;
    grid[10][6] = 0;
    grid[11][6] = 0;
    grid[12][6] = 0;
    grid[14][6] = 0;

    grid[9][8] = 0;
    grid[10][8] = 0;
    grid[11][8] = 0;
    grid[13][8] = 0;
    grid[14][8] = 0;

    // places for Home entrance. For Blue (indicated by -1)
    grid[7][1] = -1;
    grid[7][2] = -1;
    grid[7][3] = -1;
    grid[7][4] = -1;
    grid[7][5] = -1;
    // places for Home entrance. For Green (indicated by -5)
    grid[7][9] = -5;
    grid[7][10] = -5;
    grid[7][11] = -5;
    grid[7][12] = -5;
    grid[7][13] = -5;
    // places for Home entrance. For Yellow (indicated by -6)
    grid[1][7] = -6;
    grid[2][7] = -6;
    grid[3][7] = -6;
    grid[4][7] = -6;
    grid[5][7] = -6;
    // places for Home entrance. For Red (indicated by -2)
    grid[9][7] = -2;
    grid[10][7] = -2;
    grid[11][7] = -2;
    grid[12][7] = -2;
    grid[13][7] = -2;

    // Places for players and tokens will be add dynamically.
}

void displayGrid(int grid[15][15])
{
    cout << endl;
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            cout << setw(2);

            if (grid[i][j] == 0) // White
                cout << WHITE << "   " << RESET;

            else if (grid[i][j] == -3)  // Gray safe place 
                cout << GRAY << "   " << RESET;

            else if (grid[i][j] == -4) // Gray  safe place
                cout << GRAY << "   " << RESET;

            else if (grid[i][j] == -1) // Blue Home
                cout << BLUE << "   " << RESET;

            else if (grid[i][j] == -2) // Red Home
                cout << RED << "   " << RESET;

            else if (grid[i][j] == -5) // Green Home Home
                cout << GREEN << "   " << RESET;

            else if (grid[i][j] == -6) // Yellow Home Home
                cout << YELLOW << "   " << RESET;

            else if (grid[i][j] == 1) // Blue token places
                cout << BLUE << " 1 " << RESET;

            else if (grid[i][j] == 2) // Red token places
                cout << RED << " 2 " << RESET;

            else if (grid[i][j] == 3) // Green token places
                cout << GREEN << " 3 " << RESET;

            else if (grid[i][j] == 4) // Yellow token places
                cout << YELLOW << " 4 " << RESET;

            else if (grid[i][j] == -8) // Indicating player token has open
                cout << WHITE << "   " << RESET;


            else
                cout << "   ";
        }
        cout << endl;
    }
}

void initializeGridWithPlayersAndTokens(vector<Player>& players, int numPlayers, int numTokens, int grid[15][15]) {

    players.clear();

    int blueHome[4][2] = { {2, 2}, {2, 4}, {4, 2}, {4, 4} };                        // grid position for blue tokens
    int redHome[4][2] = { {10, 2}, {10, 4}, {12, 2}, {12, 4} };                    // grid position for red Tokens
    int greenHome[4][2] = { {10, 10}, {10, 12}, {12, 10}, {12, 12} };             // grid position for green Tokens
    int yellowHome[4][2] = { {2, 10}, {2, 12}, {4, 10}, {4, 12} };               // grid position for yellow Tokens

    // Loop through players and assign tokens to their respective home coordinates
    for (int i = 0; i < numPlayers; i++) {

        Player player;

        player.id = i + 1;
        player.color = (i == 0 ? "Blue" : i == 1 ? "Red" : i == 2 ? "Green" : "Yellow");
        player.isTurn = false;

        for (int j = 0; j < numTokens; j++)
        {
            Token token;
            token.id = j + 1;
            token.isUnlocked = false;

            if (i == 0)   // Blue
            {
                token.initialPosition = { blueHome[j][0], blueHome[j][1] };   // Assigning Home Posiitons for Player 1
                token.position = { blueHome[j][0], blueHome[j][1] };  // Assigning initial token position for player 1
                grid[blueHome[j][0]][blueHome[j][1]] = 1;

            }
            else if (i == 1) // Red
            {
                token.initialPosition = { redHome[j][0], redHome[j][1] };    // Assigning Home Posiitons for Player 2
                token.position = { redHome[j][0], redHome[j][1] };  // Assigning initial token position for player 2
                grid[redHome[j][0]][redHome[j][1]] = 2;
            }
            else if (i == 2) // Green
            {
                token.initialPosition = { greenHome[j][0], greenHome[j][1] };    // Assigning Home Posiitons for Player 3
                token.position = { greenHome[j][0], greenHome[j][1] };  // Assigning initial token position for player 3
                grid[greenHome[j][0]][greenHome[j][1]] = 3;
            }
            else if (i == 3) // Yellow
            {
                token.initialPosition = { yellowHome[j][0], yellowHome[j][1] };    // Assigning Home Posiitons for Player 4
                token.position = { yellowHome[j][0], yellowHome[j][1] };    // Assigning initial token position for player 3
                grid[yellowHome[j][0]][yellowHome[j][1]] = 4;
            }

            player.tokens.push_back(token);    // Adding token to the player's tokens vector/list 
        }

        players.push_back(player);
    }
}
