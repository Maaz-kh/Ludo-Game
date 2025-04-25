#include <iostream>
#include <iomanip>
#include<string>
#include<vector>
#include<thread>
#include<mutex>
#include<random>
#include<chrono>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include"movementFunctions.h"
#include "playerAndToken.h"
#include "gridInitializationAndDisplay.h"

using namespace std;


int dice;       // shared dice variable
int grid[15][15];   // shared grid
int dummyGrid[15][15];  // Dummy grid to save the inital values of grid 
atomic<bool> gameEnd(false);    // Variable indicating that is the game ended or not.

mutex mtx;

vector<Player> players;
vector<Winners> winners;

void rollADice()
{
    static thread_local mt19937 generator(random_device{}()); // Thread-local random generator
    uniform_int_distribution<int> distribution(1, 6); // Dice values range from 1 to 6
    dice = distribution(generator);
}

void isTokenHitted(pair<int, int> updatedPosition, Player& player, Token& token)
{

    if (dummyGrid[updatedPosition.first][updatedPosition.second] == -3 || dummyGrid[updatedPosition.first][updatedPosition.second] == -4)
    {
        // If Player is at safe position return without hitting
        return;  
    }

    // Check if the current position forms a block (two or more tokens of the same player on the same spot)
    int samePlayerTokenCount = 0;
    for (Token& playerToken : player.tokens)
    {
        if (playerToken.isUnlocked && playerToken.position == updatedPosition)
        {
            samePlayerTokenCount++;
        }
    }

    // If a block is formed, tokens of other players cannot hit this position
    if (samePlayerTokenCount > 1)
    {
        // If a player has formed a block other players cannot land and pass it.
        return;
    }

    // Check if the token lands on an opposing player's token and hits it
    for (Player& otherPlayer : players)
    {
        if (otherPlayer.id == player.id)    // Skip tokens of the same player
            continue;

        for (Token& otherToken : otherPlayer.tokens)
        {
            if (otherToken.isUnlocked && otherToken.position == updatedPosition)    // Token is hit
            {

                cout << "\nPlayer " << player.id << " has hitted Player " << otherPlayer.id << "'s Token!" << endl;

                // Update the grid
                grid[updatedPosition.first][updatedPosition.second] = dummyGrid[updatedPosition.first][updatedPosition.second]; // Reset the hit position
                grid[otherToken.initialPosition.first][otherToken.initialPosition.second] = otherPlayer.id;   // Send the hit token back to its home
                otherToken.position = { otherToken.initialPosition.first, otherToken.initialPosition.second };  // Update the hitted token position
                grid[updatedPosition.first][updatedPosition.second] = player.id; 

                otherToken.isUnlocked = false;    // Lock the hit token
                otherPlayer.tokensInPlay--;       // Update tokens in play for the hit player
                player.hitRate++;                 // Increment the hit rate of the player

                return;
            }
        }
    }
}

// Thread function to check if the player has hitted a token
void checkIfTokenIsHitted(pair<int, int> updatedPosition, Player& player, Token& token) 
{
    isTokenHitted(updatedPosition, player, token);
}

// Function to remove token of a player which has entered its home
void removeToken(Player& player, int tokenId)
{
    for (auto it = player.tokens.begin(); it != player.tokens.end(); ++it) 
    {
        if (it->id == tokenId) 
        {
            grid[it->initialPosition.first][it->initialPosition.second] = -9;    // Update Value of Token's initial value
            player.tokens.erase(it); // Erase the token and break
            player.tokensInPlay--;    // Updating tokens in play
            player.tokensInHome++;    // Updating tokens in Home
            break;
        }
    }
}

