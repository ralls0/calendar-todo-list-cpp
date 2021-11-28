/**
 *
 * @author  Marco Manco
 * @date    28/11/21.
 * @file    WindowNewCalendar.cpp
 * @brief
 *
 */

#include "WindowNewCalendar.h"

WindowNewCalendar::WindowNewCalendar(QWidget *parent) : QWidget(parent) {
  _newCalendarDialog = new NewCalendarDialog;
  createPreviewGroupBox();

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(_previewGroupBox, 0, 0);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(layout);

  setWindowTitle(tr("Calendar Widget"));
}

void WindowNewCalendar::createPreviewGroupBox() {
  _previewGroupBox = new QGroupBox(tr("Main"));

  QPushButton *btn_addCalendar = new QPushButton("Add Calendar", nullptr);
  connect(btn_addCalendar, &QPushButton::clicked, _newCalendarDialog,
          &QWidget::show);

  _previewLayout = new QGridLayout;
  _previewLayout->addWidget(btn_addCalendar, 0, 0, Qt::AlignCenter);
  _previewGroupBox->setLayout(_previewLayout);
}