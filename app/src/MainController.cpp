//
// Created by natalija on 3/1/25.
//

#include "MainController.hpp"

#include "GUIController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "spdlog/spdlog.h"

#include <engine/graphics/GraphicsController.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>


namespace app {

class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};

void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto gui_controller = engine::core::Controller::get<GUIController>();
    if (!gui_controller->is_enabled()) {
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        camera->rotate_camera(position.dx, position.dy);
    }
}

void MainController::initialize() {
    // spdlog::info("Main controller initialized");
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
    engine::graphics::OpenGL::enable_depth_testing();
}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KEY_ESCAPE).is_down()) {
        return false;
    }
    return true;
}

void MainController::draw_sun() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("sun");
    auto sun = engine::core::Controller::get<engine::resources::ResourcesController>()->model("sun");

    shader->use();
    shader->set_vec3("objectColor", glm::vec3(1.0f, 1.0f, 0.0f));
    shader->set_vec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()
                                     ->view_matrix());
    glm::mat4 model_sun = glm::mat4(1.0f);
    model_sun = glm::translate(model_sun, glm::vec3(0.0f, -0.0f, 3.0f));
    model_sun = glm::scale(model_sun, glm::vec3(0.3f));
    shader->set_mat4("model", model_sun);


    sun->draw(shader);
}

void MainController::set_lamp() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("lamp");
    auto camera = graphics->camera();

    shader->use();
    shader->set_int("material.diffuse", 0);
    shader->set_int("material.specular", 1);
    shader->set_vec3("light.position", camera->Position);
    shader->set_vec3("light.direction", camera->Front);
    shader->set_float("light.cutOff", glm::cos(glm::radians(12.5f)));
    shader->set_float("light.outerCutOff", glm::cos(glm::radians(15.0f)));
    shader->set_vec3("viewPos", camera->Position);
    //
    shader->set_vec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader->set_vec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->set_vec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shader->set_float("light.constant", 1.0f);
    shader->set_float("light.linear", 0.09f);
    shader->set_float("light.quadratic", 0.032f);
    //
    shader->set_float("material.shininess", 32.0f);
}


void MainController::draw_bench() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("bench");
    auto camera = graphics->camera();
    auto bench = engine::core::Controller::get<engine::resources::ResourcesController>()->model("bench");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()
                                     ->view_matrix());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.3f));
    shader->set_mat4("model", model);

    shader->set_vec3("light.position", camera->Position);
    shader->set_vec3("light.direction", camera->Front);
    shader->set_float("light.cutOff", glm::cos(glm::radians(12.5f)));
    shader->set_float("light.outerCutOff", glm::cos(glm::radians(17.5f)));

    shader->set_vec3("light.ambient", glm::vec3(0.1f));
    shader->set_vec3("light.diffuse", glm::vec3(0.8f));
    shader->set_vec3("light.specular", glm::vec3(1.0f));

    shader->set_float("light.constant", 1.0f);
    shader->set_float("light.linear", 0.09f);
    shader->set_float("light.quadratic", 0.032f);

    shader->set_vec3("viewPos", camera->Position);

    shader->set_int("material.diffuse", 0);
    shader->set_int("material.specular", 1);
    shader->set_float("material.shininess", 32.0f);


    bench->draw(shader);
}

void MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void MainController::draw_skybox() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto skybox = resources->skybox("basic_skybox");
    auto shader = resources->shader("skybox");
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    // auto camera = graphics->camera();
    // glm::mat4 view = glm::mat4(glm::mat3(camera->view_matrix()));
    // shader->set_mat4("view", view);
    // shader->set_mat4("projection", graphics->projection_matrix());

    graphics->draw_skybox(shader, skybox);
}

void MainController::draw() {
    set_lamp();
    draw_bench();
    draw_sun();
    // draw_skybox();
}

void MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}

void MainController::update_camera() {
    auto gui_controller = engine::core::Controller::get<GUIController>();
    if (gui_controller->is_enabled()) {
        return;
    }
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    float dt = platform->dt();

    if (platform->key(engine::platform::KEY_W).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
    }

    if (platform->key(engine::platform::KEY_S).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
    }

    if (platform->key(engine::platform::KEY_A).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
    }

    if (platform->key(engine::platform::KEY_D).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
    }

    // camera->update(platform->time());
}

void MainController::update() {
    update_camera();
}


}// namespace app