void moveTokenInGrid(Token& token, int diceValue, Player& player)
{
    int row = token.position.first;
    int col = token.position.second;

    bool isTokenHome = false;

    pair<int, int> updatedPosition = { row, col };

    cout << "\nPlayer: " << player.id << " Initial Position: (" << row << ", " << col << ")"<<endl; 
    cout<<"Dice Value: " << diceValue << endl;

    grid[row][col] = dummyGrid[row][col];   // Update the grid position before moving token to its original value;

    // Horizontal right movement in row 6 from column 0-5
    if (row == 6 && col >= 0 && col <= 5)
    {
        updatedPosition = moveHorizontallyRight(row, col, diceValue, 5, true);    // colEnd = 5, moveUp
    }
    // Vertically upward movement in col 6 and row 5-0
    else if (col == 6 && row >= 0 && row <= 5)
    {
        // Player 4 Home Movement if possible
        if (player.id == 4 && player.hitRate > 0)   // If hit rate is greater than 0, player will enter its home
        {
            updatedPosition = moveVerticallyUp(row, col, diceValue, 0, false, 7);   // Move right, rowEnd = 0, colEnd = 7
        }
        else
        {
            updatedPosition = moveVerticallyUp(row, col, diceValue, 0, false, 8); // moveRight, rowEnd = 0, colEnd = 8
        }
    }
    // Horizontal right movement in row 0 and column 6 - 8 
    else if (row == 0 && col >= 6 && col <= 8)
    {
        // Player 4 Home Movement if possible
        if (player.id == 4 && player.hitRate > 0 && col != 8)   // If hit rate is greater than 0, player will enter its home
        {
            updatedPosition = moveHorizontallyRight(row, col, diceValue, 7, false);   // colEnd = 7, moveDown
        }
        else
        {
            updatedPosition = moveHorizontallyRight(row, col, diceValue, 8, false);   // colEnd = 8, moveDown
        }
    }
    // Vertically downward movement in col 8 and row 0-5
    else if (col == 8 && row >= 0 && row <= 5)
    {
        updatedPosition = moveVerticallyDown(row, col, diceValue, 5, true);    // rowEnd = 5, moveRight
    }
    // Horizontal right movement in row 6 and column 9 - 14 
    else if (row == 6 && col >= 9 && col <= 14)
    {
        // Player 3 Home Movement if possible
        if (player.id == 3 && player.hitRate > 0)    // If hit rate is greater than 0 player will enter its home
        {
            updatedPosition = moveHorizontallyRight(row, col, diceValue, 14, false, 7); //  colEnd = 14, rowEnd = 7,  moveDown 
        }
        else
        {
            updatedPosition = moveHorizontallyRight(row, col, diceValue, 14, false, 8); //  colEnd = 14, rowEnd = 8,  moveDown 
        }
    }
    // vertically downward movement in col 14 and row 6 - 8 
    else if (col == 14 && row >= 6 && row <= 8)
    {
        // Player 3 Home Movement if possible
        if (player.id == 3 && player.hitRate > 0 && row != 8)
        {
            updatedPosition = moveVerticallyDown(row, col, diceValue, 7, false);    // rowEnd = 7, moveLeft
        }
        else
        {
            updatedPosition = moveVerticallyDown(row, col, diceValue, 8, false);    // rowEnd = 8, moveLeft
        }
    }
    // Horizontal left movement in row 8 and column 14 - 9 
    else if (row == 8 && col <= 14 && col >= 9)
    {
        updatedPosition = moveHorizontallyLeft(row, col, diceValue, 9, true);    // colEnd = 9, moveDown
    }
    // Vertically downward movement in col 8 and row 9-14
    else if (col == 8 && row >= 9 && row <= 14)
    {
        // Player 2 Home Movement if possible
        if (player.id == 2 && player.hitRate > 0)    // If hit rate is greater than 0 player will enter its home
        {
            updatedPosition = moveVerticallyDown(row, col, diceValue, 14, false, 7); // moveLeft, colEnd = 7, rowEnd = 14
        }
        else
        {
            updatedPosition = moveVerticallyDown(row, col, diceValue, 14, false, 6); // moveLeft, colEnd = 6, rowEnd = 14
        }
    }
    // Horizontally left movement in row 14 and col 8 - 6
    else if (row == 14 && col <= 8 && col >= 6)
    {
        // Player 2 Home Movement if possible
        if (player.id == 2 && player.hitRate > 0 && col != 6)    // If hit rate is greater than 0 player will enter its home
        {
            updatedPosition = moveHorizontallyLeft(row, col, diceValue, 7, false);   // moveUp, colEnd = 7
        }
        else
        {
            updatedPosition = moveHorizontallyLeft(row, col, diceValue, 6, false);   // moveUp, colEnd = 6
        }
    }
    // Vertically upward movement in col 6 and row 14 - 9
    else if (col == 6 && row <= 14 && row >= 9)
    {
        updatedPosition = moveVerticallyUp(row, col, diceValue, 9, true); // moveLeft, rowEnd = 9;
    }
    // Horizontal left movement in row 8 and column 5 - 0 
    else if (row == 8 && col >= 0 && col <= 5)
    {
        // Player 2 Home Movement if possible
        if (player.id == 1 && player.hitRate > 0)    // If hit rate is greater than 0 player will enter its home
        {
            updatedPosition = moveHorizontallyLeft(row, col, diceValue, 0, false, 7);  // moveUp, rowEnd = 7, colEnd = 0
        }
        else
        {
            updatedPosition = moveHorizontallyLeft(row, col, diceValue, 0, false, 6);  // moveUp, rowEnd = 6, colEnd = 0
        }
    }
    // vertically upward movement in col 0 and row 8 - 6
    else if (col == 0 && row <= 8 && row >= 6)
    {
        if (player.id == 1 && player.hitRate > 0 && row != 6)
        {
            updatedPosition = moveVerticallyUp(row, col, diceValue, 7, false);   // moveRight, rowEnd = 7
        }
        else
        {
            updatedPosition = moveVerticallyUp(row, col, diceValue, 6, false);   // moveRight, rowEnd = 7
        }
    }
    // Player 1's token Home Movement
    else if (player.id == 1 && row == 7 && col >= 0 && col <= 5 && player.hitRate > 0)
    {
        col += diceValue;   // Move horizontally right

        if (col == 6)    // Indicating token has reached home
        {
            cout << "Player 1's Token " << token.id << " has reached home!" << endl;
            isTokenHome = true;
            removeToken(player, token.id);   // remove the token from players.token
        }
        else if (col > 6)      // if move inside home is not possible
        {
            cout << "Invalid move! Player 1's Token " << token.id << " cannot enter home with this dice value." << endl;
            col -= diceValue;
        }

        updatedPosition = { row, col };
    }
    // Player 2's token Home Movement
    else if (player.id == 2 && col == 7 && row >= 9 && col <= 14 && player.hitRate > 0)
    {
        row -= diceValue;   // Move vertically up

        if (row == 8)    // Indicating token has reached home
        {
            cout << "Player 2's Token " << token.id << " has reached home!" << endl;
            isTokenHome = true;
            // remove the token from players.token
            removeToken(player, token.id);

        }
        else if (row < 8)       // if move inside home is not possible
        {
            cout << "Invalid move! Player 2's Token " << token.id << " cannot enter home with this dice value." << endl;
            row += diceValue;
        }
        updatedPosition = { row, col };
    }
    // Player 3's token Home Movement
    else if (player.id == 3 && row == 7 && col >= 9 && col <= 14 && player.hitRate > 0)
    {
        col -= diceValue;           // Move horizontally left

        if (col == 8)           // Indicating token has reached home
        {
            cout << "Player 3's Token " << token.id << " has reached home!" << endl;
            isTokenHome = true;
            // remove the token from players.token
            removeToken(player, token.id);

        }
        else  if (col < 8)     // if move inside home is not possible
        {
            cout << "Invalid move! Player 2's Token " << token.id << " cannot enter home with this dice value." << endl;
            col += diceValue;
        }
        updatedPosition = { row, col };
    }
    // Player 4's token Home Movement
    else if (player.id == 4 && col == 7 && row >= 0 && row <= 5 && player.hitRate > 0)
    {
        row += diceValue;   // Move vertically downwards

        if (row == 6)    // Indicating token has reached home
        {
            cout << "Player 4's Token " << token.id << " has reached home!" << endl;
            isTokenHome = true;
            // remove the token from players.token
            removeToken(player, token.id);

        }
        else if (row > 6)       // if move inside home is not possible
        {
            cout << "Invalid move! Player 4's Token " << token.id << " cannot enter home with this dice value." << endl;
            row -= diceValue;
        }
        updatedPosition = { row, col };
    }


    thread checkTokenHit(checkIfTokenIsHitted, updatedPosition, ref(player), ref(token));

    row = updatedPosition.first;
    col = updatedPosition.second;

    if (isTokenHome)
    {
        grid[row][col] = dummyGrid[token.initialPosition.first][token.initialPosition.second];   // Update the grid position after it has reached home (eventually removing the token from the grid)

        isTokenHome = false;
    }
    else {

        grid[row][col] = player.id;   // Update the grid position after moving token;
        token.position = { row, col };
    }

    cout << "Player: " << player.id << " New Position: (" << row << ", " << col << ")" << endl;

    checkTokenHit.join();    // Join the checkTokenHit Token at the end of the function

}

