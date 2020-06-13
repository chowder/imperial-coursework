#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "Piece.h"
#include "ChessUtils.h"
#include <map>
#include <string>

typedef std::map <std::string, Piece*> Board;

class ChessBoard {
  bool _isWhiteTurn;
  bool _isCheck;
  bool _isCheckmate;
  bool _isStalemate;
  bool _isGraphical;

  Board _board;
  Board _sandbox(Board &board);
  void _setBoard();
  void _freeBoard(Board &board);
  void _printBoard(Board &board);
  void _printError(int err, std::string fromPosition, std::string toPosition);
  void _switchTurns();
  void _setGraphical();
  int _isGameOver();

  bool _isValidInput(std::string toPosition, std::string fromPosition);
  int _tryMove(std::string toPosition, std::string fromPosition);
  std::string _findKing(Colour colour, Board &board);
  void _move(std::string fromPosition, std::string toPosition, Board &board,
             bool verbose=false);
  bool _isKingSafe(Colour colour, Board &board);
  void _adjSpecialMoves(std::string fromPosition, std::string toPosition,
                        Board &board, int move);
  bool _hasValidMove(Colour colour, Board &board);

public:
  ChessBoard();
  ~ChessBoard();
  int submitMove(std::string fromPosition, std::string toPosition);
  void resetBoard();
};

#endif
