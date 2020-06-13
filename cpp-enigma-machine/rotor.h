#ifndef ROTOR_H
#define ROTOR_H

#include "errors.h"
#include <map>
#include <fstream>
#include <vector>

/* The Rotor class simulates a rotor of an enigma machine using a linked-list
   implementation to indicate the neighbouring rotors */
class Rotor{
  std::map<char, char> rotor_map_;
  std::map<char, char> rotor_reverse_map_;
  std::vector<int> notches_;
  int position_;

  StatusCode get_config_(char filename[]);
  StatusCode check_form_(std::vector<std::string> &str_data, std::vector<int> &int_data);

public:
  Rotor(){};
  Rotor* next_rotor = nullptr;
  Rotor* prev_rotor = nullptr;

  StatusCode configure(char filename[], int position);
  void rotate();
  char forward_parse(char character) const;
  char reverse_parse(char character) const;
};

#endif