void unlockToken(Player& player, Token& token)
{
    if (player.id == 1)  // Player 1 --> Blue
    {
        token.isUnlocked = true;
        cout << "Token " << token.id << " unlocked" << endl;

        grid[token.position.first][token.position.second] = -8;       // Indicating Player token is open
        grid[6][1] = player.id;
        player.tokensInPlay++;      // Token is unlocked. Increment tokens in play
        token.position = { 6, 1 };
    }
    else if (player.id == 2)  // Player 2 --> Red
    {
        token.isUnlocked = true;
        cout << "Token " << token.id << " unlocked" << endl;
        
        grid[token.position.first][token.position.second] = -8;    // Indicating Player token is open
        grid[13][6] = player.id;
        player.tokensInPlay++;      // Token is unlocked. Increment tokens in play
        token.position = { 13, 6 };
        
    }
    else if (player.id == 3)  // Player 3 --> Green
    {
        token.isUnlocked = true;
        cout << "Token " << token.id << " unlocked" << endl;

        grid[token.position.first][token.position.second] = -8;    // Indicating Player token is open
        grid[8][13] = player.id;
        player.tokensInPlay++;      // Token is unlocked. Increment tokens in play
        token.position = { 8, 13 };
    }
    if (player.id == 4)  // Player 4 --> Yellow
    {
        token.isUnlocked = true;
        cout << "Token " << token.id << " unlocked" << endl;
        grid[token.position.first][token.position.second] = -8;    // Indicating Player token is open
        grid[1][8] = player.id;
        player.tokensInPlay++;      // Token is unlocked. Increment tokens in play
        token.position = { 1, 8 };
    }
}

