//================================================================================
// @ FontManager.h
// 
// Class: FontManager
//
// Class for loading, storing and handing out true type fonts. Only one instance 
// of the class is needed; as font resource in the Text class. The static member
// ttf::folder defines which folder the font files are located. This is defined
// in Resources.cpp.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef TTFCONTAINER_H
#define TTFCONTAINER_H

#include <list>
#include "DgTypes.h"

typedef struct _TTF_Font TTF_Font;


namespace impl
{
	//--------------------------------------------------------------------------------
	//		Wrapper class for true type fonts
	//--------------------------------------------------------------------------------
	class ttf
	{
	public:
		//Constructor/Destructor
		ttf(std::string _name, uint16 _size);
		~ttf();

		//Copy constructor
		ttf(const ttf&);

		//comparison
		bool IsEqual(const std::string tag, uint16 sze) const;

		//Return functions
		TTF_Font* Font() const { return font; }

	private:
		//Data
		TTF_Font* font;
		std::string name;
		uint16 size;

		//Folt folder
		static const std::string folder;

	};


	//--------------------------------------------------------------------------------
	//	@	FontManager
	//--------------------------------------------------------------------------------
	class FontManager
	{
	public:
		//Constructor/destructor
		FontManager(){}
		~FontManager() {}

		//Return a font from the list. Loads fonts as needed.
		TTF_Font* GetFont(std::string str, uint16 val);

		//Remove font by attributes.
		void RemoveFont(std::string str, uint16 val);

		//Clear all fonts.
		void Clear() {fontlist.clear();}

	private:
		//Data members
		std::list<ttf> fontlist;

		//Disallow copy operations
		FontManager(const FontManager&);
		FontManager& operator= (const FontManager&);

	};
}

#endif