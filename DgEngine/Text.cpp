//================================================================================
// @ Text.cpp
// 
// Description:
//
// Definitions for the Text class.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#include "Text.h"
#include "SDL_ttf.h"
#include "CommonGraphics.h"
#include "Dg_io.h"
#include "SDL.h"
#include "Viewport.h"
#include "ViewportHandler.h"
#include "DgRect.h"


//--------------------------------------------------------------------------------
//		Static members
//--------------------------------------------------------------------------------
impl::FontManager Text::fontlist;


//--------------------------------------------------------------------------------
//		Defaults
//--------------------------------------------------------------------------------
static const std::string	DEF_name	= "Sansation_Regular";
static const uint16			DEF_size	= 10;
static const Color			DEF_clr		= Color::BLACK;
static const int32			DEF_xy		= 0;
static const std::string	DEF_string	= "";
static const float			DEF_spacing = 1.5f;
static const float			DEF_tab		= 4.0f;


//--------------------------------------------------------------------------------
//	@	Text::Text()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Text::Text() : size(DEF_size), name(DEF_name), clr(DEF_clr), x(DEF_xy), y(DEF_xy), 
	str(DEF_string), spacing(DEF_spacing), tab(DEF_tab)
{
	//Generate images
	GenerateImages();

}	//End: Text::Text()


//--------------------------------------------------------------------------------
//	@	Text::Text()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Text::Text(std::string _name, uint16 _size, Color _clr) :size(_size), name(_name), 
clr(_clr), x(DEF_xy), y(DEF_xy), str(DEF_string), spacing(DEF_spacing), tab(DEF_tab)
{
	//Generate new image
	GenerateImages();

}	//End: Text::Text()


//--------------------------------------------------------------------------------
//	@	Text::~Text()
//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
Text::~Text()
{
}	//End: Text::~Text()


//--------------------------------------------------------------------------------
//	@	Text::Text()
//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Text::Text(const Text& t) : size(t.size), name(t.name), clr(t.clr), 
	spacing(t.spacing), tab(t.tab), x(t.x), y(t.y), str(t.str)
{
	//Copy parameters
	images = t.images;
	
}	//End: Text::Text()


//--------------------------------------------------------------------------------
//	@	Text::operator=()
//--------------------------------------------------------------------------------
//		Assignment
//--------------------------------------------------------------------------------
Text& Text::operator=(const Text& t)
{
	//if same object
	if (this == &t)
		return *this;

	//Copy parameters
	images = t.images;
	size = t.size;
	name = t.name;
	clr = t.clr;
	spacing = t.spacing;
	tab = t.tab;
	x = t.x;
	y = t.y;
	str = t.str;

	return *this;

}	//End: Text::operator=()


//--------------------------------------------------------------------------------
//	@	Text::GenerateImages()
//--------------------------------------------------------------------------------
//		Generate a new series of text images. Image list is cleared if
//		font failed to load.
//--------------------------------------------------------------------------------
void Text::GenerateImages()
{
	//GetFont
	TTF_Font* font = fontlist.GetFont(name, size);

	if (font == NULL)
	{
        std::cerr << "@Text::GenerateImages() -> Failed to load font: " << name << std::endl;
		return;
	}

	//Free old images
	images.clear();
	
	//Get pixel values for tab and spacing
	uint16 tab_p = uint16(float(size)*tab);
	uint16 spacing_p = uint16(float(size)*spacing);

	//Break the string up
	DgString all(str);			//Get string as an instream
	std::string line, group;	//used to get a line or a tab group
	int yval = y;				//Current height of the group of text

	//Get next complete line of text each iteration
	while (std::getline(all, line))	
	{
		//Reset current x coordinate of the group
		int xval = x;

		DgString current(line);

		//Get text up to next tab, if one exists, otherwise whole line
		while (std::getline(current, group, '\t'))	
		{
			//Tabs, but no letters
			if (group == "")
			{
				//Increment x to next tab space
				xval += tab_p;
				continue;
			}
			
			//Find x, y coordinates of the group...

			//Create textline
			impl::Word temp_textline;
			
			//Create a new image
			temp_textline.surface.Set(TTF_RenderText_Blended(font, group.c_str(), clr), true);

			temp_textline.x = xval;
			temp_textline.y = yval;

			//Increment x to next tab space
			xval = ((xval + temp_textline.surface.w())/tab_p)*tab_p + tab_p;

			//Add to images
			images.push_back(temp_textline);
		}

		//Increment yval
		yval += spacing_p;
	}

}	//End: Text::GenerateImages()


//--------------------------------------------------------------------------------
//	@	Text::SetFont()
//--------------------------------------------------------------------------------
//		Sets the font of this text object
//--------------------------------------------------------------------------------
void Text::SetFont(std::string newfont)
{
	//Assign new name
	name = newfont;

	//Generate new image
	GenerateImages();

}	//End: Text::SetFont()


//--------------------------------------------------------------------------------
//	@	Text::SetSize()
//--------------------------------------------------------------------------------
//		Set Font size
//--------------------------------------------------------------------------------
void Text::SetSize(uint16 newsize)
{
	//Assign new size and spacing
	size = newsize;

	//Generate new image
	GenerateImages();

}	//End: Text::SetFont()