void moveSelectedToken(Player& player, int diceValue)
{
                                         
    if (player.tokensInPlay > 1)
    {
        int tokenId;
        cout << "You have " <<player.tokensInPlay <<" tokens in play.Enter token number(1 - 4) to move : ";
        cin >> tokenId;
        cout << endl;

        for (Token& token : player.tokens)
        {
            if (token.isUnlocked && token.id == tokenId)
            {
                moveTokenInGrid(token, diceValue, player);
                break;
            }
        }
    }
    else
    {
        for (Token& token : player.tokens)
        {
            if (token.isUnlocked)
            {
                moveTokenInGrid(token, diceValue, player);
                break;
            }
        }
    }
}

void unlockOrMoveToken(vector<int> diceValues, Player &player)
{
    for (int diceValue : diceValues)
    {
        if (player.tokensInPlay >= 1)   // Check if atleast one token is in play
        {
            if (diceValue == 6)       // Unlock or move the token based on user input
            {
                                            // Add input Validation
                char choice;

                do {
                    if (player.tokensInPlay == 1)
                    {
                        cout << "\nDo you want to unlock new token (y) or move the unlocked token (n): ";
                    }
                    else
                    {
                        cout << "\nYou have " << player.tokensInPlay << " tokens in play. Do you want to unlock a new token (y) or move a token (n): ";
                    }

                    cin >> choice;
                    cout << endl;

                    // Convert to lowercase if needed:
                    choice = tolower(choice);

                    if (choice != 'y' && choice != 'n') {
                        cout << "Invalid input! Please enter 'y' or 'n'.\n";
                    }

                } while (choice != 'y' && choice != 'n');

                if (choice == 'y')   // If user wants to unlock new token
                {
                    for (Token& token : player.tokens)
                    {
                        if (!token.isUnlocked)
                        {
                            unlockToken(player, token);
                            break;
                        }
                    }
                }
                else         // if user wants to move an unlocked token
                {
                    moveSelectedToken(player, diceValue); 
                }
            }
            else     // Move an unlocked token for dice value != 6
            {
                moveSelectedToken(player, diceValue); 
            }
        }
        else if (player.tokensInPlay == 0)   // No tokens in Play.  
        {
            for (Token& token : player.tokens)
            {
                if (diceValue == 6 && !token.isUnlocked)  // Unlock the token if dice value is 6.
                {
                    unlockToken(player, token);
                    break;
                }
            }
        }
    }

    displayGrid(grid);
    cout << endl;
    system("pause");
}

