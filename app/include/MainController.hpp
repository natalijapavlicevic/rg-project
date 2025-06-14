//
// Created by natalija on 3/1/25.
//

#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H
#include <engine/core/Controller.hpp>

namespace app {

class MainController : public engine::core::Controller {
    void initialize() override;
    bool loop() override;


public:
    std::string_view name() const override {
        return "app::MainController";
    }

private:
    // void draw_skybox();
    void draw_backpack();
    void begin_draw() override;
    void draw_skybox();
    void draw() override;
    void end_draw() override;
    void update_camera();
    void update() override;
};

}// namespace app

#endif//MAINCONTROLLER_H
