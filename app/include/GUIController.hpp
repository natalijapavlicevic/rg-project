//
// Created by natalija on 6/14/25.
//

#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H
#include "engine/core/Controller.hpp"

namespace app {

class GUIController : public engine::core::Controller {
public:
    std::string_view name() const override {
        return "app::GUIController";
    }

private:
    void initialize() override;
    void draw() override;
    void poll_events() override;
};

}// namespace app

#endif//GUICONTROLLER_H
