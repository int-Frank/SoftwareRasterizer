//================================================================================
// @ FontManager.cpp
// 
// Description:
//
// Definitions for the FontManager class.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "FontManager.h"
#include "DgError.h"
#include "SDL_ttf.h"
#include "DgTypes.h"


//--------------------------------------------------------------------------------
//	@	ttf::ttf()
//--------------------------------------------------------------------------------
//		ttf Constructor
//--------------------------------------------------------------------------------
impl::ttf::ttf(std::string _name, Uint16 _size)
{
	//Set parameters
	size = _size;
	name = _name;

	//Ready path
	std::string path = folder + name + ".ttf";

	//Load Default font
	font = TTF_OpenFont(path.c_str(), size);

	//Check if loaded
	if (font == NULL)
	{
		std::string str;
		str = "@ttf::ttf(constructor) -> Failed to load font '" + name 
			+ "'";
		ERROR_OUT(str);
	}

}	//End: ttf::ttf()


//--------------------------------------------------------------------------------
//	@	ttf::~ttf()
//--------------------------------------------------------------------------------
//		ttf Destructor
//--------------------------------------------------------------------------------
impl::ttf::~ttf()
{
	//Release font
	TTF_CloseFont( font );
}


//--------------------------------------------------------------------------------
//	@	ttf::ttf()
//--------------------------------------------------------------------------------
//		ttf Copy Constructor
//--------------------------------------------------------------------------------
impl::ttf::ttf(const ttf& t)
{
	//Set parameters
	size = t.size;
	name = t.name;

	//Ready path
	std::string path = folder + name + ".ttf";

	//Load Default font
	font = TTF_OpenFont(path.c_str(), size);

	//Check if loaded
	if (font == NULL)
	{
		std::string str;
		str = "@ttf::ttf(copy) -> Failed to load font '" + name 
			+ "'";
		ERROR_OUT(str);
	}
	
}	//End: ttf::ttf()


//--------------------------------------------------------------------------------
//	@	ttf::IsEqual()
//--------------------------------------------------------------------------------
//		Comparison
//--------------------------------------------------------------------------------
bool impl::ttf::IsEqual(const std::string tag, uint16 input_size) const
{
	return (name == tag) && (size == input_size);

}	//End: ttf::operator==()







//--------------------------------------------------------------------------------
//	@	FontManager::GetFont()
//--------------------------------------------------------------------------------
//		Get a font from the list. Attempts to load the font is not
//		already in the list.
//--------------------------------------------------------------------------------
TTF_Font* impl::FontManager::GetFont(std::string str, uint16 val)
{
	//fontlist iterator
	std::list<ttf>::iterator it;

	//Loop through list, searching for the font
	for (it = fontlist.begin(); it != fontlist.end(); ++it)
	{
		//If font already exists
		if (it->IsEqual(str, val))
			return it->Font();
	}

	//If font not found

	//Try to load font
	impl::ttf temp(str, val);
	if (temp.Font() == NULL)
	{
		return NULL;
	}

	//Add to list and return pointer
	fontlist.push_back(temp);
	return fontlist.back().Font();

}	//End: FontManager::GetFont()


//--------------------------------------------------------------------------------
//	@	FontManager::RemoveFont()
//--------------------------------------------------------------------------------
//		Remove a font from the list
//--------------------------------------------------------------------------------
void impl::FontManager::RemoveFont(std::string str, uint16 val)
{
	//fontlist iterator
	std::list<ttf>::iterator it;

	//Loop through list, searching for the font
	for (it =  fontlist.begin(); it != fontlist.end(); ++it)
	{
		//If font exists
		if (it->IsEqual(str, val))
		{
			//Remove font
			it = fontlist.erase(it);
		}
	}

}	//End: FontManager::RemoveFont()