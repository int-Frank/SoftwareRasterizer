/*!
 * @file Shape.h
 *
 * @author Frank Hart
 * @date 22/01/2014
 *
 * Class declaration: Shape
 */

#ifndef SHAPE_H
#define SHAPE_H

class Image;

/*!
 * @ingroup graphics_shapes
 *
 * @class Shape
 *
 * @brief Simple shapes that can be drawn to an Image.
 *
 * [detailed description]
 *
 * @author Frank Hart
 * @date 22/01/2014
 */
class Shape
{
public:

	Shape(){}
	virtual ~Shape(){}

	virtual void Draw(Image&) const = 0;

private:

};

#endif