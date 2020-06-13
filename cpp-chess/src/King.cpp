#include "King.h"
#include <iostream>
#include <string>

using namespace std;
using namespace ChessUtils;

King* King::clone(){
  King* king = new King(isWhite() ? White : Black);
  king->setMoved(hasMoved());
  king->setKing(true);
  return king;
}

int King::isValidMove(string fromPosition, string toPosition, Board &board, int &moveType){
  // Check if it's trying to move onto itself
  if (fromPosition == toPosition)
    return ChessErr::DEST_EQUALS_SOURCE;
  // Check for white castling move
  if (_isValidCastle(fromPosition, toPosition, board, moveType))
    return ChessErr::NO_ERROR;
  // Check for rank and file advancements
  int rankDelta = toPosition[RANK_INDEX] - fromPosition[RANK_INDEX];
  int fileDelta = toPosition[FILE_INDEX] - fromPosition[FILE_INDEX];
  if (abs(rankDelta) > 1 || abs(fileDelta) > 1){
    return ChessErr::INVALID_MOVE;
  }
  // Check if it's a friendly at the destination
  if (isFriend(toPosition, board))
    return ChessErr::FRIEND_AT_DEST;
  // All good, no error
  moveType = REGULAR_MOVE;
  return ChessErr::NO_ERROR;
}

bool King::_isValidCastle(string fromPosition, string toPosition, Board &board, int &moveType){
  // Check if king has been moved
  if (hasMoved()) return false;
  // Check if the castle is within the same rank
  if (fromPosition[RANK_INDEX] != toPosition[RANK_INDEX])
    return false;
  // Check if destination is a valid castling square (we can do this since there's only 4 of them)
  if (toPosition != "C1" && toPosition != "G1" &&
      toPosition != "C8" && toPosition != "G8")
    return false;
  // Check if the castling rook have been moved
  string rookPosition = toPosition;
  rookPosition[FILE_INDEX] = (toPosition[FILE_INDEX] == 'C') ? 'A' : 'H';
  if (board[rookPosition] && board[rookPosition]->hasMoved())
    return false;
  // Check if the squares inbetween are occupied
  if (isHorObstructed(fromPosition, rookPosition, board))
    return false;
  // Check if king passes through an attacked square
  string passThrough = fromPosition;
  passThrough[FILE_INDEX] = (toPosition[FILE_INDEX] == 'C') ? 'D' : 'F';
  for (auto piece: board){
    if (piece.second && piece.second->isWhite() != isWhite()){
      // we reuse the moveType variable here for convenience's sake, but we will
      // change it back if the test fails in isValidMove()
      if (!piece.second->isValidMove(piece.first, passThrough, board, moveType))
        return false;
    }
  }
  // Valid move, check and send proper move type
  moveType = CASTLING_MOVE;
  return true;
}

string King::getSymbol(){
  return isWhite() ? "♚" : "♔";
}

string King::getName(){
  return "King";
}