// Function for player thread that handles their moves, rolling the dice, and moving tokens.
void playerThread(Player &player)
{
    vector<int> diceValues;   // to keep track of multiple dice values 

    // 1. check if the player has taken turn or not 
    if (!player.isTurn)
    {
        // 2. Aplly lock to roll a dice and token movement
        mtx.lock();

        // 3. player turn --> true; 
        player.isTurn = true;

        cout << "\nPlayer " << player.id << "'s turn. To Roll a dice. " << endl;
        system("pause");
        cout << endl;
       
       // 4. roll a dice
        rollADice();
        
        diceValues.push_back(dice);
        cout << "Player: " << player.id << " rolled a dice. Dice Value: " << diceValues.back() << endl;

        // Check for the first six
        if (diceValues.back() == 6)
        {
            rollADice();
            diceValues.push_back(dice);
            cout << "Player: " << player.id << " rolled a dice. Dice Value: " << diceValues.back() << endl;

            // Check for the second six
            if (diceValues.back() == 6)
            {
                rollADice();
                diceValues.push_back(dice);
                cout << "Player: " << player.id << " rolled a dice. Dice Value: " << diceValues.back() << endl;

                // Check for the third six
                if (diceValues.back() == 6)
                {
                    cout << "3 Consecutive Sixes! Turn Lost for Player " << player.id << endl;

                    // Reset player's turn and unlock mutex
                    player.isTurn = false;
                    system("cls");  
                    mtx.unlock();
                    return; 
                }
            }
        }

        // 5. make move  To implement
        unlockOrMoveToken(diceValues, player);

        if (player.tokensInHome == 4)   // Indicating Player has won
        {
            cout << "Player's " << player.id << " Has moved all tokens in home";
            player.hasWon = true;
            // Adding Winning Details of Player (position, hitRate and player id);
            Winners winner;
            winner.playerId = player.id;
            winner.hitRate = player.hitRate;
            winner.winningPosition = static_cast<int>(winners.size());
            winners.push_back(winner);
        }

        player.isTurn = false;   // set player to false after turn has completed (for next round)
        system("cls");  

        // Unlock the dice and grid for next player
        mtx.unlock();

    }
};
void masterThreadFunction(int numPlayers)
{
   
    int round = 1;

    while (!gameEnd)
    {
        cout << " Round # " << round << " Started!" << endl;
        
        vector<thread> playerThreads;
        
        for (int i = 0; i < numPlayers; i++)
        {
            if (!players[i].hasWon)    // if player has won its thread will not be created.
            {
                playerThreads.emplace_back(playerThread, ref(players[i]));
            }
        }

        for (thread& t : playerThreads)
        {
            t.join();
        }

        int activePlayers = 0;      // Active Players == 1 End the Game
        for (Player& player : players)
        {
            if (!player.hasWon)     // If Player has won active players will decrement.
            {
                activePlayers++; 
            }
        }

        if (activePlayers <= 1)   // If one or no player remains end the game;
        {
            gameEnd = true;
            cout << "\n\nGame Over!" << endl << endl;
            for (const auto& winner : winners)
            {
                cout << "Player " << winner.playerId
                    << " | HitRate: " << winner.hitRate
                    << " | Winning Position: " << winner.winningPosition << endl;
            }
        }


        cout << " Round # " << round << " Ended!" << endl;
        round++;
        system("cls");
    }
    
}

void displayWelcomeScreen()
{
    system("cls");
    cout << "\t\t\t\t\t\t" << "=========================" << endl;
    cout << "\t\t\t\t\t\t| Welcome to LUDO Game  |" << endl;
    cout << "\t\t\t\t\t\t"<< "=========================" << endl << endl;
    cout << "1. Start Game" << endl;
    cout << "2. Exit" << endl;
}

int main()
{
    srand(time(0));
    int choice;

    do {
        displayWelcomeScreen();
        cout << "\nEnter your choice: ";
        cin >> choice;

        if (cin.fail() || (choice != 1 && choice != 2)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input! Please enter 1 to Start or 2 to Exit.\n\n";
            system("pause");
            system("cls");
        }

    } while (choice != 1 && choice != 2);

    if (choice == 2) {
        cout << "\nThanks for visiting! Exiting game..." << endl;
        return 0;
    }

    // Game starts
    int numPlayers;
    do {
        cout << "\nEnter the number of players (2 to 4): ";
        cin >> numPlayers;

        if (cin.fail() || numPlayers < 2 || numPlayers > 4) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number between 2 and 4.\n";
        }

    } while (numPlayers < 2 || numPlayers > 4);

    cout << endl;

    initializeGrid(grid);
    initializeGridWithPlayersAndTokens(players, numPlayers, 4, grid);

    // Display the grid
    displayGrid(grid);

    cout << endl;
    system("pause");
    cout << endl;
    system("cls");

    // initalizing the dummy grid wiht the original grid to keep track of grid values
    for (int i = 0;i < 15;i++)
    {
        for (int j = 0;j < 15;j++)
        {
            dummyGrid[i][j] = grid[i][j];
        }
    }

    // Creating master thread that will be responsible for creating player threads and 
    thread masterThread(masterThreadFunction, numPlayers);

    masterThread.join();


}

