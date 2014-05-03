/*!
* @file Dg_io.cpp
*
* @author Frank Hart
* @date 2/01/2014
*
* This file contains definitions and functions to assist with reading and
* writing to and from different media.
*/

#include "Dg_io.h"


//--------------------------------------------------------------------------------
//	@	IgnoreComments()
//--------------------------------------------------------------------------------
/*!
 * If the next character in the stream is the flag, this function discards all
 * subsequect characters until the std::eol character.
 */
DgReader& IgnoreComments(DgReader& in, char c)
{
	char t;

	while (in >> t)
	{
		if (t == c)
		{
			std::string str;
			std::getline(in,str);
		}
		else
		{
			in.putback(t);
			break;
		}
	}

	return in;

}	//End: IgnoreComments()
