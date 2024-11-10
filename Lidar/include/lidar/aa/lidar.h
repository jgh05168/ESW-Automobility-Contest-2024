///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                        
/// Copyright, 2021 PopcornSAR Co., Ltd. All rights reserved.                                              
/// This software is copyright protected and proprietary to PopcornSAR Co., Ltd.                           
/// PopcornSAR Co., Ltd. grants to you only those rights as set out in the license conditions.             
///                                                                                                        
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AUTOSAR VERSION                   : R20-11
/// GENERATED BY                      : PARA, Adaptive Application Generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GENERATED FILE NAME               : lidar.h
/// SOFTWARE COMPONENT NAME           : Lidar
/// GENERATED DATE                    : 2024-11-07 14:01:17
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PARA_AA_GEN_SOFTWARE_COMPONENT_LIDAR_AA_H
#define PARA_AA_GEN_SOFTWARE_COMPONENT_LIDAR_AA_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "lidar/aa/port/lidardata.h"
#include "sl_lidar.h"
 
#include "para/swc/port_pool.h"
 
namespace lidar
{
namespace aa
{
 
class Lidar
{
public:
    /// @brief Constructor
    Lidar();
    
    /// @brief Destructor
    ~Lidar();
    
    /// @brief Initialize software component
    bool Initialize();
    
    /// @brief Start software component
    void Start();
    
    /// @brief Terminate software component
    void Terminate();

    void TaskGenerateLEventValue();
 
private:
    /// @brief Run software component
    void Run();
 
private:
    /// @brief Pool of port
    ::para::swc::PortPool m_workers;
    
    /// @brief Logger for software component
    ara::log::Logger& m_logger;
    
    /// @brief Instance of Port {Lidar.LidarData}
    std::shared_ptr<lidar::aa::port::LidarData> m_LidarData;

    std::shared_ptr<sl_lidar::ILidarDriver> drv;

    bool m_running;
};
 
} /// namespace aa
} /// namespace lidar
 
#endif /// PARA_AA_GEN_SOFTWARE_COMPONENT_LIDAR_AA_H