///////////////////////////////////////////////////////////////////////////////////
//   Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.          //
//                                                                               //
//   Licensed under the Apache License, Version 2.0 (the "License").             //
//   You may not use this file except in compliance with the License.            //
//   You may obtain a copy of the License at                                     //
//                                                                               //
//       http://www.apache.org/licenses/LICENSE-2.0                              //
//                                                                               //
//   Unless required by applicable law or agreed to in writing, software         //
//   distributed under the License is distributed on an "AS IS" BASIS,           //
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    //
//   See the License for the specific language governing permissions and         //
//   limitations under the License.                                              //
///////////////////////////////////////////////////////////////////////////////////

#include "inference/aa/image_process.hpp"
#include <opencv2/opencv.hpp>
#include <vector>
namespace
{
    bool cvtToCVObjResize(const std::vector<uchar> &output, cv::Mat &retImg,
                          const std::unordered_map<std::string, int> &params)
    {

        cv::Mat cvImg;

        try
        {
            cvImg = cv::imdecode(output, cv::IMREAD_COLOR);
        }
        catch (cv::Exception &e)
        {
            return false;
        }
        // 이미지 크기 조정 파라미터 확인
        auto itWidth = params.find("width");
        if (itWidth == params.end())
        {
            return false;
        }

        auto itHeight = params.find("height");
        if (itHeight == params.end())
        {
            return false;
        }

        // 이미지 resize
        try
        {
            cv::resize(cvImg, retImg, cv::Size(itWidth->second, itHeight->second));
        }
        catch (cv::Exception &e)
        {
            return false;
        }

        return true;
    }
    
}

namespace inference
{
    namespace aa
    {
        namespace InferTask
        {
            void RGB::processImage(const deepracer::service::fusiondata::proxy::events::FEvent::SampleType &fusiondata, cv::Mat &retImg,
                                   const std::unordered_map<std::string, int> &params)
            {
                cvtToCVObjResize(fusiondata.camera_data[0], retImg, params);
            }

            // 수정 필요 
            const std::string RGB::getEncode() const
            {
                return "bgr8";
            }

            Grey::Grey(bool isThreshold, bool isMask)
                : isThreshold_(isThreshold),
                  isMask_(isMask)
            {
            }

            void Grey::processImageVec(const deepracer::service::fusiondata::proxy::events::FEvent::SampleType &fusiondata, 
                                cv::Mat &retImg, const std::unordered_map<std::string, int> &params)
            {
                // Left camera image is sent as the top image and the right camera image is sent as second in the std::vector.
                try
                {
                    std::vector<cv::Mat> channels;
                    for (const auto &image_msg : fusiondata.camera_data)
                    {
                        cv::Mat img;
                        cvtToCVObjResize(image_msg, img, params);
                        cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
                        channels.push_back(img);
                    }
                    cv::merge(channels, retImg);
                }
                catch (...)
                {
                    return;
                }
            }

            void Grey::reset()
            {
                imageStack_.clear();
            }

            const std::string Grey::getEncode() const
            {
                return "mono8";
            }

        }

    }
}