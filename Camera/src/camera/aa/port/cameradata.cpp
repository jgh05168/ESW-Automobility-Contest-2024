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
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <stdint.h>

#include "camera/aa/port/cameradata.h"
 
namespace deepracer
{
namespace service
{
namespace cameradata
{
namespace skeleton
{
 
SvCameraDataSkeletonImpl::SvCameraDataSkeletonImpl(ara::core::InstanceSpecifier instanceSpec, ara::com::MethodCallProcessingMode mode)
    : SvCameraDataSkeleton(instanceSpec, mode)
    , m_logger(ara::log::CreateLogger("CAM", "PORT", ara::log::LogLevel::kVerbose))
{
}
 
} /// namespace skeleton
} /// namespace cameradata
} /// namespace service
} /// namespace deepracer

namespace camera
{
namespace aa
{
namespace port
{
 
CameraData::CameraData()
    : m_logger(ara::log::CreateLogger("CAM", "PORT", ara::log::LogLevel::kVerbose))
    , m_running{false}
    , m_CEventData{{0U, 0U, 0U}, {0U, 0U, 0U}, 0LL}
{
}
 
CameraData::~CameraData()
{
}
 
void CameraData::Start()
{
    m_logger.LogVerbose() << "CameraData::Start";
    
    // construct skeleton
    ara::core::InstanceSpecifier specifier{"Camera/AA/CameraData"};
    m_interface = std::make_shared<deepracer::service::cameradata::skeleton::SvCameraDataSkeletonImpl>(specifier);
    
    // offer service
    auto offer = m_interface->OfferService();
    if (offer.HasValue())
    {
        m_running = true;
        m_logger.LogVerbose() << "CameraData::Start::OfferService";
    }
    else
    {
        m_running = false;
        m_logger.LogError() << "CameraData::Start::OfferService::" << offer.Error().Message();
    }
}
 
void CameraData::Terminate()
{
    m_logger.LogVerbose() << "CameraData::Terminate";
    
    // stop port
    m_running = false;
    
    // stop offer service
    m_interface->StopOfferService();
    m_logger.LogVerbose() << "CameraData::Terminate::StopOfferService";
}

/*==================================================*/
// 카메라 찾기
bool Camera::scanCameraIndex(const std::vector<int32_t>& cameraIdxList) {
    for (auto idx : cameraIdxList) {
        m_logger.LogVerbose() << "스캔 중: 카메라 인덱스 " << idx;

        cv::VideoCapture cap(idx, cv::CAP_V4L); // 카메라 캡처 생성
        cv::Mat test_frame;
        cap >> test_frame;

        if (test_frame.empty() || !cap.isOpened()) {
            m_logger.LogError() << "카메라를 열 수 없습니다: 인덱스 " << idx;
            continue;
        }

        cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        videoCaptureList_.emplace_back(std::move(cap));
    }

    return !videoCaptureList_.empty();
}
// 프레임 저장하기
void CameraData::produceFrames() {
    // 임시 데이터 구조체 생성
    deepracer::service::cameradata::skeleton::events::CEvent::SampleType& frame_data;  // 임시 데이터 구조체 생성

    // 각 카메라에서 프레임 캡처 및 JPEG 인코딩
    for (auto i = 0; i < videoCaptureList_.size() && i < 2; ++i) {  // 최대 두 카메라 사용
        cv::Mat frame;
        videoCaptureList_[i] >> frame;

        if (frame.empty()) {
            m_logger.LogError() << "프레임을 가져올 수 없습니다: 카메라 인덱스 " << i;
            continue;
        }

        // 프레임을 JPEG 형식으로 인코딩
        std::vector<uint8_t> buffer;
        cv::imencode(".jpeg", frame, buffer);

        // 첫 번째 또는 두 번째 카메라 데이터에 저장
        if (i == 0) {
            std::copy(buffer.begin(), buffer.begin() + std::min(buffer.size(), frame_data.camera_data0.size()), frame_data.camera_data0.begin());
        } else if (i == 1) {
            std::copy(buffer.begin(), buffer.begin() + std::min(buffer.size(), frame_data.camera_data1.size()), frame_data.camera_data1.begin());
        }
    }

    // 타임스탬프 설정
    frame_data.timestamp = getCurrentTimestamp();

    // frame_data를 m_CEventData에 저장
    WriteDataCEvent(frame_data);
}
// 밀리초 단위 타임스탬프 반환
int64_t CameraData::getCurrentTimestamp() const {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto now_ms = time_point_cast<milliseconds>(now);
    return now_ms.time_since_epoch().count();
}
/*==================================================*/
void CameraData::WriteDataCEvent(const deepracer::service::cameradata::skeleton::events::CEvent::SampleType& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_CEventData = data;
}
 
void CameraData::SendEventCEventCyclic()
{
    while (m_running)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto send = m_interface->CEvent.Send(m_CEventData);
            if (send.HasValue())
            {
                m_logger.LogVerbose() << "CameraData::SendEventCEventCyclic::Send";
            }
            else
            {
                m_logger.LogError() << "CameraData::SendEventCEventCyclic::Send::" << send.Error().Message();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
 
void CameraData::SendEventCEventTriggered()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto send = m_interface->CEvent.Send(m_CEventData);
    if (send.HasValue())
    {
        m_logger.LogVerbose() << "CameraData::SendEventCEventTriggered::Send";
    }
    else
    {
        m_logger.LogError() << "CameraData::SendEventCEventTriggered::Send::" << send.Error().Message();
    }
}
 
void CameraData::SendEventCEventTriggered(const deepracer::service::cameradata::skeleton::events::CEvent::SampleType& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_CEventData = data;
    auto send = m_interface->CEvent.Send(m_CEventData);
    if (send.HasValue())
    {
        m_logger.LogVerbose() << "CameraData::SendEventCEventTriggered::Send";
    }
    else
    {
        m_logger.LogError() << "CameraData::SendEventCEventTriggered::Send::" << send.Error().Message();
    }
}
 
} /// namespace port
} /// namespace aa
} /// namespace camera
 
/// EOF