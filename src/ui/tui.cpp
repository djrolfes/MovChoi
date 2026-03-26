#include "ui/tui.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>

using namespace ui;
using namespace ftxui;

// Helper for the footer
Element KeyBind(const std::string &key, const std::string &action) {
  return hbox({text(key) | color(Color::BlueLight), text(" " + action + " ")});
}

// TUI::myTable
Element TUI::myTable::getWindow() {
  decorate();
  return window(text(windowName), vscroll_indicator(frame(mtab.Render()))) |
         flex;
}

int TUI::myTable::decorate() {
  updateTableData();
  auto tab = getTable();
  mtab = Table(tab);
  mtab.SelectAll().Border(EMPTY);
  mtab.SelectAll().Separator(EMPTY);
  mtab.SelectRow(0).DecorateCells(color(Color::GrayDark));
  if (!tableData.empty()) {
    auto selection = mtab.SelectRow(selectedIndex + 1);
    selection.DecorateCells(inverted);
    selection.DecorateCells(focus);
  }
  return 0;
}

std::vector<std::vector<std::string>> TUI::myTable::getTable() {
  std::vector<std::vector<std::string>> rtn{tableHeader};
  for (int i = 0; i < tableData.size(); ++i) {
    rtn.push_back(tableData[i]);
  }
  return rtn;
}

int TUI::myTable::updateTableData() {
  if (mdataUpdate) {
    tableData = mdataUpdate();
    return 0;
  }
  return 1;
}
// Inside your myTable struct's event handler or FTXUI component wrapper:
bool TUI::myTable::handleEvent(ftxui::Event event) {
  if (mEvents) {
    return mEvents(*this, event);
  }
  return false;
}

// TUI
void TUI::init() {
  initTables();
  activeView = 0;
  misInit = true;
}

void TUI::initTables() {
  myTable user(
      [this]() { return mdb.getUsernamesVertical(); },
      [&](myTable &table, Event event) {
        if (event == Event::Character('j') || event == Event::ArrowDown) {
          table.selectedIndex = std::min(table.selectedIndex + 1,
                                         (int)table.tableData.size() - 1);
          return true; // Return true to indicate the event was handled
        }
        if (event == Event::Character('k') || event == Event::ArrowUp) {
          table.selectedIndex = std::max(table.selectedIndex - 1, 0);
          return true;
        }

        if (event == Event::Character('a')) {
          // TODO: Replace this hardcoded string with actual user input later
          std::string newUsername = "NewUser_" + std::to_string(rand() % 1000);

          if (mdb.addUser(newUsername) ==
              0) { // Assuming 0 is success based on your previous code
            table.updateTableData();

            // Optional: Move selection to the newly added user (the bottom)
            table.selectedIndex = table.tableData.size() - 1;
          }
          return true;
        }

        if (event == Event::Character('d')) {
          if (!table.tableData.empty()) {
            // Grab the username from the currently selected row
            // (Assuming the username is in the first column, index 0)
            std::string userToDelete = table.tableData[table.selectedIndex][0];

            mdb.removeUser(userToDelete);

            table.updateTableData();

            table.selectedIndex =
                std::min(table.selectedIndex,
                         std::max(0, (int)table.tableData.size() - 1));
          }
          return true;
        }
        return false; // Let unhandled events pass through
        ;
      });
  user.footer =
      vbox({hbox({KeyBind("j,↓", "Down"), text(" | "), KeyBind("k,↑", "Up"),
                  text(" | "), KeyBind("q", "Quit")}) |
                center,
            hbox({
                KeyBind("a", "Add"),
                text(" | "),
                KeyBind("d", "Delete"),
            }) | center});
  user.tableHeader = {"User"};
  user.windowName = " Users ";
  mytables.push_back(std::move(user));
}

void TUI::run() {
  mrenderer = std::move(Renderer([&] {
    return vbox({mytables[0].getWindow(), separatorEmpty(),
                 mytables[activeView].footer});
  }));
  auto component = CatchEvent(mrenderer, [&](Event event) {
    return mytables[activeView].handleEvent(event);
  });

  ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();

  component |= CatchEvent([&](Event event) {
    if (event == Event::Character('q') || event == Event::Escape) {
      screen.Exit();
      return true;
    }
    return false;
  });

  screen.Loop(component);
}
