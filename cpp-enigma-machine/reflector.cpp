#include "errors.h"
#include "reflector.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <cctype>

using namespace std;

/* Configures the reflector given a configuration file, filename */
StatusCode Reflector::configure(char filename[]){
  StatusCode status;
  status = get_config_(filename);
  status.file_name(filename);
  return status;
}


/* Checks if the contents of the configuration file are well formed */
StatusCode Reflector::check_form_(vector<string> &str_data, vector<int> &int_data){
  StatusCode status;
  ostringstream message;
  for (auto item: str_data){
    // Check if we already has 26 digits in our data vector
    if (int_data.size() == 26){
      message << "Excess number of parameters in reflector file";
      status.error_message(message.str());
      status.error_code(INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS);
      return status;
    }
    // Check for non-numeric character
    for (auto character: item){
      if (!isdigit(character)){
        message << "Non-numeric character '" << character
                << "' encountered in reflector file";
        status.error_item(string(1, character));
        status.error_message(message.str());
        status.error_code(NON_NUMERIC_CHARACTER);
        return status;
      }
    }
    // Check if mapping is out of bounds
    int digit = stoi(item.c_str());
    if (25 < digit || digit < 0){
      message << "Out-of-bounds integer '" << digit
              << "' encountered in reflector file";
      status.error_item(to_string(digit));
      status.error_message(message.str());
      status.error_code(INVALID_INDEX);
      return status;
    }
    // Check for repeated mappings
    for (auto p: int_data){
      if (p == digit){
        message << "Repeated mapping of '" << digit
                << "' encountered in reflector file";
        status.error_item(to_string(digit));
        status.error_message(message.str());
        status.error_code(INVALID_REFLECTOR_MAPPING);
        return status;
      }
    }
    int_data.push_back(digit);
  }
  // Check if there are 26 input digits
  if (int_data.size() != 26){
    message << "Received insufficient " << int_data.size()
            << " (EXPECTED 26) mappings in reflector file";
    status.error_message(message.str());
    status.error_code(INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS);
  }
  return status;
}


/* Gets the configuration from the configuration file to create the reflector
   character map */
StatusCode Reflector::get_config_(char filename[]){
  StatusCode status;
  ifstream input;
  // Check if file can be opened
  input.open(filename);
  if (input.fail()){
    status.error_message("Unable to open plugboard configuration file.");
    status.error_code(ERROR_OPENING_CONFIGURATION_FILE);
    return status;
  }

  // Get contents of file
  vector<string> str_data;
  string str;
  while(input >> str)
    str_data.push_back(str);
  input.close();

  // Check if data is well formed and convert to int if it is
  vector<int> int_data;
  if (status = check_form_(str_data, int_data))
    return status;

  // File is good, create reflector map
  for (int i = 0; i < (int)int_data.size(); i += 2){
    char key = (char)(int_data[i] + 'A');
    char value = (char)(int_data[i+1] + 'A');
    rf_map_.insert(pair<char,char>(key, value));
    rf_map_.insert(pair<char,char>(value, key));
  }
  return status;
}


/* Takes a character and returns the corresponding character on the plugboard
   map */
char Reflector::parse(char character) const{
  auto search = rf_map_.find(character);
  return search->second;
}
