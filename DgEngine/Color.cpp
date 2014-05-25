#include "Color.h"
#include "SDL.h"
#include "DgError.h"

//--------------------------------------------------------------------------------
//		Static members
//--------------------------------------------------------------------------------
const Color Color::COLORKEY(255,0,255,0); 
const Color Color::RED(255,0,0);
const Color Color::GREEN(0,255,0);
const Color Color::BLUE(0,0,255);
const Color Color::YELLOW(255,255,0);
const Color Color::INDIGO(255,0,255);
const Color Color::CYAN(0,255,255);
const Color Color::BLACK(0,0,0);
const Color Color::WHITE(255,255,255);


//--------------------------------------------------------------------------------
//	@	Color::Color()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Color::Color(uint8 r, uint8 g, uint8 b, uint8 a)
{
	rgba.red = r;
	rgba.green = g;
	rgba.blue = b;
	rgba.alpha = a;
}


//--------------------------------------------------------------------------------
//	@	Color::operator=()
//--------------------------------------------------------------------------------
//		Assignment operator
//--------------------------------------------------------------------------------
Color& Color::operator= (const Color& clr)
{
	i = clr.i;

	return *this;

}	//End: Color::operator=()


//--------------------------------------------------------------------------------
//	@	Color::operator SDL_Color()
//--------------------------------------------------------------------------------
//		Conversion
//--------------------------------------------------------------------------------
Color::operator SDL_Color() const
{
	SDL_Color temp;
	temp.r = rgba.red;
	temp.g = rgba.green;
	temp.b = rgba.blue;
	temp.a = rgba.alpha;

	return temp;

}	//End: Color::operator SDL_Color()


//--------------------------------------------------------------------------------
//	@	operator<<()
//--------------------------------------------------------------------------------
//		Output 
//--------------------------------------------------------------------------------
DgWriter& operator<<(DgWriter& out, const Color& source)
{
	out << '<' << source.rgba.blue << ' ' <<
		source.rgba.green << ' ' <<
		source.rgba.red << ' ' <<
		source.rgba.alpha << '>';

	return out;
    
}   // End of operator<<()


//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Build vector from in stream
//--------------------------------------------------------------------------------
DgReader& operator>> (DgReader& in, Color& dest)
{
	//Read to values
	uint32 r,g,b,a;
	in >> r >> g >> b >> a;

	dest.rgba.red = uint8(r);
	dest.rgba.green = uint8(g);
	dest.rgba.blue = uint8(b);
	dest.rgba.alpha = uint8(a);

	if (!in)
	{
		std::cerr << "@operator>>(Color) -> Bad read." << std::endl;
	}

	return in;

}	//End: operator>>()


//--------------------------------------------------------------------------------
//	@	Color::operator==()
//--------------------------------------------------------------------------------
//		Comparison
//--------------------------------------------------------------------------------
bool Color::operator==(const Color& a) const
{
	return (i == a.i);

}	//End: operator==(Color)


//--------------------------------------------------------------------------------
//	@	Color::operator!=()
//--------------------------------------------------------------------------------
//		Comparison
//--------------------------------------------------------------------------------
bool Color::operator!=(const Color& a) const
{
	return (i != a.i);

}	//End: operator==(Color)
