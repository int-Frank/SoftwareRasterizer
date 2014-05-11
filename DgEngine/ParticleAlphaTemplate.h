/*!
* @file ParticleAlphaTemplate
*
* @author Frank Hart
* @date 9/01/2014
*
* struct declaration: ParticleAlphaTemplate
*/

#ifndef PARTICLEALPHATEMPLATE_H
#define PARTICLEALPHATEMPLATE_H

#include "DgTypes.h"

/*!
 * @ingroup particle_systems
 *
 * @class ParticleAlphaTemplate
 *
 * @brief Per pixel alpha values for a particle.
 *
 * [detailed description]
 *
 * @author Frank Hart
 * @date 9/01/2014
 */
class ParticleAlphaTemplate
{
public:

	//! The dimension of all alpha templates.
	static const uint32 SIZE = 128;

	ParticleAlphaTemplate() { Set(1.5); }
	ParticleAlphaTemplate(double exp) { Set(exp); }
	~ParticleAlphaTemplate() {}

	//! Polynomial alpha fall off from 255(center) to 0(edges)
	//! A master alpha value can be set all well to reduce
	//! the alpha of the entire template.
	void Set(double exp);

	//! Return the data
	const uint8* GetTemplate() const { return data; }

private:

	uint8 data[SIZE * SIZE];

};

#endif