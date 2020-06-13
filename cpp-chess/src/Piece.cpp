#include "Piece.h"
#include <iostream>

using namespace std;
using namespace ChessUtils;

typedef std::map <std::string, Piece*> Board;

Piece::Piece(Colour colour){
  _isWhite = (colour == White);
}


bool Piece::isWhite(){
  return _isWhite;
}


bool Piece::isKing(){
  return _isKing;
}


bool Piece::enPassant(){
  return _enPassant;
}


bool Piece::hasMoved(){
  return _hasMoved;
}


void Piece::setWhite(bool set){
  _isWhite = set;
}


void Piece::setKing(bool set){
  _isKing = set;
}


void Piece::setMoved(bool set){
  _hasMoved = set;
}


void Piece::setEnPassant(bool set){
  _enPassant = set;
}


bool Piece::inSameFile(string fromPosition, string toPosition){
  return fromPosition.at(FILE_INDEX) == toPosition.at(FILE_INDEX);
}


bool Piece::inSameRank(string fromPosition, string toPosition){
  return fromPosition.at(RANK_INDEX) == toPosition.at(RANK_INDEX);
}


bool Piece::inSameDiagonal(string fromPosition, string toPosition){
  return abs(fromPosition.at(RANK_INDEX) - toPosition.at(RANK_INDEX)) ==
         abs(fromPosition.at(FILE_INDEX) - toPosition.at(FILE_INDEX));
}


bool Piece::isFriend(string position, Board &board){
  return board[position] ? _isWhite == board[position]->isWhite() : false;
}


bool Piece::isEnemy(string position, Board &board){
  return board[position] ? _isWhite != board[position]->isWhite() : false;
}


bool Piece::isVerObstructed(std::string fromPosition,
                            std::string toPosition,
                            Board &board){
  int delta = toPosition[RANK_INDEX] - fromPosition[RANK_INDEX];
  string position = (delta > 0) ? fromPosition : toPosition;
  for (int i = 1; i < abs(delta); i++){
    position[RANK_INDEX] += 1;
    if (board[position])
      return true;
  }
  return false;
}


bool Piece::isHorObstructed(std::string fromPosition,
                            std::string toPosition,
                            Board &board){
  int delta = toPosition[FILE_INDEX] - fromPosition[FILE_INDEX];
  string position = (delta > 0) ? fromPosition : toPosition;
  for (int i = 1; i < abs(delta); i++){
    position[FILE_INDEX] += 1;
    if (board[position])
      return true;
  }
  return false;
}


bool Piece::isDiaObstructed(std::string fromPosition,
                            std::string toPosition,
                            Board &board){
  int file_delta = toPosition[FILE_INDEX] - fromPosition[FILE_INDEX];
  int rank_delta = toPosition[RANK_INDEX] - fromPosition[RANK_INDEX];
  bool gradient = (file_delta * rank_delta) > 0;
  string lower_position = (file_delta > 0) ? fromPosition : toPosition;
  for (int i = 1; i < abs(file_delta); i++){
    lower_position[FILE_INDEX] += 1;
    lower_position[RANK_INDEX] += gradient ? 1 : -1;
    if (board[lower_position])
      return true;
  }
  return false;
}
