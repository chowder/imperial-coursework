#include "errors.h"
#include "enigma.h"
#include "rotor.h"
#include "plugboard.h"
#include "reflector.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cctype>

using namespace std;

/* Takes the input parameters given to the program and uses them to configure the enigma
   machine */
StatusCode EnigmaMachine::configure(int argc, char** argv){
  StatusCode status;
  ostringstream message;

  // Verify program input parameters
  if (status = verify_inputs_(argc, argv)){
    return status;
  }

  // Get rotor starting positions
  vector<int> rotor_positions;
  if (status = get_rotor_positions_(argv[argc-1], rotor_positions)){
    status.file_name(argv[argc-1]);
    return status;
  }

  // Check if we have enough given positions for the number of rotors
  if (number_of_rotors_ > (int)rotor_positions.size()){
    message << "Insufficient starting rotor positions, given " << rotor_positions.size()
            << " (expected " << number_of_rotors_ << ")";
    status.file_name(argv[argc-1]);
    status.error_message(message.str());
    status.error_code(NO_ROTOR_STARTING_POSITION);
    return status;
  }

  // Initialise plugboard and reflector
  if (status = plugboard_.configure(argv[1]))
    return status;
  if (status = reflector_.configure(argv[2]))
    return status;

  // Initialise initial rotor to the head
  Rotor *new_rotor;
  if (number_of_rotors_){
    rotor_head_ = new Rotor;
    status = rotor_head_->configure(argv[3], rotor_positions[0]);
    if (status)
      return status;
    rotor_tail_ = rotor_head_;
  }

  // Add the rest of the rotors
  for (int i = 1; i < number_of_rotors_; i++){
    // Create a new rotor
    new_rotor = new Rotor;
    status = new_rotor->configure(argv[3+i], rotor_positions[i]);
    if (status)
      return status;
    // Push back the current rotor head
    rotor_head_->prev_rotor = new_rotor;
    new_rotor->next_rotor = rotor_head_;
    // Newly created rotor is now the new head
    rotor_head_ = new_rotor;
  }
  return status;
}


/* Verify the input parameters passed to the program by checking the file extensions */
StatusCode EnigmaMachine::verify_inputs_(int argc, char** argv){
  StatusCode status;

  // Get number of each component, identified by extension
  int plugboards = 0, reflectors = 0, rotors = 0, positions = 0;
  for (int i = 1; i < argc; i++){
    string parameter = argv[i];
    string extension = parameter.substr(parameter.find('.'));
    if (extension == ".pb")
      plugboards++;
    else if (extension == ".rf")
      reflectors++;
    else if (extension == ".rot")
      rotors++;
    else if (extension == ".pos")
      positions++;
  }

  // Ensure that there is at least one plugboard and one reflector, and at least one rotor
  // position file if one rotor is given
  if (plugboards < 1 || reflectors < 1 || (rotors >= 1 && positions < 1)){
    ostringstream message;
    message << "\n'./enigma plugboard-file reflector-file (<rotor-file>* rotor-positions)?'\n"
            << "(1) plugboard-file must have extension .pb\n"
            << "(2) reflector-file must have extension .rf\n"
            << "(3) rotor-file (optional) must have extension .rot\n"
            << "(4) rotor-positions must have extension .pos\n"
            << "*a rotor-position file is only required if a rotor file is provided";
    status.error_message(message.str());
    status.error_code(INSUFFICIENT_NUMBER_OF_PARAMETERS);
    return status;
  }
  // While we're here, also get the number of rotors that we have
  number_of_rotors_ = rotors;
  return status;
}


/* Attempts to read the rotor positions file while checking for well-formedness */
StatusCode EnigmaMachine::get_rotor_positions_(char filename[], vector<int> &positions){
  StatusCode status;
  ostringstream message;
  ifstream input;
  input.open(filename);

  // Check if file can be opened
  if (input.fail()){
    status.error_message("Unable to open rotor positions configuration file.");
    status.error_code(ERROR_OPENING_CONFIGURATION_FILE);
    return status;
  }

  // Get contents of file
  string str;
  vector <string> str_data;
  while (input >> str)
    str_data.push_back(str);
  input.close();

  // Begin check for well-formedness
  for (auto position: str_data){
    // Check for non-numerical characters
    for (auto character: position){
      if (!isdigit(character)){
        message << "Non-numerical character '" << character
                << "' encountered in rotor positions file.";
        status.error_item(string(1, character));
        status.error_message(message.str());
        status.error_code(NON_NUMERIC_CHARACTER);
        return status;
      }
    }
    // Check if the digit is within range
    int digit = stoi(position.c_str());
    if (25 < digit || digit < 0){
      message << "Out-of-bounds integer '" << digit
              << "' encountered in rotor positions file.";
      status.error_item(to_string(digit));
      status.error_message(message.str());
      status.error_code(INVALID_INDEX);
      return status;
    }
    // All good, add to data vector
    positions.push_back(digit);
  }
  return status;
}


/* Takes a character and parses it through each component of the enigma machine, and returns
   its corresponding character */
StatusCode EnigmaMachine::parse(char character, char &parsed){
  StatusCode status;
  ostringstream message;

  // Check if input character is valid
  if (character < 'A' || character > 'Z'){
    message << "Tried to parse non-valid input character '" << character << "'"
            << " (input must be uppercase letter A-Z)";
    status.error_message(message.str());
    status.error_code(INVALID_INPUT_CHARACTER);
    return status;
  }

  // Parse through plugboard
  parsed = character;
  parsed = plugboard_.parse(parsed);

  // Rotate the first rotor
  if (rotor_head_ != nullptr)
    rotor_head_->rotate();

  // Parse through rotors
  Rotor *head_ptr = rotor_head_;
  while (head_ptr != nullptr){
    parsed = head_ptr->forward_parse(parsed);
    head_ptr = head_ptr->next_rotor;
  }

  // Parse through reflector
  parsed = reflector_.parse(parsed);

  // Parse back through rotors
  Rotor *tail_ptr = rotor_tail_;
  while (tail_ptr != nullptr){
    parsed = tail_ptr->reverse_parse(parsed);
    tail_ptr = tail_ptr->prev_rotor;
  }

  // Parse through plugboard again
  parsed = plugboard_.parse(parsed);

  return status;
}


/* Destructor to ensure that all instances of Rotor are destroyed upon exit */
EnigmaMachine::~EnigmaMachine(){
  // If we don't have a rotor, nothing to do
  if (rotor_head_ == nullptr)
    return;
  // Otherwise, delete all rotors
  Rotor* next_rotor;
  while (rotor_head_ != nullptr){
    next_rotor = rotor_head_->next_rotor;
    delete rotor_head_;
    rotor_head_ = next_rotor;
  }
}
