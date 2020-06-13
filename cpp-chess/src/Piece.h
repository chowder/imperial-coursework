#ifndef PIECE_H
#define PIECE_H

#include "ChessUtils.h"
#include <string>
#include <map>

enum Colour { Black, White };

class Piece{
  bool _isWhite = false;
  bool _isKing = false;
  bool _hasMoved = false;
  bool _enPassant = false;

public:
  typedef std::map <std::string, Piece*> Board;
  Piece(Colour colour);

  // Get piece properties
  bool isWhite();
  bool isKing();
  bool hasMoved();
  bool enPassant();

  // Set piece properties
  void setWhite(bool set);
  void setKing(bool set);
  void setMoved(bool set);
  void setEnPassant(bool set);

  // Movement properties
  bool inSameFile(std::string fromPosition, std::string toPosition);
  bool inSameRank(std::string fromPosition, std::string toPosition);
  bool inSameDiagonal(std::string fromPosition, std::string toPosition);
  bool isFriend(std::string position, Board &board);
  bool isEnemy(std::string position, Board &board);
  bool isVerObstructed(std::string fromPosition, std::string toPosition,
                               Board &board);
  bool isHorObstructed(std::string fromPosition, std::string toPosition,
                               Board &board);
  bool isDiaObstructed(std::string fromPosition, std::string toPosition,
                               Board &board);

  // Pure virtual functions
  virtual Piece* clone() = 0;
  virtual int isValidMove(std::string fromPosition, std::string toPosition,
                          Board &board, int &moveType) = 0;
  virtual std::string getSymbol() = 0;
  virtual std::string getName() = 0;
  virtual ~Piece(){};
};

#endif
