
#pragma once
#include "core/DatabaseManager.hpp"
#include <algorithm>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <iostream>
#include <string_view>

// TODO: BIG: use Objects for the different fields
namespace ui {
class TUI {
  struct State {};
  struct myTable {
    int selectedIndex = 0;
    std::vector<std::string> tableHeader;
    std::vector<std::vector<std::string>> tableData;
    ftxui::Element footer;
    std::string windowName;
    bool handleEvent(ftxui::Event event);

    ftxui::Element getWindow();
    int updateTableData();

    myTable() = default; // todo: should be better
    myTable(std::function<std::vector<std::vector<std::string>>()> dataUpdate,
            std::function<bool(myTable &table, ftxui::Event event)> events)
        : mdataUpdate(dataUpdate), mEvents(events) {}

  private:
    int decorate();
    std::vector<std::vector<std::string>> getTable();
    ftxui::Table mtab;
    std::function<std::vector<std::vector<std::string>>()> mdataUpdate;
    std::function<bool(myTable &table, ftxui::Event event)> mEvents;
  };

public:
  void init();

  void initTables();

  void run();

  ftxui::Component createRenderer();

  TUI(std::string dbname = "data.db3") : mdb(dbname) {};

private:
  bool misInit = false;
  myTable musers;
  myTable *activeView;
  core::DatabaseManager mdb;
  ftxui::Component mrenderer;
  ftxui::Component mcomponent;
};
} // namespace ui
