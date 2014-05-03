//================================================================================
// @ Text.h
// 
// Class: Text
//
// Class for handling and displaying text. Text objects are essentially 
// alpha-keyed Images.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef TEXT_H
#define TEXT_H

#include "Color.h"
#include "FontManager.h"
#include "Image.h"
#include "DgTypes.h"
#include "Dg_io.h"
#include <string>
#include <vector>

class Image;
class Viewport;
typedef struct _TTF_Font TTF_Font;
struct DgRect;

//--------------------------------------------------------------------------------
//		One 'word' of text. Lines of text are broken up into words. 
//		Characters that break up words are tab and new line.
//--------------------------------------------------------------------------------
namespace impl
{
	struct Word
	{
		//Constructors
		Word(){}
		Word(const Image& g, int _x, int _y) : surface(g), x(_x), y(_y) {}
		~Word(){}

		//Data members
		Image surface;
		int x, y;

	};
}

//--------------------------------------------------------------------------------
//		Class for storing and displaying text on the screen
//--------------------------------------------------------------------------------
class Text
{
public:
	//Constructor/Destructor
	Text();
	Text(std::string name, uint16 _size, Color = Color::BLACK);
	~Text();

	//Copy operations
	Text(const Text&);
	Text& operator= (const Text&);

	//Input
	friend DgReader& operator>>(DgReader& in, Text& dest);

	//Manipulators
	void SetFont(std::string);
	void SetSize(uint16);
	void SetColor(Color newclr);
	void SetPosition(int xval, int yval);
	void SetString(std::string newstr);
	void SetSpacing(float s);
	void SetTab(float t);

	//Return functions
	int X() {return x;}
	int Y() {return y;}
	std::string GetString() {return str;}
	
	//Display methods
	void Draw(Image&) const;
	void Draw(Viewport&) const;
	void Draw(viewportID) const;

	//Get the bounds of the text
	DgRect GetBounds() const;

	//Clears resources.
	//Must be called before exit
	static void ClearResources() {fontlist.Clear();}

private:

	//Text attributes
	Color clr;						//Color of the font
	int32 x, y;						//Position to be drawn to
	std::string str;				//The text to display
	float spacing;					//Line spacing
	float tab;						//Tab size

	//The lines of text
	std::vector<impl::Word> images;	//list of all images

	//Font parameters
	uint16 size;
	std::string name;	//Name of the font

	//--------------------------------------------------------------------------------
	//		Functions
	//--------------------------------------------------------------------------------

	//Render new image
	void GenerateImages();
	void SetSpacings();

	//Resource manager
	static impl::FontManager fontlist;

};

#endif