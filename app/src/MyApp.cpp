//
// Created by natalija on 3/1/25.
//

#include "MyApp.hpp"

#include "GUIController.hpp"
#include "MainController.hpp"
#include "spdlog/spdlog.h"

#include <engine/platform/PlatformController.hpp>

namespace app {
class GUIController;
void MyApp::app_setup() {
    spdlog::info("App setup completed");
    auto main_controller = register_controller<MainController>();
    auto gui_controller = register_controller<GUIController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
    gui_controller->after(main_controller);
}
}// namespace app