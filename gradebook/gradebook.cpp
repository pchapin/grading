/*! \file    grades.cpp
 *  \brief   Gradebook server.
 *  \author  Peter C. Chapin <pchapin@vtc.edu>
 */

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <string.h>
// #include <strings.h>  // Needed for strcasecmp
#include <sstream>

#include "cgiread.hpp"
#include "gfile.hpp"
#include "grecord.hpp"
#include "helper.hpp"

using namespace std;

//---------------------------------
//           Global Data
//---------------------------------

typedef list< GradeRecord > grade_list_t;

grade_list_t homework_list;
grade_list_t quiz_list;
grade_list_t exam_list;
grade_list_t project_list;
grade_list_t lab_list;
grade_list_t final_list;
  // These objects hold the grades for each type of assignment. I'm not bothering to use poly-
  // morphism here. Seems like it would be overkill and I don't have time to design it right
  // anyway.

//-------------------------------------------------
//           Internally Linked Functions
//-------------------------------------------------

//
// The following function is called at the end of each assignment record. It takes information
// about the student's grade in that assignment and adds it to the appropriate list.
//
static void close_assignment(GradeRecord &current)
{
    if( current.class_count == 0 )
        throw string( "Zero grades in an assignment. Can't cope" );

    current.class_average /= current.class_count;
    switch( current.type ) {
    case 'H': homework_list.push_back( current ); break;
    case 'Q': quiz_list.push_back( current ); break;
    case 'P': project_list.push_back( current ); break;
    case 'L': lab_list.push_back( current ); break;
    case 'E': exam_list.push_back( current ); break;
    case 'F': final_list.push_back( current ); break;
    }
}

//
// The following function displays an HTML table containing the contents of one of the
// assignment catagory lists.
//
static void display_table(
  char   *header,
  grade_list_t &the_grades,
  int    &overall_possible_points,
  double &overall_actual_points,
  double &overall_class_points )
{
    grade_list_t::iterator current;
    int    total_possible_points = 0;
    double total_actual_points   = 0.0;
    double total_class_points    = 0.0;
    
    // Print out the table header.
    cout << "<HR><H2>" << header << "</H2>\n";
    cout << "<TABLE BORDER><TR>"
            "<TH>Number</TH>"
            "<TH>Title</TH>"
            "<TH>Your Grade</TH>"
            "<TH>Class Avg</TH>"
            "<TH>Class Max</TH>"
            "<TH>Class Min</TH>"
            "<TH>Class Count</TH></TR>\n";

    cout.setf( ios::fixed );

    // Loop over all grade records in this list.
    for( current = the_grades.begin( ); current != the_grades.end( ); ++current ) {

        // Update our accumulated records.
        total_possible_points += current->max_possible;
        total_actual_points   += current->the_grade;
        total_class_points    += current->class_average;

        // Print a table row for this assignment.
        cout << "<TR><TD ALIGN=\"right\">" << current->number << "</TD>"
             << "<TD>" << current->title.c_str( ) << "</TD>"
             << "<TD ALIGN=\"right\">" << setw( 5 ) << setprecision( 1 )
             << current->the_grade << "/" << current->max_possible << " ("
             << setw(5) << setprecision( 1 )
             << 100.0 * current->the_grade/current->max_possible << "%)</TD>"
             << "<TD ALIGN=\"right\">" << setw( 5 ) << setprecision( 1 )
             << 100.0 * current->class_average/current->max_possible << "%</TD>"
             << "<TD ALIGN=\"right\">" << setw( 5 ) << setprecision( 1 )
             << 100.0 * current->class_max/current->max_possible << "%</TD>"
             << "<TD ALIGN=\"right\">" << setw( 5 ) << setprecision( 1 )
             << 100.0 * current->class_min/current->max_possible << "%</TD>"
             << "<TD ALIGN=\"right\">" << current->class_count << "</TD></TR>\n";
    }

    // Now print a table row for the totals.
    cout << "<TR><TD ALIGN=\"right\">Total</TD>"
         << "<TD></TD>"
         << "<TD ALIGN=\"right\">" << setw( 6 ) << setprecision( 1 )
         << total_actual_points << "/" << total_possible_points << " ("
         << setw( 5 ) << setprecision( 1 )
         << 100.0 * total_actual_points/total_possible_points << "%)</TD>"
         << "<TD ALIGN=\"right\">" << setw( 5 ) << setprecision( 1 )
         << 100.0 * total_class_points/total_possible_points << "%</TD></TR>\n";

    // Close off the table.
    cout << "</TABLE>\n";

    overall_possible_points += total_possible_points;
    overall_actual_points   += total_actual_points;
    overall_class_points    += total_class_points;
}