//--------------------------------------------------------------------------------
//	@	Text::SetColor()
//--------------------------------------------------------------------------------
//		Set font color
//--------------------------------------------------------------------------------
void Text::SetColor(Color newclr)
{
	//Set new color
	clr = newclr;

	//Generate new image
	GenerateImages();

}	//End: Text::SetColor()


//--------------------------------------------------------------------------------
//	@	Text::SetPosition()
//--------------------------------------------------------------------------------
//		Change the position of the text
//--------------------------------------------------------------------------------
void Text::SetPosition(int newx, int newy)
{
	//Set new position
	int xdif = newx - x;
	int ydif = newy - y;

	for (unsigned int i = 0; i < images.size(); i++)
	{
		images[i].x += xdif;
		images[i].y += ydif;
	}

	x = newx;
	y = newy;

}	//End: Text::SetColor()


//--------------------------------------------------------------------------------
//	@	Text::SetString()
//--------------------------------------------------------------------------------
//		Set string
//--------------------------------------------------------------------------------
void Text::SetString(std::string newstr)
{
	//Set new string
	str = newstr;

	//Generate new image
	GenerateImages();

}	//End: Text::SetColor()


//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Set string with an instream.
//--------------------------------------------------------------------------------
DgReader& operator>>(DgReader& in, Text& dest)
{
	//Assign new string
	dest.str = std::string( (std::istreambuf_iterator<char>(in) ),
                       (std::istreambuf_iterator<char>()    ) );

	//Generate new image
	dest.GenerateImages();

	return in;

}	//End: Text::SetStringFromFile()


//--------------------------------------------------------------------------------
//	@	Text::SetSpacing()
//--------------------------------------------------------------------------------
//		Set line spacing
//--------------------------------------------------------------------------------
void Text::SetSpacing(float new_spacing)
{
	//Set new string
	spacing = new_spacing;

	//Generate new image
	GenerateImages();

}	//End: Text::SetSpacing()


//--------------------------------------------------------------------------------
//	@	Text::SetTab()
//--------------------------------------------------------------------------------
//		Set tab size
//--------------------------------------------------------------------------------
void Text::SetTab(float new_tab)
{
	//Set new string
	tab = new_tab;

	//Generate new image
	GenerateImages();

}	//End: Text::SetColor()


//--------------------------------------------------------------------------------
//	@	Text::Draw()
//--------------------------------------------------------------------------------
//		Display Text directly onto an image
//--------------------------------------------------------------------------------
void Text::Draw(Image& dest) const
{
	for (unsigned int i = 0; i < images.size(); i++)
	{
		ApplyImage(	images[i].surface, dest,
					images[i].x, images[i].y, 
					DgGraphics::BlendType::ALPHA);
	}

}	//End: Text::Draw()


//--------------------------------------------------------------------------------
//	@	Text::Draw()
//--------------------------------------------------------------------------------
//		Display Text onto a viewport
//--------------------------------------------------------------------------------
void Text::Draw(Viewport& dest) const
{
	for (unsigned int i = 0; i < images.size(); i++)
	{
		dest.BlitImage(	images[i].surface,
						images[i].x, images[i].y, 
						DgGraphics::BlendType::ALPHA);
	}

}	//End: Text::Draw()


//--------------------------------------------------------------------------------
//	@	Text::Draw()
//--------------------------------------------------------------------------------
//		Display Text on a viewport from the ViewportHandler.
//--------------------------------------------------------------------------------
void Text::Draw(viewportID id) const
{
	Viewport* dest = ViewportHandler::GetViewport(id);

	if (dest == NULL)
		return;

	for (unsigned int i = 0; i < images.size(); i++)
	{
		dest->BlitImage(images[i].surface,
			images[i].x, images[i].y,
			DgGraphics::BlendType::ALPHA);
	}

}	//End: Text::Draw()


//--------------------------------------------------------------------------------
//	@	Text::GetBounds()
//--------------------------------------------------------------------------------
//		Returns the bounds of the text object
//--------------------------------------------------------------------------------
DgRect Text::GetBounds() const
{
	if (images.empty())
		return DgRect(0,0,1,1);

	int32 xmin = images[0].x;
	int32 ymin = images[0].y;
	int32 xmax = xmin + images[0].surface.w();
	int32 ymax = ymin + images[0].surface.h();

	//loop through all words to find bounds
	for (uint32 i = 1; i < images.size(); ++i)
	{
		if (images[i].x < xmin)
			xmin = images[i].x;
		if (images[i].y < ymin)
			ymin = images[i].y;
		if (images[i].x + int32(images[i].surface.w()) > xmax)
			xmax = images[i].x + int32(images[i].surface.w());
		if (images[i].y + int32(images[i].surface.h()) > ymax)
			ymax = images[i].y + int32(images[i].surface.h());
	}

	return DgRect(xmin, ymin, (xmax - xmin), (ymax - ymin));

}	//End: Text::GetBounds()