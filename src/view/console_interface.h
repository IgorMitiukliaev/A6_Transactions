#ifndef SRC_VIEW_CONSOLE_INTERFACE_H_
#define SRC_VIEW_CONSOLE_INTERFACE_H_

#include <cstring>
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

  auto Autors() -> void;
  auto FirstMenu() -> void;
  auto SecondMenu() -> void;
  auto ShooseFirstType() -> void;
  auto ShooseSecondCommand() -> void;
  auto SwitchType(int) -> void;
  auto Input() -> std::string;

  auto Invalid() -> void;
  std::string line_top = "╔════════════════════════════════════════════════════"
                         "═══════════════════════════════════════════════════╗";
  std::string line_bottom =
      "╚═══════════════════════════════════════════════════════════════════════"
      "════════════════════════════════╝";
  std::string line_empty =
      "                                                                        "
      "                                 ";
  std::string clear_csi = "\u001b[0m";
  std::string clear_csi_n = "\u001b[0m\n";
  std::string style1 = "\u001b[1;38;5;165m";
  std::string style2 = "\u001b[1;48;5;25;38;5;117m";
  std::string style2_1 = "\u001b[1;48;5;25;38;5;154m";
  std::string style3 = "\u001b[1;48;5;117;38;5;20m";
  std::string style4 = "\u001b[1;38;5;117m";
};
} // namespace s21

#endif //  SRC_VIEW_CONSOLE_INTERFACE_H_