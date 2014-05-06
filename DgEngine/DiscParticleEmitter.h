/*!
* @file DiscParticleEmitter.h
*
* @author Frank Hart
* @date 16/01/2014
*
* class declaration: DiscParticleEmitter
*/

#ifndef DISCPARTICLEEMITTER_H
#define DISCPARTICLEEMITTER_H

#include "ParticleEmitter.h"
#include "OBB.h"
#include "ObjectPair.h"
#include "SimpleRNG.h"

/*!
* @ingroup particle_systems
*
* @class DiscParticleEmitter
*
* @brief A particle emitter which emits particles in a defined OBB. 
*
* The base disk always has the attributes:
*       radius = 1
*       origin = (0,0,0)
*       axis = (1,0,0)
*
* [detailed description]
*
* @author Frank Hart
* @date 16/01/2014
*/
class DiscParticleEmitter : public ParticleEmitter
{
public:

	DiscParticleEmitter();
	~DiscParticleEmitter() {}

	DiscParticleEmitter(const DiscParticleEmitter&);
	DiscParticleEmitter& operator= (const DiscParticleEmitter&);

	//! Create a deep copy of the object
	DiscParticleEmitter* clone() const { return new DiscParticleEmitter(*this); }

	//! Build from xml_node
	friend pugi::xml_node& operator>>(pugi::xml_node&, DiscParticleEmitter& dest);

	/*!
	* The vqs affects the following:
	*      - size, but only on initial construction of particles
	*      - speed, but only on newly emitted particles
	*      - position, but only on newly emitted particles.
	*/
	void SetVQS(const VQS&);

	//!The angle of deviation outer particles will birth at.
	void SetOuterAngle(float ang);

private:

	SimpleRNG generator;

	//Axis of the disc
	Point4 origin;
	Vector4 axis;
	float discRadius;

	//Particles birthing on the outer rim of the disc will veer off at this angle 
	//to the axis
	float outerAngle;

	//--------------------------------------------------------------------------------
	//		Functions
	//--------------------------------------------------------------------------------
    void SetNewParticle(Particle*);

private:

    static const Point4 ORIGIN;
    static const Vector4 AXIS;
    static const float RADIUS;
};

#endif