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
#ifndef PARA_AA_GEN_SOFTWARE_COMPONENT_RPORT_SENSORFUSION_AA_LIDARDATA_H
#define PARA_AA_GEN_SOFTWARE_COMPONENT_RPORT_SENSORFUSION_AA_LIDARDATA_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "deepracer/service/lidardata/svlidardata_proxy.h"
 
#include "ara/log/logger.h"
 
#include <mutex>
#include <thread>
 
namespace sensorfusion
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
    
    /// @brief Subscribe event, LEvent
    void SubscribeLEvent();
     
    /// @brief Stop event subscription, LEvent
    void StopSubscribeLEvent();
     
    /// @brief Event receive handler, LEvent
    void ReceiveEventLEventTriggered();
     
    /// @brief Event receive handler, LEvent
    void ReceiveEventLEventCyclic();
     
    /// @brief Read event data, LEvent
    void ReadDataLEvent(ara::com::SamplePtr<deepracer::service::lidardata::proxy::events::LEvent::SampleType const> samplePtr);
    
    void SetReceiveEventLEventHandler(
        std::function<void(const deepracer::service::lidardata::proxy::events::LEvent::SampleType&)> handler);
    
    
private:
    /// @brief Callback for find service
    void Find(ara::com::ServiceHandleContainer<deepracer::service::lidardata::proxy::SvLidarDataProxy::HandleType> handles,
              ara::com::FindServiceHandle findHandle);
    
    /// @brief Callback for event receiver, LEvent
    void RegistReceiverLEvent();
    
    
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
    std::shared_ptr<deepracer::service::lidardata::proxy::SvLidarDataProxy> m_interface;
    
    /// @brief Find service handle
    std::shared_ptr<ara::com::FindServiceHandle> m_findHandle;

    std::function<void(const deepracer::service::lidardata::proxy::events::LEvent::SampleType&)> m_receiveEventLEventHandler;

};
 
} /// namespace port
} /// namespace aa
} /// namespace sensorfusion
 
#endif /// PARA_AA_GEN_SOFTWARE_COMPONENT_RPORT_SENSORFUSION_AA_LIDARDATA_H