/*!
* @file Utility.h
*
* @author Frank Hart
* @date 3/01/2014
*
* System functions
*/

#ifndef INITSDL_H
#define INITSDL_H

#include "DgTypes.h"

//--------------------------------------------------------------------------------
//	@	START()
//--------------------------------------------------------------------------------
/*!
 * @ingroup utility_system
 *
 * @brief Start all systems.
 *
 * @return Returns true if successful initialization of all systems.
 */
bool START();


//--------------------------------------------------------------------------------
//	@	SHUTDOWN()
//--------------------------------------------------------------------------------
/*!
 * @ingroup utility_system
 *
 * @brief Shutdown all systems.
 */
void SHUTDOWN();


//--------------------------------------------------------------------------------
//	@	RESIZE_WINDOW()
//--------------------------------------------------------------------------------
/*! 
 * @ingroup utility_system
 *
 * @brief Resizes the global WINDOW and viewports.
 */
void RESIZE_WINDOW(uint32 w, uint32 h);

#endif