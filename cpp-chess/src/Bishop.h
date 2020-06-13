#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"
#include <string>

class Bishop: public Piece {

public:
  Bishop* clone();
  Bishop(Colour colour) : Piece(colour) {};
  int isValidMove(std::string fromPosition, std::string toPosition,
                  Board &board, int &move);
  std::string getSymbol();
  std::string getName();
};

#endif
