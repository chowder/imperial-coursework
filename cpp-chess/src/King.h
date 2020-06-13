#ifndef KING_H
#define KING_H

#include "Piece.h"
#include <string>

class King: public Piece {
  bool _isValidCastle(std::string fromPosition, std::string toPosition,
                      Board &board, int &move);
public:
  King* clone();
  King(Colour colour) : Piece(colour) {
    setKing(true);
  };
  int isValidMove(std::string fromPosition, std::string toPosition,
                  Board &board, int &move);
  std::string getSymbol();
  std::string getName();
};

#endif
