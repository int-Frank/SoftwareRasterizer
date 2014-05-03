/*!
* @file Dg_io.h
*
* @author Frank Hart
* @date 2/02/2014
*
* This file contains functions to assist with error handling.
*/

#ifndef DGERROR
#define DGERROR

#include <string>
#include <assert.h>

//--------------------------------------------------------------------------------
//	@	ERROR_OUT()
//--------------------------------------------------------------------------------
/*!
 * @ingroup utility_error
 *
 * @brief Logs an error in the error file.
 *
 * @param[in] tag A string describing the error.
 */
void ERROR_OUT(std::string tag);


//--------------------------------------------------------------------------------
//	@	ERROR_EXIT()
//--------------------------------------------------------------------------------
/*!
 * @ingroup utility_error
 *
 * @brief Logs an error in the error file and exits program.
 *
 * @param[in] tag A string describing the error.
 */
void ERROR_EXIT(std::string tag);


//--------------------------------------------------------------------------------
//	@	CLEAR_LOG()
//--------------------------------------------------------------------------------
/*!
* @ingroup utility_error
*
* @brief Clears the error file.
*/
void CLEAR_LOG();


/*!
* @ingroup utility_error
*
* @brief The name of the error log file.
*/
extern const std::string ERRORFILE;

#endif