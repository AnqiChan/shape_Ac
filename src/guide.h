#ifndef GUIDE_H
#define GUIDE_H
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class Guide: public QWidget
//guide window to show the basic information and users' guide of the game
{
    Q_OBJECT
public:
    Guide(QWidget *parent = nullptr);
    void change_state();

private:
    bool state=false;
    QVBoxLayout *layout;
    QLabel *title;
    QLabel *basic;
    QLabel *basics[4];
    QLabel *obj_info;
    QLabel *obj_infos[2];
    QLabel *up;
    QLabel *ups[3];
};

#endif // GUIDE_H
