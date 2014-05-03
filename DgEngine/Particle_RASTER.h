/*!
 * @file Particle_RASTER
 *
 * @author Frank Hart
 * @date 9/01/2014
 *
 * struct declaration: Particle_RASTER
 */

#ifndef PARTICLE_RASTER_H
#define PARTICLE_RASTER_H

#include "Point4.h"
#include "DgTypes.h"
#include "Drawable.h"
#include "Rasterizer.h"

class ParticleAlphaTemplate;

/*!
 * @ingroup particle_systems
 *
 * @struct Particle_RASTER
 *
 * @brief This structure contains the minimum amount of data
 * required to draw a particle on the screen.
 *
 * @author Frank Hart
 * @date 9/01/2014
 */
struct Particle_RASTER : public Drawable
{
	bool operator<(const Particle_RASTER& other)
	{return position.Z() < other.position.Z();}

	bool operator>(const Particle_RASTER& other)
	{return position.Z() > other.position.Z();}

	bool operator==(const Particle_RASTER& other)
	{return position.Z() == other.position.Z();}

	bool operator!=(const Particle_RASTER& other)
	{return position.Z() != other.position.Z();}

	void Draw(Rasterizer& r) { r.DrawParticle(*this); }
	float GetSortValue() const { return position.Z(); }

	Point4 position;
	float radius;	
	uint32 color;
	const ParticleAlphaTemplate* alphaTemplate;
};

#endif