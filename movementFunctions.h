// Maaz Khalid (22i-2736), Muhammad Bilal (22i-1533), Usman Asif (22i-8802)

#pragma once
#include<iostream>
#include <utility>
using namespace std;

// Function to moveToken Horizontally right
pair<int, int> moveHorizontallyRight(int row, int col, int diceValue, int colEnd, bool moveUp, int rowEnd = -1)
{
    col += diceValue;   // Move horizontally right

    if (col > colEnd)
    {
        if (moveUp)    //  move vertically up (1st direction change)
        {
            int extraSteps = col - colEnd;     // calculate extra steps when changing direction
            col = colEnd + 1;
            row -= extraSteps;     // start moving vertically upwards

        }
        else     // move vertically down   (1st direction change)
        {
            int extraSteps = col - colEnd;     // calculate extra steps when changing direction
            col = colEnd;
            row += extraSteps;        // start moving vertically downwards

            if (rowEnd != -1 && row > rowEnd)    // move horizontally left after moving vertically down (2nd direction change)
            {
                int extraSteps = row - rowEnd;
                row = rowEnd;
                col -= extraSteps;    // Start Moving Hosrizontally left
            }
        }
    }

    return { row, col };
}
// Function to moveToken Horizontally Left
pair<int, int> moveHorizontallyLeft(int row, int col, int diceValue, int colEnd, bool moveDown, int rowEnd = -1)
{
    col -= diceValue;   // Move horizontally left

    if (col < colEnd)
    {
        if (moveDown)    //  move vertically Down    (1st direction change)
        {
            int extraSteps = colEnd - col;     // calculate extra steps when changing direction
            col = colEnd - 1;
            row += extraSteps;     // start moving vertically upwards
        }
        else     // move vertically up   (1st direction change)
        {
            int extraSteps = colEnd - col;     // calculate extra steps when changing direction
            col = colEnd;
            row -= extraSteps;        // start moving vertically upwards

            if (rowEnd != -1 && row < rowEnd)    // move horizontally right after moving vertically up (2nd direction change)
            {
                int extraSteps = rowEnd - row;
                row = rowEnd;
                col += extraSteps;      // Start Moving Hosrizontally right
            }
        }
    }

    return { row, col };
}
// Function to moveToken Vertically Upwards
pair<int, int> moveVerticallyUp(int row, int col, int diceValue, int rowEnd, bool moveLeft, int colEnd = -1)
{
    row -= diceValue;  // Move vertically upward

    if (row < rowEnd)
    {
        if (moveLeft)    //  move horizontally Left    (1st direction change)
        {
            int extraSteps = rowEnd - row;     // calculate extra steps when changing direction
            row = rowEnd - 1;
            col -= extraSteps;     // start moving horizontally left
        }
        else     // move horizontally right   (1st direction change)
        {
            int extraSteps = rowEnd - row;     // calculate extra steps when changing direction
            row = rowEnd;
            col += extraSteps;     // start moving horizontally right

            if (colEnd != -1 && col > colEnd)    // move vertically down after moving horizontally right (2nd direction change)
            {
                int extraSteps = col - colEnd;
                col = colEnd;
                row += extraSteps;      // Start Moving vertically down
            }
        }
    }

    return { row, col };
}
// Function to moveToken Vertically Downwards
pair<int, int> moveVerticallyDown(int row, int col, int diceValue, int rowEnd, bool moveRight, int colEnd = -1)
{
    row += diceValue;  // Move vertically downwards

    if (row > rowEnd)
    {
        if (moveRight)    //  move horizontally Right    (1st direction change)
        {
            int extraSteps = row - rowEnd;     // calculate extra steps when changing direction
            row = rowEnd + 1;
            col += extraSteps;     // start moving horizontally right
        }
        else     // move horizontally left   (1st direction change)
        {
            int extraSteps = row - rowEnd;     // calculate extra steps when changing direction
            row = rowEnd;
            col -= extraSteps;     // start moving horizontally left

            if (colEnd != -1 && col < colEnd)    // move vertically down after moving horizontally right (2nd direction change)
            {
                int extraSteps = colEnd - col;
                col = colEnd;
                row -= extraSteps;      // Start Moving vertically up
            }
        }
    }

    return { row, col };
}
