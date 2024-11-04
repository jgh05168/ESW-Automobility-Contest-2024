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
/// GENERATED FILE NAME               : navigatedata.cpp
/// SOFTWARE COMPONENT NAME           : NavigateData
/// GENERATED DATE                    : 2024-10-25 13:47:26
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "servo/aa/port/navigatedata.h"
 
namespace servo
{
namespace aa
{
namespace port
{
 
NavigateData::NavigateData()
    : m_logger(ara::log::CreateLogger("SRV", "PORT", ara::log::LogLevel::kVerbose))
    , m_running{false}
    , m_found{false}
{
}
 
NavigateData::~NavigateData()
{
}
 
void NavigateData::Start()
{
    m_logger.LogVerbose() << "NavigateData::Start";
    
    // regist callback
    ara::core::InstanceSpecifier specifier{"Servo/AA/NavigateData"};
    auto handler = [this](ara::com::ServiceHandleContainer<deepracer::service::navigatedata::proxy::SvNavigateDataProxy::HandleType> handles,
                          ara::com::FindServiceHandle findHandle) {
        this->Find(handles, findHandle);
    };
    
    // find service
    auto find = deepracer::service::navigatedata::proxy::SvNavigateDataProxy::StartFindService(handler, specifier);
    if (find.HasValue())
    {
        m_logger.LogVerbose() << "NavigateData::Start::StartFindService";
    }
    else
    {
        m_logger.LogError() << "NavigateData::Start::StartFindService::" << find.Error().Message();
    }
    
    // run port
    m_running = true;
}
 
void NavigateData::Terminate()
{
    m_logger.LogVerbose() << "NavigateData::Terminate";
    
    // stop port
    m_running = false;
    
    // clear service proxy
    if (m_interface)
    {
        // stop subscribe
        StopSubscribeNEvent();
        
        // stop find service
        m_interface->StopFindService(*m_findHandle);
        m_found = false;
        
        m_logger.LogVerbose() << "NavigateData::Terminate::StopFindService";
    }
}
 
void NavigateData::Find(ara::com::ServiceHandleContainer<deepracer::service::navigatedata::proxy::SvNavigateDataProxy::HandleType> handles, ara::com::FindServiceHandle findHandle)
{
    // check finding handles
    if (handles.empty())
    {
        m_logger.LogVerbose() << "NavigateData::Find::Service Instances not found";
        return;
    }
    else
    {
        for (auto& handle : handles)
        {
            m_logger.LogVerbose() << "NavigateData::Find::Searched Instance::ServiceId =" << 
                                     handle.GetServiceHandle().serviceId << 
                                     ", InstanceId =" << 
                                     handle.GetServiceHandle().instanceId;
        }
    }
    
    // create proxy
    if (m_interface)
    {
        m_logger.LogVerbose() << "NavigateData::Find::Proxy is already running";
    }
    else
    {
        m_logger.LogVerbose() << "NavigateData::Find::Using Instance::ServiceId =" << 
                                 handles[0].GetServiceHandle().serviceId << 
                                 ", InstanceId =" << 
                                 handles[0].GetServiceHandle().instanceId;
        m_interface = std::make_shared<deepracer::service::navigatedata::proxy::SvNavigateDataProxy>(handles[0]);
        m_findHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
        m_found = true;
        
        // subscribe events
        SubscribeNEvent();
    }
}
 
void NavigateData::SubscribeNEvent()
{
    if (m_found)
    {
        // regist receiver handler
        // if you want to enable it, please uncomment below code
        // 
        // RegistReceiverNEvent();
        
        // request subscribe
        auto subscribe = m_interface->NEvent.Subscribe(1);
        if (subscribe.HasValue())
        {
            m_logger.LogVerbose() << "NavigateData::SubscribeNEvent::Subscribed";
        }
        else
        {
            m_logger.LogError() << "NavigateData::SubscribeNEvent::" << subscribe.Error().Message();
        }
    }
}
 
void NavigateData::StopSubscribeNEvent()
{
    if (m_found)
    {
        // request stop subscribe
        m_interface->NEvent.Unsubscribe();
        m_logger.LogVerbose() << "NavigateData::StopSubscribeNEvent::Unsubscribed";
    }
}
 
void NavigateData::RegistReceiverNEvent()
{
    if (m_found)
    {
        // set callback
        auto receiver = [this]() -> void {
            return ReceiveEventNEventTriggered();
        };
        
        // regist callback
        auto callback = m_interface->NEvent.SetReceiveHandler(receiver);
        if (callback.HasValue())
        {
            m_logger.LogVerbose() << "NavigateData::RegistReceiverNEvent::SetReceiveHandler";
        }
        else
        {
            m_logger.LogError() << "NavigateData::RegistReceiverNEvent::SetReceiveHandler::" << callback.Error().Message();
        }
    }
}
 
void NavigateData::ReceiveEventNEventTriggered()
{
    if (m_found)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_interface->NEvent.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
        {
            auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->NEvent.GetNewSamples([&](auto samplePtr) {
                NavigateData::ReadDataNEvent(std::move(samplePtr));
            }));
            if (recv->HasValue())
            {
                m_logger.LogVerbose() << "NavigateData::ReceiveEventNEvent::GetNewSamples::" << recv->Value();
            }
            else
            {
                m_logger.LogError() << "NavigateData::ReceiveEventNEvent::GetNewSamples::" << recv->Error().Message();
            }
        }
    }
}
 
void NavigateData::ReceiveEventNEventCyclic()
{
    while (m_running)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_found)
        {
            if (m_interface->NEvent.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->NEvent.GetNewSamples([&](auto samplePtr) {
                    NavigateData::ReadDataNEvent(std::move(samplePtr));
                }));
                if (recv->HasValue())
                {
                    m_logger.LogVerbose() << "NavigateData::ReceiveEventNEvent::GetNewSamples::" << recv->Value();
                }
                else
                {
                    m_logger.LogError() << "NavigateData::ReceiveEventNEvent::GetNewSamples::" << recv->Error().Message();
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
 
void NavigateData::ReadDataNEvent(ara::com::SamplePtr<deepracer::service::navigatedata::proxy::events::NEvent::SampleType const> samplePtr)
{
    auto data = *samplePtr.Get();
    // put your logic
}

void NavigateData::SetReceiveEventNEventHandler(
    std::function<void(const deepracer::service::navigatedata::proxy::events::NEvent::SampleType&)> handler)
{
    m_receiveEventNEventHandler = handler;
}
 
} /// namespace port
} /// namespace aa
} /// namespace servo
 
/// EOF