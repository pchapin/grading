/*! \file    helper.cpp
 *  \brief   Implementation of various supporting functions.
 *  \author  Peter C. Chapin <pchapin@vtc.edu>
 */

#include <cstdio>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
// #include <strings.h>  // Needed for strcasecmp
#include <string.h>

#include "gfile.hpp"
#include "helper.hpp"

using namespace std;


// These arrays are used to print out the date/time in a nice way in the closing message.
static char *day_names[] = {
  "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static char *month_names[] = {
  "Jan", "Feb", "Mar", "Apr",
  "May", "Jun", "Jul", "Aug",
  "Sep", "Oct", "Nov", "Dec"
};


//
// The following function handles the work of creating the HTML header and title.
//
void opening( char *title )
{
    // cout << "Content-type: text/html\n\n";

    cout << "<HTML>\n";
    cout << "<HEAD><TITLE>" << title << "</TITLE></HEAD>\n";
    cout << "<BODY>\n";
    cout << "<H1>" << title << "</H1>\n";
}


//
// The following function handles the work of creating a footer. It displays the date and time
// when the document was generated. Note that the time is displayed in Universal Time in order
// to be meaningful everywhere in the world.
//
void closing( )
{
    time_t raw_time = time( 0 );
    struct tm *cooked_time = gmtime( &raw_time );

    cout << "<HR>\n";
    cout << "<B>gradebook</B>, Version 2.1b (Windows version)<BR>\n";
    cout << "&copy; Copyright 2011 by <A HREF=\"http://web.vtc.edu/users/pcc09070/\">Peter Chapin</A><BR>\n";
    cout << "<A HREF=\"mailto:PChapin@vtc.vsc.edu\">PChapin@vtc.vsc.edu</A><BR>\n";
    cout << "This document was generated on: ";
    cout << "<B>"    << day_names[cooked_time->tm_wday];
    cout << " "      << month_names[cooked_time->tm_mon];
    cout << " "      << cooked_time->tm_mday;
    cout << ", "     << setw( 2 ) << setfill( '0' ) << cooked_time->tm_hour;
    cout << ":"      << setw( 2 ) << setfill( '0' ) << cooked_time->tm_min;
    cout << ":"      << setw( 2 ) << setfill( '0' ) << cooked_time->tm_sec;
    cout << " UTC, " << cooked_time->tm_year + 1900;
    cout << "</B><BR>\n";
    cout << "</BODY>\n";
    cout << "</HTML>\n";
}


//
// The following function computes a letter grade based on the student's numeric grade. In this
// version of the program it is very simple. Students should be made aware that their actual
// grade might be different than what is computed here!
//
char *grade_string( double grade )
{
    if( grade < 60.0 ) return "F";
    if( grade < 62.5 ) return "D-";
    if( grade < 67.5 ) return "D";
    if( grade < 70.0 ) return "D+";
    if( grade < 72.5 ) return "C-";
    if( grade < 77.5 ) return "C";
    if( grade < 80.0 ) return "C+";
    if( grade < 82.5 ) return "B-";
    if( grade < 87.5 ) return "B";
    if( grade < 90.0 ) return "B+";
    if( grade < 92.5 ) return "A-";
    if( grade < 97.5 ) return "A";
    return "A+";
}


//
// The following function searches the password file for a student's entry. It returns the
// student's password. If the password is not found, it throws an exception.
//
void get_password( string &password_buffer, const string &first_name, const string &last_name )
{
    GradeFile  password_file;
    string     line_buffer;
    bool       found = false;

    // Open the password file.
    if( !password_file.open( PASSWORD_FILE ) )
        throw string( "Can't open the password file. (Does it exist?)" );

    // Read every line in it until we either find what we want or come to the end.
    while( password_file.gets( line_buffer ) != 0 ) {

        // Try to break the line into three parts.
        istringstream scanner( line_buffer );
        string        first_part, second_part, third_part;

        scanner >> first_part >> second_part >> third_part;
        if( first_part.length( )  == 0 ||
            second_part.length( ) == 0 ||
            third_part.length( )  == 0    )
            throw string( "Bad password file format" );

        // Did we find what we are looking for?
        if( _stricmp( last_name.c_str( ), first_part.c_str( ) ) == 0 &&
            _stricmp( first_name.c_str( ), second_part.c_str( ) ) == 0 ) {
            password_buffer = third_part;
            found = true;
            break;
        }
    }

    // If we didn't find it, we have a problem.
    if( !found )
        throw string( "Can't locate you in the password file" );
}


//
// The following function looks up the name of the file containing a class's grades given the
// class's name. It uses the master gradebook file as an index.
//
void get_class_file( string &name_buffer, const string &class_name )
{
    GradeFile master_file;
    string    line_buffer;
    bool      found = false;

    // Open the master grade book file.
    if( !master_file.open( GRADEBOOK_FILE ) )
        throw string( "Can't open master gradebook file. (Does it exist?)" );

    // Read every line in it until we either find what we want or come to the end.
    while( master_file.gets( line_buffer ) != 0 ) {

        // Try to break the line into two parts.
        istringstream scanner( line_buffer );
        string        first_part, second_part;

        scanner >> first_part >> second_part;
        if( first_part.length( ) == 0 || second_part.length( ) == 0 )
            throw string( "Bad master file format" );

        // Did we find what we are looking for?
        if( class_name == first_part ) {
            name_buffer = second_part;
            found = true;
            break;
        }
    }

    // If we didn't find it, we have a problem.
    if( !found )
        throw string( "Can't locate course in the master gradebook file" );
}
