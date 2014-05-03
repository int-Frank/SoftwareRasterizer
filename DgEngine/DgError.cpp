/*!
* @file Dg_io.cpp
*
* @author Frank Hart
* @date 2/02/2014
*
* This file contains functions to assist with error handling.
*/

#include "DgError.h"
#include <fstream>


//--------------------------------------------------------------------------------
//	@	ERROR_OUT()
//--------------------------------------------------------------------------------
//		Logs the error, but still allows the program to run.
//--------------------------------------------------------------------------------
void ERROR_OUT(std::string tag)
{
	//Print string to file
	std::ofstream ost(ERRORFILE, std::ios::app);
	ost << std::endl << "Error: " << tag;

}	//End: ERROR_OUT()


//--------------------------------------------------------------------------------
//	@	ERROR_EXIT()
//--------------------------------------------------------------------------------
//		Logs the error and shuts down the program
//--------------------------------------------------------------------------------
void ERROR_EXIT(std::string tag)
{
	//Print to errorlog.txt
	ERROR_OUT(tag);

	//Close application
	exit(1);

}	//End: ERROR_EXIT()


//--------------------------------------------------------------------------------
//	@	CLEAR_LOG()
//--------------------------------------------------------------------------------
//		Clears the error log file.
//--------------------------------------------------------------------------------
void CLEAR_LOG()
{
	std::ofstream ost(ERRORFILE);
	ost << "Error list for last startup.";

}	//End: CLEAR_LOG()