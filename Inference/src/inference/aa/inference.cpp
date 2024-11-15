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
/// GENERATED FILE NAME               : inference.cpp
/// SOFTWARE COMPONENT NAME           : Inference
/// GENERATED DATE                    : 2024-11-07 14:01:17
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "inference/aa/inference.h"
#include "inference/aa/image_process.hpp"
#include <exception>
#define RAD2DEG(x) ((x) * 180. / M_PI)
#include <opencv2/opencv.hpp>
#include <vector>
#include <mutex>
// 추가한 코드 (1줄)
#include "deepracer/type/impl_type_inferencedatanode.h"
std::mutex inference_mutex;
using namespace cv;
const std::string LIDAR = "LIDAR";
const std::string STEREO = "STEREO_CAMERAS";
const std::string FRONT = "FRONT_FACING_CAMERA";
const std::string OBS = "observation";
const std::string LEFT = "LEFT_CAMERA";

namespace inference
{
    namespace aa
    {

        /* 여기서부터 intel_inference_eng.cpp 가져오기 */

        namespace
        {
            class InferenceExcept : public std::exception
            {
                /// Simple exception class that is used to send a message to the catch clause.
            public:
                /// @param msg Message to be logged
                InferenceExcept(std::string msg)
                    : msg_(msg)
                {
                }
                virtual const char *what() const throw() override
                {
                    return msg_.c_str();
                }

            private:
                /// Store message in class so that the what method can dump it when invoked.
                const std::string msg_;
            };
            InferenceEngine::InferRequest setMultiHeadModel(std::string artifactPath, const std::string &device,
                                                            InferenceEngine::Core core, std::vector<std::string> &inputNamesArr,
                                                            std::string &outputName, const InferenceEngine::Precision &inputPrec,
                                                            const InferenceEngine::Precision &outputPrec)
            {

                // Validate the artifact path.
                auto strIdx = artifactPath.rfind('.');
                if (strIdx == std::string::npos)
                {
                    throw InferenceExcept("Artifact missing file extension");
                }
                if (artifactPath.substr(strIdx + 1) != "xml")
                {
                    throw InferenceExcept("No xml extension found");
                }

                auto network = core.ReadNetwork(artifactPath);
                // Loop through the inputNamesArr and set the precision
                for (const auto &pair : network.getInputsInfo())
                {
                    if (pair.first.rfind(OBS) != std::string::npos || pair.first.rfind(LIDAR) != std::string::npos || pair.first.rfind(FRONT) != std::string::npos || pair.first.rfind(STEREO) != std::string::npos || pair.first.rfind(LEFT) != std::string::npos)
                    {
                        inputNamesArr.push_back(pair.first);
                        pair.second->setPrecision(inputPrec);
                    }
                }
                auto outputInfo = network.getOutputsInfo().begin()->second;
                outputName = network.getOutputsInfo().begin()->first;
                outputInfo->setPrecision(outputPrec);

                auto executableNetwork = core.LoadNetwork(network, device);
                return executableNetwork.CreateInferRequest();
            }

            /// Helper method that loads grey images into the inference engine input
            /// @param inputPtr Pointer to the input data.
            /// @param imgProcessPtr Pointer to the image processing algorithm.
            /// @param imgData ROS message containing the image data.
            /// @param params Hash map of relevant parameters for image processing.
            // template<typename T, typename V> void load1DImg(V *inputPtr,
            //                                                 cv::Mat &retImg,
            //                                                 std::shared_ptr<InferTask::ImgProcessBase> imgProcessPtr,
            //                                                 const sensor_msgs::msg::Image &imgData,
            //                                                 const std::unordered_map<std::string, int> &params) {
            // imgProcessPtr->processImage(imgData, retImg, params);
            // if (retImg.empty()) {
            //     throw InferenceExcept("No image after pre-process");
            // }
            // int height = retImg.rows;
            // int width = retImg.cols;

            // for (int  h = 0; h < height; h++) {
            //     for (int w = 0; w < width; w++) {
            //         inputPtr[h * width + w] = retImg.at<T>(h, w);
            //     }
            // }
            // }

            /// Helper method that loads multi channel images into the inference engine input
            /// @param inputPtr Pointer to the input data.
            /// @param imgProcessPtr Pointer to the image processing algorithm.
            /// @param imgData ROS message containing the image data.
            /// @param params Hash map of relevant parameters for image processing.
            template <typename T, typename V>
            void loadStackImg(V *inputPtr,
                              cv::Mat &retImg,
                              std::shared_ptr<InferTask::ImgProcessBase> imgProcessPtr,
                              const deepracer::service::fusiondata::proxy::events::FEvent::SampleType &fusiondata,
                              const std::unordered_map<std::string, int> &params)
            {
                imgProcessPtr->processImage(fusiondata, retImg, params);
                if (retImg.empty())
                {
                    throw InferenceExcept("No image after-pre process");
                }
                const int channelSize = retImg.rows * retImg.cols;

                for (size_t pixelNum = 0; pixelNum < channelSize; ++pixelNum)
                {
                    for (size_t ch = 0; ch < retImg.channels(); ++ch)
                    {
                        inputPtr[(ch * channelSize) + pixelNum] = retImg.at<T>(pixelNum)[ch];
                    }
                }
            }

