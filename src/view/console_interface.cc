#include "console_interface.h"
#include <ios>
#include <limits>

using s21::MainWindow;
using Controller = s21::Controller;
using BaseType = Controller::BaseType;

MainWindow::MainWindow() { controller_ = std::make_unique<s21::Controller>(); }

auto MainWindow::Show() -> void {
  Autors();
  FirstMenu();
  ShooseFirstType();
};

auto MainWindow::Autors() -> void {
  std::system("clear");
  std::cout << style2 << line_top << std::endl;
  std::cout << "║                                        T R A N S A C T I O N "
               "S                                        ║"
            << std::endl;
  std::cout << "║                                                              "
               "                                         ║"
            << std::endl;
  std::cout << "║                                powered by " << style2_1
            << "wloyd" << style2 << " && " << style2_1 << "pfidelia" << style2
            << " && " << style2_1 << "imagnifi" << style2
            << "                               ║" << std::endl;
  std::cout << line_bottom << std::endl;
  std::cout << clear_csi;
};

auto MainWindow::FirstMenu() -> void {
  std::cout << style3 << line_top << std::endl;
  std::cout << "║                                           Shoose conteiner "
               "type                                       ║"
            << std::endl;
  std::cout << "║ 1. Hash table                                                "
               "                                         ║"
            << std::endl;
  std::cout << "║ 2. Binary search trees                                       "
               "                                         ║"
            << std::endl;
  std::cout << "║ 3. Research                                                  "
               "                                         ║"
            << std::endl;
  std::cout << "║ 0. Exit                                                      "
               "                                         ║"
            << std::endl;
  std::cout << line_bottom << std::endl;
  std::cout << clear_csi;
};

auto MainWindow::SecondMenu() -> void {
  std::cout << style3 << line_top << std::endl;
  std::cout << "║                                           ENTER COMMAND      "
               "                                         ║"
            << std::endl;
  std::cout << "║                                                              "
               "                                         ║"
            << std::endl;
  std::cout << "║ SET <key> <surname> <name> <year> <city> <coins> EX <life "
               "time>                            Add element║"
            << std::endl;
  std::cout << "║ GET <key>                                                    "
               "    Get the value associated with the key║"
            << std::endl;
  std::cout << "║ EXISTS <key>                                                 "
               "                    Check if object exist║"
            << std::endl;
  std::cout << "║ DEL <key>                                                    "
               "                        Delete the record║"
            << std::endl;
  std::cout << "║ UPDATE <key> <surname> <name> <year> <city> <coins> *        "
               "                     Update elements data║"
            << std::endl;
  std::cout << "║ KEYS                                                         "
               "                         Get all the keys║"
            << std::endl;
  std::cout << "║ RENAME <old key> <new key>                                   "
               "                               Rename key║"
            << std::endl;
  std::cout << "║ TTL <key>                                                    "
               "                            Show elements║"
            << std::endl;
  std::cout << "║ FIND <surname> <name> <year> <city> <coins> *                "
               "                             Find element║"
            << std::endl;
  std::cout << "║ SHOWALL                                                      "
               "                          Get all records║"
            << std::endl;
  std::cout << "║ UPLOAD <file path>                                           "
               "                  Upload data from a file║"
            << std::endl;
  std::cout << "║ EXPORT <file path>                                           "
               "                    Save the data to file║"
            << std::endl;
  std::cout << "║ CLEAR                                                        "
               "                            Clear storage║"
            << std::endl;
  std::cout << "║ STORAGE                                                      "
               "                           Change storage║"
            << std::endl;
  std::cout << "║ EXIT                                                         "
               "                        Exit from program║"
            << std::endl;
  std::cout << line_bottom << std::endl;
  std::cout << clear_csi_n;
}

auto MainWindow::ShooseFirstType() -> void {
  int res = 0;
  if (std::cin >> res) {
    if (res < 0 && res > 3) {
      Invalid();
      ShooseFirstType();
    }
  }
  std::cin.clear();
  SwitchType(res);
}

auto MainWindow::ShooseSecondCommand() -> void {
  std::string src = Input();
  controller_->Command(src);
}

auto MainWindow::SwitchType(int type) -> void {
  std::system("clear");
  switch (type) {
  case 1:
    controller_->Init(::Controller::SBT);
    SecondMenu();
    ShooseSecondCommand();
    break;
  case 2:
    controller_->Init(::Controller::HASH);
    SecondMenu();
    ShooseSecondCommand();
    break;
  case 3:
    // controller_->ClearStorage();
    FirstMenu();
    ShooseFirstType();
    break;
  default:
    break;
  }
}

auto MainWindow::Input() -> std::string {
  std::string str;
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  if (!std::getline(std::cin, str)) {
    exit(0);
  }
  return str;
}

auto MainWindow::Invalid() -> void {
  std::cout << "ERROR : try again" << std::endl;
}