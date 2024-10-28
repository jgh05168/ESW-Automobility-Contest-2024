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
/// GENERATED FILE NAME               : cameradata.cpp
/// SOFTWARE COMPONENT NAME           : CameraData
/// GENERATED DATE                    : 2024-10-25 13:47:26
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "sensorfusion/aa/port/cameradata.h"
 
namespace sensorfusion
{
namespace aa
{
namespace port
{
 
CameraData::CameraData()
    : m_logger(ara::log::CreateLogger("SSFU", "PORT", ara::log::LogLevel::kVerbose))
    , m_running{false}
    , m_found{false}
{
}
 
CameraData::~CameraData()
{
}
 
void CameraData::Start()
{
    m_logger.LogVerbose() << "CameraData::Start";
    
    // regist callback
    ara::core::InstanceSpecifier specifier{"SensorFusion/AA/CameraData"};
    auto handler = [this](ara::com::ServiceHandleContainer<deepracer::service::cameradata::proxy::SvCameraDataProxy::HandleType> handles,
                          ara::com::FindServiceHandle findHandle) {
        this->Find(handles, findHandle);
    };
    
    // find service
    auto find = deepracer::service::cameradata::proxy::SvCameraDataProxy::StartFindService(handler, specifier);
    if (find.HasValue())
    {
        m_logger.LogVerbose() << "CameraData::Start::StartFindService";
    }
    else
    {
        m_logger.LogError() << "CameraData::Start::StartFindService::" << find.Error().Message();
    }
    
    // run port
    m_running = true;
}
 
void CameraData::Terminate()
{
    m_logger.LogVerbose() << "CameraData::Terminate";
    
    // stop port
    m_running = false;
    
    // clear service proxy
    if (m_interface)
    {
        // stop subscribe
        StopSubscribeCEvent();
        
        // stop find service
        m_interface->StopFindService(*m_findHandle);
        m_found = false;
        
        m_logger.LogVerbose() << "CameraData::Terminate::StopFindService";
    }
}
 
void CameraData::Find(ara::com::ServiceHandleContainer<deepracer::service::cameradata::proxy::SvCameraDataProxy::HandleType> handles, ara::com::FindServiceHandle findHandle)
{
    // check finding handles
    if (handles.empty())
    {
        m_logger.LogVerbose() << "CameraData::Find::Service Instances not found";
        return;
    }
    else
    {
        for (auto& handle : handles)
        {
            m_logger.LogVerbose() << "CameraData::Find::Searched Instance::ServiceId =" << 
                                     handle.GetServiceHandle().serviceId << 
                                     ", InstanceId =" << 
                                     handle.GetServiceHandle().instanceId;
        }
    }
    
    // create proxy
    if (m_interface)
    {
        m_logger.LogVerbose() << "CameraData::Find::Proxy is already running";
    }
    else
    {
        m_logger.LogVerbose() << "CameraData::Find::Using Instance::ServiceId =" << 
                                 handles[0].GetServiceHandle().serviceId << 
                                 ", InstanceId =" << 
                                 handles[0].GetServiceHandle().instanceId;
        m_interface = std::make_shared<deepracer::service::cameradata::proxy::SvCameraDataProxy>(handles[0]);
        m_findHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
        m_found = true;
        
        // subscribe events
        SubscribeCEvent();
    }
}
 
void CameraData::SubscribeCEvent()
{
    if (m_found)
    {
        // regist receiver handler
        // if you want to enable it, please uncomment below code
        // 
        // RegistReceiverCEvent();
        
        // request subscribe
        auto subscribe = m_interface->CEvent.Subscribe(1);
        if (subscribe.HasValue())
        {
            m_logger.LogVerbose() << "CameraData::SubscribeCEvent::Subscribed";
        }
        else
        {
            m_logger.LogError() << "CameraData::SubscribeCEvent::" << subscribe.Error().Message();
        }
    }
}
 
void CameraData::StopSubscribeCEvent()
{
    if (m_found)
    {
        // request stop subscribe
        m_interface->CEvent.Unsubscribe();
        m_logger.LogVerbose() << "CameraData::StopSubscribeCEvent::Unsubscribed";
    }
}
 
void CameraData::RegistReceiverCEvent()
{
    if (m_found)
    {
        // set callback
        auto receiver = [this]() -> void {
            return ReceiveEventCEventTriggered();
        };
        
        // regist callback
        auto callback = m_interface->CEvent.SetReceiveHandler(receiver);
        if (callback.HasValue())
        {
            m_logger.LogVerbose() << "CameraData::RegistReceiverCEvent::SetReceiveHandler";
        }
        else
        {
            m_logger.LogError() << "CameraData::RegistReceiverCEvent::SetReceiveHandler::" << callback.Error().Message();
        }
    }
}
 
void CameraData::ReceiveEventCEventTriggered()
{
    if (m_found)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_interface->CEvent.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
        {
            auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->CEvent.GetNewSamples([&](auto samplePtr) {
                CameraData::ReadDataCEvent(std::move(samplePtr));
            }));
            if (recv->HasValue())
            {
                m_logger.LogVerbose() << "CameraData::ReceiveEventCEvent::GetNewSamples::" << recv->Value();
            }
            else
            {
                m_logger.LogError() << "CameraData::ReceiveEventCEvent::GetNewSamples::" << recv->Error().Message();
            }
        }
    }
}
 
void CameraData::ReceiveEventCEventCyclic()
{
    while (m_running)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_found)
        {
            if (m_interface->CEvent.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->CEvent.GetNewSamples([&](auto samplePtr) {
                    CameraData::ReadDataCEvent(std::move(samplePtr));
                }));
                if (recv->HasValue())
                {
                    m_logger.LogVerbose() << "CameraData::ReceiveEventCEvent::GetNewSamples::" << recv->Value();
                }
                else
                {
                    m_logger.LogError() << "CameraData::ReceiveEventCEvent::GetNewSamples::" << recv->Error().Message();
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
 
void CameraData::ReadDataCEvent(ara::com::SamplePtr<deepracer::service::cameradata::proxy::events::CEvent::SampleType const> samplePtr)
{
    auto data = *samplePtr.Get();
    // put your logic
}
 
} /// namespace port
} /// namespace aa
} /// namespace sensorfusion
 
/// EOF