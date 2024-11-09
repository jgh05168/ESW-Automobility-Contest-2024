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
/// GENERATED FILE NAME               : lidardata.cpp
/// SOFTWARE COMPONENT NAME           : LidarData
/// GENERATED DATE                    : 2024-11-07 14:01:17
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "sl_lidar.h" 
#include "sl_lidar_driver.h"

#include "lidar/aa/port/lidardata.h"
 
using namespace sl;
 
namespace deepracer
{
namespace service
{
namespace lidardata
{
namespace skeleton
{
 
SvLidarDataSkeletonImpl::SvLidarDataSkeletonImpl(ara::core::InstanceSpecifier instanceSpec, ara::com::MethodCallProcessingMode mode)
    : SvLidarDataSkeleton(instanceSpec, mode)
    , m_logger(ara::log::CreateLogger("LID", "PORT", ara::log::LogLevel::kVerbose))
{
}
 
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
 
LidarData::LidarData()
    : m_logger(ara::log::CreateLogger("LID", "PORT", ara::log::LogLevel::kVerbose))
    , m_running{false}
    , m_LEventData{0LL, {0.0f, 0.0f, 0.0f}}
{
}

LidarData::~LidarData()
{
}
 
void LidarData::Start()
{
    m_logger.LogVerbose() << "LidarData::Start";
    
    // construct skeleton
    ara::core::InstanceSpecifier specifier{"Lidar/AA/LidarData"};
    m_interface = std::make_shared<deepracer::service::lidardata::skeleton::SvLidarDataSkeletonImpl>(specifier);
    
    // offer service
    auto offer = m_interface->OfferService();
    if (offer.HasValue())
    {
        m_running = true;
        m_logger.LogVerbose() << "LidarData::Start::OfferService";
    }
    else
    {
        m_running = false;
        m_logger.LogError() << "LidarData::Start::OfferService::" << offer.Error().Message();
    }
}

void LidarData::Terminate()
{
    m_logger.LogVerbose() << "LidarData::Terminate";
    
    // stop port
    m_running = false;
    
    // stop offer service
    m_interface->StopOfferService();
    m_logger.LogVerbose() << "LidarData::Terminate::StopOfferService";
}
 
void LidarData::produceScanning() {
    // 드라이버 인스턴스 생성
	ILidarDriver * drv = *createLidarDriver();
    // scan data 및 존별 최소 거리 초기화
    deepracer::service::lidardata::skeleton::events::LEevent::SampleType& scan_data;
    for (size_t i = 0; i < 8; i++) {
        scan_data[i].distance = 9999;
    }
    while (1) {
        sl_lidar_response_measurement_node_hq_t nodes[8192];
        size_t count = sizeof(nodes) / sizeof(sl_lidar_response_measurement_node_hq_t);

        // 데이터 캡처
        auto op_result = drv->grabScanDataHq(nodes, count);

        if (SL_IS_OK(op_result)) {
            drv->ascendScanData(nodes, count);

            for (size_t pos = 0; pos < (uint32_t)count; ++pos) {
                //앵글 센싱
                float theta = (nodes[pos].angle_z_q14 * 90.f) / 16384.f;
                // 각도에 따른 존 할당
                uint32_t zone;
                if (theta >= 40.0 && theta < 75.0) {
                    zone = 0; // 존1
                } else if (theta >= 75.0 && theta < 110.0) {
                    zone = 1; // 존2
                } else if (theta >= 110.0 && theta < 145.0) {
                    zone = 2; // 존3
                } else if (theta >= 145.0 && theta < 180.0) {
                    zone = 3; // 존4
                } else if (theta >= 180.0 && theta < 215.0) {
                    zone = 4; // 존5
                } else if (theta >= 215.0 && theta < 250.0) {
                    zone = 5; // 존6
                } else if (theta >= 250.0 && theta < 285.0) {
                    zone = 6; // 존7
                } else if (theta >= 285.0 && theta < 320.0) {
                    zone = 7; // 존8
                } else {
                    continue; // 범위 밖 데이터 무시
                }

                // 거리 센싱
                float dist = nodes[pos].dist_mm_q2 / 4.0f;
                if (dist == 0) continue; // 거리가 0인 데이터는 무시
                
                // 해당 존에 최소 거리 업데이트
                if (dist < scan_data[zone].distance) {
                    scan_data[zone].distance = dist;
                }
            } 
        }
        // m_LEeventData에 업데이트
        scan_data.timestamp = getCurrentTimestamp();
        WriteDataLEevent(scan_data);
        // SL_IS_OK가 아니라면 에러로그
        else {
            m_logger.LogError() << "Failed to grab scan data from Lidar.";
        }
    }
}

int64_t LidarData::getCurrentTimestamp() const {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void LidarData::WriteDataLEvent(const deepracer::service::lidardata::skeleton::events::LEvent::SampleType& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_LEventData = data;
}

void LidarData::SendEventLEventCyclic()
{
    while (m_running)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto send = m_interface->LEvent.Send(m_LEventData);
            if (send.HasValue())
            {
                m_logger.LogVerbose() << "LidarData::SendEventLEventCyclic::Send";
            }
            else
            {
                m_logger.LogError() << "LidarData::SendEventLEventCyclic::Send::" << send.Error().Message();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
 
void LidarData::SendEventLEventTriggered()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto send = m_interface->LEvent.Send(m_LEventData);
    if (send.HasValue())
    {
        m_logger.LogVerbose() << "LidarData::SendEventLEventTriggered::Send";
    }
    else
    {
        m_logger.LogError() << "LidarData::SendEventLEventTriggered::Send::" << send.Error().Message();
    }
}
 
void LidarData::SendEventLEventTriggered(const deepracer::service::lidardata::skeleton::events::LEvent::SampleType& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_LEventData = data;
    auto send = m_interface->LEvent.Send(m_LEventData);
    if (send.HasValue())
    {
        m_logger.LogVerbose() << "LidarData::SendEventLEventTriggered::Send";
    }
    else
    {
        m_logger.LogError() << "LidarData::SendEventLEventTriggered::Send::" << send.Error().Message();
    }
}

 
} /// namespace port
} /// namespace aa
} /// namespace lidar
 
/// EOF