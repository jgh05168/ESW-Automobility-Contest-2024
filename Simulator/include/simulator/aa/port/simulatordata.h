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
/// GENERATED FILE NAME               : simulatordata.h
/// SOFTWARE COMPONENT NAME           : SimulatorData
/// GENERATED DATE                    : 2024-10-25 13:47:26
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PARA_AA_GEN_SOFTWARE_COMPONENT_PPORT_SIMULATOR_AA_SIMULATORDATA_H
#define PARA_AA_GEN_SOFTWARE_COMPONENT_PPORT_SIMULATOR_AA_SIMULATORDATA_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "deepracer/service/fusiondata/svfusiondata_skeleton.h"
 
#include "ara/log/logger.h"
 
#include <mutex>
#include <thread>
 
namespace deepracer
{
namespace service
{
namespace fusiondata
{
namespace skeleton
{
 
class SvFusionDataSkeletonImpl : public SvFusionDataSkeleton
{
public:
    /// @brief Constructor
    SvFusionDataSkeletonImpl(ara::core::InstanceSpecifier instanceSpec, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent);
    
    /// @brief Request with Response method, FMethod
    ara::core::Future<SvFusionDataSkeleton::FMethodOutput> FMethod() override;
    
    
private:
    /// @brief Logger for this port
    ara::log::Logger& m_logger;
    
    
};
 
} /// namespace skeleton
} /// namespace fusiondata
} /// namespace service
} /// namespace deepracer

namespace simulator
{
namespace aa
{
namespace port
{
 
class SimulatorData
{
public:
    /// @brief Constructor
    SimulatorData();
    
    /// @brief Destructor
    ~SimulatorData();
    
    /// @brief Start port
    void Start();
    
    /// @brief Terminate port
    void Terminate();
    
private:
    /// @brief Logger for this port
    ara::log::Logger& m_logger;
    
    /// @brief Flag of port status
    bool m_running;
    
    /// @brief Mutex for this port
    std::mutex m_mutex;
    
    /// @brief AUTOSAR Port Interface
    std::shared_ptr<deepracer::service::fusiondata::skeleton::SvFusionDataSkeletonImpl> m_interface;
    
};
 
} /// namespace port
} /// namespace aa
} /// namespace simulator
 
#endif /// PARA_AA_GEN_SOFTWARE_COMPONENT_PPORT_SIMULATOR_AA_SIMULATORDATA_H