//-------------------------------------------------
//           Externally Linked Functions
//-------------------------------------------------

void get_grades( const string &file_name, const string &first_name, const string &last_name )
{
    GradeFile grades;
    string line_buffer;

    GradeRecord current;
    bool in_assignment = false;

    // Open the grade file.
    if( !grades.open( file_name ) ) {
        ostringstream formatter;
        formatter << "Can't open grade file: " << file_name << ". (Does it exist?)";
        throw formatter.str( );
    }

    // Read every line until we either find what we want or come to the end.
    //
    while( grades.gets( line_buffer ) ) {

        // Is this line the beginning of a new assignment?
        if( line_buffer[0] == '-' ) {

            // It is. Close the old one (if there is one).
            if( in_assignment ) close_assignment( current );

            // Prepare for a new assignment.
            current.init( );

            // Scan information out of the assignment's header.
            istringstream scanner( line_buffer );
            char dummy;

            scanner >> dummy
                    >> current.type >> current.number >> current.max_possible;
            current.type = toupper( current.type );

            string::size_type position = 0;
            position = line_buffer.find_first_of( " \t", 0 );
            if( position != string::npos ) {
                position = line_buffer.find_first_not_of( " \t", position );
                if( position != string::npos ) {
                    position = line_buffer.find_first_of( " \t", position );
                    if( position != string::npos ) {
                        position = line_buffer.find_first_not_of( " \t", position );
                        if( position != string::npos ) {
                            current.title = line_buffer.substr( position );
                        }
                    }
                }
            }
            in_assignment = true;
        }

        // Otherwise, it's a grade record.
        else {

            // I assume we are processing an assignment?
            if( !in_assignment )
                throw string( "Bad grade file format: students with no assignment" );

            // Try to break the line into three parts.
            istringstream scanner( line_buffer );
            string        first_part, second_part, third_part;

            scanner >> first_part >> second_part >> third_part;
            if( first_part.length( )  == 0 ||
                second_part.length( ) == 0 ||
                third_part.length( )  == 0    )
                throw string( "Bad grade file format: Invalid number of fields" );

            // Update global information about this assignment.
            double this_grade = atof( third_part.c_str( ) );

            // If this grade is a zero, skip it. I don't want to blend zeros into the average
            // since they (most likely) reflect people who haven't yet turned in their papers.
            //
            if( this_grade == 0.0 ) continue;

            current.class_count++;
            current.class_average += this_grade;
            if( this_grade > current.class_max ) current.class_max = this_grade;
            if( this_grade < current.class_min ) current.class_min = this_grade;

            // Did we find the person we care about?
            if( _stricmp( last_name.c_str( ), first_part.c_str( ) ) == 0 &&
                _stricmp( first_name.c_str( ), second_part.c_str( ) ) == 0 ) {
                
                // Yes. Remember the grade.
                current.the_grade = this_grade;
            }
        }
    }
    close_assignment( current );
}


