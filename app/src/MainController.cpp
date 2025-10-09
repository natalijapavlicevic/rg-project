//
// Created by natalija on 3/1/25.
//

#include "MainController.hpp"

#include "../../engine/libs/glad/include/glad/glad.h"
#include "GUIController.hpp"
#include "engine/graphics/Camera.hpp"
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

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();

    camera->Position = glm::vec3(-3.0f, 2.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.5f, 3.2f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    camera->Front = glm::normalize(cameraTarget - camera->Position);
    camera->Right = glm::normalize(glm::cross(camera->Front, cameraUp));
    camera->Up = glm::cross(camera->Right, camera->Front);
}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KEY_ESCAPE).is_down()) {
        return false;
    }
    return true;
}


void MainController::poll_events() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KEY_R).state() == engine::platform::Key::State::JustPressed) {
        light_color = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    if (platform->key(engine::platform::KEY_G).state() == engine::platform::Key::State::JustPressed) {
        light_color = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (platform->key(engine::platform::KEY_B).state() == engine::platform::Key::State::JustPressed) {
        light_color = glm::vec3(0.0f, 0.0f, 1.0f);
    }
    if (platform->key(engine::platform::KEY_SPACE).state() == engine::platform::Key::State::JustPressed) {
        light_color = glm::vec3(1.0f);
    }
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
    model = glm::translate(model, glm::vec3(0.0f, 0.5f, 3.0f));
    model = glm::scale(model, glm::vec3(1.1f));
    shader->set_mat4("model", model);

    shader->set_vec3("spotlight.position", camera->Position);
    shader->set_vec3("spotlight.direction", camera->Front);
    shader->set_float("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
    shader->set_float("spotlight.outerCutOff", glm::cos(glm::radians(17.5f)));

    shader->set_vec3("spotlight.ambient", glm::vec3(0.2f));
    shader->set_vec3("spotlight.diffuse", light_color * 1.2f);
    shader->set_vec3("spotlight.specular", light_color);

    shader->set_float("spotlight.constant", 1.0f);
    shader->set_float("spotlight.linear", 0.05f);
    shader->set_float("spotlight.quadratic", 0.01f);
    shader->set_vec3("lightColor", light_color);

    shader->set_vec3("viewPos", camera->Position);

    shader->set_int("material.diffuse", 0);
    shader->set_int("material.specular", 1);
    shader->set_float("material.shininess", 32.0f);

    shader->set_vec3("dirlight.direction", camera->Front);
    shader->set_vec3("dirlight.ambient", glm::vec3(0.1f));
    shader->set_vec3("dirlight.diffuse", glm::vec3(0.3f));
    shader->set_vec3("dirlight.specular", glm::vec3(0.2f));

    bench->draw(shader);
}

void MainController::draw_grass() {
    auto grass = engine::core::Controller::get<engine::resources::ResourcesController>()->model("grass");
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("grass");
    auto camera = graphics->camera();
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", camera
                                     ->view_matrix());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
    model = glm::scale(model, glm::vec3(0.03f));
    shader->set_mat4("model", model);

    shader->set_vec3("spotlight.position", camera->Position);
    shader->set_vec3("spotlight.direction", camera->Front);
    shader->set_float("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
    shader->set_float("spotlight.outerCutOff", glm::cos(glm::radians(17.5f)));

    shader->set_vec3("spotlight.ambient", glm::vec3(0.1f));
    shader->set_vec3("spotlight.diffuse", light_color * 0.8f);
    shader->set_vec3("spotlight.specular", glm::vec3(0.0f));

    shader->set_float("spotlight.constant", 1.0f);
    shader->set_float("spotlight.linear", 0.05f);
    shader->set_float("spotlight.quadratic", 0.01f);
    shader->set_vec3("lightColor", light_color);
    shader->set_vec3("viewPos", camera->Position);
    shader->set_int("material.diffuse", 0);
    shader->set_int("material.specular", 0);
    shader->set_float("material.shininess", 5.0f);

    shader->set_vec3("dirlight.direction", camera->Front);
    shader->set_vec3("dirlight.ambient", glm::vec3(0.1f));
    shader->set_vec3("dirlight.diffuse", glm::vec3(0.1f));
    shader->set_vec3("dirlight.specular", glm::vec3(0.0f));

    grass->draw(shader);
}

void MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void MainController::draw_skybox() {
    glClearColor(0.02f, 0.05f, 0.1f, 1.0f);
}

void MainController::draw_tree() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("bench");
    auto camera = graphics->camera();
    auto tree = engine::core::Controller::get<engine::resources::ResourcesController>()->model("tree");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()
                                     ->view_matrix());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(0.02f));
    shader->set_mat4("model", model);

    shader->set_vec3("spotlight.position", camera->Position);
    shader->set_vec3("spotlight.direction", camera->Front);
    shader->set_float("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
    shader->set_float("spotlight.outerCutOff", glm::cos(glm::radians(17.5f)));

    shader->set_vec3("spotlight.ambient", glm::vec3(0.2f));
    shader->set_vec3("spotlight.diffuse", light_color * 1.2f);
    shader->set_vec3("spotlight.specular", glm::vec3(0.2f));

    shader->set_float("spotlight.constant", 1.0f);
    shader->set_float("spotlight.linear", 0.05f);
    shader->set_float("spotlight.quadratic", 0.01f);
    shader->set_vec3("lightColor", light_color);

    shader->set_vec3("viewPos", camera->Position);

    shader->set_int("material.diffuse", 0);
    shader->set_int("material.specular", 1);
    shader->set_float("material.shininess", 5.0f);

    shader->set_vec3("dirlight.direction", camera->Front);
    shader->set_vec3("dirlight.ambient", glm::vec3(0.1f));
    shader->set_vec3("dirlight.diffuse", glm::vec3(0.3f));
    shader->set_vec3("dirlight.specular", glm::vec3(0.0f));

    tree->draw(shader);
}

void MainController::draw_cat() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("bench");
    auto camera = graphics->camera();
    auto cat = engine::core::Controller::get<engine::resources::ResourcesController>()->model("cat");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()
                                     ->view_matrix());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.8f, 3.5f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.012f));
    shader->set_mat4("model", model);

    shader->set_vec3("spotlight.position", camera->Position);
    shader->set_vec3("spotlight.direction", camera->Front);
    shader->set_float("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
    shader->set_float("spotlight.outerCutOff", glm::cos(glm::radians(17.5f)));

    shader->set_vec3("spotlight.ambient", glm::vec3(0.2f));
    shader->set_vec3("spotlight.diffuse", light_color * 1.2f);
    shader->set_vec3("spotlight.specular", light_color);

    shader->set_float("spotlight.constant", 1.0f);
    shader->set_float("spotlight.linear", 0.05f);
    shader->set_float("spotlight.quadratic", 0.01f);
    shader->set_vec3("lightColor", glm::vec3(0.0f));

    shader->set_vec3("viewPos", camera->Position);

    shader->set_int("material.diffuse", 0);
    shader->set_int("material.specular", 0);
    shader->set_float("material.shininess", 5.0f);

    shader->set_vec3("dirlight.direction", camera->Front);
    shader->set_vec3("dirlight.ambient", glm::vec3(0.3f));
    shader->set_vec3("dirlight.diffuse", glm::vec3(0.3f));
    shader->set_vec3("dirlight.specular", glm::vec3(0.0f));

    cat->draw(shader);
}

void MainController::draw() {
    draw_bench();
    draw_tree();
    draw_grass();
    draw_cat();
    draw_skybox();
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