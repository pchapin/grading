/*! \file    helper.hpp
 *  \brief   Various supporting functions.
 *  \author  Peter C. Chapin <pchapin@vtc.edu>
 *
 * This file declares some "helper" functions that serve mostly as utilities for the rest of the
 * program.
 */

#ifndef HELPER_HPP
#define HELPER_HPP

#include <string>

#define GRADEBOOK_FILE "W:\\gradebook\\grade.bok"
#define PASSWORD_FILE  "W:\\gradebook\\password.txt"

void opening( char *title );
  // Outputs the header HTML with the give title.

void closing( );
  // Outputs the footer HTML.

char *grade_string( double grade );
  // Returns a string containing the letter grade given a numeric grade (in the range 0.0 to
  // 99.9).

void get_password(
        std::string &pass_buffer,
  const std::string &first_name,
  const std::string &last_name
  );
  // Searches the password file for the entry corresponding to first_name last_name. It returns
  // the password into pass_buffer or throws an exception if the password was not found.

void get_class_file( std::string &name_buffer, const std::string &class_name );
  // Looks up the name of the file containing a class's grades given the class's name. It uses
  // the master gradebook file as an index.

#endif
