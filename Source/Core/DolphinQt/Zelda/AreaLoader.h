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
    void OnStageIndexChange(int index);
    void OnRoomIndexChange(int index);
    void OnSpawnIndexChange(int index);
    void OnStateIndexChange(int index);

    QLayout* m_layout = nullptr;

    QLabel* m_logo = nullptr;

    QLabel* m_group_label = nullptr;
    QComboBox* m_group_select = nullptr;

    QLabel* m_stage_label = nullptr;
    QComboBox* m_stage_select = nullptr;

    QLabel* m_room_label = nullptr;
    QComboBox* m_room_select = nullptr;

    QLabel* m_spawn_label = nullptr;
    QComboBox* m_spawn_select = nullptr;

    QLabel* m_state_label = nullptr;
    QComboBox* m_state_select = nullptr;

    QPushButton* m_trigger_load = nullptr;

  };
}  // namespace Zelda
