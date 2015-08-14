/*************************************************************************//**
 * @file 
 *
 * @brief All Input, Output, Allocation, and picture option functions.
 ****************************************************************************/
#include "function.h"


/**************************************************************************//** 
 * @author Johnathan Ackerman
 * 
 * @par Description: 
 * This function reads in data from the inpute file and test to make sure that
 the file can be opened.
 * 
 * @param[in]      fin - File opened in main
 * @param[out]     vars.magic_number - string containing the File number that
							determines what kind of picture the file contains
 * @param[out]     vars.rows - Amount of rows of pixels per colorband
 * @param[out]     vars.cols - Amount of cols of pixels per colorband
 * @param[out]     vars.comment - String that preserves the comment of the
							picture
 * @param[out]	   vars.max_value - the maximum pixel value
 * 
 *****************************************************************************/
void read_in_header(image& vars, ifstream& fin)
{
	//reads in first string
	fin >> vars.magic_number;

	//ignors the \n so fin.peek can check for #
	fin.ignore();

	//Looks for # which determines a comment
	if(fin.peek() == '#')
	{
		getline(fin, vars.comment, '\n');
	}

	//reads in the rest of the header
	fin >> vars.cols;
	fin >> vars.rows;
	fin >> vars.max_value;

	//ignores the last \n found in the header for binary
	fin.ignore();

	//checks to see if the header is right
	//cout << vars.magic_number << " " << vars.rows << " " << vars.cols
	//	<< " " << vars.max_value << endl;


	return;
}

/**************************************************************************//** 
 * @author Johnathan Ackerman
 * 
 * @par Description: 
 * This array allocates a 2 demensional array and checks to make sure that
 * all parts of the array are allocated properly
 * 
 * @param[in]      rows - determins the row size of arrays
 * @param[in]      cols - determins the column size of arrays
 * 
 * @returns this_array program ran successful.
 * @returns nullptr the program fail or there was an error.  
 * 
 *****************************************************************************/
pixel **d2array (int rows, int cols)
{
	//loop variables
	int i = 0;
	int j = 0;
	
	//makes sure the array is initialized to nullptr
	pixel **this_array = nullptr;

	//allocates memory
	this_array = new (nothrow) pixel * [rows];

	//checks if memory was allocated
	if (this_array == nullptr)
		return nullptr;

	//loops through and allocated the second part
	for (i=0; i < rows; i++)
	{
		this_array[i] = new (nothrow) pixel [cols];

		//delets all allocated memory if an error occured
		if (this_array[i] == nullptr)
		{
			for (j=0; j < rows; j++)
			{
				delete [] this_array[j];
			}
			delete [] this_array;
			this_array = nullptr;
			return nullptr;
		}
	}
	return this_array;
}

/**************************************************************************//** 
 * @author Johnathan Ackerman
 * 
 * @par Description: 
 * uses 2dallocate to allocate and error check all arrays needed for program
 * 
 * @param[in]      fin - passes in open file to be closed if there is an error
 * @param[in]      checker- a temporary hold for the option given from the
 *							command line
 * @param[out]	   vars.red - allocated color band
 * @param[out]	   vars.green - allocated color band
 * @param[out]	   vars.blue - allocated color band
 * @param[out]	   vars.grey - allocated color band
 * 
 * 
 *****************************************************************************/
void array_maker(image &vars, ifstream &fin, string checker)
{
	// dynamicaly creates the red green and blue arrays
	vars.red = d2array(vars.rows, vars.cols);

	//checks for allocation errors and closes program
	if (vars.red == nullptr)
	{
		cout << "memory or allocation error red";
		fin.close();
		all_array_delete( vars);
		exit(0);
	}

	vars.green = d2array(vars.rows, vars.cols);

	if (vars.green == nullptr)
	{
		cout << "memory or allocation error green";
		fin.close();
		all_array_delete( vars );
		exit(0);
	}


	vars.blue = d2array(vars.rows, vars.cols);


	if (vars.blue == nullptr)
	{
		cout << "memory or allocation error blue";
		fin.close();
		all_array_delete( vars );
		exit(0);
	}

	// allocates a greyscale array if the picture is to be outputted in grey
						//scale
	if (checker == string("-g") || checker == string("-c") )
	{
		vars.grey = d2array(vars.rows, vars.cols);
		if (vars.grey == nullptr)
		{
			cout << "memory or allocation error grey";
			all_array_delete( vars );
			fin.close();
			exit(0);
		}
	}
	else vars.grey = nullptr;

	return;
}


