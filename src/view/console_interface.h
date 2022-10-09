#ifndef SRC_VIEW_CONSOLE_INTERFACE_H_
#define SRC_VIEW_CONSOLE_INTERFACE_H_

#include <memory>

#include "../controller/controller.h"

namespace s21 {
class MainWindow {
 public:
  MainWindow();
  ~MainWindow() = default;
  auto Show() -> void;

 private:
  std::unique_ptr<Controller> controller_ = nullptr;
};
}  // namespace s21

#endif  //  SRC_VIEW_CONSOLE_INTERFACE_H_