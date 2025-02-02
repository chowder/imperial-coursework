#include "ChessBoard.h"
#include "Piece.h"
#include "Pawn.h"
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>

using namespace std;

int main(){

  cout << "===========================" << endl;
  cout << "Testing the Chess Engine" << endl;
  cout << "===========================" << endl;
  cout << endl;

  ChessBoard cb;
  cout << endl;

  cb.submitMove("D7", "D6");
  cout << endl;

  cb.submitMove("D4", "H6");
  cout << endl;

  cb.submitMove("D2", "D4");
  cout << endl;

  cb.submitMove("F8", "B4");
  cout << endl;

  cout << "===========================" << endl;
  cout << "Alekhine vs. Vasic (1931)" << endl;
  cout << "===========================" << endl;
  cout << endl;

  cb.resetBoard();
  cout << endl;

  cb.submitMove("E2", "E4");
  cb.submitMove("E7", "E6");
  cout << endl;

  cb.submitMove("D2", "D4");
  cb.submitMove("D7", "D5");
  cout << endl;

  cb.submitMove("B1", "C3");
  cb.submitMove("F8", "B4");
  cout << endl;

  cb.submitMove("F1", "D3");
  cb.submitMove("B4", "C3");
  cout << endl;

  cb.submitMove("B2", "C3");
  cb.submitMove("H7", "H6");
  cout << endl;

  cb.submitMove("C1", "A3");
  cb.submitMove("B8", "D7");
  cout << endl;

  cb.submitMove("D1", "E2");
  cb.submitMove("D5", "E4");
  cout << endl;

  cb.submitMove("D3", "E4");
  cb.submitMove("G8", "F6");
  cout << endl;

  cb.submitMove("E4", "D3");
  cb.submitMove("B7", "B6");
  cout << endl;

  cb.submitMove("E2", "E6");
  cb.submitMove("F7", "E6");
  cout << endl;

  cb.submitMove("D3", "G6");
  cout << endl;

  cout << "===========================" << endl;
  cout << "Fool's Mate" << endl;
  cout << "===========================" << endl;
  cout << endl;

  cb.resetBoard();
  cout << endl;

  cb.submitMove("F2", "F3");
  cb.submitMove("E7", "E5");
  cout << endl;

  cb.submitMove("G2", "G4");
  cb.submitMove("D8", "H4");
  cout << endl;

  cout << "===========================" << endl;
  cout << "Scholar's Mate" << endl;
  cout << "===========================" << endl;
  cout << endl;

  cb.resetBoard();
  cout << endl;

  cb.submitMove("E2", "E4");
  cb.submitMove("E7", "E5");
  cout << endl;

  cb.submitMove("F1", "C4");
  cb.submitMove("B8", "C6");
  cout << endl;

  cb.submitMove("D1", "H5");
  cb.submitMove("G8", "F6");
  cout << endl;

  cb.submitMove("H5", "F7");
  cout << endl;

  // ChessBoard cb;
  //
  // string from, to;
  // int err;
  //
  // while (!cin.eof()){
  //   cout << "Submit a move:" << endl;
  //   cin >> from;
  //   cin >> to;
  //   err = cb.submitMove(from, to);
  //   cout << err << endl;
  // }

  return ChessErr::NO_ERROR;
}