/**************************************************************************//** 
 * @author Johnathan Ackerman
 * 
 * @par Description: 
 * fills the clorband arrays from an ascii picture file
 * 
 * @param[in]		   fin - File opened in main
 * @param[in]  	       vars.rows - Amount of rows of pixels per colorband
 * @param[in]		   vars.cols - Amount of cols of pixels per colorband
 * @param[in][out]	   vars.red - allocated color band
 * @param[in][out]	   vars.green - allocated color band
 * @param[in][out]	   vars.blue - allocated color band
 * 
 *****************************************************************************/
void ascii_fill( image &vars, ifstream &fin )
{
	//temporary hold for read in data
	int r = 0;
	int g = 0;
	int b = 0;

	//loop variables
	int i = 0;
	int j = 0;

	//fill loop
	for( i = 0; i < vars.rows; i++ )
	{
		for( j = 0; j < vars.cols; j++)
		{
			//uses temp to fill because fin works well with ints
			fin >> r;
			fin >> g;
			fin >> b;

			//type casts to pixel to fill array
			vars.red[i][j] = (pixel)r;
			vars.green[i][j] = (pixel)g;
			vars.blue[i][j] = (pixel)b;
		}
	}
	
	return;
}

/**************************************************************************//** 
 * @author Johnathan Ackerman
 * 
 * @par Description: 
 * fills the clorband arrays from an binary picture file
 * 
 * @param[in]		   fin - File opened in main
 * @param[in]  	       vars.rows - Amount of rows of pixels per colorband
 * @param[in]		   vars.cols - Amount of cols of pixels per colorband
 * @param[in][out]	   vars.red - allocated color band
 * @param[in][out]	   vars.green - allocated color band
 * @param[in][out]	   vars.blue - allocated color band
 * 
 *****************************************************************************/
void binary_fill( image &vars, ifstream &fin )
{

	//loop variables
	int i = 0;
	int j = 0;

	//fill loops
	for( i = 0; i < vars.rows; i++ )
	{
		for( j = 0; j < vars.cols; j++ )
		{
			fin.read((char*) &vars.red[i][j], sizeof(pixel));
			//fin.ignore(); <-- attempt at fixing bugs early on
			fin.read((char*) &vars.green[i][j], sizeof(pixel));
			//fin.ignore();
			fin.read((char*) &vars.blue[i][j], sizeof(pixel));
			//fin.ignore();
		}
	}
	//zipps up file
	fin.close();

	return;
}

/**************************************************************************//** 
 * @author Johnathan Ackerman
 * 
 * @par Description: 
 * clears all allocated array data
 * 
 * @param[in]		   vars.rows - Amount of cols of pixels per colorband
 * @param[in][out]	   vars.red - allocated color band
 * @param[in][out]	   vars.green - allocated color band
 * @param[in][out]	   vars.blue - allocated color band
 * @param[in][out]	   vars.grey - allocated color band
 * 
 *****************************************************************************/
void all_array_delete( image &vars)
{
	//deletes all allocated memory
	d2array_delet(vars.rows, vars.red);
	d2array_delet(vars.rows, vars.blue);
	d2array_delet(vars.rows, vars.green);

	//checks if gray has been used
	if (vars.grey != nullptr)
		d2array_delet(vars.rows, vars.grey);

	return;
}

/**************************************************************************//** 
 * @author Johnny Ackerman
 * 
 * @par Description: 
 * A detailed description of the function.  It can
 * extend to multiple lines
 * 
 * @param[in]		   rows - Amount of cols of pixels per colorband
 * @param[in][out]	   this_array - passed in colorband array
 * 
 *****************************************************************************/	
