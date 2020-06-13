#include "ChessBoard.h"
#include "ChessUtils.h"
#include "Piece.h"
#include "Pawn.h"
#include "King.h"
#include "Rook.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include <map>
#include <string>
#include <iostream>

using namespace std;
using namespace ChessUtils;

ChessBoard::ChessBoard(){
  _isWhiteTurn = true;
  _isCheck = false;
  _isCheckmate = false;
  _isStalemate = false;
  _isGraphical = false;
  _setBoard();
  cout << "A new game is started!" << endl;
}


void ChessBoard::_setBoard(){
  // Set black pawns
  string position;
  for (char file = FILE_BEGIN; file <= FILE_END; file++){
    position = {file, BLACK_PAWN_RANK};
    _board.insert({position, new Pawn(Black)});
    position.clear();
  }
  // Set remaining black pieces
  _board.insert({"A8", new Rook(Black)});
  _board.insert({"B8", new Knight(Black)});
  _board.insert({"C8", new Bishop(Black)});
  _board.insert({"D8", new Queen(Black)});
  _board.insert({"E8", new King(Black)});
  _board.insert({"F8", new Bishop(Black)});
  _board.insert({"G8", new Knight(Black)});
  _board.insert({"H8", new Rook(Black)});

  // Set white pawns
  for (char file = FILE_BEGIN; file <= FILE_END; file++){
    position = {file, WHITE_PAWN_RANK};
    _board.insert({position, new Pawn(White)});
    position.clear();
  }

  // Set remaining white pieces
  _board.insert({"A1", new Rook(White)});
  _board.insert({"B1", new Knight(White)});
  _board.insert({"C1", new Bishop(White)});
  _board.insert({"D1", new Queen(White)});
  _board.insert({"E1", new King(White)});
  _board.insert({"F1", new Bishop(White)});
  _board.insert({"G1", new Knight(White)});
  _board.insert({"H1", new Rook(White)});

  // Populate the remaining squares with null pointers
  for (char file = FILE_BEGIN; file <= FILE_END; file++){
    for (char rank = RANK_BEGIN; rank <= RANK_END; rank++){
      position = {file, rank};
      _board.insert({position, nullptr});
      position.clear();
    }
  }
}


void ChessBoard::_freeBoard(Board &board){
  for (auto item: board){
    if (item.second){
      delete item.second;
      item.second = nullptr;
    }
  }
  board.clear();
}


Board ChessBoard::_sandbox(Board &board){
  Board sandbox;
  Piece* piece;
  for (auto &p: board){
    piece = p.second ? p.second->clone() : nullptr;
    sandbox.insert({p.first, piece});
  }
  return sandbox;
}


bool ChessBoard::_isValidInput(std::string toPosition, std::string fromPosition){
  return (toPosition[FILE_INDEX] >= FILE_BEGIN &&
          toPosition[FILE_INDEX] <= FILE_END &&
          toPosition[RANK_INDEX] >= RANK_BEGIN &&
          toPosition[RANK_INDEX] <= RANK_END &&
          fromPosition[FILE_INDEX] >= FILE_BEGIN &&
          fromPosition[FILE_INDEX] <= FILE_END &&
          fromPosition[RANK_INDEX] >= RANK_BEGIN &&
          fromPosition[RANK_INDEX] <= RANK_END);
}


void ChessBoard::_move(string fromPosition, string toPosition, Board &board, bool verbose){
  if (verbose) cout << (board[fromPosition]->isWhite() ? "White's " : "Black's ")
                    << board[fromPosition]->getName()
                    << " moves from " << fromPosition
                    << " to " << toPosition;
  if (board[toPosition]){
    if (verbose) cout << " taking "
                      << (board[toPosition]->isWhite() ? "White's " : "Black's ")
                      << board[toPosition]->getName();
    delete board[toPosition];
  }
  if (verbose) cout << endl;
  board[toPosition] = board[fromPosition];
  board[fromPosition] = nullptr;
  board[toPosition]->setMoved(true);
  // Remove en passant for all squares
  for (auto &piece: board){
    if (piece.second && piece.second->getName() == "Pawn")
      piece.second->setEnPassant(false);
  }
}


