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
    dg_Fail,
    dg_Fatal
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
typedef unsigned __int32	entityID;
enum ENTITYID{
    ROOT            = 0x00000000,
    ERROR           = 0x06660000,
    E_BAD_ID        = 0x00010000,
    E_REC_CLASS     = 0x00020000,
    E_INS_FULL      = 0x00030000,
    E_INV_CLASS     = 0x00040000,
    CUTOFF          = 0x0FFF0000,
    CLASS_MASK      = 0xFFFF0000,
    INSTANCE_MASK   = 0x0000FFFF
};

//! @brief ID type for the viewport system.
typedef unsigned __int32	viewportID;
enum VIEWPORTID{
    NONE = 0x0000
};

#define NULL 0
/** @}*/

#endif