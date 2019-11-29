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
  setWindowTitle(tr("TP Area Loader (C) AECX"));
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
  m_logo->setPixmap(Resources::GetScaledPixmap("Twilight_Princess").scaledToWidth(300));

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

  m_group_select->addItems(QStringList{tr("Ordon"), tr("Kakariko")});
  m_group_select->setFont(h3);

  m_stage_select->addItems(QStringList{tr("F_SP108"), tr("F_SP104")});
  m_stage_select->setFont(h3);

  m_room_select->addItem(tr("1"));
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
  connect(m_trigger_load, &QPushButton::clicked, this, &AreaLoader::OnTriggerLoad);
}

void AreaLoader::OnStateChanged(Core::State state)
{
}

void AreaLoader::OnTriggerLoad()
{
  std::string t{"hello world"};
}

void AreaLoader::OnGroupIndexChange(int index)
{
  std::string t{"hello world"};
}

}  // namespace Zelda
