#include "localizationkf.h"
#include "Storage/logController.h"

#include <Eigen/Core>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <Eigen/Sparse>

LocalizationKF::LocalizationKF(LogController& logcontroller):
_logcontroller(logcontroller)
{};


