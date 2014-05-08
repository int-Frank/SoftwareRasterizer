/*!
* @file DgTypes.h
*
* @author Frank Hart
* @date 2/01/2014
*
* This file defines some common types.
*/

#ifndef DGTYPES_H
#define DGTYPES_H

/** \addtogroup utility_types
*  @{
*/

enum DgReturnCode
{
    dg_OK,
    dg_Fail
};

typedef unsigned __int8     uint8;
typedef __int8              int8;
typedef unsigned __int16    uint16;
typedef __int16             int16;
typedef unsigned __int32    uint32;
typedef __int32             int32;
typedef unsigned __int64	uint64;
typedef __int64             int64;
typedef float		        f32;
typedef double		        f64;

//! @brief Primary ID type for the entity system.
typedef unsigned int	entityID;

//! @brief Secondary ID type for the entity system.
typedef unsigned int	elementID;

//! @brief ID type for the viewport system.
typedef int				viewportID;

//! Particle emmitter ID
typedef unsigned int	emitterID;

#define NULL 0

#define MyDelete(ptr)		delete ptr; ptr = NULL;
#define MyDelete_array(ptr) delete[] ptr; ptr = NULL; 
/** @}*/

#endif