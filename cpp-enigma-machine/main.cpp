#include "plugboard.h"
#include "rotor.h"
#include "reflector.h"
#include "enigma.h"
#include "errors.h"
#include <iostream>
#include <string>
#include <cctype>
#include <limits>

using namespace std;

int main(int argc, char** argv){

  StatusCode status;
  EnigmaMachine enigma;

  if (status = enigma.configure(argc, argv)){
    status.print_error();
    return status.get_error_code();
  }

  char character;
  char parsed;

  cin >> ws;
  while(cin.get(character)){
    status = enigma.parse(character, parsed);
    if (status) {
      status.print_error();
      return status.get_error_code();
    }
    cout << parsed;
    cin >> ws;
  }
  return status.get_error_code();
}
