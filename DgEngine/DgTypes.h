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
typedef unsigned char       uint8;
typedef char                int8;
typedef unsigned short      uint16;
typedef short               int16;
typedef unsigned int        uint32;
typedef int                 int32;
typedef unsigned long long  uint64;
typedef long long           int64;
typedef float				f32;
typedef double				f64;

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