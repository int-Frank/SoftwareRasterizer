

#include "Rasterizer.h"
#include "ParticleAlphaTemplate.h"
#include "Particle_RASTER.h"
#include "rasterizer_defines.h"

//--------------------------------------------------------------------------------
//	@	Rasterizer::Draw()
//--------------------------------------------------------------------------------
//		Entry point for drawing Particles. 
//--------------------------------------------------------------------------------
void Rasterizer::DrawParticle(const Particle_RASTER& input)
{
	//Extract bounds
	float x_min = input.position.X() - input.radius;
	float x_max = input.position.X() + input.radius;
	float y_min = input.position.Y() - input.radius;
	float y_max = input.position.Y() + input.radius;

	//Is the particle on the screen?
	if (x_min > float(output_W) || y_min > float(output_H) ||
		x_max < 0.0f || y_max < 0.0f)
		return;

	//Clip the particle to the screen
	if (x_min < 0.0)
	{
		xs = 0;
		us = int32(-x_min / (input.radius * 2.0f) *
			cvrt_alphatemp) * ParticleAlphaTemplate::SIZE;
	}
	else
	{
		xs = int32(x_min);
		us = 0;
	}

	if (x_max > output_W)
	{
		xe = output_W;
		ue = int32((1.0f - (x_max - float(output_W)) / (input.radius * 2.0f)) *
			cvrt_alphatemp) * ParticleAlphaTemplate::SIZE;
	}
	else
	{
		xe = int32(x_max);
		ue = (1 << alpha_SHFT) * ParticleAlphaTemplate::SIZE;
	}

	if (y_min < 0.0)
	{
		y_start = 0;
		vs = int32(-y_min / (input.radius * 2.0f) *
			cvrt_alphatemp) * ParticleAlphaTemplate::SIZE;
	}
	else
	{
		y_start = int32(y_min);
		vs = 0;
	}

	if (y_max > output_H)
	{
		y_end = output_H;
		ve = int32((1.0f - (y_max - float(output_H)) / (input.radius * 2.0f)) *
			cvrt_alphatemp) * ParticleAlphaTemplate::SIZE;
	}
	else
	{
		y_end = int32(y_max);
		ve = (1 << alpha_SHFT) * ParticleAlphaTemplate::SIZE;
	}

	//One final cull check
	if (xe == xs || y_start == y_end)
		return;

	//Find alpha template interpolants
	dudy_right = (ue - us) / (xe - xs);				//x-interpolant
	dvdy_right = (ve - vs) / (y_end - y_start);	//y-interpolant

	//Get z value
	zs = int32(cvrt_z / input.position.Z());

	//Extract data
	reds = (input.color >> 16) & 0xFF;
	greens = (input.color >> 8) & 0xFF;
	blues = input.color & 0xFF;
	alphaMaster_top = (input.color >> 24);

	DrawParticle(input.alphaTemplate->GetTemplate(), ParticleAlphaTemplate::SIZE);

}	//End: Rasterizer::Draw()


//--------------------------------------------------------------------------------
//	@	Rasterizer::DrawParticle()
//--------------------------------------------------------------------------------
//		Draw a particle, per pixel alpha, no master alpha. 
//--------------------------------------------------------------------------------
void Rasterizer::DrawParticle(const uint8* data, uint32 pitch)
{
	for (int y = y_start; y < y_end; ++y)
	{
		int32 ui = us;
		int32 v = vs >> alpha_SHFT;

		//Precalculate
		int32 SCREEN_W_by_y = output_W*y;

		for (int x = xs; x < xe; ++x)
		{
			//Calculate screen pixel reference
			int32 ref = SCREEN_W_by_y + x;

			//Find alpha pixel
			int32 u = ui >> alpha_SHFT;

			//Test if the pixel is alpha
			uint32 alpha_top = uint32(data[v*pitch + u]);

			if (zs > zBuffer[ref] && alpha_top != 0)
			{
				//Alpha in 24:8 format---     32:0                 32:0
				alpha_top *= alphaMaster_top;
				uint32 alpha_bottom = (0xFFFF * (0xFFFF - alpha_top)) >> 16;

				uint32 red1 =   (reds   * alpha_top);
				uint32 green1 = (greens * alpha_top);
				uint32 blue1 =  (blues  * alpha_top);

				uint32 pxl = output_pixels[ref];
				uint32 red2 =   ((pxl >> 16) & 0xFF) * alpha_bottom;
				uint32 green2 = ((pxl >> 8)  & 0xFF) * alpha_bottom;
				uint32 blue2 =  ((pxl)       & 0xFF) * alpha_bottom;

				output_pixels[ref] = (((
					A_BITMASK
					| ((red1 + red2) & R_BITMASK))
					| (((green1 + green2) >> 8) & G_BITMASK))
					| (((blue1 + blue2) >> 16) & B_BITMASK));
			}

			//Increment u
			ui += dudy_right;
		}

		//Increment v
		vs += dvdy_right;

	}
}