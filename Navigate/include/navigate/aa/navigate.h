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
/// GENERATED FILE NAME               : navigate.h
/// SOFTWARE COMPONENT NAME           : Navigate
/// GENERATED DATE                    : 2024-11-07 14:01:17
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PARA_AA_GEN_SOFTWARE_COMPONENT_NAVIGATE_AA_H
#define PARA_AA_GEN_SOFTWARE_COMPONENT_NAVIGATE_AA_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "navigate/aa/port/inferencedata.h"
#include "navigate/aa/port/navigatedata.h"
 
#include "para/swc/port_pool.h"
 
//새로운 변수 정의
#include <map>
#include <vector>

#include "constants.hpp"
 
namespace navigate
{
namespace aa
{
 
class Navigate
{
public:
    /// @brief Constructor
    Navigate();
    
    /// @brief Destructor
    ~Navigate();
    
    /// @brief Initialize software component
    bool Initialize();
    
    /// @brief Start software component
    void Start();
    
    /// @brief Terminate software component
    void Terminate();
 
private:
    /// @brief Run software component
    void Run();
    void SetActionSpace()
    void TaskReceiveIEventCyclic();
    void InferenceCb(const deepracer::service::inferencedata::proxy::events::IEvent::SampleType& inferenceMsg);
    void ProcessInferenceData(const deepracer::service::inferencedata::proxy::events::IEvent::SampleType& inferenceMsg, deepracer::service::navigatedata::proxy::events::NEvent::SampleType& navigateMsg)
    float GetMaxScaledValue(float actionValue, const std::string& actionKey);
    float GetNonLinearlyMappedSpeed(float scaledActionSpaceSpeed);
    float ScaleContinuousValue(float action, float minOld, float maxOld, float minNew, float maxNew);
    void SetActionSpaceScales();

 
private:
    /// @brief Pool of port
    ::para::swc::PortPool m_workers;
    
    /// @brief Logger for software component
    ara::log::Logger& m_logger;
    
    /// @brief Instance of Port {Navigate.NavigateData}
    std::shared_ptr<navigate::aa::port::NavigateData> m_NavigateData;
    
    /// @brief Instance of Port {Navigate.InferenceData}
    std::shared_ptr<navigate::aa::port::InferenceData> m_InferenceData;

    // 새로운 멤버 변수 정의
    float m_throttleScale; // 스로틀 스케일 변수
    ActionSpaceTypes m_actionSpaceType; // 액션 스페이스 타입
    std::vector<std::map<std::string, double>> m_actionSpace; // 액션 스페이스 데이터 구조
    std::map<std::string, float> m_maxActionSpaceValues; // 최대 액션 스페이스 값들
    std::map<char, float> m_speedMappingCoeficients; // 속도 매핑 계수
};
 
} /// namespace aa
} /// namespace navigate
 
#endif /// PARA_AA_GEN_SOFTWARE_COMPONENT_NAVIGATE_AA_H