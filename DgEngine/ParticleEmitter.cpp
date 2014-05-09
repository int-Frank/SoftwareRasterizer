/*!
 * @file ParticleEmitter.cpp
 *
 * @author Frank Hart
 * @date 12/01/2014
 *
 * class definitions: ParticleEmitter
 */

#include "ParticleEmitter.h"
#include "ParticleEngineDefaults.inl"
#include "CommonMath.h"
#include "pugixml.hpp"
#include "Viewport.h"
#include "Frustum.h"


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::ParticleEmitter()
//--------------------------------------------------------------------------------
//		Constructor. Uses default parameters.
//--------------------------------------------------------------------------------
ParticleEmitter::ParticleEmitter() : velocity_global(0.0f, 0.0f, 0.0f),
									 acceleration_global(0.0f, 0.0f, 0.0f),
									 dtLinear(0.0f)
{
	//Set up the default params

	//--------------------------------------------------------------------------------
	//		Radii
	//--------------------------------------------------------------------------------
	ndb_f_param RADIUS_START(PE_DEFAULT_RADIUS_START_MEAN, PE_DEFAULT_RADIUS_START_SD,
		PE_DEFAULT_RADIUS_START_MIN, PE_DEFAULT_RADIUS_START_MAX);

	ndb_f_param RADIUS_END(PE_DEFAULT_RADIUS_END_MEAN, PE_DEFAULT_RADIUS_END_SD,
		PE_DEFAULT_RADIUS_END_MIN, PE_DEFAULT_RADIUS_END_MAX);

	//--------------------------------------------------------------------------------
	//		Entry speed and acceleration
	//--------------------------------------------------------------------------------
	ndb_f_param ENTRY_SPEED(PE_DEFAULT_ENTRYSPEED_MEAN, PE_DEFAULT_ENTRYSPEED_SD,
		PE_DEFAULT_ENTRYSPEED_MIN, PE_DEFAULT_ENTRYSPEED_MAX);

	ndb_f_param ENTRY_ACCELERATION(PE_DEFAULT_FORCE_MEAN, PE_DEFAULT_FORCE_SD,
		PE_DEFAULT_FORCE_MIN, PE_DEFAULT_FORCE_MAX);

	//--------------------------------------------------------------------------------
	//		Life/dead times
	//--------------------------------------------------------------------------------
	ndb_f_param ALIVE(PE_DEFAULT_ALIVE_MEAN, PE_DEFAULT_ALIVE_SD,
		PE_DEFAULT_ALIVE_MIN, PE_DEFAULT_ALIVE_MAX);

	//--------------------------------------------------------------------------------
	//		Start/end colors
	//--------------------------------------------------------------------------------
	ndb_f_param RED_START(PE_DEFAULT_RED_START_MEAN, PE_DEFAULT_RED_START_SD,
		PE_DEFAULT_RED_START_MIN, PE_DEFAULT_RED_START_MAX);

	ndb_f_param GREEN_START(PE_DEFAULT_GREEN_START_MEAN, PE_DEFAULT_GREEN_START_SD,
		PE_DEFAULT_GREEN_START_MIN, PE_DEFAULT_GREEN_START_MAX);

	ndb_f_param BLUE_START(PE_DEFAULT_BLUE_START_MEAN, PE_DEFAULT_BLUE_START_SD,
		PE_DEFAULT_BLUE_START_MIN, PE_DEFAULT_BLUE_START_MAX);

	ndb_f_param ALPHA_START(PE_DEFAULT_ALPHA_START_MEAN, PE_DEFAULT_ALPHA_START_SD,
		PE_DEFAULT_ALPHA_START_MIN, PE_DEFAULT_ALPHA_START_MAX);

	ndb_f_param RED_END(PE_DEFAULT_RED_END_MEAN, PE_DEFAULT_RED_END_SD,
		PE_DEFAULT_RED_END_MIN, PE_DEFAULT_RED_END_MAX);

	ndb_f_param GREEN_END(PE_DEFAULT_GREEN_END_MEAN, PE_DEFAULT_GREEN_END_SD,
		PE_DEFAULT_GREEN_END_MIN, PE_DEFAULT_GREEN_END_MAX);

	ndb_f_param BLUE_END(PE_DEFAULT_BLUE_END_MEAN, PE_DEFAULT_BLUE_END_SD,
		PE_DEFAULT_BLUE_END_MIN, PE_DEFAULT_BLUE_END_MAX);

	ndb_f_param ALPHA_END(PE_DEFAULT_ALPHA_END_MEAN, PE_DEFAULT_ALPHA_END_SD,
		PE_DEFAULT_ALPHA_END_MIN, PE_DEFAULT_ALPHA_END_MAX);

	//--------------------------------------------------------------------------------
	//		Set the data
	//--------------------------------------------------------------------------------
	SetMaxNumber(PE_DEFAULT_NUMBER);
	
	SetSpawnMethod_POISON(PE_DEFAULT_EMISSIONRATE);

	SetRadii(RADIUS_START, RADIUS_END);

	SetEntrySpeed(ENTRY_SPEED);

	SetForce(ENTRY_ACCELERATION);

	SetGlobal(PE_DEFAULT_VELOCITY, PE_DEFAULT_ACCELERATION);

	SetLifeTime(ALIVE);

	SetColor(RED_START, GREEN_START, BLUE_START, ALPHA_START,
			 RED_END,   GREEN_END,   BLUE_END,   ALPHA_END);

	//Generate default particles. This will increase any load times,
	//but is safe
	Initiate();

}	//End: ParticleEmitter::ParticleEmitter()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::init()
//--------------------------------------------------------------------------------
//		Initiate from other
//--------------------------------------------------------------------------------
void ParticleEmitter::init(const ParticleEmitter& other)
{
	emissionRate_Poisson = other.emissionRate_Poisson;
	emissionType = other.emissionType;
	linearBirthTime = other.linearBirthTime;
	dtLinear = other.dtLinear;

	alphaTemplate = other.alphaTemplate;

	radius_start = other.radius_start;
	radius_end = other.radius_end;
	entry_speed = other.entry_speed;
	force = other.force;
	lifeTime = other.lifeTime;

	red_start = other.red_start;
	green_start = other.green_start;
	blue_start = other.blue_start;
	alpha_start = other.alpha_start;
	red_end = other.red_end;
	green_end = other.green_end;
	blue_end = other.blue_end;
	alpha_end = other.alpha_end;

	velocity_global = other.velocity_global;
	acceleration_global = other.acceleration_global;

	vqs = other.vqs;

	flags = other.flags;

	//As particles are really separate objets to the emitter, do not 
	//copy particles. Only set the list size.
	particles.resize(other.particles.max_size());
	Initiate();

}	//End: ParticleEmitter::()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::ParticleEmitter()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
ParticleEmitter::ParticleEmitter(const ParticleEmitter& other)
{
	init(other);
}	//End: ParticleEmitter::ParticleEmitter()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::operator=()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
ParticleEmitter& ParticleEmitter::operator=(const ParticleEmitter& other)
{
	if (this == &other)
		return *this;

	init(other);

	return *this;
}	//End: ParticleEmitter::operator=()


