/*
Assignment: Board Game, Chess (capture_the_king) Final Submission
   Date: 5/15/2026

   Program Description:
   This program implements a simplified two‑player chess game using
   king‑capture rules. The board uses uppercase characters for White
   pieces and lowercase characters for Black. Players enter moves in
   classic algebraic notation using "from to" format (e.g., e2 e4).
   The game continues until one king is captured.

   Version 2 Update:
   The main game loop was redesigned using two while loops to support
   multiple game sessions without restarting the program. The outer
   loop controls replay functionality, allowing the user to start a
   new game after one ends. The inner loop runs a single match,
   processing moves, updating the board, and checking for win
   conditions until a king is captured.
*/

#include <iostream>
#include <string>

using namespace std;

// ------------------------------------------------------------
// Function: parseMove
// Converts chess notation ("e2", "e4") to 0-indexed matrix coordinates.
//----------------------
// Coordinate Logic:
// Input: Algebraic string (e.g. "e2")
// Validate input, boundary= 'a'-'h' and '1'-'8'
// Identify file and rank
// Horizontal Map: 'a'= ASCII offset for columns (0-7)
// Vertical Map: '1' = offset, subtract from 7 to invert y-axis
// for top-down array indexing
// File(column): index=inputChar-'a'
// Rank(row): relativeVal=inputChar-'1'
// Inversion: index = maxIndex - indexBoundary
// ------------------------------------------------------------
bool parseMove(const string &from, const string &to,
               int &fr, int &fc, int &tr, int &tc)
{
    // chess notaion validation, two chars, File + Rank
    if (from.size() != 2 || to.size() != 2)
        return false;

    // Identify File(column) and Rank(row), for start and end square
    char fFile = from[0]; // start column
    char fRank = from[1]; // start row
    char tFile = to[0];   // target column
    char tRank = to[1];   // target row

    // Verify File boundary 'a'-'h'
    // Verify Rank boundary '1'-'8'
    if (fFile < 'a' || fFile > 'h')
        return false;
    if (tFile < 'a' || tFile > 'h')
        return false;
    if (fRank < '1' || fRank > '8')
        return false;
    if (tRank < '1' || tRank > '8')
        return false;

    // Normalize coordinate columns
    // Map: 'a' to '0', ... ,'h' to '7'by subtracting base ASCII value
    // e.g: ('e')-> 101('e')- 97('a')=4
    fc = fFile - 'a';
    tc = tFile - 'a';

    // Inversion coordinates for rows
    // Chess rank '1' is the physical bottom of board(index 7 in a 2D array)
    // Chess rank '8' is the physical top of board (index 0)
    // To normalize: maxIndex - (Rank - baseChar)
    fr = 7 - (fRank - '1');
    tr = 7 - (tRank - '1');

    return true;
}

// ------------------------------------------------------------
// Function: isPlayersPiece
// Piece owner validator
// Mathematical and logical flow
// ------------------------------
// Function uses parity check (modulo) and ASCII range validation
// to ensure piece belongs to active player
//
// Logic flow:
//---------------------------------------------
// Validate coordinate contains a piece
// Evaluate if the turn index is, even(W) or odd(B)
// Validate range, is character ASCII value within expected
// case range (upper vs lower):
// Uppercase: 'A' <= P <= 'Z', White
// Lowercase: 'a' <= P <= 'z'. Black
// Even turn: turn % 2 ==0, maps to White (uppercase)
// Odd turn: turn % 2 != 0, maps to Black (lowercase)
// ------------------------------------------------------------
bool isPlayersPiece(char piece, int turn)
{
    // If space is empty(' '), space belongs to neither player
    if (piece == ' ')
        return false;

    // Turn parity evaluation, is turn index even or odd
    if (turn % 2 == 0) // White=even
        // Check if piece is within ASCII range(Uppercase)
        return (piece >= 'A' && piece <= 'Z');
    else // Black=odd
        // Check if piece is within ASCII range(Lowercase)
        return (piece >= 'a' && piece <= 'z');
}

// ------------------------------------------------------------
// Function: printBoard
// Renders the 8x8 chess board to the console with algebraic
// labels (Ranks 1-8 and Files a-h)
//-------------------------------------------------------------
// Mathematical and Logical flow:
//-------------------------------
// Array mapping:
// Board is stored in a 2D array [8][8]
// Rows (r) range from 0 to 7
// Columns (c) range from 0 to 7
// ------------------------------------------------------------
void printBoard(char board[8][8])

{ // Print top file labels (a-h)
    cout << "\n    a   b   c   d   e   f   g   h\n";
    cout << "  +---+---+---+---+---+---+---+---+\n";
    for (int r = 0; r < 8; ++r)
    {
        // Print leading rank label, using mathematical inversion:
        // 8-r
        cout << 8 - r << " |";
        for (int c = 0; c < 8; ++c)
        {
            // Access element at board[row][column] and wrap in cell borders
            cout << " " << board[r][c] << " |";
        }
        // Print trailing rank label
        cout << " " << 8 - r << "\n";
        cout << "  +---+---+---+---+---+---+---+---+\n";
    }
    // Print bottom file labels (a-h)
    cout << "    a   b   c   d   e   f   g   h\n\n";
}

