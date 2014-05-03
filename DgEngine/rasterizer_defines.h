//--------------------------------------------------------------------------------
//		Shifts for fixed point math
//--------------------------------------------------------------------------------

#define Z_SFT				1073741824		//z0_inv, z1_inv, z2_inv, dz, zs, ze, dzdy
#define DY_INV_SFT			1073741824		//dy_inv, 2^30
#define LOWER_30_BITS_IN_64	1073741823		//for & operator
#define YBUMP				1073741824		//Piviot for the y-bump value
#define cvrt_x				1048576.0f		//Linked with X_SFT
#define cvrt_uv				1073600000.0f	//converts float to fixed-point number
#define cvrt_z				1073741824.0f	//Conversion from float to fixed point constants
#define cvrt_dy				1073741824.0f
#define cvrt_ybump			1073741824.0f
#define cvrt_color			65536.0f		//Convert color values to 16:16 format

#define cvrt_alphatemp		65536.0f
#define alpha_SHFT			16

#define X_SFT				20				//[x0, x1, x2], xs, xe, dxdy, dx
#define COLOR_SHFT			16
#define magic_int			1024			//compensates for rounding issues (reduces 'tears' between polygons)

#define A_BITMASK			0xFF000000
#define R_BITMASK			0x00FF0000
#define G_BITMASK			0x0000FF00
#define B_BITMASK			0x000000FF