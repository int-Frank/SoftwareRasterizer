#include "Materials.h"
#include "CommonMath.h"
#include "Vertex.h"
#include "Polygon.h"
#include "Mesh.h"
#include "pugixml.hpp"
#include <string>


//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Materials::Materials(): alphaMaster(0xFFFF)
{
	//Set defaults
	emission.Set(0.0f);
	reflection.Set(1.0f);

}	//End: Materials::Materials()


//--------------------------------------------------------------------------------
//		Read from pugi xml_node
//--------------------------------------------------------------------------------
pugi::xml_node& operator>>(pugi::xml_node& node, Materials& dest)
{
	//iterate through all attributes
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
		//Get the name of the attribute
		std::string tag = it->name();

		if (tag == "emission")
		{
			Tuple<float> t;
			DgString(it->child_value()) >> t;
			dest.SetEmission(t);
		}
		else if (tag == "reflection")
		{
			Tuple<float> t;
			DgString(it->child_value()) >> t;
			dest.SetReflection(t);
		}
		else if (tag == "alphaMaster")
		{
			double t;
			DgString(it->child_value()) >> t;
			dest.SetAlpha(t);
		}
		else if (tag == "isemissive")
		{
			bool b = ToBool(it->child_value());
			dest.SwitchEmission(b);
		}
		else if (tag == "isreflective")
		{
			bool b = ToBool(it->child_value());
			dest.SwitchReflection(b);
		}
		else if (tag == "isdoublesided")
		{
			bool b = ToBool(it->child_value());
			dest.SwitchDoubleSided(b);
		}
		else if (tag == "isalphaPP")
		{
			bool b = ToBool(it->child_value());
			dest.SwitchAlphaPP(b);
		}
		else if (tag == "isalphaMaster")
		{
			bool b = ToBool(it->child_value());
			dest.SwitchAlphaM(b);
		}
		else if (tag == "ismaster")
		{
			bool b = ToBool(it->child_value());
			dest.SwitchMaster(b);
		}

    }

	//Return node
	return node;
}	//End: Materials::Materials()


//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
Materials::~Materials()
{
}	//End: Materials::~Materials()


//--------------------------------------------------------------------------------
//		Copy constructor
//--------------------------------------------------------------------------------
Materials::Materials(const Materials& m): emission(m.emission),
	reflection(m.reflection), alphaMaster(m.alphaMaster), flags(m.flags)
{
}	//End: Materials::Materials()


//--------------------------------------------------------------------------------
//		Assignment operator
//--------------------------------------------------------------------------------
Materials& Materials::operator=(const Materials& m)
{
	if (this == &m)
		return *this;

	//Copy data
	emission = m.emission;
	reflection = m.reflection;
	alphaMaster = m.alphaMaster;
	flags = m.flags;
	
	return *this;
}	//End: Materials::Materials()


//--------------------------------------------------------------------------------
//		Set Emission values
//--------------------------------------------------------------------------------
void Materials::SetReflection(const Tuple<float>& refl)
{
	//Set data
	reflection[0] = DgMax(refl[0], 0);
	reflection[1] = DgMax(refl[1], 0);
	reflection[2] = DgMax(refl[2], 0);

}	//End: Materials::SetEmission()


//--------------------------------------------------------------------------------
//		Set Emission values
//--------------------------------------------------------------------------------
void Materials::SetEmission(const Tuple<float>& emis)
{
	//Set data
	emission[0] = DgMax(emis[0], 0);
	emission[1] = DgMax(emis[1], 0);
	emission[2] = DgMax(emis[2], 0);

}	//End: Materials::SetEmission()


//--------------------------------------------------------------------------------
//		Set alpha value
//--------------------------------------------------------------------------------
void Materials::SetAlpha(double a)
{
	//Set data
	alphaMaster = uint32(a * 65536.0);

	//Check range
	if (alphaMaster > 0xFFFF)
		alphaMaster = 0xFFFF;

}	//End: Materials::SetEmission()


//--------------------------------------------------------------------------------
//		Adjust vertex colors
//--------------------------------------------------------------------------------
void Materials::AdjustVertex(Vertex& vertex) const
{
	if (!flags.master)
		return;

	//Adjust for reflection
	if (IsReflective())
		vertex.clr *= reflection;

	//Adjust for emission
	if (IsEmissive())
		vertex.clr += emission;

	Saturate(vertex.clr);

}	//End: Materials::AdjustVertex()


//--------------------------------------------------------------------------------
//	@	Materials::Saturate()
//--------------------------------------------------------------------------------
//		Scales back a color tuple so that no element is above 1.
//--------------------------------------------------------------------------------
void Materials::Saturate(Tuple<float>& tpl) const
{
	//Find the max of the tuple
	float mx = tpl.Max();

	//Ensure all colors are below the max
	if (mx > 1.0f)
	{
		tpl /= mx;
	}

}	//End: Materials::Saturate()


//--------------------------------------------------------------------------------
//		Adjust Mesh colors
//--------------------------------------------------------------------------------
void Materials::AdjustMesh(Mesh& mesh) const
{
	if (!flags.master)
		return;

	if (IsEmissive() && IsReflective())
	{
		for (uint32 i = 0; i < mesh.VList.size(); ++i)
		{
			if (mesh.VList[i].state == 'x')
				continue;
			
			mesh.VList[i].clr *= reflection;
			mesh.VList[i].clr += emission;
			Saturate(mesh.VList[i].clr);
		}
	}
	else if (IsEmissive())
	{
		for (uint32 i = 0; i < mesh.VList.size(); ++i)
		{
			if (mesh.VList[i].state == 'x')
				continue;
			
			mesh.VList[i].clr += emission;
			Saturate(mesh.VList[i].clr);
		}
	}
	else if (IsReflective())
	{
		for (uint32 i = 0; i < mesh.VList.size(); ++i)
		{
			if (mesh.VList[i].state == 'x')
				continue;
			
			mesh.VList[i].clr *= reflection;
			Saturate(mesh.VList[i].clr);
		}
	}
	

}	//End: Materials::AdjustVertex()