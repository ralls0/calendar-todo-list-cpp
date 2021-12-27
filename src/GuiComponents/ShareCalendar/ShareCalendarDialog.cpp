/**
 *
 * @author  Marco Manco
 * @date    27/12/21.
 * @file    ShareCalendarDialog.cpp
 * @brief
 *
 */

#include "ShareCalendarDialog.h"

ShareCalendarDialog::ShareCalendarDialog(QList<QString> cals, QWidget *parent)
    : QDialog(parent) {
  createBaseLayout(cals);
  createButtonGroupBox();

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(gb_baseInfo, 0, 0);
  layout->addWidget(_buttonBox, 2, 0, Qt::AlignRight);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(layout);

  setWindowTitle(tr("Create"));
}

ShareCalendarDialog::~ShareCalendarDialog() {
  delete _baseInfoLayout;
  delete gb_baseInfo;
}

void ShareCalendarDialog::createBaseLayout(QList<QString> cals) {

}

void ShareCalendarDialog::createButtonGroupBox() {
  btn_cancel = new QPushButton(tr("Close"), this);
  btn_cancel->setCheckable(true);
  QPixmap pixmapC(CLOSE_PATH);
  QIcon CloseIcon(pixmapC);
  btn_cancel->setIcon(CloseIcon);
  btn_share = new QPushButton(tr("Share"), this);
  btn_share->setCheckable(true);
  QPixmap pixmapA(ADD_PATH);
  QIcon AddIcon(pixmapA);
  btn_share->setIcon(AddIcon);

  connect(btn_share, &QPushButton::clicked, this, &ShareCalendarDialog::onShareClick);
  connect(btn_cancel, &QPushButton::clicked, this, &QWidget::close);

  _buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
  _buttonBox->addButton(btn_share, QDialogButtonBox::AcceptRole);

  _buttonBox->addButton(btn_cancel, QDialogButtonBox::RejectRole);
}

void ShareCalendarDialog::onShareClick(void) {

}
