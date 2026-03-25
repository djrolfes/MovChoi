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
  return window(text(windowName), mtab.Render()) | flex;
}

int TUI::myTable::decorate() {
  updateTableData();
  mtab = Table(getTable());
  mtab.SelectAll().Border(EMPTY);
  mtab.SelectAll().Separator(EMPTY);
  mtab.SelectRow(0).DecorateCells(color(Color::GrayDark));
  mtab.SelectRow(selectedIndex + 1).DecorateCells(inverted);
  return 0;
}

std::vector<std::vector<std::string>> TUI::myTable::getTable() {
  std::vector<std::vector<std::string>> rtn{tableHeader};
  for (int i = 0; i <= tableData.size(); ++i) {
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
  activeView = &musers;
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
}

void TUI::run() {
  mrenderer = Renderer([&] {
    return vbox({musers.getWindow(), separatorEmpty(), activeView->footer});
  });
  auto component = CatchEvent(
      mrenderer, [&](Event event) { return activeView->handleEvent(event); });

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
