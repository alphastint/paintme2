#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

template <class Archive>
void serialize(Archive &archive, QPoint &point) {
  archive(point.rx());
  archive(point.ry());
}

class Widget : public QWidget {
  Q_OBJECT
 private:
  int CoordX = -1;
  int CoordY = -1;
  bool DrawText = false;
  std::vector<QPoint> Path;
  int counter = 10000100;

 public:
  template <class Archiver>
  void serialize(Archiver &archive) {
    archive(CEREAL_NVP(CoordX));
    archive(CEREAL_NVP(CoordY));
    archive(CEREAL_NVP(DrawText));
    archive(CEREAL_NVP(Path));
    archive(CEREAL_NVP(counter));
  }

  void Render(QPaintDevice *PaintDevice);

 public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

  bool eventFilter(QObject *obj, QEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

  void paintEvent(QPaintEvent *event);

 private slots:
  void on_pushButton_clicked();

  void on_saveImageButton_clicked();

  void on_SaveJsonButton_clicked();

  void on_LoadJsonButton_clicked();

  void on_guitButton_clicked();
  void on_aboutButton_clicked();

 private:
  Ui::Widget *ui;
};
#endif  // WIDGET_H
