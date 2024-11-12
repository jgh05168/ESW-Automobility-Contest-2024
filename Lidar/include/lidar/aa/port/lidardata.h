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
/// GENERATED FILE NAME               : lidardata.h
/// SOFTWARE COMPONENT NAME           : LidarData
/// GENERATED DATE                    : 2024-11-07 14:01:17
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PARA_AA_GEN_SOFTWARE_COMPONENT_PPORT_LIDAR_AA_LIDARDATA_H
#define PARA_AA_GEN_SOFTWARE_COMPONENT_PPORT_LIDAR_AA_LIDARDATA_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "deepracer/service/lidardata/svlidardata_skeleton.h"
 
#include "ara/log/logger.h"
#include "sl_lidar.h" 
 
#include <mutex>
#include <thread>
 
namespace deepracer
{
namespace service
{
namespace lidardata
{
namespace skeleton
{
 
class SvLidarDataSkeletonImpl : public SvLidarDataSkeleton
{
public:
    /// @brief Constructor
    SvLidarDataSkeletonImpl(ara::core::InstanceSpecifier instanceSpec, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent);
    
private:
    /// @brief Logger for this port
    ara::log::Logger& m_logger;
    
    
};
 
} /// namespace skeleton
} /// namespace lidardata
} /// namespace service
} /// namespace deepracer

namespace lidar
{
namespace aa
{
namespace port
{
 
class LidarData
{
public:
    /// @brief Constructor
    LidarData();
    
    /// @brief Destructor
    ~LidarData();
    
    /// @brief Start port
    void Start();
    
    /// @brief Terminate port
    void Terminate();
    
    /// @brief Write event data to buffer, LEvent
    void WriteDataLEvent(const deepracer::service::lidardata::skeleton::events::LEvent::SampleType& data);
     
    /// @brief Send event cyclic from buffer data, LEvent
    void SendEventLEventCyclic();
     
    /// @brief Send event directly from buffer data, LEvent
    void SendEventLEventTriggered();
     
    /// @brief Send event directly with argument, LEvent
    void SendEventLEventTriggered(const deepracer::service::lidardata::skeleton::events::LEvent::SampleType& data);

    void produceScanning();

    int64_t getCurrentTimestamp() const;
     
private:
    /// @brief Logger for this port
    ara::log::Logger& m_logger;
    
    /// @brief Flag of port status
    bool m_running;
    
    /// @brief Mutex for this port
    std::mutex m_mutex;
    
    /// @brief AUTOSAR Port Interface
    std::shared_ptr<deepracer::service::lidardata::skeleton::SvLidarDataSkeletonImpl> m_interface;
    
    /// @brief Data for event, LEvent
    deepracer::service::lidardata::skeleton::events::LEvent::SampleType m_LEventData;
};
 
} /// namespace port
} /// namespace aa
} /// namespace lidar
 
#endif /// PARA_AA_GEN_SOFTWARE_COMPONENT_PPORT_LIDAR_AA_LIDARDATA_H