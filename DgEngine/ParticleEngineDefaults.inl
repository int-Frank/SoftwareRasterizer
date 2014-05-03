//--------------------------------------------------------------------------------
//		Base class default parameters
//--------------------------------------------------------------------------------

//Flags
#define PE_DEFAULT_FLAG_ACTIVE				true

//Number
#define PE_DEFAULT_NUMBER					100

//Emissionrate
#define PE_DEFAULT_EMISSIONRATE				50.0f

//Radii
#define PE_DEFAULT_RADIUS_START_MEAN		0.2f
#define PE_DEFAULT_RADIUS_START_SD			0.02f
#define PE_DEFAULT_RADIUS_START_MIN			0.14f
#define PE_DEFAULT_RADIUS_START_MAX			0.26f
#define PE_DEFAULT_RADIUS_END_MEAN			0.1f
#define PE_DEFAULT_RADIUS_END_SD			0.01f
#define PE_DEFAULT_RADIUS_END_MIN			0.07f
#define PE_DEFAULT_RADIUS_END_MAX			0.13f

//Entry speed
#define PE_DEFAULT_ENTRYSPEED_MEAN			1.0f
#define PE_DEFAULT_ENTRYSPEED_SD			0.1f
#define PE_DEFAULT_ENTRYSPEED_MIN			0.7f
#define PE_DEFAULT_ENTRYSPEED_MAX			1.3f

//Entry acceleration
#define PE_DEFAULT_FORCE_MEAN				1.1f
#define PE_DEFAULT_FORCE_SD					0.05f
#define PE_DEFAULT_FORCE_MIN				1.0f
#define PE_DEFAULT_FORCE_MAX				1.3f

//Global acceleration
#define PE_DEFAULT_VELOCITY					Vector4(0.0f, 0.0f, 0.0f)
#define PE_DEFAULT_ACCELERATION				Vector4(0.0f, 0.0f, 0.0f)

//Life/dead times
#define PE_DEFAULT_ALIVE_MEAN				2.0f
#define PE_DEFAULT_ALIVE_SD					0.1f
#define PE_DEFAULT_ALIVE_MIN				1.7f
#define PE_DEFAULT_ALIVE_MAX				2.3f

//Color: RED values
#define PE_DEFAULT_RED_START_MEAN			240.0f
#define PE_DEFAULT_RED_START_SD				5.0f
#define PE_DEFAULT_RED_START_MIN			225.0f
#define PE_DEFAULT_RED_START_MAX			255.0f
#define PE_DEFAULT_RED_END_MEAN				240.0f
#define PE_DEFAULT_RED_END_SD				5.0f
#define PE_DEFAULT_RED_END_MIN				225.0f
#define PE_DEFAULT_RED_END_MAX				255.0f

//Color: GREEN values
#define PE_DEFAULT_GREEN_START_MEAN			240.0f
#define PE_DEFAULT_GREEN_START_SD			5.0f
#define PE_DEFAULT_GREEN_START_MIN			225.0f
#define PE_DEFAULT_GREEN_START_MAX			255.0f
#define PE_DEFAULT_GREEN_END_MEAN			15.0f
#define PE_DEFAULT_GREEN_END_SD				5.0f
#define PE_DEFAULT_GREEN_END_MIN			0.0f
#define PE_DEFAULT_GREEN_END_MAX			30.0f

//Color: BLUE values
#define PE_DEFAULT_BLUE_START_MEAN			10.0f
#define PE_DEFAULT_BLUE_START_SD			3.0f
#define PE_DEFAULT_BLUE_START_MIN			1.0f
#define PE_DEFAULT_BLUE_START_MAX			19.0f
#define PE_DEFAULT_BLUE_END_MEAN			10.0f
#define PE_DEFAULT_BLUE_END_SD				3.0f
#define PE_DEFAULT_BLUE_END_MIN				1.0f
#define PE_DEFAULT_BLUE_END_MAX				19.0f

//Color: ALPHA values
#define PE_DEFAULT_ALPHA_START_MEAN			245.0f
#define PE_DEFAULT_ALPHA_START_SD			5.0f
#define PE_DEFAULT_ALPHA_START_MIN			225.0f
#define PE_DEFAULT_ALPHA_START_MAX			255.0f
#define PE_DEFAULT_ALPHA_END_MEAN			100.0f
#define PE_DEFAULT_ALPHA_END_SD				20.0f
#define PE_DEFAULT_ALPHA_END_MIN			40.0f
#define PE_DEFAULT_ALPHA_END_MAX			160.0f


//--------------------------------------------------------------------------------
//		Box emitter class default parameters
//--------------------------------------------------------------------------------
#define BE_DEFAULT_ORIGIN					Point4(0.0f, 0.0f, 0.0f)

#define BE_DEFAULT_LENGTH_X0				10.0f
#define BE_DEFAULT_LENGTH_X1				10.0f
#define BE_DEFAULT_LENGTH_X2				10.0f

#define BE_DEFAULT_AXIS						Vector4::xAxis

#define BE_DEFAULT_THETA_MEAN				0.5f
#define BE_DEFAULT_THETA_SD					0.1f
#define BE_DEFAULT_THETA_MIN				0.2f
#define BE_DEFAULT_THETA_MAX				0.8f


//--------------------------------------------------------------------------------
//		Box emitter class default parameters
//--------------------------------------------------------------------------------

#define DE_DEFAULT_ORIGIN					Point4(0.0f, 0.0f, 0.0f)
#define DE_DEFAULT_AXIS						Vector4(1.0f, 0.0f, 0.0f)

#define DE_DEFAULT_RADIUS					0.5f
#define DE_DEFAULT_OUTERANGLE				0.5f