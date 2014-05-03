/*!
 * @file Particle.h
 *
 * @author Frank Hart
 * @date 10/01/2014
 *
 * struct declaration: Particle;
 */

#ifndef PARTICLE_H
#define PARTICLE_H

#include "Point4.h"
#include "Vector4.h"
#include "DgTypes.h"

/*!
 * @ingroup particle_systems
 *
 * @class Particle
 *
 * @brief A particle is a semi-transparent 2D disk ejected into the
 * game world by a ParticleEmitter. 
 *
 * A particle has a start and end size, color and transparency which
 * changes linearly over the its lifetime. It has an
 * initial velocity, acceleration and force which acts on it. 
 *
 * @author Frank Hart
 * @date 10/01/2014
 */
struct Particle
{
	//Current data
	Point4 position;
	Vector4 velocity;
	Vector4 acceleration;
	float radius;
	float life;
	
	//Starting data
	float radius_start;
	float d_radius;

	float life_total;

	float force;

	float alpha_start;
	float d_alpha;
	float red_start;
	float d_red;
	float green_start;
	float d_green;
	float blue_start;
	float d_blue;

	///For rendering
	Point4 screenPosition;
	float screenSize;
	bool seen;
};

#endif