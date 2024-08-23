#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QScrollBar>
#include <QStackedLayout>
#include <QTimer>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>

#include "tools.h"
#include "create_map.h"
#include "layer_object.h"
#include "layer_machinery.h"
#include "layer_moving.h"
#include "layer_info.h"
#include "game_status.h"
#include "filing.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void set_start();
    void set_game(Tools *tools, Map *map);
    QMediaPlayer *music;
    QAudioOutput *audio;

private slots:
    void start();
    void load();
    void update_clk(int r);
    void larger_map();
    void get_bg();

signals:
    void check();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QPushButton *startButton;
    QPushButton *loadButton;
    QStackedLayout *Layers;
    QWidget *layered;
    QScrollArea *scrollArea;
    QLabel *bg;
    QLabel *sign;
    LayerObject *layer_obj;
    LayerMoving *layer_moving;
    LayerMachinery *layer_machinery;
    LayerInfo *layer_info;
    Map *map;
    Tools *tools;
    GameStatus *status;
    Filing *filer;

    QTimer *timer1;

    int clk_ori = 2000;
};
#endif // MAINWINDOW_H
