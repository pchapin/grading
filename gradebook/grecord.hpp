/*! \file    grecord.hpp
 *  \brief   Interface to class GradeRecord.
 *  \author  Peter C. Chapin <pchapin@vtc.edu>
 *
 * This file contains the definition of a structure that holds information about a single
 * assignment. Keep in mind that the program is only trying to present grades for a single
 * student. However many of the members in this structure contain class averages or totals so
 * that single student can get some idea of his/her place in class.
*/

#ifndef GRECORD_HPP
#define GRECORD_HPP

#include <string>

struct GradeRecord {
    char    type;           // Homework, quiz, exam, etc.
    int     number;         // Which homework, quiz, exam, etc.
    std::string title;      // The title for the assignment.
    double  the_grade;      // The student's grade on this assignment.
    int     class_count;    // Number of non-zero grades in the class.
    double  class_average;  // Average of non-zero grades.
    double  class_max;      // Highest grade in the class.
    double  class_min;      // Lowest non-zero grade in the class.
    int     max_possible;   // ?

    GradeRecord( ) { init( ); }
    void init( );           // Sets the members to some reasonable value.
};

// The reason zero grades in an assignment are ignored when computing class averages and totals
// is because I don't want people who have just not handed an assignment in to distort the
// averages.

#endif
