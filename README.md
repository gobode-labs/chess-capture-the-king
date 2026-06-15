# Chess (Capture‑the‑King) — C++ Console Game

A simplified two‑player chess game implemented in modern C++.
Players enter moves using algebraic notation (e.g., `e2 e4`).
The game ends when one king is captured.

## Gameplay Rules

- White uses uppercase pieces (`R N B Q K P`)
- Black uses lowercase pieces (`r n b q k p`)
- Moves use `"from to"` notation, e.g.:


- No full chess legality rules:
- You must move your own piece
- You cannot capture your own piece
- Game ends when a king is captured
- After a game ends, you can choose to play again

## Core Functions

- `parseMove` — converts algebraic notation to board indices
- `isPlayersPiece` — validates piece ownership using ASCII ranges
- `printBoard` — renders the 8×8 board with rank/file labels
- `applyMove` — moves a piece and overwrites captured pieces
- `checkWin` — detects when a king is missing
- `initBoard` — sets up the starting position
- `main` — game loop + replay loop

## Build & Run

```bash
make
./chess
