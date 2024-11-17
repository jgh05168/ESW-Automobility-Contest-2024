#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>
#include <map>
#include <vector>

enum class ActionSpaceTypes {
    DISCRETE = 1,
    CONTINUOUS = 2
};

class ModelMetadataKeys {
public:
    static const std::string ACTION_SPACE_TYPE;
    static const std::string CONTINUOUS_HIGH;
    static const std::string CONTINUOUS_LOW;
    static const std::string STEERING;
    static const std::string SPEED;
};

// 기본 동작 공간
using ActionSpace = std::map<std::string, double>;  // 각 액션의 속성과 값을 나타내는 맵

extern const std::vector<ActionSpace> DEFAULT_ACTION_SPACE;  // 기본 동작 공간 선언
extern const std::vector<double> DEFAULT_SPEED_SCALES;       // 기본 속도 스케일 값 선언

#endif // CONSTANTS_HPP
