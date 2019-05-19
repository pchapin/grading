/*! \file    cgiread.hpp
 *  \brief   Interface to a CGI form processing class.
 *  \author  Peter C. Chapin <pchapin@vtc.edu>
 *
 * This class provides for very simple form processing. It collects the form data at the
 * standard input (POST method) and loads a map with the resulting (NAME, VALUE) pairs. After a
 * CGI_FormData object is constructed you can just access the form data using normal map access
 * methods.
 *
 * There are forms for which this simple approach doesn't work. However, this class is useful in
 * many applications.
*/

#ifndef CGIREAD_HPP
#define CGIREAD_HPP

#include <map>
#include <string>

namespace vtc {

    class CGI_FormData : public std::map< std::string, std::string > {
    public:
      
        /*!
         * The constructor attempts to read stdin and parse the CGI information that it finds
         * there. It then loads the map with the resulting (NAME, VALUE) pairs. An object of
         * type CGI_FormData thus acts just like a std::map<string, string> except that its
         * constructor does special things.
         */
        CGI_FormData( );
    };

}

#endif
