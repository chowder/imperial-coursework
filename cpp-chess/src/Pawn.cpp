#include "Pawn.h"
#include <iostream>
#include <string>

using namespace std;
using namespace ChessUtils;

Pawn* Pawn::clone(){
  Pawn* pawn = new Pawn(isWhite() ? White : Black);
  pawn->setMoved(hasMoved());
  pawn->setEnPassant(enPassant());
  return pawn;
}

int Pawn::isValidMove(string fromPosition, string toPosition, Board &board, int &moveType){
  // Check if it's trying to move onto itself
  if (fromPosition == toPosition)
    return ChessErr::DEST_EQUALS_SOURCE;
  // Check if it's trying to perform a pawn capture
  if (inSameDiagonal(fromPosition, toPosition)){
    // Check if movement (relative to player) is forward
    int rankDelta = toPosition[RANK_INDEX] - fromPosition[RANK_INDEX];
    rankDelta *= isWhite() ? 1 : -1;
    if (rankDelta != 1)
      return ChessErr::INVALID_MOVE;
    // Check if destination is an enemy piece
    if (isEnemy(toPosition, board))
      return ChessErr::NO_ERROR;
    // Check for en passant pawn capture
    string position {toPosition[FILE_INDEX], fromPosition[RANK_INDEX]};
    // If en passant square is empty
    if (!board[position])
      return ChessErr::INVALID_MOVE;
    // Verify if square contains a pawn vulnerable to en passant
    if (board[position]->getName() == "Pawn" && board[position]->enPassant()){
      moveType = EN_PASSANT_MOVE;
      return ChessErr::NO_ERROR;
    }
    return ChessErr::INVALID_MOVE;
  }

  // Check if there's a friendly at the position
  if (isFriend(toPosition, board))
    return ChessErr::FRIEND_AT_DEST;
  // Check if the movement is at least within a file
  if (!inSameFile(fromPosition, toPosition))
    return ChessErr::INVALID_MOVE;
  // Check if rank advancement is valid
  int rankDeltaAllowed = (hasMoved() ? 1 : 2);
  int rankDelta = toPosition[RANK_INDEX] - fromPosition[RANK_INDEX];
  rankDelta *= isWhite() ? 1 : -1;
  if (rankDelta > rankDeltaAllowed || rankDelta < 0)
    return ChessErr::INVALID_MOVE;
  // Check if it's (vertically) obstructed or if a piece is at the destination
  if (isVerObstructed(fromPosition, toPosition, board) || board[toPosition])
    return ChessErr::OBSTRUCTED_MOVE;
  // All good, indicate pawn double step move (to set en passant later)
  if (abs(rankDelta) == 2)
    moveType = PAWN_DOUBLE_STEP;
  return ChessErr::NO_ERROR;
}

string Pawn::getSymbol(){
  return isWhite() ? "♟" : "♙";
}

string Pawn::getName(){
  return "Pawn";
}
