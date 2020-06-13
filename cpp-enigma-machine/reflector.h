#ifndef REFLECTOR_H
#define REFLECTOR_H

#include "errors.h"
#include <map>
#include <fstream>
#include <vector>

/* The Reflector class simulates a reflector of an enigma machine */
class Reflector{
  std::map<char, char> rf_map_;

  StatusCode get_config_(char filename[]);
  StatusCode check_form_(std::vector<std::string> &str_data, std::vector<int> &int_data);

 public:
  Reflector(){};
  StatusCode configure(char filename[]);
  char parse(char character) const;
  ~Reflector(){};
};

#endif