// ------------------------------------------------------------
// Function: applyMove
// Relocate piece on the board
// Moves a piece from (fr,fc) to (tr,tc)
// No legality checks; players must validate
//"defeating a piece" by overwriting char on target space
// ------------------------------------------------------------
void applyMove(int fr, int fc, int tr, int tc, char board[8][8])
{
    // Piece being moved from source
    char piece = board[fr][fc];

    // Clear source square
    board[fr][fc] = ' ';

    // Place piece on the target square, overwrite char if any
    board[tr][tc] = piece;
}

// ------------------------------------------------------------
// Function: checkWin
// Returns true if either king is missing
// Logical flow:
// Scan all 64 squares for king
// Determine wich king is present
// Set flags and return true if a king is gone
// ------------------------------------------------------------
bool checkWin(const char board[8][8], bool &whiteWins, bool &blackWins)
{
    bool whiteKing = false; // Track presence of white king
    bool blackKing = false; // Track presence of black king

    // Scan the board for kings
    for (int r = 0; r < 8; ++r)
    {
        for (int c = 0; c < 8; ++c)
        {
            if (board[r][c] == 'K')
                whiteKing = true;
            if (board[r][c] == 'k')
                blackKing = true;
        }
    }

    // White wins
    whiteWins = whiteKing && !blackKing;

    // Black wins
    blackWins = blackKing && !whiteKing;

    // Game ends when either king is captured
    return (whiteWins || blackWins);
}

// ------------------------------------------------------------
// Function: initBoard
// Initialize the board to starting position using loops
// Logic: fill rows based on piece rank type
// major pieces, pawns, empty
// ------------------------------------------------------------
void initBoard(char board[8][8])
{
    // Black major pieces
    const char blackMajor[8] = {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'};
    for (int c = 0; c < 8; ++c)
        board[0][c] = blackMajor[c];

    // Black pawns
    for (int c = 0; c < 8; ++c)
        board[1][c] = 'p';

    // Empty rows
    for (int r = 2; r <= 5; ++r)
        for (int c = 0; c < 8; ++c)
            board[r][c] = ' ';

    // White pawns
    for (int c = 0; c < 8; ++c)
        board[6][c] = 'P';

    // White major pieces
    const char whiteMajor[8] = {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};
    for (int c = 0; c < 8; ++c)
        board[7][c] = whiteMajor[c];
}

// ------------------------------------------------------------
// Main game loop:
// players alternate entering moves until a king is captured
// or a player quits, when a king is captured
// the game ends and the user is given the option to play again
// ------------------------------------------------------------
int main()
{
    // Game board (8x8 grid)
    char board[8][8];

    // Turn counter, even = White, odd = Black
    int turn = 0;

    // Controls if new game after king-capture
    bool playAgain = true;

    // Outer loop: allows full game restart after a king is captured
    while (playAgain)
    {
        // Reset board and turn counter for a new game
        initBoard(board);
        turn = 0;

        bool gameOver = false;

        cout << "Simple Chess game (king-capture)\n";
        cout << "Enter moves as: from to   e.g.  e2 e4\n";
        cout << "Type 'quit' to exit.\n";

        // Inner loop: runs one complete game session
        while (!gameOver)
        {
            printBoard(board);

            string from, to;

            // Display active player based on turn parity
            if (turn % 2 == 0)
                cout << "White to move: ";
            else
                cout << "Black to move: ";

            // Read source square
            cin >> from;
            if (!cin || from == "quit" || from == "exit")
                return 0;

            // Read target square
            cin >> to;
            if (!cin || to == "quit" || to == "exit")
                return 0;

            int fr, fc, tr, tc;

            // Convert algebraic notation to board indices
            if (!parseMove(from, to, fr, fc, tr, tc))
            {
                cout << "Invalid notation. Use like: e2 e4\n";
                continue;
            }

            // Validate source piece belongs to current player
            char src = board[fr][fc];
            if (!isPlayersPiece(src, turn))
            {
                cout << "You must move your own piece from a non-empty square.\n";
                continue;
            }

            // Prevent capturing own piece
            char dst = board[tr][tc];
            if (isPlayersPiece(dst, turn))
            {
                cout << "You cannot capture your own piece.\n";
                continue;
            }

            // Execute move
            applyMove(fr, fc, tr, tc, board);

            // Check if a king has been captured
            bool whiteWins = false, blackWins = false;
            if (checkWin(board, whiteWins, blackWins))
            {
                printBoard(board);

                if (whiteWins)
                    cout << "White wins! Black king captured.\n";
                else
                    cout << "Black wins! White king captured.\n";

                gameOver = true; // End this game session
            }
            else
            {
                ++turn; // Next player's turn
            }
        }

        // Ask player if they want to restart a new game
        char choice;
        cout << "Play again? (y/n): ";
        cin >> choice;

        // Only 'y' or 'Y' restarts the game
        if (choice != 'y' && choice != 'Y')
            playAgain = false;
    }

    cout << "Game over.\n";
    return 0;
}
