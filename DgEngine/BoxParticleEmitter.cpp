/*!
* @file BoxParticleEmitter.cpp
*
* @author Frank Hart
* @date 16/01/2014
*
* class definitions: BoxParticleEmitter
*/

#include "BoxParticleEmitter.h"
#include "pugixml.hpp"


//--------------------------------------------------------------------------------
//	@	BoxParticleEmitter::BoxParticleEmitter()
//--------------------------------------------------------------------------------
//		Constructor. Uses default parameters.
//--------------------------------------------------------------------------------
BoxParticleEmitter::BoxParticleEmitter() : ParticleEmitter()
{
	//Set the box
	Tuple<float> t(BE_DEFAULT_LENGTH_X0,
				   BE_DEFAULT_LENGTH_X1,
				   BE_DEFAULT_LENGTH_X2);
	BasisR3 b;
	Point4 p(BE_DEFAULT_ORIGIN);

	box.Set(OBB(p, b, t));

	//Set the direction data
	ndb_f_param c(BE_DEFAULT_THETA_MEAN,
				  BE_DEFAULT_THETA_SD,
				  BE_DEFAULT_THETA_MIN,
				  BE_DEFAULT_THETA_MAX);

	SetDirection(BE_DEFAULT_AXIS, c);

}	//End: BoxParticleEmitter::BoxParticleEmitter()


//--------------------------------------------------------------------------------
//	@	BoxParticleEmitter::BoxParticleEmitter()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
BoxParticleEmitter::BoxParticleEmitter(const BoxParticleEmitter& other) :
ParticleEmitter(other), box(other.box), generator(other.generator),
direction(other.direction), direction_BASE(other.direction_BASE), theta(other.theta)
{
}	//End: BoxParticleEmitter::BoxParticleEmitter()


//--------------------------------------------------------------------------------
//	@	BoxParticleEmitter::operator=()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
BoxParticleEmitter& BoxParticleEmitter::operator=(const BoxParticleEmitter& other)
{
	if (this == &other)
		return *this;

	ParticleEmitter::operator=(other);

	box = other.box;
	generator = other.generator;
	direction = other.direction;
	direction_BASE = other.direction_BASE;
	theta = other.theta;

	return *this;
}	//End: BoxParticleEmitter::operator=()


//--------------------------------------------------------------------------------
//	@	operator>>()
//--------------------------------------------------------------------------------
//		Build particle emitter from an xml node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, BoxParticleEmitter& dest)
{
	//Build base data
	dest.BuildBase(node);

	//iterate through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
	{
		//Get the name of the node
		std::string tag = it->name();

		if (tag == "box")
		{
			OBB b;
			*it >> b;
			dest.SetOBB(b);
		}
		else if (tag == "deviationAngle")
		{
			ndb_f_param t;
			Vector4 v;
			DgString(it->child_value()) >> v >> t;
			dest.SetDirection(v,t);
		}
	}

	return node;

}	//End: Read_VQS()


//--------------------------------------------------------------------------------
//	@	BoxParticleEmitter::SetVQS()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void BoxParticleEmitter::SetVQS(const VQS& in_vqs)
{
	//Set vqs
	vqs = in_vqs;

	//Transform vectors
	direction = direction_BASE;
	vqs.RotateSelf(direction);

	//Transform box.
	box.TransformQuick(vqs);

}	//End: ParticleEmitter::SetNumberParticles()


//--------------------------------------------------------------------------------
//	@	BoxParticleEmitter::SetDirection()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void BoxParticleEmitter::SetDirection(const Vector4& d, const ndb_f_param& t)
{
	direction_BASE = d;

	direction = direction_BASE;
	vqs.RotateSelf(direction);

	theta = t;

}	//End: ParticleEmitter::SetDirection()


//--------------------------------------------------------------------------------
//	@	BoxParticleEmitter::SetVQS()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void BoxParticleEmitter::SetOBB(const OBB& b)
{
	//Set box
	box.Set(b);

	//Transform box.
	box.TransformQuick(vqs);

}	//End: BoxParticleEmitter::SetVQS()


//--------------------------------------------------------------------------------
//	@	BoxParticleEmitter::GetNumberToEmit()
//--------------------------------------------------------------------------------
//		Culls particles against a frustum
//--------------------------------------------------------------------------------
void BoxParticleEmitter::SetNewParticle(Particle* par)
{
	par->position = box.current.GetPointInside();
	par->velocity = GetRandomConeVector(direction, theta.Get()) * entry_speed.Get() * vqs.S() + velocity_global;
	par->acceleration = acceleration_global;
	par->life = 0.0f;

}	//End: ParticleEmitter::FrustumCull()