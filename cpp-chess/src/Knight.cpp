#include "Knight.h"
#include <iostream>
#include <string>

using namespace std;
using namespace ChessUtils;

Knight* Knight::clone(){
  Knight* knight = new Knight(isWhite() ? White : Black);
  knight->setMoved(hasMoved());
  return knight;
}

int Knight::isValidMove(string fromPosition, string toPosition, Board &board, int &move){
  // Check if it's trying to move on itself
  if (fromPosition == toPosition)
    return ChessErr::DEST_EQUALS_SOURCE;
  // Check if knight movement is in an L shape of dimensions 2, 1
  int rankDelta = abs(toPosition[RANK_INDEX] - fromPosition[RANK_INDEX]);
  int fileDelta = abs(toPosition[FILE_INDEX] - fromPosition[FILE_INDEX]);
  // For the dimension condition to be satisfied, the rank and file advancement
  // must be greater than one, and their sum must equal to 3
  if (!(rankDelta > 0 && fileDelta > 0 && (rankDelta + fileDelta) == 3))
    return ChessErr::INVALID_MOVE;
  // Check if it's a friendly at the destination
  if (isFriend(toPosition, board))
    return ChessErr::FRIEND_AT_DEST;
  // All good, no error
  return ChessErr::NO_ERROR;
}

string Knight::getSymbol(){
  return isWhite() ? "♞" : "♘";
}

string Knight::getName(){
  return "Knight";
}
