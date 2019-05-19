/*! \file    cgiread.cpp
 *  \brief   Class to facilitate reading form data in a CGI script.
 *  \author  Peter C. Chapin <pchapin@vtc.edu>
 */

#include <cctype>
#include <cstring>
#include <iostream>
#include <map>
#include <string>

// #define TESTING

#if defined(TESTING)
#include <fstream>
#endif

#include "cgiread.hpp"

using namespace std;

namespace vtc {

    //------------------------------------------------------------------
    //           Internally Linked Functions and Data Objects
    //------------------------------------------------------------------

    static bool init_ok = false;       // =true after the CGI form data read.
    static int  content_length = 0;    // The number of bytes left on stdin.

    /*!
     * The following function takes two characters that compose a hexidecimal string and return
     * the string's numerical equivalent. If either of the characters are not hex digits, this
     * function returns 0. Upper or lower case letters are accepted for the hex digits A..F.
     */
    static int convert_hex( char most, char least )
    {
        char *conversion_table = "0123456789ABCDEF";

        most  = static_cast< char >( toupper( most ) );
        least = static_cast< char >( toupper( least ) );

        // Return an error code if we don't have two legal hex digits.
        if( !isxdigit( most ) || !isxdigit( least ) ) return 0;

        // Look up each digit in the conversion table and compute the result.
        return 16 * ( strchr( conversion_table, most ) - conversion_table ) +
            ( strchr( conversion_table, least ) - conversion_table );
    }

    /*!
     * The following function adjusts the given string to remove CGIisms from it. The result
     * string will be much more palatable to the calling program.
     */
    static void de_CGI( string &workspace )
    {
        string new_workspace;
        string::iterator p;    // Used to scan over the incoming string.
        int    number;         // The value of a %XX escape.
        char   MSD, LSD;       // The two raw digits in a %XX escape.

        // Scan down the given string looking at each character...
        for( p = workspace.begin( ); p != workspace.end( ); ++p ) {

            switch( *p ) {

            // Convert plus signs to spaces.
            case '+':
                new_workspace.append( 1, ' ' );
                break;

            // Percent signs signal the beginning of a hex escape sequence.
            case '%':

                MSD = LSD = '\0';

                // Check to be sure there are in fact characters after the '%' sign.
                if( ++p == workspace.end( ) ) --p; else MSD = *p;
                if( ++p == workspace.end( ) ) --p; else LSD = *p;

                // Convert the hex digits into an ASCII code.
                number = convert_hex( MSD, LSD );

                // Add that ASCII code to the string as a character.
                new_workspace.append( 1, static_cast< char >( number ) );
                break;

            // All other characters we leave unchanged.
            default:
                new_workspace.append( 1, *p );
                break;
            }
        }

        workspace = new_workspace;
        return;
    }


    //--------------------------------------
    //           Public Functions
    //--------------------------------------

    CGI_FormData::CGI_FormData( )
    {
        // If the caller is trying to initialize twice in a row, ignore it.
        if( init_ok ) return;

        #if defined(TESTING)
        ifstream cin( "testcgi.dat" );
        #endif

        // Find the environment variable that tells us how much input there is.
        #if defined(TESTING)
        char *length = "58";
        #else
        char *length = getenv( "CONTENT_LENGTH" );
        #endif

        // If there was no CONTENT_LENGTH variable or if it's value makes no sense, ignore it.
        if( length == 0 ) return;
        content_length = atoi( length );
        if( content_length <= 0 ) return;

        // Otherwise, we're fine.
        init_ok = true;

        // Now read over the input and load up the map.
        
        while( content_length > 0 ) {
            int    state = 0; // =0 when we are getting a name. =1 for the value.
            string new_name;  // The incoming name.
            string new_value; // The associated incoming value.

            // Read the input until there's none left or until we come to the end of this (name,
            // value) pair.
            //
            while( 1 ) {
                if( content_length == 0 ) break;
                
                char ch;
                cin.get( ch );
                content_length--;
                if( ch == '&' ) break;

                // If we are collecting a name...
                if( state == 0 ) {

                    // If we've come to an equal sign, start collecting the value.
                    if( ch == '=' ) state = 1;
                    else new_name.append( 1, ch );
                }

                else {
                    // ... we are collecting a value.
                    new_value.append( 1, ch );
                }
            }

            // De-CGI the value string and install it in the map.
            de_CGI( new_value );
            ( *this )[new_name] = new_value;
        }
    }
    
}
