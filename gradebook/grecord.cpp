/*! \file    grecord.cpp
 *  \brief   Implementation of class GradeRecord.
 *  \author  Peter C. Chapin <pchapin@vtc.edu>
 */

#include <cfloat>
#include "grecord.hpp"

void GradeRecord::init( )
{
    type          = 'x';
    number        = 0;
    title         = "(none)";
    the_grade     = 0.0;
    class_count   = 0;
    class_average = 0.0;
    class_max     = 0.0;
    class_min     = DBL_MAX;
    max_possible  = 1;          // Not zero because we divide by this.
}
