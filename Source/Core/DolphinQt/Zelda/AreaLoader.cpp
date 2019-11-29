// (C) AECX 2019
// Zora
// https://github.com/zsrtp/zora

#include "DolphinQt/Zelda/AreaLoader.h"

#include <algorithm>
#include <cstring>

#include <QComboBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QRadioButton>
#include <QSplitter>
#include <QTabWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QtWidgets>

#include "Core/ActionReplay.h"
#include "Core/ConfigManager.h"
#include "Core/Core.h"
#include "Core/Debugger/PPCDebugInterface.h"
#include "Core/HW/Memmap.h"
#include "Core/PowerPC/MMU.h"
#include "Core/PowerPC/PowerPC.h"

#include "UICommon/GameFile.h"

#include "DolphinQt/Config/ARCodeWidget.h"
#include "DolphinQt/Config/GeckoCodeWidget.h"
#include "DolphinQt/GameList/GameListModel.h"
#include "DolphinQt/Resources.h"
#include "DolphinQt/Settings.h"

namespace Zelda
{
AreaLoader::~AreaLoader() = default;

AreaLoader::AreaLoader(QWidget* parent) : QDialog(parent)
{
  setWindowTitle(tr("TP Area Loader"));
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

  connect(&Settings::Instance(), &Settings::EmulationStateChanged, this,
          &AreaLoader::OnStateChanged);

  OnStateChanged(Core::GetState());

  CreateWidgets();
  ConnectWidgets();
}

void AreaLoader::CreateWidgets()
{
  m_layout = new QVBoxLayout;

  QFont h2 = QFont();

  h2.setBold(true);
  h2.setPointSize(20);

  QFont h3 = QFont();

  h3.setBold(true);
  h3.setPointSize(15);

  m_logo = new QLabel();
  m_logo->setPixmap(Resources::GetScaledPixmap("Twilight Princess/Logo").scaledToWidth(300));

  m_group_label = new QLabel(tr("Group"));
  m_group_label->setFont(h2);

  m_stage_label = new QLabel(tr("Stage"));
  m_stage_label->setFont(h2);

  m_room_label = new QLabel(tr("Room"));
  m_room_label->setFont(h2);

  m_spawn_label = new QLabel(tr("Spawnpoint"));
  m_spawn_label->setFont(h2);

  m_state_label = new QLabel(tr("State"));
  m_state_label->setFont(h2);

  m_group_select = new QComboBox();
  m_stage_select = new QComboBox();
  m_room_select = new QComboBox();
  m_spawn_select = new QComboBox();
  m_state_select = new QComboBox();

  QStandardItem* item = new QStandardItem();
  item->setData(tr("Ordon"), Qt::DisplayRole);
  item->setData(0, Qt::UserRole);

  m_group_select->addItem(tr("Overworld"), 0);
  m_group_select->addItem(tr("Dungeon"), 1);
  m_group_select->setAutoCompletion(true);
  m_group_select->setEditable(true);
  m_group_select->setFont(h3);

  m_stage_select->addItems(QStringList{tr("Ordon"), tr("Ordon Spring")});
  m_stage_select->setAutoCompletion(true);
  m_stage_select->setFont(h3);

  m_room_select->addItem(tr("Entrance"));
  m_room_select->setAutoCompletion(true);
  m_room_select->setFont(h3);

  m_spawn_select->addItem(tr("0"));
  m_spawn_select->setFont(h3);

  m_state_select->addItem(tr("0x0"));
  m_state_select->setFont(h3);

  m_trigger_load = new QPushButton(tr("Load"));
  m_trigger_load->setFixedHeight(80);
  m_trigger_load->setFont(h2);

  m_layout->addWidget(m_logo);

  m_layout->addWidget(m_group_label);
  m_layout->addWidget(m_group_select);
  m_layout->addWidget(m_stage_label);
  m_layout->addWidget(m_stage_select);
  m_layout->addWidget(m_room_label);
  m_layout->addWidget(m_room_select);
  m_layout->addWidget(m_spawn_label);
  m_layout->addWidget(m_spawn_select);
  m_layout->addWidget(m_state_label);
  m_layout->addWidget(m_state_select);

  m_layout->addWidget(m_trigger_load);

  setLayout(m_layout);
}

void AreaLoader::ConnectWidgets()
{
  connect(m_group_select, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &AreaLoader::OnGroupIndexChange);
  connect(m_stage_select, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &AreaLoader::OnStageIndexChange);
  connect(m_room_select, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &AreaLoader::OnRoomIndexChange);
  connect(m_spawn_select, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &AreaLoader::OnSpawnIndexChange);
  connect(m_state_select, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &AreaLoader::OnStateIndexChange);


  connect(m_trigger_load, &QPushButton::clicked, this, &AreaLoader::OnTriggerLoad);

}

void AreaLoader::OnStateChanged(Core::State state)
{
}

void AreaLoader::OnTriggerLoad()
{
  
}

void AreaLoader::OnGroupIndexChange(int index)
{
}

void AreaLoader::OnStageIndexChange(int index)
{
}

void AreaLoader::OnRoomIndexChange(int index)
{
}

void AreaLoader::OnSpawnIndexChange(int index)
{
}
void AreaLoader::OnStateIndexChange(int index)
{
}

}  // namespace Zelda
