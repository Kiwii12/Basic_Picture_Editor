/*************************************************************************//**
 * @file 
 *
 * @brief this file contains the basic information for function.cpp.  It 
 * should be included with function.cpp.
 ****************************************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cctype>
#include <cstring>
#include <string>


using namespace std;


#ifndef  __FUNCTION__H__
#define __FUNCTION__H__


typedef unsigned char pixel; //defines the type for all arrays


/*!
 * @brief holds the header information and pixel arrays for
 *				the program
 *
 * @details values are passed using vars.<value>
 */
struct image
{
	//all ints found in header
	int rows;		/*!< holds the number of rows */
	int cols;		/*!< holds the number of columns */
	int max_value;  /*!< holds the maximum pixel value */


	//values held for contrast
	int min;		/*!< holds the minimum greyscale value */
	int max;		/*!< holds the maximum greyscale value */

	string fileName; /*!< holds the filename from the commandline */

	string magic_number;/*!< holds the value that determines the type of 
								picture */
	string comment; /*!< holds the pictures comment */

	//all colorband arrays
	pixel **red;	/*!< holds the red pixel array */
	pixel **green;  /*!< holds the green pixel array */
	pixel **blue;   /*!< holds the blue pixel array */
	pixel **grey;   /*!< holds the grey pixel array */
};



/*******************************************************************************
 *                         Function Prototypes
 ******************************************************************************/
void read_in_header(image& vars, ifstream &fin);

void array_maker(image& vars, ifstream &fin, string checker);
pixel **d2array (int rows, int cols);

void all_array_delete( image& vars);
void d2array_delet( int rows, pixel **this_array);

void ascii_fill( image& vars, ifstream &fin);
void binary_fill( image& vars, ifstream &fin);

void read_out_header(image& vars, ofstream &fout);
void ascii_out( image &vars, ofstream &fout);
void binary_out( image &vars, ofstream &fout);

void negate( image &vars );

void brighten( image &vars, int value);
void brighten_formula( pixel **this_array, image &vars, int value );

void greyscale(image &vars);
void contrast(image &vars);

void sharpen( image &vars );
void sub_trac( pixel **&this_array, int rows, int cols, pixel **&cpy_array);

void smooth( pixel **&this_array, image vars);

void commandStatement();
void fileOutput( string &checker, image &vars, char *argv[]);
void runOption( string &checker, image &vars, char *argv[], int argc, int &val);

//void add_up ( pixel **this_array, image vars,  pixel **cpy_array );



#endif