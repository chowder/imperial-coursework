#ifndef PLUGBOARD_H
#define PLUGBOARD_H

#include "errors.h"
#include <map>
#include <fstream>
#include <vector>

/* The Plugboard class simulates a plugboard of an enigma machine */
class Plugboard{
  std::map<char, char> pb_map_;

  StatusCode get_config_(char filename[]);
  StatusCode check_form_(std::vector<std::string> &str_data, std::vector<int> &int_data);

public:
  Plugboard(){};
  StatusCode configure(char filename[]);
  char parse(char character) const;
  ~Plugboard(){};
};

#endif
