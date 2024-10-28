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
/// GENERATED FILE NAME               : fusiondata.h
/// SOFTWARE COMPONENT NAME           : FusionData
/// GENERATED DATE                    : 2024-10-25 13:47:26
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PARA_AA_GEN_SOFTWARE_COMPONENT_RPORT_INFERENCE_AA_FUSIONDATA_H
#define PARA_AA_GEN_SOFTWARE_COMPONENT_RPORT_INFERENCE_AA_FUSIONDATA_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "deepracer/service/fusiondata/svfusiondata_proxy.h"
 
#include "ara/log/logger.h"
 
#include <mutex>
#include <thread>
 
namespace inference
{
namespace aa
{
namespace port
{
 
class FusionData
{
public:
    /// @brief Constructor
    FusionData();
    
    /// @brief Destructor
    ~FusionData();
    
    /// @brief Start port
    void Start();
    
    /// @brief Terminate port
    void Terminate();
    
    
    
    /// @brief Request with Response method using by software component, FMethod
    void RequestFMethod();
    
private:
    /// @brief Callback for find service
    void Find(ara::com::ServiceHandleContainer<deepracer::service::fusiondata::proxy::SvFusionDataProxy::HandleType> handles,
              ara::com::FindServiceHandle findHandle);
    
    
    
private:
    /// @brief Logger for this port
    ara::log::Logger& m_logger;
    
    /// @brief Flag of port status
    bool m_running;
    
    /// @brief Flag of find service status
    bool m_found;
    
    /// @brief Mutex for this port
    std::mutex m_mutex; 
    
    /// @brief AUTOSAR Port Interface
    std::shared_ptr<deepracer::service::fusiondata::proxy::SvFusionDataProxy> m_interface;
    
    /// @brief Find service handle
    std::shared_ptr<ara::com::FindServiceHandle> m_findHandle;
};
 
} /// namespace port
} /// namespace aa
} /// namespace inference
 
#endif /// PARA_AA_GEN_SOFTWARE_COMPONENT_RPORT_INFERENCE_AA_FUSIONDATA_H