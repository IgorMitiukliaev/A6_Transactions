#include "console_interface.h"

#include <memory>

using s21::MainWindow;
using Controller = s21::Controller;
using BaseType = Controller::BaseType;

MainWindow::MainWindow() { controller_ = std::make_unique<s21::Controller>(); };

auto MainWindow::Show() -> void { controller_->Init(::Controller::HASH); };