#ifndef LR_BUTTON_H
#define LR_BUTTON_H
#include <QPushButton>
#include <QMouseEvent>

class LRbutton : public QPushButton
//inherits from QPushButton, to create a button with left and right click signals
{
    Q_OBJECT

public:
    LRbutton(QWidget *parent = nullptr);

signals:
    void leftClicked();
    void rightClicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // LR_BUTTON_H
