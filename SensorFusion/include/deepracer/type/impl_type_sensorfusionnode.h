///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                        
/// Copyright, 2021 PopcornSAR Co., Ltd. All rights reserved.                                              
/// This software is copyright protected and proprietary to PopcornSAR Co., Ltd.                           
/// PopcornSAR Co., Ltd. grants to you only those rights as set out in the license conditions.             
///                                                                                                        
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AUTOSAR VERSION                   : R20-11
/// GENERATED BY                      : PARA, ARA::COM Generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GENERATED FILE NAME               : impl_type_sensorfusionnode.h
/// IMPLEMENTATION DATA TYPE NAME     : SensorFusionNode
/// GENERATED DATE                    : 2024-11-13 13:00:52
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                        
/// CAUTION!! AUTOMATICALLY GENERATED FILE - DO NOT EDIT                                                   
///                                                                                                        
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PARA_ARA_COM_GEN_CPP_IMPL_TYPE_SENSORFUSIONNODE_H
#define PARA_ARA_COM_GEN_CPP_IMPL_TYPE_SENSORFUSIONNODE_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @uptrace{SWS_CM_00402}
#include <cstdint>
#include <ara/core/structure.h>
#include "deepracer/type/impl_type_cameradatavector.h"
#include "deepracer/type/impl_type_floatvector.h"
/// @uptrace{SWS_CM_10375}
namespace deepracer
{
namespace type
{
struct SensorFusionNode
{
    deepracer::type::CameraDataVector camera_data;
    deepracer::type::FloatVector lidar_data;
    std::int64_t timestamp;
    PARA_STRUCTURE(SensorFusionNode, camera_data, lidar_data, timestamp);
};
} /// namespace type
} /// namespace deepracer
#endif /// PARA_ARA_COM_GEN_CPP_IMPL_TYPE_SENSORFUSIONNODE_H