void d2array_delet( int rows, pixel **this_array)
{
	//loop variable
	int j = 0;

	//deletes array starting with furthest allocation
	for (j=0; j < rows; j++)
	{
		delete [] this_array[j];
	}
	delete [] this_array;

	//sets array back to nullptr
	this_array = nullptr;

	return;
}


/**************************************************************************//** 
 * @author Johnathan Ackerman
 * 
 * @par Description: 
 * out puts all the header information to the top of the file
 * 
 * @param[in]      fin - File opened in main
 * @param[out]     vars.magic_number - string containing the File number that
							determines what kind of picture the file contains
 * @param[out]     vars.rows - Amount of rows of pixels per colorband
 * @param[out]     vars.cols - Amount of cols of pixels per colorband
 * @param[out]     vars.comment - String that preserves the comment of the
							picture
 * @param[out]	   vars.max_value - the maximum pixel value
 * 
 *****************************************************************************/
void read_out_header(image& vars, ofstream &fout)
{	
	//writes header to file
	fout << vars.magic_number << '\n';
	fout << vars.comment << '\n';
	fout << vars.cols << ' ' << vars.rows << '\n';
	fout << vars.max_value << '\n';

	return;
}

/**************************************************************************//** 
 * @author Johnny Ackerman
 * 
 * @par Description: 
 * out puts ascii picture file data
 * 
 * @param[in]		   fout - out put File opened in main
 * @param[in]  	       vars.rows - Amount of rows of pixels per colorband
 * @param[in]		   vars.cols - Amount of cols of pixels per colorband
 * @param[in]		   vars.red - allocated color band
 * @param[in]		   vars.green - allocated color band
 * @param[in]		   vars.blue - allocated color band
 * @param[in]		   vars.grey - allocated color band
 * @param[in]		   vars.magic_number - determines picture type
 * 
 *****************************************************************************/
void ascii_out( image &vars, ofstream &fout )
{
	//loop variables
	int i = 0;
	int j = 0;

	//checks for greyscale
	if ( vars.magic_number == string("P2"))
	{
		//outputs greyscale
		for( i = 0; i < vars.rows; i++ )
		{
			for( j = 0; j < vars.cols; j++)
			{
				//typecasts to int to ensure fout works correctly
				fout << int (vars.grey[i][j]) << '\n';
			}
		}
	}
	else
	{
		//outputs other colorbands
		for( i = 0; i < vars.rows; i++ )
		{
			for( j = 0; j < vars.cols; j++)
			{
				//typecasts to int to ensure fout works correctly
				fout << int(vars.red[i][j]) << '\n';
				fout << int(vars.green[i][j]) << '\n';
				fout << int(vars.blue[i][j]) << '\n';
			}
		}
	}

	return;
}

/**************************************************************************//** 
 * @author Johnny Ackerman
 * 
 * @par Description: 
 * out puts binary picture file data
 * 
 * @param[in]		   fout - out put File opened in main
 * @param[in]  	       vars.rows - Amount of rows of pixels per colorband
 * @param[in]		   vars.cols - Amount of cols of pixels per colorband
 * @param[in]		   vars.red - allocated color band
 * @param[in]		   vars.green - allocated color band
 * @param[in]		   vars.blue - allocated color band
 * @param[in]		   vars.grey - allocated color band
 * @param[in]		   vars.magic_number - magic number that determines
									picture type
 * 
 *****************************************************************************/
void binary_out( image &vars, ofstream &fout)
{
	//loop variables
	int i = 0;
	int j = 0;

	//checks for greyscale
	if ( vars.magic_number == string("P5"))
	{
		//outputs grey 2D array
		for( i = 0; i < vars.rows; i++ )
		{
			for( j = 0; j < vars.cols; j++)
			{
				fout.write( (char*) &vars.grey[i][j], sizeof(pixel));
			}
		}
	}
	else
	{
		//out puts colorband 2D arrays
		for( i = 0; i < vars.rows; i++ )
		{
			for( j = 0; j < vars.cols; j++)
			{
				fout.write( (char*) &vars.red[i][j], sizeof(pixel));
				//fout << '\n'; <-- trying to find error with other options
				fout.write( (char*) &vars.green[i][j], sizeof(pixel));
				//fout << '\n';
				fout.write( (char*) &vars.blue[i][j], sizeof(pixel));
				//fout << '\n';
			}
		}
	}

	return;
}

