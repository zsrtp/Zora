// (C) AECX 2019
// Zora
// https://github.com/zsrtp/Zora

#pragma once

#include <functional>
#include <memory>
#include <vector>

#include <QDialog>

#include "Common/CommonTypes.h"

class QComboBox;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
class QSplitter;
class QTabWidget;
class QTableWidget;
class QTableWidgetItem;
struct Result;

namespace Core
{
enum class State;
}

namespace UICommon
{
class GameFile;
}
namespace Zelda
{
  class AreaLoader final : public QDialog
  {
    Q_OBJECT
  public:
    explicit AreaLoader(QWidget* parent = nullptr);
    ~AreaLoader();

  private:
    void CreateWidgets();
    void ConnectWidgets();
    void OnStateChanged(Core::State state);
    void OnTriggerLoad();
    void OnGroupIndexChange(int index);


    QLayout* m_layout;

    QLabel* m_logo;

    QLabel* m_group_label;
    QComboBox* m_group_select;

    QLabel* m_stage_label;
    QComboBox* m_stage_select;

    QLabel* m_room_label;
    QComboBox* m_room_select;

    QLabel* m_spawn_label;
    QComboBox* m_spawn_select;

    QLabel* m_state_label;
    QComboBox* m_state_select;

    QPushButton* m_trigger_load;

  };
}  // namespace Zelda