//
// The following function prints out the results (in HTML format).
//
void print_results( const string &first_name, const string &last_name, const string &class_Name )
{
  int    OPPs = 0;    // Overall Possible Points.
  double OAPs = 0.0;  // Overall Actual Points.
  double OCPs = 0.0;  // Overall Class Points.

  // Print the header.
  cout << "Grades for <B>" << first_name << " " << last_name
       << "</B> in <B>" << class_Name
       << "</B> as currently recorded in Peter Chapin</A>'s grade book.<P>\n";

  // Display the tables and update the "overall" points information.
  if( homework_list.size( ) > 0 )
    display_table( "Homework", homework_list, OPPs, OAPs, OCPs );

  if( quiz_list.size( ) > 0 )
    display_table( "Quizzes", quiz_list, OPPs, OAPs, OCPs );

  if( project_list.size( ) > 0 )
    display_table( "Projects", project_list, OPPs, OAPs, OCPs );

  if( lab_list.size( ) > 0 )
    display_table( "Labs", lab_list, OPPs, OAPs, OCPs );

  if( exam_list.size( ) > 0 )
    display_table( "Exams", exam_list, OPPs, OAPs, OCPs );

  if( final_list.size( ) > 0 )
    display_table( "Final", final_list, OPPs, OAPs, OCPs );

  // Compute the student's average as a percentage.
  double Average = 100.0 * OAPs/OPPs;

  // Display the overall information.
  cout << "<HR>Your overall average is as follows<P>\n";
  cout << "<TABLE BORDER><TR><TH>Points</TH><TH>Value</TH></TR>\n";
  cout << "<TR><TD ALIGN=\"right\">Total Possible</TD><TD ALIGN=\"right\">"
       << OPPs << "</TD></TR>\n";
  cout << "<TR><TD ALIGN=\"right\">Your Total</TD><TD ALIGN=\"right\">"
       << setw( 7 ) << setprecision( 1 ) << OAPs << "</TD></TR>\n";
  cout << "<TR><TD ALIGN=\"right\">Your Average</TD><TD ALIGN=\"right\">"
       << setw( 5 ) << setprecision( 1 ) << 100.0 * OAPs/OPPs << "%</TD></TR>\n";
  cout << "<TR><TD ALIGN=\"right\">Class Average</TD><TD ALIGN=\"right\">"
       << setw( 5 ) << setprecision( 1 ) << 100.0 * OCPs/OPPs << "%</TD></TR>\n";
  cout << "</TABLE><P>\n";

  // Tell the student what his/her grade in the class is right now.
  cout << "If I graded strictly by the numbers,"
          " your current grade would be: <B>" << grade_string( Average )
       << "</B><P>\n";

  // Display disclaimer string.
  cout << "If you have any concerns or questions about your grades please email me at\n"
          "<A HREF=\"mailto:pchapin@vtc.edu\">"
          "pchapin@vtc.edu</A>\n"
          "or come to my office in WIL-415 (Williston campus). I will be happy to discuss your concerns with you.<P>\n";
}


//
// The following function does most of the processing. It deals with verifying the password,
// looking up the name of the grade file, and calling lower level functions to do the actual
// work.
// 
void process_grades(
  const string &first_name,
  const string &last_name,
  const string &password,
  const string &class_name
  )
{
    string class_file_name;
    string real_password;

    try {
        // If the password is the special, magic password, accept it no matter what.
        if( password != "godlikeseeing" ) {

            // Look up the password for the given user.
            get_password( real_password, first_name, last_name );
            if( password != real_password )
                throw string( "Invalid password" );
        }

        // Look up the name of the class file from the master gradebook file.
        get_class_file( class_file_name, class_name );

        // Read the grades into memory.
        get_grades( class_file_name, first_name, last_name );

        // Spew forth the formatted results!
        print_results( first_name, last_name, class_name );
    }
    catch( const string &message ) {
        cout << "Exception caught! (" << message << ")\n";
    }
}


//----------------------------------
//           Main Program
//----------------------------------

int main( )
{
    // Send out the header.
    opening( "Grade Report" );

    // Get relevant data from the form the user filled out.
    vtc::CGI_FormData form_data;
    string last_name  = form_data["LNAME"];
    string first_name = form_data["FNAME"];
    string password   = form_data["PWORD"];
    string course     = form_data["COURSE"];

    // Do the work.
    process_grades( first_name, last_name, password, course );

    // Print the closing heading and terminate.
    closing( );
    return 0;
}