/**************************************************************************//** 
 * @author Johnathan Ackerman
 * 
 * @par Description: 
 * Negates the image
 * 
 * @param[in]  	       vars.rows - Amount of rows of pixels per colorband
 * @param[in]		   vars.cols - Amount of cols of pixels per colorband
 * @param[in]		   vars.max_value - maximum pixel value
 * @param[in][out]	   vars.red - allocated color band
 * @param[in][out]	   vars.green - allocated color band
 * @param[in][out]	   vars.blue - allocated color band
 * 
 *****************************************************************************/
void negate( image &vars )
{
	//loop variables
	int i = 0;
	int j = 0;

	//loops though and changes all pixels
	for( i = 0; i < vars.rows; i++ )
	{
		for( j = 0; j < vars.cols; j++)
		{
			//type casts 255 to pixel to ensure the currect data
			vars.red[i][j] = (pixel)vars.max_value - vars.red[i][j];
			vars.green[i][j] = (pixel)vars.max_value - vars.green[i][j];
			vars.blue[i][j] = (pixel)vars.max_value - vars.blue[i][j];
		}
	}
	return;
}

/**************************************************************************//** 
 * @author Johnathan Ackerman
 * 
 * @par Description: 
 * changes image brightness
 * 
 * @param[in]  	       vars.rows - Amount of rows of pixels per colorband
 * @param[in]		   vars.cols - Amount of cols of pixels per colorband
 * @ param[in]		   vars.max_value - maximum pixel value
 * @param[in]		   value - value from command line that affects brightness
 * @param[in][out]	   vars.red - allocated color band
 * @param[in][out]	   vars.green - allocated color band
 * @param[in][out]	   vars.blue - allocated color band
 * 
 *****************************************************************************/
void brighten( image &vars, int value)
{
	//loop variables
	int i = 0;
	int j = 0;


	brighten_formula( vars.red, vars, value );
	brighten_formula( vars.green, vars, value );
	brighten_formula( vars.blue, vars, value );

	return;
}

/**************************************************************************//** 
 * @author Johnathan Ackerman
 * 
 * @par Description: 
 * changes image brightness
 * 
 * @param[in]  	       vars.rows - Amount of rows of pixels per colorband
 * @param[in]		   vars.cols - Amount of cols of pixels per colorband
 * @ param[in]		   vars.max_value - maximum pixel value
 * @param[in]		   value - value from command line that affects brightness
 * @param[in][out]	   this_array - colorband array passed in
 * 
 *****************************************************************************/
void brighten_formula( pixel **this_array, image &vars, int value )
{
	//loop variables
	int i = 0;
	int j = 0;

	//temporary hold for brighten data
	int temporary = 0;

	//for loop that changes every pixel
	for( i = 0; i < vars.rows; i++ )
	{
		for( j = 0; j < vars.cols; j++)
		{
			temporary = (this_array[i][j] + (value));
			if(temporary > (vars.max_value))
				temporary = (vars.max_value);
			if(temporary < (0))
				temporary = (0);
			this_array[i][j] = (pixel)temporary;
		}
	}

	return;
}

/**************************************************************************//** 
 * @author Johnathan Ackerman
 * 
 * @par Description: 
 * Changes image to greyscale
 * 
 * @param[in]  	       vars.rows - Amount of rows of pixels per colorband
 * @param[in]		   vars.cols - Amount of cols of pixels per colorband
 * @param[in]		   vars.min - minimum value of greyscale array used in
								contrast equation
 * @param[in]		   vars.max - maximum value of greyscale array used in
								contrast equation
 * @param[in][out]	   vars.red - allocated color band
 * @param[in][out]	   vars.green - allocated color band
 * @param[in][out]	   vars.blue - allocated color band
 * @param[in][out]	   vars.grey - allocated color band
 * 
 *****************************************************************************/
