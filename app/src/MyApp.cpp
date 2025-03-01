//
// Created by natalija on 3/1/25.
//

#include "MyApp.h"

#include "MainController.h"
#include "spdlog/spdlog.h"

#include <engine/platform/PlatformController.hpp>

namespace app {
void MyApp::app_setup() {
    spdlog::info("App setup completed");
    auto main_controller = register_controller<MainController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
}
}// namespace app