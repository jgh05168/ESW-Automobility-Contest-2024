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
/// GENERATED FILE NAME               : inferencedata.cpp
/// SOFTWARE COMPONENT NAME           : InferenceData
/// GENERATED DATE                    : 2024-11-07 14:01:17
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string>
#include "navigate/aa/port/inferencedata.h"
 
#include "deepracer/service/inferencedata/svinferencedata_proxy.h"

namespace navigate
{
namespace aa
{
namespace port
{
 
InferenceData::InferenceData()
    : m_logger(ara::log::CreateLogger("NAV", "PORT", ara::log::LogLevel::kVerbose))
    , m_running{false}
    , m_found{false}
{
}
 
InferenceData::~InferenceData()
{
}
 
void InferenceData::Start()
{
    m_logger.LogVerbose() << "InferenceData::Start";
    
    // regist callback
    ara::core::InstanceSpecifier specifier{"Navigate/AA/InferenceData"};
    auto handler = [this](ara::com::ServiceHandleContainer<deepracer::service::inferencedata::proxy::SvInferenceDataProxy::HandleType> handles,
                          ara::com::FindServiceHandle findHandle) {
        this->Find(handles, findHandle);
    };
    
    // find service
    auto find = deepracer::service::inferencedata::proxy::SvInferenceDataProxy::StartFindService(handler, specifier);
    if (find.HasValue())
    {
        m_logger.LogVerbose() << "InferenceData::Start::StartFindService";
    }
    else
    {
        m_logger.LogError() << "InferenceData::Start::StartFindService::" << find.Error().Message();
    }
    
    // run port
    m_running = true;
}
 
void InferenceData::Terminate()
{
    m_logger.LogVerbose() << "InferenceData::Terminate";
    
    // stop port
    m_running = false;
    
    // clear service proxy
    if (m_interface)
    {
        // stop subscribe
        StopSubscribeIEvent();
        
        // stop find service
        m_interface->StopFindService(*m_findHandle);
        m_found = false;
        
        m_logger.LogVerbose() << "InferenceData::Terminate::StopFindService";
    }
}
 
void InferenceData::Find(ara::com::ServiceHandleContainer<deepracer::service::inferencedata::proxy::SvInferenceDataProxy::HandleType> handles, ara::com::FindServiceHandle findHandle)
{
    // check finding handles
    if (handles.empty())
    {
        m_logger.LogVerbose() << "InferenceData::Find::Service Instances not found";
        return;
    }
    else
    {
        for (auto& handle : handles)
        {
            m_logger.LogVerbose() << "InferenceData::Find::Searched Instance::ServiceId =" << 
                                     handle.GetServiceHandle().serviceId << 
                                     ", InstanceId =" << 
                                     handle.GetServiceHandle().instanceId;
        }
    }
    
    // create proxy
    if (m_interface)
    {
        m_logger.LogVerbose() << "InferenceData::Find::Proxy is already running";
    }
    else
    {
        m_logger.LogVerbose() << "InferenceData::Find::Using Instance::ServiceId =" << 
                                 handles[0].GetServiceHandle().serviceId << 
                                 ", InstanceId =" << 
                                 handles[0].GetServiceHandle().instanceId;
        m_interface = std::make_shared<deepracer::service::inferencedata::proxy::SvInferenceDataProxy>(handles[0]);
        m_findHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
        m_found = true;
        
        // subscribe events
        SubscribeIEvent();
    }
}
 
void InferenceData::SubscribeIEvent()
{
    if (m_found)
    {
        // regist receiver handler
        // if you want to enable it, please uncomment below code
        // 
        // RegistReceiverIEvent();
        
        // request subscribe
        auto subscribe = m_interface->IEvent.Subscribe(1);
        if (subscribe.HasValue())
        {
            m_logger.LogVerbose() << "InferenceData::SubscribeIEvent::Subscribed";
        }
        else
        {
            m_logger.LogError() << "InferenceData::SubscribeIEvent::" << subscribe.Error().Message();
        }
    }
}
 
void InferenceData::StopSubscribeIEvent()
{
    if (m_found)
    {
        // request stop subscribe
        m_interface->IEvent.Unsubscribe();
        m_logger.LogVerbose() << "InferenceData::StopSubscribeIEvent::Unsubscribed";
    }
}
 
void InferenceData::RegistReceiverIEvent()
{
    if (m_found)
    {
        // set callback
        auto receiver = [this]() -> void {
            return ReceiveEventIEventTriggered();
        };
        
        // regist callback
        auto callback = m_interface->IEvent.SetReceiveHandler(receiver);
        if (callback.HasValue())
        {
            m_logger.LogVerbose() << "InferenceData::RegistReceiverIEvent::SetReceiveHandler";
        }
        else
        {
            m_logger.LogError() << "InferenceData::RegistReceiverIEvent::SetReceiveHandler::" << callback.Error().Message();
        }
    }
}
 
void InferenceData::ReceiveEventIEventTriggered()
{
    if (m_found)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_interface->IEvent.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
        {
            auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->IEvent.GetNewSamples([&](auto samplePtr) {
                InferenceData::ReadDataIEvent(std::move(samplePtr));
            }));
            if (recv->HasValue())
            {
                m_logger.LogVerbose() << "InferenceData::ReceiveEventIEvent::GetNewSamples::" << recv->Value();
            }
            else
            {
                m_logger.LogError() << "InferenceData::ReceiveEventIEvent::GetNewSamples::" << recv->Error().Message();
            }
        }
    }
}
 
void InferenceData::ReceiveEventIEventCyclic()
{
    while (m_running)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_found)
        {
            if (m_interface->IEvent.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->IEvent.GetNewSamples([&](auto samplePtr) {
                    InferenceData::ReadDataIEvent(std::move(samplePtr));
                }));
                if (recv->HasValue())
                {
                    m_logger.LogVerbose() << "InferenceData::ReceiveEventIEvent::GetNewSamples::" << recv->Value();
                }
                else
                {
                    m_logger.LogError() << "InferenceData::ReceiveEventIEvent::GetNewSamples::" << recv->Error().Message();
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}
 
void InferenceData::ReadDataIEvent(ara::com::SamplePtr<deepracer::service::inferencedata::proxy::events::IEvent::SampleType const> samplePtr)
{
    auto data = *samplePtr.Get();
    // put your logic

    // REvent 핸들러가 등록되어 있을시 해당 핸들러는 값과 함께 호출한다.
    if (m_receiveEventIEventHandler != nullptr)
    {
        m_receiveEventIEventHandler(data);
    }
}

void InferenceData::SetReceiveEventIEventHandler(
    std::function<void(const deepracer::service::inferencedata::proxy::events::IEvent::SampleType&)> handler)
{
    m_receiveEventIEventHandler = handler;
}
 
} /// namespace port
} /// namespace aa
} /// namespace navigate
 
/// EOF