void ChessBoard::_adjSpecialMoves(string fromPosition, string toPosition, Board &board, int moveType){
  switch(moveType){
    case PAWN_DOUBLE_STEP:{
      board[toPosition]->setEnPassant(true);
      break;
    }
    case EN_PASSANT_MOVE:{
      string position {toPosition[FILE_INDEX], fromPosition[RANK_INDEX]};
      delete board[position];
      board[position] = nullptr;
      break;
    }
    case CASTLING_MOVE:{
      // Get the position of the rook involved in the castling
      string rookPosition {toPosition[FILE_INDEX] == 'C' ? 'A' : 'H',
                           toPosition[RANK_INDEX]};
      // Get the position of the rook post-castling
      string castlePosition {toPosition[FILE_INDEX] == 'C' ? 'D' : 'F',
                             toPosition[RANK_INDEX]};
      _move(rookPosition, castlePosition, board);
      break;
    }
  }
}


bool ChessBoard::_hasValidMove(Colour colour, Board &board){
  bool isWhite = (colour == White);
  // For every piece of the board matching the color we want to find
  for (auto piece: board){
    if (piece.second && piece.second->isWhite() == isWhite){
      // Loop through every square on the board
      for (char file = FILE_BEGIN; file <= FILE_END; file++){
        for (char rank = RANK_BEGIN; rank <= RANK_END; rank++){
          // And try to make a move for that particular square
          int move = REGULAR_MOVE;
          string position {file, rank};
          if (!piece.second->isValidMove(piece.first, position, board, move)){
            // Test this move on a sandbox
            Board sandbox = _sandbox(board);
            _move(piece.first, position, sandbox);
            _adjSpecialMoves(piece.first, position, sandbox, move);
            // Test is king if safe under this move
            bool safe = isWhite ? _isKingSafe(White, sandbox):
                                  _isKingSafe(Black, sandbox);
            if (safe){
              _freeBoard(sandbox);
              return true;
            }
            _freeBoard(sandbox);
          }
        }
      }
    }
  }
  return false;
}


void ChessBoard::_printBoard(Board &board){
  // Print top frame
  cout << string("  +=================+") << endl;
  // Print board contents starting from rank 8 to 1
  string position;
  for (char rank = RANK_END; rank >= RANK_BEGIN ; rank--){
    // Prints left axis labels
    position += rank;
    cout << position << " | ";
    // Print every piece in each file in that rank
    for (char file = FILE_BEGIN; file <= FILE_END; file++){
      position.insert(0, 1, file);
      auto search = board.find(position);
      cout << ((search->second != nullptr) ? search->second->getSymbol() : ".");
      cout << " ";
      position.erase(0, 1);
    }
    // Print right frame
    cout << "|" << endl;
    position.clear();
  }
  // Print bottom frame
  cout << string("  +=================+") << endl;
  cout << string("    a b c d e f g h") << endl;
}


string ChessBoard::_findKing(Colour colour, Board &board){
  bool isWhite = (colour == White);
  for (auto p: board){
    if (p.second && p.second->isKing() && p.second->isWhite() == isWhite)
      return p.first;
  }
  // Ideally we will never reach here, a board has to have a king right?
  return "";
}


bool ChessBoard::_isKingSafe(Colour colour, Board &board){
  string kingPosition = _findKing(colour, board);
  int move, err;
  for (auto &piece: board){
    if (piece.second){
      // For each piece, try to capture the king
      err = piece.second->isValidMove(piece.first, kingPosition, board, move);
      // If it's a valid move then the king is not safe
      if (err == ChessErr::NO_ERROR){
        return false;
      }
    }
  }
  return true;
}


void ChessBoard::_switchTurns(){
  _isWhiteTurn = !_isWhiteTurn;
}


void ChessBoard::_setGraphical(){
  _isGraphical = !_isGraphical;
}


int ChessBoard::_isGameOver(){
  if (_isCheckmate){
    cout << (_isWhiteTurn ? "White" : "Black") << " is in checkmate!" << endl;
    return ChessErr::GAME_OVER_WIN;
  }
  else if (_isStalemate){
    cout << "Stalemate!" << endl;
    return ChessErr::GAME_OVER_STALEMATE;
  }
  return 0;
}


int ChessBoard::submitMove(string fromPosition, string toPosition){
  int err = _tryMove(fromPosition, toPosition);
  if (err) {
    _printError(err, fromPosition, toPosition);
    return err;
  }
  return ChessErr::NO_ERROR;
}


