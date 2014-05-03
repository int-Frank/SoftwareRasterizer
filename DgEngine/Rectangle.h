/*!
* @file Rectangle.h
*
* @author Frank Hart
* @date 22/01/2014
*
* Class declaration: Rectangle
*/

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"
#include "DgTypes.h"
#include "DgRect.h"

/*!
* @ingroup graphics_shapes
*
* @class Rectangle
*
* @brief Simple shapes that can be drawn to an Image.
*
* [detailed description]
*
* @author Frank Hart
* @date 22/01/2014
*/
class Rectangle : public Shape
{
public:

	Rectangle();
	Rectangle(uint32 w, uint32 h, int32 x, int32 y, uint32 c);

	void SetDimensions(uint32 _w, uint32 _h) { rect.w = _w; rect.h = _h; }
	void SetPosition(int32 _x, int32 _y) { rect.x = _x; rect.y = _y; }
	void SetBox(const DgRect& b) { rect = b; }
	void SetColor(uint32 c) { color = c; }

	void Draw(Image&) const;

private:

	DgRect rect;
	uint32 color;


};

#endif