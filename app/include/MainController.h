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
        return "MainController";
    }
};

}// namespace app

#endif//MAINCONTROLLER_H
