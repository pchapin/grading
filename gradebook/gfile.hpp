/*! \file    gfile.hpp
 *  \brief   Interface to the grade file class.
 *  \author  Peter C. Chapin <pchapin@vtc.edu>
 */

#ifndef GFILE_HPP
#define GFILE_HPP

#include <fstream>
#include <string>

class GradeFile {
 public:
    bool open( const std::string &name );
        // Returns true if file opened ok.

    bool gets( std::string &line );
        // Reads a line and returns false if end or error. This function does not return blank
        // lines or comment lines (it simply ignores them) and it trims all leading and trailing
        // white space.

private:
    std::ifstream raw_file;
};

inline bool GradeFile::open( const std::string &name )
    { raw_file.open( name.c_str() ); return raw_file.is_open( ); }

#endif
