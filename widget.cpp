#include "widget.h"
#include <QDebug>
#include <QEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QString>
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>
#include "./ui_widget.h"

void Widget::Render(QPaintDevice *PaintDevice) {
  QPainter painter(PaintDevice);
  painter.setRenderHint(QPainter::Antialiasing, true);
  auto OrigPen = painter.pen();
  if (DrawText) {
    painter.setPen(Qt::blue);
    painter.setFont(QFont("Arial", 30));
    painter.drawText(rect(), Qt::AlignCenter, "Qt");
  }
  painter.setPen(OrigPen);
  if (Path.size() >= 2) {
    for (int q = 0; q < Path.size() - 1; q++) {
      painter.drawLine(Path.at(q), Path.at(q + 1));
    }
  }
  if (Path.size() > 0) {
    painter.drawLine(Path.back(), QPoint(CoordX, CoordY));
  }
}

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  setMouseTracking(true);
  installEventFilter(this);
}

Widget::~Widget() { delete ui; }

bool Widget::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::MouseMove) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    CoordX = mouseEvent->pos().x();
    CoordY = mouseEvent->pos().y();
  }
  update();
  return false;
}

void Widget::mousePressEvent(QMouseEvent *event) {
  Path.push_back(event->pos());
}

void Widget::mouseReleaseEvent(QMouseEvent *event) { update(); }

void Widget::paintEvent(QPaintEvent *event) { Render(this); }

void Widget::on_pushButton_clicked() {
  DrawText = true;
  update();
}

void Widget::on_saveImageButton_clicked() {
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save File"),
      "/home/ujoimro/doc/university/sje/2020_21_osz/c++ programozás",
      tr("Images (*.png)"));

  if (!fileName.isEmpty()) {
    QImage image(1024, 1024, QImage::Format_RGB32);
    image.fill(QColor(255, 255, 255));
    Render(&image);

    image.save(fileName);
  }
}

void Widget::on_SaveJsonButton_clicked() {
  auto fileName = QFileDialog::getSaveFileName(
      this, tr("Save Image Json"), "/home/ujoimro", tr("Image Files (*.json)"));

  if (!fileName.isEmpty()) {
    std::ofstream ofs(fileName.toStdString());
    cereal::JSONOutputArchive archive(ofs);
    archive(*this);
  }
}

void Widget::on_LoadJsonButton_clicked() {
  auto fileName = QFileDialog::getOpenFileName(
      this, tr("Save Image Json"), "/home/ujoimro", tr("Image Files (*.json)"));

  if (!fileName.isEmpty()) {
    std::ifstream ifs(fileName.toStdString());
    cereal::JSONInputArchive archive(ifs);
    archive(*this);
  }

  Render(this);
}

void Widget::on_aboutButton_clicked() {
  QMessageBox msgBox;
  msgBox.setText("Created by SJE.");
  msgBox.exec();
}

void Widget::on_guitButton_clicked() { QApplication::exit(); }
