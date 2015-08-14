/*************************************************************************//**
 * @file 
 *
 * @mainpage prog1 - Picture Editor
 * 
 * @section course_section Course Information 
 *
 * @author Johnathan Ackerman
 * 
 * @date 08/14/2013
 * 
 * @par Professor: Roger L Schrader
 *         
 * 
 * @par Course: 
 *         250 - M001-2013FA -  1-2PM MWTF
 * 
 * @par Location: 
 *         Class Room - 116
 *
 * @section program_section Program Information 
 * 
 * @details This program can take in a binary or ascii file and output a banary
 * binary or ascii file. It can also change the picture itself by 
 * changing the brightness, turning to negative, adapting to greyscale
 * taking the contrast of greyscale, applying smoothness, or applying
 * sharpness
 *
 * The program expects commandline aurguments to know what the output file type
 * should be, and what picture option should be applied if any.
 *
 * The program is divided into three files: function_h, function.cpp, and
 * prog1.cpp. the function header holds all the function prototypes and defines
 * a structor. Function.cpp defines all of the functions. Prog1 runs the
 * program.
 *
 * @section compile_section Compiling and Usage 
 *
 * @par Compiling Instructions: 
 *      The Program requires a picture file and 4 to 6 imputs via commandline 
				aurguments
 * 
 * @par Usage: 
   @verbatim  
   c:\> prog1.exe 
   d:\> c:\bin\prog1.exe 
   @endverbatim 
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 * 
 * @bug sharpen causes unrealistic colors
 * 
 * @par Modifications and Development Timeline: 
   @verbatim 
   Date          Modification 
   ------------  -------------------------------------------------------------- 
   Oct  2, 2013  Created Prog 1, wrote some code
   oct  6, 2013  Created header functions
   oct  7, 2013  created memory allocation functions
   oct  8, 2013  Wrote all the inpute and output functions for ascii and binary
   oct  9, 2013  Rewrote all the work I had done to this point, crushed many
                 many bugs, officially got header functions working 100%
				 rewrote all code to encorperate stuctors properly
   oct 10, 2013  Fixed all output and import functions, program will out put
				 binary to ascii, ascii to ascii, ascii to binary, and binary
				 to binary, wrote a working negative function.
   oct 13, 2013  finished doxegen and wrote contrast, still many bugs at this
				 point
   oct 14, 2013  fixed brighten, greyscale, and contrast, rewrote sharpen
				 and smooth several times to no success, turned in program
   oct 15, 2013  fixed read in file from reversing columns and rows, smooth
				 works. Sharpen is all that doesn't work at this point.
   @endverbatim
 *
 *****************************************************************************/



#include "function.h"

/**************************************************************************//** 
 * @author Johnathan Ackerman
 * 
 * @par Description: 
 * This function runs the whole program. It determines what picture option if 
 * any to run, what formate to output the picture file, and handles lots of
 * errors.
 * 
 * @param[in]      argc - amount of aurguments in argv
 * @param[in]      argv - list of aurments from commandline
 * 
 * @returns 0 program ran successful
 * @returns -1 program had an error
 * @returns -2 program had improper aurguments
 * 
 *****************************************************************************/
int main (int argc, char *argv[])
{
	//used to pass brightness value to function
	int val = 0;

	//sets value to brightness number
	if (argc == 6)
	{
		val = atoi(argv[2]);
	}

	//checks commandline for usage
	if (argc < 4 || argc > 6)
	{
		commandStatement();
		return -2;
	}

	//defines variables for file opening
	std::ifstream fin;
	std::ofstream fout;

	//pulls in variables from the structor
	image vars;

	//checker is a tempory holding string used thoughout the program
	string checker = "";

	//sets checker to the option set by commandline
	checker = (argv [1]);



	//opens file using file name from the commandline
	fin.open(argv[argc-1], ios::in | ios::binary);//uses binary opening to make
													//that all files types are
													//openable
	//checks if file was opened succesfully
	if (!fin)
	{
		cout << "Error opening file";
		//all_array_delete( vars );
		return(-1);
	}

	//grabs picture header from the file
	read_in_header(vars, fin);

	//makes arrays to store the pixel data
	array_maker( vars, fin, checker);

	//checks if ascii picture type
	if ( vars.magic_number == string("P3") )
		ascii_fill( vars, fin );
	//checks if binary picture type
	else if ( vars.magic_number == string("P6") )
		binary_fill(vars, fin );
	//checks if magic number was read in correctly
	else
	{
		//cleans up program before exiting
		cout << "Error with magic number" << endl;
		all_array_delete( vars);
		fin.close();
		return(-1);
	}

	//closes input file
	fin.close();


	vars.fileName = argv[argc-2];

	runOption( checker, vars, argv, argc, val);

	//sets checker to the output variable
	checker = argv[argc - 3];

	fileOutput( checker, vars, argv);

	//cleans up all arrays
	all_array_delete( vars );

	//checked program run
	//cout << "program got to end" << endl;

	return 0;

}