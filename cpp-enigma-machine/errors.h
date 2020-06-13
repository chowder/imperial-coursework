#ifndef ERRORS_H
#define ERRORS_H
/* error codes  */
#define INSUFFICIENT_NUMBER_OF_PARAMETERS         1
#define INVALID_INPUT_CHARACTER                   2
#define INVALID_INDEX                             3
#define NON_NUMERIC_CHARACTER                     4
#define IMPOSSIBLE_PLUGBOARD_CONFIGURATION        5
#define INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS  6
#define INVALID_ROTOR_MAPPING                     7
#define NO_ROTOR_STARTING_POSITION                8
#define INVALID_REFLECTOR_MAPPING                 9
#define INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS  10
#define ERROR_OPENING_CONFIGURATION_FILE          11
#define NO_ERROR                                  0

#include <string>

/* The StatusCode class handles errors by storing the
(1) Error code
(2) Message associated with the error
(3) File name where the error was found (if applicable)
(4) A string of the error found
It also overloads the bool operator so the class can be implicitly evaluated
based on whether an error was found */
class StatusCode{
  int error_code_;
  std::string error_message_;
  std::string filename_;
  std::string file_contents_;
  std::string error_item_;
  void get_file_contents_();

public:
  StatusCode();
  void error_code(int code);
  void error_message(std::string message);
  void error_item(std::string item);
  void file_name(char filename[]);
  void print_error();
  int get_error_code() const;
  explicit operator bool() const;
};

#endif
