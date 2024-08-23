#include "lr_button.h"

LRbutton::LRbutton(QWidget *parent) : QPushButton(parent) {}

void LRbutton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit leftClicked();
    }
    else if (event->button() == Qt::RightButton)
    {
        emit rightClicked();
    }
    // Call the base class implementation
    QPushButton::mousePressEvent(event);
}
