// Maaz Khalid (22i-2736), Muhammad Bilal (22i-1533), Usman Asif (22i-8802)

#pragma once
#include<utility>
#include<string>
#include<vector>

using namespace std;

struct Token {
    int id;         // representing token id       
    bool isUnlocked;     // representing if token is in play(unlocled)
    pair<int, int> initialPosition;    // representing Home(initial) Positions of the token
    pair<int, int> position;    // representing row and column of the token
};

struct Player {
    int id;    // Representing the player (1,2,3,4)
    string color;  // representing the color of the player (1->Blue, 2->Red, 3->Green, 4->Yellow)
    bool isTurn;   // representing that player has taken turn or not;
    int hitRate = 0;   // represnenting the hitting rate of the player
    int tokensInPlay = 0;   // representing how many tokens are in Play (Unlocked)
    int tokensInHome = 0;   // representing token that has entered home
    bool hasWon = false;
    vector<Token> tokens;    // representing that each player has multiple tokens
};

struct Winners {    // To keep track of winning players
    int playerId;
    int hitRate;
    int winningPosition;
};
