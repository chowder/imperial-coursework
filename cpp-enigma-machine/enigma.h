#ifndef ENIGMA_H
#define ENIGMA_H

#include "errors.h"
#include "rotor.h"
#include "plugboard.h"
#include "reflector.h"
#include <vector>

/* The EnigmaMachine class emulates an enigma machine */
class EnigmaMachine{
  Plugboard plugboard_;
  Reflector reflector_;
  int number_of_rotors_;
  // Pointer to the rightmost rotor
  Rotor* rotor_head_ = nullptr;
  // Pointer to the leftmost rotor
  Rotor* rotor_tail_ = nullptr;

  StatusCode get_rotor_positions_(char filename[], std::vector<int> &positions);
  StatusCode verify_inputs_(int argc, char** argv);

 public:
  EnigmaMachine(){};
  StatusCode configure(int argc, char** argv);
  StatusCode parse(char character, char &parsed);
  ~EnigmaMachine();
};

#endif