            /// Helper method that loads multi channel images into the inference engine input
            /// @param inputPtr Pointer to the input data.
            /// @param imgProcessPtr Pointer to the image processing algorithm.
            /// @param imgData ROS message containing the image data.
            /// @param params Hash map of relevant parameters for image processing.
            template <typename T, typename V>
            void loadStereoImg(V *inputPtr,
                               cv::Mat &retImg,
                               std::shared_ptr<InferTask::ImgProcessBase> imgProcessPtr,
                               const deepracer::service::fusiondata::proxy::events::FEvent::SampleType &fusiondata,
                               const std::unordered_map<std::string, int> &params)
            {
                imgProcessPtr->processImageVec(fusiondata, retImg, params);
                if (retImg.empty())
                {
                    throw InferenceExcept("No image after-pre process");
                }

                const int width = retImg.cols;
                const int height = retImg.rows;
                const int channel = retImg.channels();

                for (int c = 0; c < channel; c++)
                {
                    for (int h = 0; h < height; h++)
                    {
                        for (int w = 0; w < width; w++)
                        {
                            inputPtr[c * width * height + h * width + w] = retImg.at<T>(h, w)[c];
                        }
                    }
                }
            }

            /// Helper method that loads 1D data into the inference engine input
            /// @param inputPtr Pointer to the input data.
            /// @param lidarData ROS message containing the lidar data.
            void loadLidarData(float *inputPtr,
                               const deepracer::service::fusiondata::proxy::events::FEvent::SampleType &fusiondata)
            {
                size_t pixelNum = 0;
                for (const auto &lidar_value : fusiondata.lidar_data)
                {
                    inputPtr[pixelNum] = lidar_value;
                    ++pixelNum;
                }
            }
        }

        namespace IntelInferenceEngine
        {

            bool RLInferenceModel::loadModel(const char *artifactPath,
                                             std::shared_ptr<inference::aa::InferTask::Grey> imgProcess)
            {
                if (!imgProcess)
                {
                    // RCLCPP_ERROR(inferenceNode->get_logger(), "Invalid image processing algorithm");
                    return false;
                }
                // Set the image processing algorithms
                imgProcess_ = imgProcess;
                // Load the model
                try
                {
                    inferRequest_ = setMultiHeadModel(artifactPath, "CPU", core_, inputNamesArr_,
                                                      outputName_, InferenceEngine::Precision::FP32,
                                                      InferenceEngine::Precision::FP32);
                    for (size_t i = 0; i != inputNamesArr_.size(); ++i)
                    {
                        auto input = inferRequest_.GetBlob(inputNamesArr_[i]);
                        std::unordered_map<std::string, int> params_ = {{"width", input->getTensorDesc().getDims()[3]},
                                                                        {"height", input->getTensorDesc().getDims()[2]},
                                                                        {"channels", input->getTensorDesc().getDims()[1]}};
                        paramsArr_.push_back(params_);
                    }
                }
                catch (const std::exception &ex)
                {
                    // RCLCPP_ERROR(inferenceNode->get_logger(), "Model failed to load: %s", ex.what());
                    return false;
                }
                return true;
            }

            // void RLInferenceModel::startInference() {
            //     // Reset the image processing algorithm.
            //     if (imgProcess_) {
            //         imgProcess_->reset();
            //     }
            //     // doInference_ = true;
            // }

            // void RLInferenceModel::stopInference() {
            //     doInference_ = false;
            // }

            deepracer::type::InferenceDataNode RLInferenceModel::sensorCB(const deepracer::service::fusiondata::proxy::events::FEvent::FEvent::SampleType &fusiondata)
            {
                try
                {
                    for (size_t i = 0; i < inputNamesArr_.size(); ++i)
                    {
                        auto inputPtr = inferRequest_.GetBlob(inputNamesArr_[i])->buffer().as<InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type *>();
                        /*
                        deepracer_interfaces_pkg::msg::EvoSensorMsg::
                        */

                        // Object that will hold the data sent to the inference engine post processed.
                        cv::Mat retData;
                        if (inputNamesArr_[i].find(STEREO) != std::string::npos)
                        {
                            loadStereoImg<cv::Vec2b, float>(inputPtr, retData, imgProcess_, fusiondata, paramsArr_[i]);
                        }
                        // else if (inputNamesArr_[i].find(FRONT) != std::string::npos
                        //           || inputNamesArr_[i].find(LEFT) != std::string::npos
                        //           || inputNamesArr_[i].find(OBS) != std::string::npos) {
                        //     load1DImg<uchar, float>(inputPtr, retData, imgProcess_, fusiondata, paramsArr_[i]);
                        // }
                        else if (inputNamesArr_[i].find(LIDAR) != std::string::npos)
                        {
                            loadLidarData(inputPtr, fusiondata);
                        }
                        // else
                        // {
                        //     return;
                        // }
                        imgProcess_->reset();
                    }
                    // Do inference
                    inferRequest_.Infer();

                    auto output = inferRequest_.GetBlob(outputName_);
                    // Package the results and publish to all subscribers.

                    auto outputDims = output->getTensorDesc().getDims();
                    auto outputData = output->buffer().as<InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type *>();

                    deepracer::type::InferenceDataNode inferMsg;

                    inferMsg.timestamp = fusiondata.timestamp;

                    for (size_t label = 0; label < outputDims[1]; ++label)
                    {
                        // 삭제한 코드: 딥레이서 데이터 타입말고 (1줄)
                        // auto inferData = deepracer_interfaces_pkg::msg::InferResults();
                        // 추가한 코드: 우리꺼 쓰자 (1줄)
                        deepracer::type::InferenceData inferData;
                        inferData.class_label = label;
                        inferData.class_prob = outputData[label];
                        // Set bounding box data to -1 to indicate to subscribers that this model offers no
                        // localization information.
                        inferData.x_min = -1.0;
                        inferData.y_min = -1.0;
                        inferData.x_max = -1.0;
                        inferData.y_max = -1.0;
                        inferMsg.inference_data.push_back(inferData);
                    }
                    // 이 아래에 navigate로 Event를 보내는 코드 작성 ( 수정 필요 )

                    // 추가한 코드: 데이터 전송 (1줄)

                    return inferMsg;
                    /*

                    */
                }
                catch (const std::exception &ex)
                {
                }
            }
        }

