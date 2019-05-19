/*! \file    gfile.cpp
 *  \brief   Implementation of the grade file class.
 *  \author  Peter C. Chapin <pchapin@vtc.edu>
 */

#include "gfile.hpp"

using namespace std;

//--------------------------------------------
//           Class Member Functions
//--------------------------------------------

bool GradeFile::gets( string &line )
{
    bool acceptable   = false;
    bool return_value = false;
    char buffer[128+1];

    while( !acceptable ) {

        // Get a line from the file. If we can't, then fine. Return false.
        if( !getline(raw_file, line ) ) {
            line         = buffer;
            return_value = false;
            acceptable   = true;
        }
        else {

            // Look for a '#' character. If we find one, chop the line there.
            size_t position = line.find( '#' );
            if( position != string::npos ) line.erase( position );

            // Trim the white space at the beginning of the line.
            position = line.find_first_not_of( " \t\r\n\f" );
            line.erase( 0, position );

            // Trim the white space at the end of the line.
            position = line.find_last_not_of( " \t\r\n\f" );
            line.erase( position + 1 );

            // Let's see if this is a line we want to keep.
            if( line.length( ) > 0 ) {
                return_value = true;
                acceptable   = true;
            }
        }
    }
    return return_value;
}