void greyscale(image &vars)
{
	//loop varaibles
	int i = 0;
	int j = 0;

	//loops though and sets the greyscale data
	for( i = 0; i < vars.rows; i++ )
	{
		for( j = 0; j < vars.cols; j++)
		{
			vars.grey[i][j] = pixel( int(.3 * double(vars.red[i][j]) + .6 *
				double(vars.green[i][j]) + .1 * double(vars.green[i][j])));
		}
	}
	return;
}

/**************************************************************************//** 
 * @author Johnathan Ackerman
 * 
 * @par Description: 
 * Sharpens the image
 * 
 * @param[in]  	       vars.rows - Amount of rows of pixels per colorband
 * @param[in]		   vars.cols - Amount of cols of pixels per colorband
 * @param[in][out]	   vars.red - allocated color band
 * @param[in][out]	   vars.green - allocated color band
 * @param[in][out]	   vars.blue - allocated color band
 * 
 *****************************************************************************/
void sharpen( image &vars )
{
	//loop variables
	int i = 0;
	int j = 0;

	//creates temporary array
	pixel **cpy_array;
	cpy_array = d2array(vars.rows, vars.cols);

	//checks if array was made
	if (cpy_array == nullptr)
	{
		//deallocates all memory
		cout << "memory or allocation error";
		all_array_delete( vars);
		exit(0);
	}

	//runs smoothing formula
	sub_trac( vars.red, vars.rows, vars.cols, cpy_array);
	sub_trac( vars.green, vars.rows, vars.cols, cpy_array);
	sub_trac( vars.blue, vars.rows, vars.cols, cpy_array);

	//deallocates temporary array
	d2array_delet( vars.rows, cpy_array);
	
	return;
}

/**************************************************************************//** 
 * @author Johnny Ackerman
 * 
 * @par Description: 
 * does the sharpen formula to any array pased into the function
 * 
 * @param[in]      rows - amount of rows in array
 * @param[in]      cols - amount of rows in array
 * @param[in]	   cpy_array - a temporary array to stop data from being
							affected
 * @param[in,out]  this_array - passed in colorband that will be changed
 * 
 *****************************************************************************/
void sub_trac( pixel **&this_array, int rows, int cols, pixel **&cpy_array)
{
	//looping varialbes
	int i;
	int j;
	int b, d, e, f, h;

	//temporary holding value
	int ans = 0;

	

	//computes data storing into the cpy_array as to not affect other data
	for( i = 1; i < rows-1; i++ )
	{
		for( j = 1; j < cols-1; j++)
		{
			// turns the edge pixels to 0 as to not affect the formula
			
			//sharpening formula
				b = this_array[i-1][j];
				d = this_array[i][j-1];
				e = this_array[i][j];
				f = this_array[i][j+1];
				h = this_array[i+1][j];
				ans = ((5 * e) - b - d - f - h);

				//makes sure no variables are out of bounds
				if( ans > 255 )
					ans = 255;
				if( ans < 0 )
					ans = 0;

				cpy_array[i][j] = pixel(ans);
			
		}
	}
	//puts copied data back into colorband array
	
	swap(this_array, cpy_array);
	return;
}

/**************************************************************************//** 
 * @author Johnny Ackerman
 * 
 * @par Description: 
 * does the smooth formula to any array pased into the function
 * 
 * @param[in]      rows - amount of rows in array
 * @param[in]      cols - amount of rows in array
 * @param[in,out]  this_array - passed in colorband that will be changed
 * 
 *****************************************************************************/