        /* 여기 이후로부터는 자동 빌드된 AA 코드 */

        Inference::Inference()
            : m_logger(ara::log::CreateLogger("INFR", "SWC", ara::log::LogLevel::kVerbose)) // Logger 객체 초기화
              ,
              m_workers(2) // Run() 함수에서 m_workers.Async에 등록가능한 함수 갯수

        {
        }

        Inference::~Inference()
        {
        }

        bool Inference::Initialize()
        {
            m_logger.LogVerbose() << "Inference::Initialize";

            bool init{true};

            // fusion PPort data
            m_FusionData = std::make_shared<inference::aa::port::FusionData>();
            // inference RPort data
            m_InferenceData = std::make_shared<inference::aa::port::InferenceData>();
            // 여기서 Inference_load_model 객체 생성
            m_Model = std::make_shared<inference::aa::IntelInferenceEngine::RLInferenceModel>();
            m_Model->loadModel("/opt/aws/deepracer/artifacts/jangdeokdong-model/", std::make_shared<inference::aa::InferTask::Grey>(false, false));

            return init;
        }

        // 컴포넌트 시작 함수

        void Inference::Start()
        {
            m_logger.LogVerbose() << "Inference::Start";

            m_FusionData->Start();
            m_InferenceData->Start();

            // run software component
            Run();
        }

        // 컴포넌트 종료 함수
        void Inference::Terminate()
        {
            m_logger.LogVerbose() << "Inference::Terminate";

            m_FusionData->Terminate();
            m_InferenceData->Terminate();
        }

        // 컴포넌트 수행 함수
        void Inference::Run()
        {
            m_logger.LogVerbose() << "Inference::Run";

            // 모델 불러오기 ( 수정 필요 )
            // inference_node.cpp 132번째 줄 확인
            // m_Inference = std::make_shared<inference::aa::Inference::RLInferenceModel>();

            // 매 주기마다 IEvent 데이터를 전송
            m_workers.Async([this]
                            { m_InferenceData->SendEventIEventCyclic(); });
            m_workers.Async([this]
                            { TaskReceiveFEventCyclic(); });
            /*
            inference에서 처리해야 할 일
            1. Fusion에서 데이터 수신 : TaskRequestFMethod()
                1-2. Fusion 측에 Method 요청
            2. 요청 받으면 모델 판별 시작
            3. Navigator에 데이터 송신 : SendEventIEventCyclic()
            */

            /*
            실행 순서 :
            Run() -> TaskRequestFMethod() -> Response 처리 핸들러(SetReceiveMethodFMethodHandler) 등록
                -> RequestFMethod() -> 데이터 받았으면, OnReceiveFMethod() -> 모델 추론 -> WriteDataIEvent()
            */

            m_workers.Async([this]
                            { m_InferenceData->SendEventIEventCyclic(); });

            m_workers.Wait();
        }

        // FusionData Event의 Cyclic 수신처리에 대한 수행
        void Inference::TaskReceiveFEventCyclic()
        {

            m_FusionData->SetReceiveEventFEventHandler([this](const auto &FEvent){
                                                        std::lock_guard<std::mutex> lock(inference_mutex);
                                                        OnReceiveFEvent(FEvent); });
            m_FusionData->ReceiveEventFEventCyclic();
        }

        // Fusion FEvent에 대한 Response를 받았을시의 처리 함수
        void Inference::OnReceiveFEvent(const deepracer::service::fusiondata::proxy::events::FEvent::FEvent::SampleType &FEvent)
        {

            m_logger.LogInfo() << "Inference::OnReceiveFMethod:" << FEvent.timestamp;
            deepracer::type::InferenceDataNode inferMsg = m_Model->sensorCB(FEvent);
            m_InferenceData->WriteDataIEvent(inferMsg);
        }

    } /// namespace aa
} /// namespace inference
