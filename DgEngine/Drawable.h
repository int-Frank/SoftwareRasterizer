/*!
 * @file Drawable.h
 *
 * @author Frank Hart
 * @date
 *
 * class declaration: Drawable
 */

#ifndef DRAWABLE_H
#define DRAWABLE_H

class Rasterizer;

/*!
 * @ingroup render
 *
 * @class Drawable
 *
 * @brief Base class for all items that can be drawn on the screen
 *
 * [detailed description]
 *
 * @author Frank Hart
 * @date 19/01/2014
 */
class Drawable
{
public:

	Drawable() {}
	virtual ~Drawable() {}

	virtual void Draw(Rasterizer&)     = 0;
	virtual float GetSortValue() const = 0;

private:

};

#endif