void smooth( pixel **&this_array, image vars)
{	
	//looping variable
	int I = 0;
	int J = 0;

	//creates temporary array
	pixel **cpy_array;
	cpy_array = d2array(vars.rows, vars.cols);
	if (cpy_array == nullptr)
	{
		cout << "memory or allocation error";
		all_array_delete( vars);
		exit(0);
	}

	//initializes diffrent possitions in the array for the formula
	double a = 0; //unsigned long to typecast the rest
							//to unsigned long to avoid loss of data
	pixel b, c, d, f, g, h, i, j;

	//holds final answer before puting into array
	double ans = 0;

	//fills cpy_array with this_array data
	for( I = 0; I < vars.rows; I++ )
	{
		for( J = 0; J < vars.cols; J++)
		{
			cpy_array[I][J] = this_array[I][J];
		}
	}
	
	//loops though the entire array to change every pixel
	for( I = 1; I < vars.rows -1; I++ )
	{
		for( J = 1; J < vars.cols -1; J++)
		{
			//changes edge pixels to 0 to avoid breaking the formula
			if( (I == 0) || (J == 0) || (I == vars.rows-1) || (J == vars.cols-1))
				this_array[I][J] = (0);

			//smoothing formula
			else
			{
				//sets all points to their determined variable
				a = cpy_array[I-1][J-1];
				b = cpy_array[I-1][J];
				c = cpy_array[I-1][J+1];
				d = cpy_array[I][J-1];
				j = cpy_array[I][J];
				f = cpy_array[I][J+1];
				g = cpy_array[I+1][J-1];
				h = cpy_array[I+1][J];
				i = cpy_array[I+1][J+1];

				//actual formula
				ans = ((a + b + c + d + j + f + g + h + i) / 9.0) + .5;

			//makes sure the answer doesn't go above the maximum pixel value
	//			while( ans > vars.max_value )
		//			ans = ans - vars.max_value;
				if( ans > 255 )
					ans = 255;
				if( ans < 0 )
					ans = 0;
				//stores answer in temporary array so that the rest of the
					//data isn't effected
				this_array[I][J] = pixel(ans);
			}
		}
	}
	

	//checked if the loop was run correctly
	//cout << "ending loop vals " << I << " " << J << " ";

	//deletes temporary array
	d2array_delet( vars.rows,  cpy_array);

	return;
}


/**************************************************************************//** 
 * @author Johnathan Ackerman
 * 
 * @par Description: 
 * Contrasts the greyscale image
 * 
 * @param[in]  	       vars.rows - Amount of rows of pixels per colorband
 * @param[in]		   vars.cols - Amount of cols of pixels per colorband
 * @param[in]		   vars.min - minimum value of greyscale array used in
								contrast equation
 * @param[in]		   vars.max - maximum value of greyscale array used in
								contrast equation
 * @param[in][out]	   vars.grey - allocated color band
 * 
 *****************************************************************************/
void contrast(image &vars)
{
	//loop variables
	int i;
	int j;

	//min and max saved for contrast equation
	vars.min = 255;
	vars.max = 0;
	for( i = 0; i < vars.rows; i++ )
	{
		for( j = 0; j < vars.cols; j++)
		{
			//keeps track of greyscale min and max
			if ( vars.min > (int)vars.grey[i][j])
				vars.min = (int)vars.grey[i][j];
			if (vars.max < (int)vars.grey[i][j] )
				vars.max = (int)vars.grey[i][j];
		}
	}


	//initilizes scale
	double scale = 0;

	//sets scale value
	scale = 255.0 / (vars.max - vars.min);

	//loops though all pixels in the greyscale array
	for( i = 0; i < vars.rows; i++ )
	{
		for( j = 0; j < vars.cols; j++)
		{
			//contrast formula
			vars.grey[i][j] = pixel(scale * ( vars.grey [i][j] - vars.min ) + .5);
		}
	}
}

/**************************************************************************//** 
 * @author Johnathan Ackerman
 * 
 * @par Description: 
 * outputs a commandstatment
 * 
 *****************************************************************************/