int ChessBoard::_tryMove(string fromPosition, string toPosition){
  int err;
  // Check game status
  err = _isGameOver();
  if (err) return err;

  // Check if input is good
  if (!_isValidInput(toPosition, fromPosition))
    return ChessErr::INVALID_INPUT;

  // Check if piece even exists
  if (!_board[fromPosition])
    return ChessErr::NULL_PIECE_SELECTED;

  // Check if selected piece belongs to the player
  if (_board[fromPosition]->isWhite() != _isWhiteTurn)
    return ChessErr::ENEMY_PIECE_SELECTED;

  // Check if it's a *legal* move for that piece
  int moveType = REGULAR_MOVE;
  err = _board[fromPosition]->isValidMove(fromPosition, toPosition, _board, moveType);
  if (err) return err;

  // Check if it's a castle, as you cannot castle out of a check
  if (_isCheck && moveType == CASTLING_MOVE)
    return ChessErr::CASTLING_OUT_OF_CHECK;

  // Create a temporary board to test if the move is *safe* to make
  // (i.e. does not result in the player's king to be or remain in a check)
  Board sandbox = _sandbox(_board);
  _move(fromPosition, toPosition, sandbox);
  bool safe = _isWhiteTurn ? _isKingSafe(White, sandbox):
                             _isKingSafe(Black, sandbox);
  _freeBoard(sandbox);
  if (!safe)
    return _isCheck ? ChessErr::KING_STILL_IN_CHECK: ChessErr::UNSAFE_MOVE;

  // Move is good, make move and perform adjustments for special moves
  _move(fromPosition, toPosition, _board, true);
  _adjSpecialMoves(fromPosition, toPosition, _board, moveType);

  // Perform checks... for check...
  safe = _isWhiteTurn ? _isKingSafe(Black, _board):
                        _isKingSafe(White, _board);
  _isCheck = !safe;

  // ... and checks for checkmate or stalemate (wow what a tongue twister)
  sandbox = _sandbox(_board);
  bool hasValidMove = _isWhiteTurn ? _hasValidMove(Black, sandbox):
                                     _hasValidMove(White, sandbox);
  _freeBoard(sandbox);
  if (!hasValidMove && !_isCheck) _isStalemate = true;
  if (!hasValidMove && _isCheck) _isCheckmate = true;

  // Switch turns and check if the game is over
  _switchTurns();

  err = _isGameOver();
  if (err){
    if (_isGraphical) _printBoard(_board);
    return err;
  }
  else if (_isCheck)
    cout << (_isWhiteTurn ? "White" : "Black") << " is in check!" << endl;

  // Otherwise print the board and continue
  if (_isGraphical) _printBoard(_board);
  return ChessErr::NO_ERROR;
}


void ChessBoard::resetBoard(){
  _freeBoard(_board);
  _isWhiteTurn = true;
  _isCheck = false;
  _isCheckmate = false;
  _isStalemate = false;
  _setBoard();
  _printBoard(_board);
}


void ChessBoard::_printError(int err, string fromPosition, string toPosition){
  using namespace ChessErr;
  switch(err){
    case GAME_OVER_WIN:
      cout << "Game is over, "
           << (_isWhiteTurn ? "Black" : "White") << " wins!" << endl;
      break;
    case GAME_OVER_STALEMATE:
      cout << "Game ended in a stalemate!" << endl;
      break;
    case INVALID_INPUT:
      cout << "Invalid input given; positions range from A-H and 1-8" << endl;
      break;
    case NULL_PIECE_SELECTED:
      cout << "There is no piece at " << fromPosition << "!" << endl;
      break;
    case ENEMY_PIECE_SELECTED:
      cout << "Tried to move an enemy piece at " << fromPosition << "!" << endl;
      break;
    case INVALID_MOVE:
    case FRIEND_AT_DEST:
    case OBSTRUCTED_MOVE:
    case DEST_EQUALS_SOURCE:
    case UNSAFE_MOVE:
    case KING_STILL_IN_CHECK:
    case CASTLING_OUT_OF_CHECK:
      cout << "Invalid move for " << (_isWhiteTurn ? "White" : "Black")
           << "'s " << _board[fromPosition]->getName()
           << " from " << fromPosition << " to " << toPosition << endl;
      switch(err){
        case FRIEND_AT_DEST:
          cout << "You can't capture your own piece!" << endl;
          break;
        case OBSTRUCTED_MOVE:
          cout << "There's another piece in the way!" << endl;
          break;
        case DEST_EQUALS_SOURCE:
          cout << "You can't make a move that... doesn't move." << endl;
          break;
        case UNSAFE_MOVE:
          cout << "That move puts your king in danger!" << endl;
          break;
        case KING_STILL_IN_CHECK:
          cout << "Your king is in check, fix that." << endl;
          break;
        case CASTLING_OUT_OF_CHECK:
          cout << "You cannot castle out of a check!" << endl;
          break;
      }
  }
}


ChessBoard::~ChessBoard(){
  _freeBoard(_board);
}
