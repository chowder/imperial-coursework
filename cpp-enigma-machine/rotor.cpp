#include "errors.h"
#include "rotor.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

/* Configures the rotor given a configuration file, filename, and initial rotor
   position, position */
StatusCode Rotor::configure(char filename[], int position){
  StatusCode status;
  // Set rotor position
  position_ = position;
  // Get rotor config from file
  status = get_config_(filename);
  status.file_name(filename);
  return status;
}


/* Checks if the contents of the configuration file are well formed */
StatusCode Rotor::check_form_(vector<string> &str_data, vector<int> &int_data){
  StatusCode status;
  ostringstream message;
  for (auto item: str_data){
    // Check for non-numeric character
    for (auto character: item){
      if (!isdigit(character)){
        message << "Non-numeric character '" << character
                << "' encountered in rotor file";
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
              << "' encountered in rotor file";
      status.error_item(to_string(digit));
      status.error_message(message.str());
      status.error_code(INVALID_INDEX);
      return status;
    }
    // Check for repeated mappings (before the rotor notch positions)
    if (int_data.size() < 26){
      for (auto p: int_data){
        if (p == digit){
          message << "Repeated mapping of '" << digit
                  << "' encountered in rotor file";
          status.error_item(to_string(digit));
          status.error_message(message.str());
          status.error_code(INVALID_ROTOR_MAPPING);
          return status;
        }
      }
    }
    int_data.push_back(digit);
  }
  // Check for insufficient mappings
  if (int_data.size() < 26){
    message << "Received insufficient " << int_data.size()
            << " (EXPECTED >=26) mappings in rotor file";
    status.error_item(str_data.back());
    status.error_message(message.str());
    status.error_code(INVALID_ROTOR_MAPPING);
  }
  return status;
}


/* Gets the configuration from the configuration file to create the rotor
   character map */
StatusCode Rotor::get_config_(char filename[]){
  StatusCode status;
  ifstream input;
  // Check if file can be opened
  input.open(filename);
  if (input.fail()){
    status.error_message("Unable to open rotor configuration file.");
    status.error_code(ERROR_OPENING_CONFIGURATION_FILE);
    return status;
  }

  // Get contents of file
  vector<string> str_data;
  string str;
  while(input >> str)
    str_data.push_back(str);
  input.close();

  // Check if data is well formed
  vector<int> int_data;
  if (status = check_form_(str_data, int_data))
    return status;

  // File is good, create rotor map
  for (int i = 0; i < 26; i++){
    int value = int_data[i] + 'A';
    rotor_map_.insert(pair<char,char>('A' + i, value));
    rotor_reverse_map_.insert(pair<char,char>(value, 'A' + i));
  }

  // Get notch positions
  for (int i = 26; i < (int)int_data.size(); i++)
    notches_.push_back(int_data[i]);

  return status;
}


/* Rotates the rotor and subsequent rotors (if needed) */
void Rotor::rotate(){
  // If the rotor is at position 25, wrap back to 0
  position_ == 25 ? position_ = 0 : position_++;
  for (auto notch: notches_){
    // If the rotor rotates to a notch and it has a neighbouring rotor
    if (position_ == notch && next_rotor != nullptr)
      next_rotor->rotate();
  }
}


/* Takes a character and returns the corresponding character on the rotor
   forward map (parsing from the plugboard through the rotors) */
char Rotor::forward_parse(char character) const{
  // Account for offset by adding rotor position
  character += position_;
  // Loop around if out of range after addition of the offset
  if (character > 'Z')
    character -= 26;
  // Get the character mapped to the input character
  auto search = rotor_map_.find(character);
  character = search->second;
  // Account for offset by subtracting rotor position
  character -= position_;
  // Loop around if out of range after subtraction of the offset
  if (character < 'A')
    character += 26;
  return character;
}


/* Takes a character and returns the corresponding character on the rotor
   backwards map (parsing from the reflector through the rotors) */
char Rotor::reverse_parse(char character) const{
  // Account for offset by adding rotor position
  character += position_;
  // Loop around if out of range after addition of the offset
  if (character > 'Z')
    character -= 26;
  // Get the character mapped to the input character
  auto search = rotor_reverse_map_.find(character);
  character = search->second;
  // Account for offset by subtracting rotor position
  character -= position_;
  // Loop around if out of range after subtraction of the offset
  if (character < 'A')
    character += 26;
  return character;
}
