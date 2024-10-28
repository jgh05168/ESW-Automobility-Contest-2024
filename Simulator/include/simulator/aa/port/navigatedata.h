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
/// GENERATED FILE NAME               : navigatedata.h
/// SOFTWARE COMPONENT NAME           : NavigateData
/// GENERATED DATE                    : 2024-10-25 13:47:26
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PARA_AA_GEN_SOFTWARE_COMPONENT_RPORT_SIMULATOR_AA_NAVIGATEDATA_H
#define PARA_AA_GEN_SOFTWARE_COMPONENT_RPORT_SIMULATOR_AA_NAVIGATEDATA_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "deepracer/service/navigatedata/svnavigatedata_proxy.h"
 
#include "ara/log/logger.h"
 
#include <mutex>
#include <thread>
 
namespace simulator
{
namespace aa
{
namespace port
{
 
class NavigateData
{
public:
    /// @brief Constructor
    NavigateData();
    
    /// @brief Destructor
    ~NavigateData();
    
    /// @brief Start port
    void Start();
    
    /// @brief Terminate port
    void Terminate();
    
    /// @brief Subscribe event, NEvent
    void SubscribeNEvent();
     
    /// @brief Stop event subscription, NEvent
    void StopSubscribeNEvent();
     
    /// @brief Event receive handler, NEvent
    void ReceiveEventNEventTriggered();
     
    /// @brief Event receive handler, NEvent
    void ReceiveEventNEventCyclic();
     
    /// @brief Read event data, NEvent
    void ReadDataNEvent(ara::com::SamplePtr<deepracer::service::navigatedata::proxy::events::NEvent::SampleType const> samplePtr);
    
    
    
private:
    /// @brief Callback for find service
    void Find(ara::com::ServiceHandleContainer<deepracer::service::navigatedata::proxy::SvNavigateDataProxy::HandleType> handles,
              ara::com::FindServiceHandle findHandle);
    
    /// @brief Callback for event receiver, NEvent
    void RegistReceiverNEvent();
    
    
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
    std::shared_ptr<deepracer::service::navigatedata::proxy::SvNavigateDataProxy> m_interface;
    
    /// @brief Find service handle
    std::shared_ptr<ara::com::FindServiceHandle> m_findHandle;
};
 
} /// namespace port
} /// namespace aa
} /// namespace simulator
 
#endif /// PARA_AA_GEN_SOFTWARE_COMPONENT_RPORT_SIMULATOR_AA_NAVIGATEDATA_H