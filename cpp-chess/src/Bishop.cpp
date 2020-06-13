#include "Bishop.h"
#include <iostream>
#include <string>

using namespace std;
using namespace ChessUtils;

Bishop* Bishop::clone(){
  Bishop* bishop = new Bishop(isWhite() ? White : Black);
  bishop->setMoved(hasMoved());
  return bishop;
}

int Bishop::isValidMove(string fromPosition, string toPosition, Board &board, int &move){
  // Check if it's trying to move on itself
  if (fromPosition == toPosition)
    return ChessErr::DEST_EQUALS_SOURCE;
  // Check if bishop movement is within a diagonal
  if (!inSameDiagonal(fromPosition, toPosition))
    return ChessErr::INVALID_MOVE;
  // Check if it is obstructed
  if (isDiaObstructed(fromPosition, toPosition, board))
    return ChessErr::OBSTRUCTED_MOVE;
  // Check if it's a friendly at the destination
  if (isFriend(toPosition, board))
    return ChessErr::FRIEND_AT_DEST;
  // All good, no error
  return ChessErr::NO_ERROR;
}

string Bishop::getSymbol(){
  return isWhite() ? "♝" : "♗";
}

string Bishop::getName(){
  return "Bishop";
}
