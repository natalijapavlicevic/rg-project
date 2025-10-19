//
// Created by natalija on 3/1/25.
//

#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H
#include <engine/core/Controller.hpp>
#include <engine/core/Engine.hpp>

namespace app {

class MainController : public engine::core::Controller {
    void initialize() override;
    bool loop() override;


public:
    std::string_view name() const override {
        return "app::MainController";
    }

private:
    glm::vec3 light_color = glm::vec3(1.0f);
    glm::vec3 initial_light_color = glm::vec3(1.0f);
    bool hide_cat = false;
    bool fading = false;
    float elapsed_fading_time = 0.0f;
    std::chrono::time_point<std::chrono::steady_clock> fade_start;

    void
    begin_fading();
    void poll_events() override;

    void begin_draw() override;

    void draw_bench();
    void draw_grass();
    void draw_skybox();
    void draw_tree();
    void draw_cat();

    void draw() override;
    void end_draw() override;

    void update_camera();
    void update() override;
};

}// namespace app

#endif//MAINCONTROLLER_H