//--------------------------------------------------------------------------------
//	@operator>>()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, ParticleEmitter& dest)
{
	dest.BuildBase(node);

	return node;
}	//End: operator>>()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::BuildBase
//--------------------------------------------------------------------------------
//		Build particle emitter from an xml node
//--------------------------------------------------------------------------------
void ParticleEmitter::BuildBase(pugi::xml_node& node)
{
	//iterate through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
	{
		//Get the name of the node
		std::string tag = it->name();
		
		if (tag == "alphaTemplate")
		{
			double exp, master;
			DgString(it->child_value()) >> exp >> master;
			alphaTemplate.Set(exp, master);
		}
		else if (tag == "radii")
		{
			ndb_f_param start, end;
			DgString(it->child_value()) >> start >> end;
			SetRadii(start, end);
		}
		else if (tag == "emission_Poisson")
		{
			float r;
			DgString(it->child_value()) >> r;
			SetSpawnMethod_POISON(r);
		}
		else if (tag == "emission_Linear")
		{
			float r;
			DgString(it->child_value()) >> r;
			SetSpawnMethod_LINEAR(r);
		}
		else if (tag == "emission_Instant")
		{
			SetSpawnMethod_INSTANT();
		}
		else if (tag == "maxNumber")
		{
			uint32 n;
			DgString(it->child_value()) >> n;
			SetMaxNumber(n);
		}
		else if (tag == "life")
		{
			ndb_f_param l;
			DgString(it->child_value()) >> l;
			SetLifeTime(l);
		}
		else if (tag == "entrySpeed")
		{
			ndb_f_param spd;
			DgString(it->child_value()) >> spd;
			SetEntrySpeed(spd);
		}
		else if (tag == "force")
		{
			ndb_f_param a;
			DgString(it->child_value()) >> a;
			SetForce(a);
		}
		else if (tag == "color")
		{
			ndb_f_param	red_s, red_e,
						green_s, green_e,
						blue_s, blue_e,
						alpha_s, alpha_e;

			DgString(it->child_value()) >> red_s >> red_e >>
							green_s >> green_e >>
							blue_s >> blue_e >>
							alpha_s >> alpha_e;
			SetColor(red_s, green_s,
						  blue_s, alpha_s,
						  red_e, green_e,
						  blue_e, alpha_e);
		}
		else if (tag == "isActive")
		{
			SetActive(ToBool(it->child_value()));
		}
		else if (tag == "globals")
		{
			Vector4 v, a;
			DgString(it->child_value()) >> v >> a;
			SetGlobal(v, a);
		}
		
	}

	//Commit changes
	Initiate();

}	//End: ParticleEmitter::BuildBase


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::SetNumberParticles()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void ParticleEmitter::SetMaxNumber(uint32 value)
{
	//Resize array
	particles.resize(value);

}	//End: ParticleEmitter::SetNumberParticles()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::SetEmissionRate()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void ParticleEmitter::SetSpawnMethod_POISON(float rate)
{
	//Resize array
	emissionRate_Poisson.Set(rate);
	emissionType = POISSON;

}	//End: ParticleEmitter::SetEmissionRate()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::SetBirthMethod_LINEAR()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void ParticleEmitter::SetSpawnMethod_LINEAR(float rate)
{
	//Resize array
	if (rate == 0.0f)
		linearBirthTime = FLT_MAX;
	else
		linearBirthTime = 1.0f / rate;
	
	emissionType = LINEAR;

}	//End: ParticleEmitter::SetEmissionRate()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::SetBirthMethod_LINEAR()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void ParticleEmitter::SetSpawnMethod_INSTANT()
{
	emissionType = INSTANT;

}	//End: ParticleEmitter::SetEmissionRate()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::SetAlphaTemplate()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void ParticleEmitter::SetAlphaTemplate(double exp, double master)
{
	alphaTemplate.Set(exp, master);
}	//End: ParticleEmitter::SetAlphaTemplate()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::SetRadii()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void ParticleEmitter::SetRadii(const ndb_f_param& start, const ndb_f_param& end)
{
	radius_start.Set(start);
	radius_end.Set(end);

}	//End: ParticleEmitter::SetRadii()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::SetEntrySpeed()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void ParticleEmitter::SetEntrySpeed(const ndb_f_param& speed)
{
	entry_speed.Set(speed);

}	//End: ParticleEmitter::SetEntrySpeed()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::SetEntrySpeed()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void ParticleEmitter::SetForce(const ndb_f_param& a)
{
	if (a.lowerBound < 0.0f)
		return;

	force.Set(a);

	//Update all particles
	DgLinkedList<Particle>::iterator it = particles.begin();

	for (it; it != particles.end(); ++it)
	{
		it->force = force.Get();
	}
}	//End: ParticleEmitter::SetEntrySpeed()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::SetLifeTime()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void ParticleEmitter::SetLifeTime(const ndb_f_param& life)
{
	lifeTime.Set(life);

}	//End: ParticleEmitter::SetLifeTime()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::SetColor()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void ParticleEmitter::SetColor(
	const ndb_f_param& red_s, const ndb_f_param& green_s,
	const ndb_f_param& blue_s, const ndb_f_param& alpha_s,
	const ndb_f_param& red_e, const ndb_f_param& green_e,
	const ndb_f_param& blue_e, const ndb_f_param& alpha_e)
{
	red_start.Set(red_s);
	green_start.Set(green_s);
	blue_start.Set(blue_s);
	alpha_start.Set(alpha_s);

	red_end.Set(red_e);
	green_end.Set(green_e);
	blue_end.Set(blue_e);
	alpha_end.Set(alpha_e);

}	//End: ParticleEmitter::SetColor()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::SetVQS()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void ParticleEmitter::SetVQS(const VQS& in_vqs)
{
	//Set vqs
	vqs = in_vqs;

}	//End: ParticleEmitter::SetNumberParticles()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::SetGlobal()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void ParticleEmitter::SetGlobal(const Vector4& v, const Vector4& a)
{
	velocity_global = v;
	acceleration_global = a;

}	//End: ParticleEmitter::SetGlobal()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::SetGlobalVelocity()
//--------------------------------------------------------------------------------
//		
//--------------------------------------------------------------------------------
void ParticleEmitter::SetGlobalVelocity(const Vector4& v)
{
    velocity_global = v;

}	//End: ParticleEmitter::SetGlobalVelocity()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::FrustumCull()
//--------------------------------------------------------------------------------
//		Culls particles against a frustum
//--------------------------------------------------------------------------------
void ParticleEmitter::FrustumCull(const Frustum& f)
{
	DgLinkedList<Particle>::iterator it = particles.begin();

	for (it; it != particles.end(); ++it)
	{
		it->seen = f.TestParticle(it->position, it->radius);
	}

}	//End: ParticleEmitter::FrustumCull()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::GetNumberToAdd()
//--------------------------------------------------------------------------------
//		Culls particles against a frustum
//--------------------------------------------------------------------------------
uint32 ParticleEmitter::GetNumberToAdd(float dt)
{
	uint32 num = 0;
	switch (emissionType)
	{
	case LINEAR:
		{
			dtLinear += dt;
			float numf = (dtLinear / linearBirthTime);
			dtLinear -= linearBirthTime * floor(numf);
			num = uint32(numf);
			break;
		}
	case POISSON:
		{
			num = emissionRate_Poisson.Get(dt);
			break;
		}
	case INSTANT:
		{
			num = (particles.max_size() - particles.size());
			break;
		}
	}

	return DgMin(num, uint32(particles.max_size() - particles.size()));

}	//End: ParticleEmitter::GetNumberToAdd()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::GetNumberToEmit()
//--------------------------------------------------------------------------------
//		Culls particles against a frustum
//--------------------------------------------------------------------------------
void ParticleEmitter::SetNewParticle(Particle* par, float dt)
{
    par->position = Point4::origin + vqs.V();
    par->velocity = GetRandomVector() * entry_speed.Get() * vqs.S() + dt * velocity_global;
    par->acceleration = acceleration_global;
    par->life = 0.0f;

}	//End: ParticleEmitter::FrustumCull()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::UpdateParticle()
//--------------------------------------------------------------------------------
//		Updates a single particle in the pool
//--------------------------------------------------------------------------------
void ParticleEmitter::UpdateParticle(DgLinkedList<Particle>::iterator& it, float dt)
{
    //Find current life
    it->life += dt;

    if (it->life > it->life_total)
    {
        //Erase element
        particles.erase(it);
        --it; //Will be incremented to the next particle next loop
        return;
    }

    //How far along in life is it?
    float dl = it->life / it->life_total;

    //Adjust position and velocity
    it->velocity = (it->velocity + it->acceleration * dt) * DgPow(it->force, dt);
    it->position += (it->velocity * dt);

    //Adjust radius
    it->radius = it->radius_start + (it->d_radius * dl);
}



