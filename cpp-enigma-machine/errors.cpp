#include "errors.h"
#include <iostream>
#include <fstream>

using namespace std;

/* StatusCode constructor */
StatusCode::StatusCode(){
  error_code_ = NO_ERROR;
  error_message_ = "";
  // By default the error with the input parameters of the class
  filename_ = "ENIGMA";
  file_contents_ = "";
  error_item_ = "";
}


/* Function that fetches the contents of the file where the error was found */
void StatusCode::get_file_contents_(){
  ifstream input;
  input.open(filename_.c_str());
  input >> noskipws;

  char character;
  while (input >> character){
    // Get newlines as space to preserve form when printing to console
    if (character == '\n') {character = ' ';}
    file_contents_ += character;
  }
  input.close();
}


/* Function to stores the error code found */
void StatusCode::error_code(int code){
  if (code <= 11 && code >= 0)
    error_code_ = code;
  else
    cerr << "StatusCode: Tried to set an invalid error code." << endl;
}


/* Function to store the error message */
void StatusCode::error_message(string message){
  error_message_ = message;
}


/* Function to store the error item found */
void StatusCode::error_item(string item){
  if (isdigit(item[0])){
    item = " " + item + " ";
  }
  error_item_ = item;
}

/* Function to store the file name where the error was found */
void StatusCode::file_name(char filename[]){
  filename_ = filename;
}


/* Function to print the error to standard error stream along with its error message */
void StatusCode::print_error(){
  if (error_code_ == NO_ERROR){
    cerr << "NO ERRORS" << endl;
    return;
  }
  cerr << filename_ << " : "
       << error_message_
       << endl;
  // These are errors where the index to the error can be located and shown
  if (error_code_ == INVALID_INDEX ||
      error_code_ == NON_NUMERIC_CHARACTER ||
      error_code_ == IMPOSSIBLE_PLUGBOARD_CONFIGURATION ||
      error_code_ == INVALID_ROTOR_MAPPING ||
      error_code_ == INVALID_REFLECTOR_MAPPING){

    get_file_contents_();
    cerr << file_contents_ << endl;
    // Add leading whitespace to the file content so we can search for entire strings
    // that match the error item and not substrings
    file_contents_ = " " + file_contents_;
    size_t pos = file_contents_.find(error_item_);
    // Repeat for all occurances of the error item found
    while (pos != string::npos){
      // Add one to the width if the error item is a digit because we search for leading whitespace
      isalpha(error_item_[0]) ? cerr.width(pos) : cerr.width(pos + 1);
      cerr << "^";
      file_contents_ = file_contents_.substr(pos + 1);
      pos = file_contents_.find(error_item_);
    }
    cerr << endl;
  }
}


/* Function to return the error code */
int StatusCode::get_error_code() const {
  return error_code_;
}


/* Overloading the bool operator to allow implicit truth evaluation for the class */
StatusCode::operator bool() const {
  return error_code_;
}
