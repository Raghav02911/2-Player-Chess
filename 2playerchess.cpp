#include <iostream>
#include <cmath>
using namespace std;

// Base class for a chess piece
class Piece {
public:
    bool isWhite; // true for white, false for black
    Piece(bool isWhite) : isWhite(isWhite) {}
    virtual ~Piece() = default;
    virtual bool isValidMove(int x1, int y1, int x2, int y2, Piece* board[8][8]) = 0;
    virtual char getSymbol() = 0;
};

// Derived classes for specific chess pieces
class King : public Piece {
public:
    King(bool isWhite) : Piece(isWhite) {}
    bool isValidMove(int x1, int y1, int x2, int y2, Piece* board[8][8]) override {
        return abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1;
    }
    char getSymbol() override { return isWhite ? 'K' : 'k'; }
};

class Queen : public Piece {
public:
    Queen(bool isWhite) : Piece(isWhite) {}
    bool isValidMove(int x1, int y1, int x2, int y2, Piece* board[8][8]) override {
        return (x1 == x2 || y1 == y2 || abs(x1 - x2) == abs(y1 - y2));
    }
    char getSymbol() override { return isWhite ? 'Q' : 'q'; }
};

class Rook : public Piece {
public:
    Rook(bool isWhite) : Piece(isWhite) {}
    bool isValidMove(int x1, int y1, int x2, int y2, Piece* board[8][8]) override {
        return (x1 == x2 || y1 == y2);
    }
    char getSymbol() override { return isWhite ? 'R' : 'r'; }
};

class Bishop : public Piece {
public:
    Bishop(bool isWhite) : Piece(isWhite) {}
    bool isValidMove(int x1, int y1, int x2, int y2, Piece* board[8][8]) override {
        return abs(x1 - x2) == abs(y1 - y2);
    }
    char getSymbol() override { return isWhite ? 'B' : 'b'; }
};

class Knight : public Piece {
public:
    Knight(bool isWhite) : Piece(isWhite) {}
    bool isValidMove(int x1, int y1, int x2, int y2, Piece* board[8][8]) override {
        return (abs(x1 - x2) == 2 && abs(y1 - y2) == 1) || (abs(x1 - x2) == 1 && abs(y1 - y2) == 2);
    }
    char getSymbol() override { return isWhite ? 'N' : 'n'; }
};

class Pawn : public Piece {
public:
    Pawn(bool isWhite) : Piece(isWhite) {}
    bool isValidMove(int x1, int y1, int x2, int y2, Piece* board[8][8]) override {
        int direction = isWhite ? -1 : 1;
        if (x1 + direction == x2 && y1 == y2 && board[x2][y2] == nullptr)
            return true;
        if (x1 + direction == x2 && abs(y1 - y2) == 1 && board[x2][y2] != nullptr && board[x2][y2]->isWhite != isWhite)
            return true;
        if ((x1 == 6 && isWhite || x1 == 1 && !isWhite) && x1 + 2 * direction == x2 && y1 == y2 && board[x2][y2] == nullptr)
            return true;
        return false;
    }
    char getSymbol() override { return isWhite ? 'P' : 'p'; }
};

// Board class
class Board {
private:
    Piece* board[8][8];
public:
    Board() {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                board[i][j] = nullptr;
        setupPieces();
    }

    ~Board() {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                delete board[i][j];
    }

    void setupPieces() {
        // Place pawns
        for (int i = 0; i < 8; ++i) {
            board[1][i] = new Pawn(false);
            board[6][i] = new Pawn(true);
        }
        // Place rooks
        board[0][0] = new Rook(false);
        board[0][7] = new Rook(false);
        board[7][0] = new Rook(true);
        board[7][7] = new Rook(true);
        // Place knights
        board[0][1] = new Knight(false);
        board[0][6] = new Knight(false);
        board[7][1] = new Knight(true);
        board[7][6] = new Knight(true);
        // Place bishops
        board[0][2] = new Bishop(false);
        board[0][5] = new Bishop(false);
        board[7][2] = new Bishop(true);
        board[7][5] = new Bishop(true);
        // Place queens
        board[0][3] = new Queen(false);
        board[7][3] = new Queen(true);
        // Place kings
        board[0][4] = new King(false);
        board[7][4] = new King(true);
    }

    bool movePiece(int x1, int y1, int x2, int y2, bool isWhiteTurn) {
        if (!isValidPosition(x1, y1) || !isValidPosition(x2, y2)) return false;
        if (board[x1][y1] == nullptr || board[x1][y1]->isWhite != isWhiteTurn) return false;
        if (board[x1][y1]->isValidMove(x1, y1, x2, y2, board)) {
            if (board[x2][y2] != nullptr && board[x2][y2]->isWhite == isWhiteTurn) return false;
            delete board[x2][y2];
            board[x2][y2] = board[x1][y1];
            board[x1][y1] = nullptr;
            return true;
        }
        return false;
    }

    bool isValidPosition(int x, int y) {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    void display() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board[i][j] != nullptr)
                    cout << board[i][j]->getSymbol() << " ";
                else
                    cout << ". ";
            }
            cout << endl;
        }
    }
};

// ChessGame class
class ChessGame {
private:
    Board board;
    bool whiteTurn;
public:
    ChessGame() : whiteTurn(true) {}

    void play() {
        int x1, y1, x2, y2;
        while (true) {
            board.display();
            cout << (whiteTurn ? "White" : "Black") << "'s turn. Enter move (x1 y1 x2 y2): ";
            cin >> x1 >> y1 >> x2 >> y2;
            if (board.movePiece(x1, y1, x2, y2, whiteTurn)) {
                whiteTurn = !whiteTurn;
            } else {
                cout << "Invalid move. Try again." << endl;
            }
        }
    }
};

int main() {
    ChessGame game;
    game.play();
    return 0;
}