//--------------------------------------------------------------------------------
//	@	ParticleEmitter::GetNumberToEmit()
//--------------------------------------------------------------------------------
//		Loop through all pixels and update data
//--------------------------------------------------------------------------------
uint32 ParticleEmitter::Update(float dt)
{
	if (!IsActive())
		return 0;

	DgLinkedList<Particle>::iterator it = particles.begin();

    //Move particles along.
	for (it; it != particles.end(); ++it)
	{
        UpdateParticle(it, dt);
	}

    //Add any new particles
    uint32 toAdd = GetNumberToAdd(dt);
    
    if (toAdd < 1)
        return 1;

    //Stagger multiple particles
    float delta_t = dt / static_cast<float>(toAdd);
    float t = 0.0f;

    for (uint32 i = 0; i < toAdd; ++i)
    {
        //Open up an element in the front of the list
        if (!particles.push_front_blank())
            break;

        //Set new particle data
        Particle* par = &particles.front();
        SetNewParticle(par, t); 

        //Move the particle along
        it = particles.begin();
        UpdateParticle(it, t);
        t += delta_t;
    }

	return 1;
}	//End: ParticleEmitter::Update()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::GenerateParticles()
//--------------------------------------------------------------------------------
//		Generate all pixels. 
//--------------------------------------------------------------------------------
uint32 ParticleEmitter::Initiate()
{
	for (uint32 i = 0; i < particles.max_size(); ++i)
	{
		Particle temp;

		temp.radius_start = radius_start.Get() * vqs.S();
		temp.d_radius = radius_end.Get() * vqs.S() - temp.radius_start;

		temp.life = 0.0f;
		temp.life_total = lifeTime.Get();

		temp.alpha_start = alpha_start.Get();
		temp.d_alpha = alpha_end.Get() - temp.alpha_start;

		temp.red_start = red_start.Get();
		temp.d_red = red_end.Get() - temp.red_start;

		temp.green_start = green_start.Get();
		temp.d_green = green_end.Get() - temp.green_start;

		temp.blue_start = blue_start.Get();
		temp.d_blue = blue_end.Get() - temp.blue_start;

		temp.force = force.Get();

		particles.push_back_noresize(temp);

	}

	//Set particle list to size 0
	particles.clear();

	return 0;
}	//End: ParticleEmitter::GenerateParticles()


//--------------------------------------------------------------------------------
//	@	ParticleEmitter::Render()
//--------------------------------------------------------------------------------
//		Send particles to the viewport
//--------------------------------------------------------------------------------
void ParticleEmitter::Render(Viewport* view, VQS& T_CAM_WLD)
{
	if (!IsActive())
		return;

	DgLinkedList<Particle>::iterator it = particles.begin();

	for (it; it != particles.end(); ++it)
	{
		if (it->seen)
		{
			//Transform to camera
			it->screenPosition = it->position * T_CAM_WLD;
			it->screenSize = it->radius * T_CAM_WLD.S();

			view->AddParticle(*it, &alphaTemplate);
		}
	}
}	//End: ParticleEmitter::GenerateParticles()