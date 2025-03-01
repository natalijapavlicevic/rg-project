//
// Created by natalija on 3/1/25.
//

#include "../include/MainController.h"

#include <engine/platform/PlatformController.hpp>

namespace app {
void MainController::initialize() {
}
bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KEY_ESCAPE).is_down()) {
        return false;
    }
    return true;
}
}// namespace app