#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"
#include <string>

class Queen: public Piece {

public:
  Queen* clone();
  Queen(Colour colour) : Piece(colour) {};
  int isValidMove(std::string fromPosition, std::string toPosition,
                  Board &board, int &move);
  std::string getSymbol();
  std::string getName();
};

#endif
