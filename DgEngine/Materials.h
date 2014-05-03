#ifndef MATERIALS_H
#define MATERIALS_H

#include "Dg_io.h"
#include "Tuple.h"
#include "DgTypes.h"

class Texture;
class DgImage;
class string;
struct Vertex;
struct Polygon;
class Mesh;
namespace pugi{class xml_node;}

//--------------------------------------------------------------------------------
//		This class provides information about how a texture should be displayed.
//--------------------------------------------------------------------------------
class Materials
{
public:

	//Constructor/Destructor
	Materials();
	Materials(pugi::xml_node&);
	~Materials();

	//Copy operations
	Materials(const Materials&);
	Materials& operator=(const Materials&);

	//Set functions
	void SetEmission(const Tuple<float>&);
	void SetReflection(const Tuple<float>&);
	void SetAlpha(double);
	
	//Return functions
	const Tuple<float>& GetEmission()		const {return emission;}
	const Tuple<float>& GetReflection()		const {return reflection;}
	uint32 GetAlpha()						const { return alphaMaster; }

	//Switch attributes on and off
	void SwitchEmission(bool b)			{flags.emission = b;}
	void SwitchReflection(bool b)		{ flags.reflection = b; }
	void SwitchAlphaPP(bool b)			{ flags.alpha_PP = b; }
	void SwitchAlphaM(bool b)			{ flags.alpha_M = b; }
	void SwitchDoubleSided(bool b)		{flags.doubleSided = b; }
	void SwitchMaster(bool b)			{flags.master = b;}

	//Modify a vertex
	void AdjustVertex (Vertex&) const;
	void AdjustPolygon (Polygon&) const;
	void AdjustMesh (Mesh&) const;

	//Flags
	inline bool IsEmissive()	const	{return flags.emission;}
	inline bool IsReflective()	const	{ return flags.reflection; }
	inline bool IsAlphaPP()		const	{ return flags.alpha_PP; }
	inline bool IsAlphaMaster()	const	{ return flags.alpha_M; }
	inline bool IsDoubleSided()	const	{return flags.doubleSided; }
	inline bool IsMasterOn()	const	{return flags.master;}

private:
	//Data members

	//Light and reflection
	Tuple<float> emission;		//RGB emission values, fixed point format
	Tuple<float> reflection;	//RGB reflection values, fixed point format

	//Master alpha value in fixed point 16:16 format
	uint32 alphaMaster;


	/*
		Example Flags combinations:
		e on, r on:   emissive and reflective
		e on, r off:  only emissive
		e off, r on:  reflective, will be black if not lit
		e off, r off: will only come out black
		master on:	  go off e and r values
		master off:   No lighting rasterization, texture pixels just copied from source.
	*/

	//Flags
	struct Flags
	{
		Flags(): emission(false), reflection(true), 
				 alpha_PP(false), alpha_M(false),
				 doubleSided(false), master(true) {}

		bool emission		: 1;
		bool reflection		: 1;
		bool alpha_PP	    : 1;	//per pixel alpha
		bool alpha_M        : 1;	//transparency applied to the object
		bool doubleSided	: 1;	//Are the polygons double sided?
		bool master			: 1;

	} flags;


	//--------------------------------------------------------------------------------
	//		Functions
	//--------------------------------------------------------------------------------
	void Saturate(Tuple<float>&) const;

};


//--------------------------------------------------------------------------------
//		Read from xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node&, Materials&);


#endif