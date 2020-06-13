#include "Queen.h"
#include <iostream>
#include <string>

using namespace std;
using namespace ChessUtils;

Queen* Queen::clone(){
  Queen* queen = new Queen(isWhite() ? White : Black);
  queen->setMoved(hasMoved());
  return queen;
}

int Queen::isValidMove(string fromPosition, string toPosition, Board &board, int &move){
  // Check if it's trying to move on itself
  if (fromPosition == toPosition)
    return ChessErr::DEST_EQUALS_SOURCE;
  // Check if queen movement is within a same file or rank, or diagonal
  bool sameFile = inSameFile(fromPosition, toPosition);
  bool sameRank = inSameRank(fromPosition, toPosition);
  bool sameDiag = inSameDiagonal(fromPosition, toPosition);
  if (!sameFile && !sameRank && !sameDiag)
    return ChessErr::INVALID_MOVE;
  // Check if it is obstructed
  bool obstructed = sameDiag ? isDiaObstructed(fromPosition, toPosition, board):
                    sameFile ? isVerObstructed(fromPosition, toPosition, board):
                               isHorObstructed(fromPosition, toPosition, board);
  if (obstructed)
    return ChessErr::OBSTRUCTED_MOVE;
  // Check if it's a friendly at the destination
  if (isFriend(toPosition, board))
    return ChessErr::FRIEND_AT_DEST;
  // All good, no error
  return ChessErr::NO_ERROR;
}

string Queen::getSymbol(){
  return isWhite() ? "♛" : "♕";
}

string Queen::getName(){
  return "Queen";
}
