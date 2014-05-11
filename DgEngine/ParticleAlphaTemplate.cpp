#include "ParticleAlphaTemplate.h"
#include "CommonMath.h"
#include <math.h>


//--------------------------------------------------------------------------------
//	@	ParticleAlphaTemplate::Set_LINEAR()
//--------------------------------------------------------------------------------
//		Linear alpha fall off from 255(center) to 0(edges)
//--------------------------------------------------------------------------------
void ParticleAlphaTemplate::Set(double exp)
{
	//Find center
	int32 center = SIZE / 2 - 1;
	double side = double(center);

	//Range check input
	if (exp < 0.0)
		exp = 1.0;

	//iterate through all pixels
	for (int32 i = 0; i <= center; ++i)
	{
		for (int32 j = 0; j <= i; ++j)
		{
			//Get distance to center
			int32 dx = center - i;
			int32 dy = center - j;

			double dist = DgSqrt(double(dx*dx + dy*dy));

			//The alpha value
			uint8 alpha(0);

			//If pixel is outside the disk
			if (dist <= side)
			{
				double val = pow((dist / side), exp);
				alpha = uint8(255.0f - 255.0f * val);
			}

			//Set values
			data[i*SIZE + j] = alpha;
			data[j*SIZE + i] = alpha;

			data[i*SIZE + (SIZE - j - 1)] = alpha;
			data[j*SIZE + (SIZE - i - 1)] = alpha;

			data[(SIZE - i - 1)*SIZE + j] = alpha;
			data[(SIZE - j - 1)*SIZE + i] = alpha;

			data[(SIZE - i - 1)*SIZE + (SIZE - j - 1)] = alpha;
			data[(SIZE - j - 1)*SIZE + (SIZE - i - 1)] = alpha;

		}
	}
}	//End: ParticleAlphaTemplate::Set_LINEAR()