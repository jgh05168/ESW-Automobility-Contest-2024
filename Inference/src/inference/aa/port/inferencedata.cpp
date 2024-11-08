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
#include "inference/aa/port/inferencedata.h"
 
namespace deepracer
{
namespace service
{
namespace inferencedata
{
namespace skeleton
{
 
SvInferenceDataSkeletonImpl::SvInferenceDataSkeletonImpl(ara::core::InstanceSpecifier instanceSpec, ara::com::MethodCallProcessingMode mode)
    : SvInferenceDataSkeleton(instanceSpec, mode)
    , m_logger(ara::log::CreateLogger("INFR", "PORT", ara::log::LogLevel::kVerbose))
{
}
 
} /// namespace skeleton
} /// namespace inferencedata
} /// namespace service
} /// namespace deepracer

namespace inference
{
namespace aa
{
namespace port
{
 
InferenceData::InferenceData()
    : m_logger(ara::log::CreateLogger("INFR", "PORT", ara::log::LogLevel::kVerbose))
    , m_running{false}
    , m_IEventData{0LL, {{0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, {0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, {0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}}
{
}
 
InferenceData::~InferenceData()
{
}
 
void InferenceData::Start()
{
    m_logger.LogVerbose() << "InferenceData::Start";
    
    // construct skeleton
    ara::core::InstanceSpecifier specifier{"Inference/AA/InferenceData"};
    m_interface = std::make_shared<deepracer::service::inferencedata::skeleton::SvInferenceDataSkeletonImpl>(specifier);
    
    // offer service
    auto offer = m_interface->OfferService();
    if (offer.HasValue())
    {
        m_running = true;
        m_logger.LogVerbose() << "InferenceData::Start::OfferService";
    }
    else
    {
        m_running = false;
        m_logger.LogError() << "InferenceData::Start::OfferService::" << offer.Error().Message();
    }
}
 
void InferenceData::Terminate()
{
    m_logger.LogVerbose() << "InferenceData::Terminate";
    
    // stop port
    m_running = false;
    
    // stop offer service
    m_interface->StopOfferService();
    m_logger.LogVerbose() << "InferenceData::Terminate::StopOfferService";
}
 
void InferenceData::WriteDataIEvent(const deepracer::service::inferencedata::skeleton::events::IEvent::SampleType& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_IEventData = data;
}
 
void InferenceData::SendEventIEventCyclic()
{
    while (m_running)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto send = m_interface->IEvent.Send(m_IEventData);
            if (send.HasValue())
            {
                m_logger.LogVerbose() << "InferenceData::SendEventIEventCyclic::Send";
            }
            else
            {
                m_logger.LogError() << "InferenceData::SendEventIEventCyclic::Send::" << send.Error().Message();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
 
void InferenceData::SendEventIEventTriggered()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto send = m_interface->IEvent.Send(m_IEventData);
    if (send.HasValue())
    {
        m_logger.LogVerbose() << "InferenceData::SendEventIEventTriggered::Send";
    }
    else
    {
        m_logger.LogError() << "InferenceData::SendEventIEventTriggered::Send::" << send.Error().Message();
    }
}
 
void InferenceData::SendEventIEventTriggered(const deepracer::service::inferencedata::skeleton::events::IEvent::SampleType& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_IEventData = data;
    auto send = m_interface->IEvent.Send(m_IEventData);
    if (send.HasValue())
    {
        m_logger.LogVerbose() << "InferenceData::SendEventIEventTriggered::Send";
    }
    else
    {
        m_logger.LogError() << "InferenceData::SendEventIEventTriggered::Send::" << send.Error().Message();
    }
}
 
} /// namespace port
} /// namespace aa
} /// namespace inference
 
/// EOF