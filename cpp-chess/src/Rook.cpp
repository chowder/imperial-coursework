#include "Rook.h"
#include <iostream>
#include <string>

using namespace std;
using namespace ChessUtils;

Rook* Rook::clone(){
  Rook* rook = new Rook(isWhite() ? White : Black);
  rook->setMoved(hasMoved());
  return rook;
}

int Rook::isValidMove(string fromPosition, string toPosition, Board &board, int &move){
  // Check if it's trying to move on itself
  if (fromPosition == toPosition)
    return ChessErr::DEST_EQUALS_SOURCE;
  // Check if rook movement is within a same file or rank
  bool sameFile = inSameFile(fromPosition, toPosition);
  bool sameRank = inSameRank(fromPosition, toPosition);
  if (!sameFile && !sameRank)
    return ChessErr::INVALID_MOVE;
  // Check if it is obstructed
  bool obstructed = sameFile ? isVerObstructed(fromPosition, toPosition, board):
                               isHorObstructed(fromPosition, toPosition, board);
  if (obstructed)
    return ChessErr::OBSTRUCTED_MOVE;
  // Check if it's a friendly at the destination
  if (isFriend(toPosition, board))
    return ChessErr::FRIEND_AT_DEST;
  // All good, no error
  return ChessErr::NO_ERROR;
}

string Rook::getSymbol(){
  return isWhite() ? "♜" : "♖";
}

string Rook::getName(){
  return "Rook";
}
