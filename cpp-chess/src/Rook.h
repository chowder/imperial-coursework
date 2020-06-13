#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"
#include <string>

class Rook: public Piece {

public:
  Rook* clone();
  Rook(Colour colour) : Piece(colour) {};
  int isValidMove(std::string fromPosition, std::string toPosition,
                  Board &board, int &move);
  std::string getSymbol();
  std::string getName();
};

#endif