void commandStatement()
{
	cout << "Usage: prog1.exe [option] -o[ab] basename image.ppm" << endl;
	cout << "[option] The option changes the picture depending on the " <<
		" option code: (-n) = Negate, (-b #) = Brighten, (-p) = Sharpen" <<
		", (-s) = smooth, (-g) = Greyscale, and (-c) = Contrast." << endl;
	cout << "-o[ab] = the option to output ascii or binary" << endl;
	cout << "basename = the new name for the file" << endl;
	cout << "image.ppm = the name of the file given to the program" << endl;
}

/**************************************************************************//** 
 * @author Johnathan Ackerman
 * 
 * @par Description: 
 * outputs the file in binary or ascii
 * 
 * @param[in]  	       checker - used to test different commandline aurguments
 * @param[in]		   vars - structure of variables passed to inner functions
 * @param[in]		   argv - commandline aurgments that checker uses
 * 
 *****************************************************************************/
void fileOutput( string &checker, image &vars, char *argv[])
{
	std::ofstream fout;


	//-oa value means output in ascii
	if( checker == string("-oa") )
	{

		//opens ascii output file
		fout.open(vars.fileName);
		if (!fout)
		{
			//cleans up and exits
			cout << "Error opening output file";
			all_array_delete( vars );
			exit(-1);
		}
		
		//sets magic number for picture type
		vars.magic_number = string("P3");

		//sets checker to picture option
		checker = argv[1];

		//resets magic_number to a greyscale number if needed
		if ( checker == string("-g") || checker == string("-c") )
			vars.magic_number = string("P2");

		//reads out all data
		read_out_header(vars, fout);
		ascii_out( vars, fout);
	}

	//looks for binaray output option
	else if( checker == string("-ob") )
	{
		//opens output file in binary
		fout.open(vars.fileName, ios::out | ios::binary);
		if (!fout)
		{
			//cleans up and exits if failed
			cout << "Error opening output file";
			all_array_delete( vars );
			exit(-1);
		}

		//sets magic_number to binary file
		vars.magic_number = string("P6");

		//sets checker to option
		checker = argv[1];

		//sets magic number to binary greyscale if needed
		if ( checker == string("-g") || checker == string("-c") )
			vars.magic_number = string("P5");

		//reads out all data
		read_out_header(vars, fout);
		binary_out( vars, fout );
	}
	else
	{
		//cleans up and exits if an output file was undetected
		commandStatement();
		all_array_delete( vars );
		exit(-2);
	}

	//closes output file
	fout.close();
}

void runOption( string &checker, image &vars, char *argv[], int argc, int &val)
{
	//checks if file ending needs to be changed for greyscale options
	checker = argv[1];
	if (checker == string("-g") || checker == string("-c"))
		vars.fileName = vars.fileName.append(".pgm");
	else
		vars.fileName = vars.fileName.append(".ppm");

	//checks for options
	if (argc == 5)
	{
		//sets checker to commandline option
		checker = argv[1];

		

		//checked if option loop was entered
		//cout << "an option was detected";
	
		//determines what option to run
		if (checker == string("-n"))
			negate( vars );
		else if (checker == string("-p"))
			sharpen( vars );
		else if (checker == string("-s"))
		{
			smooth ( vars.red, vars );
			smooth ( vars.blue, vars );
			smooth ( vars.green, vars );
		}
		else if (checker == string("-g"))
		{
			greyscale( vars);

		}
		else if (checker == string("-c"))
		{
			greyscale(vars);
			contrast( vars);
		}
		else
		{
			//cleans up program if invalid option
			commandStatement();
			all_array_delete( vars);
			exit(-2);
		}
	}

	//looks at command aurgements
	if (argc == 6)
	{
		checker = argv[1];

		//checks actual value
		//cout << "this is the value   " << value << " ";

		//checks actual aurgment
		if (checker != string("-b"))
		{
			//cleans up and exits
			commandStatement();
			all_array_delete( vars );
			exit(-2);
		}

		//checks the brightness value
		if (val > 256 || val < -256)
		{
			//cleans up and exits
			commandStatement();
			all_array_delete( vars );
			exit(-2);
		}
		//runs brightness
		brighten(vars, val);
	}
}