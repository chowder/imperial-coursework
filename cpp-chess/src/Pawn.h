#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"
#include <string>

class Pawn: public Piece {

public:
  Pawn* clone();
  Pawn(Colour colour) : Piece(colour) {};
  int isValidMove(std::string fromPosition, std::string toPosition,
                  Board &board, int &move);
  std::string getSymbol();
  std::string getName();
};

#endif
