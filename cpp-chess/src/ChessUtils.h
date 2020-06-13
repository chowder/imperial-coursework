#ifndef CHESSUTILS_H
#define CHESSUTILS_H

#include "Piece.h"
#include <map>
#include <string>
#include <iostream>

namespace ChessUtils {
  // Reference constants
  const int FILE_INDEX = 0;
  const int RANK_INDEX = 1;

  const char RANK_BEGIN = '1';
  const char RANK_END = '8';
  const char FILE_BEGIN = 'A';
  const char FILE_END = 'H';
  const int RANK_SIZE = RANK_END - RANK_BEGIN;
  const int FILE_SIZE = FILE_END - FILE_BEGIN;

  const char WHITE_KING_RANK = '1';
  const char WHITE_PAWN_RANK = '2';
  const char BLACK_PAWN_RANK = '7';
  const char BLACK_KING_RANK = '8';

  // Special move types
  const int REGULAR_MOVE = 0;
  const int PAWN_DOUBLE_STEP = 1;
  const int EN_PASSANT_MOVE = 2;
  const int PAWN_PROMOTION_MOVE = 3;
  const int CASTLING_MOVE = 4;
}

namespace ChessErr {
  const int NO_ERROR = 0;
  const int GAME_OVER_WIN = 1;
  const int GAME_OVER_STALEMATE = 2;

  const int INVALID_MOVE = 10;
  const int FRIEND_AT_DEST = 11;
  const int OBSTRUCTED_MOVE = 12;
  const int DEST_EQUALS_SOURCE = 13;
  const int UNSAFE_MOVE = 14;
  const int KING_STILL_IN_CHECK = 15;

  const int INVALID_INPUT = 30;
  const int NULL_PIECE_SELECTED = 31;
  const int ENEMY_PIECE_SELECTED = 32;
  const int CASTLING_OUT_OF_CHECK = 33;
}

#endif
