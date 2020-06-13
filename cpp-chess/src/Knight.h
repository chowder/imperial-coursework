#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"
#include <string>

class Knight: public Piece {

public:
  Knight* clone();
  Knight(Colour colour) : Piece(colour) {};
  int isValidMove(std::string fromPosition, std::string toPosition,
                  Board &board, int &move);
  std::string getSymbol();
  std::string getName();
};

#endif
