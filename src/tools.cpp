#include "tools.h"

Tools::Tools(GameStatus *status, QWidget *parent)
    : QToolBar(parent)
{
    this->status = status;
    for (int i = 0; i < tool_owned; i++)
        type.push_back(true);
    for (int i = tool_owned; i < TOOL_NUM; i++)
        type.push_back(false);

    display_tools(parent);
    upgrade_window = new Upgrade(status);
    guide_window=new Guide();
}

void Tools::display_tools(QWidget *parent)
//display the toolbar
{
    this->setMovable(false);
    this->setFloatable(false);
    setMinimumHeight(60);
    setMaximumHeight(60);
    setIconSize(QSize(40, 40));

    QWidget *leftSpacer = new QWidget(this);
    leftSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QWidget *rightSpacer = new QWidget(this);
    rightSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->addWidget(leftSpacer);
    for (int i = 0; i < TOOL_NUM; ++i)
    {
        tool_buttons[i] = new QAction(this);
        QString buttonText = QString("%1").arg(i);
        tool_buttons[i]->setText(buttonText);

        tool_buttons[i]->setIcon(QIcon(tool_paths[i]));
        if (type[i])
        {
            tool_buttons[i]->setEnabled(true);
        }
        else
        {
            tool_buttons[i]->setEnabled(false);
        }
        connect(tool_buttons[i], &QAction::triggered, this, [=]()
                { start_place_tool(parent); });
        this->addAction(tool_buttons[i]);
    }
    this->addWidget(rightSpacer);

    guide= new QAction(this);
    guide->setText("Guide");
    guide->setIcon(QIcon(":/tools/imgs/guide.png"));
    connect(guide, &QAction::triggered, this, [=]()
            {
                guide_window->change_state(); });
    this->addAction(guide);

    upgrade = new QAction(this);
    upgrade->setText("Upgrade");
    upgrade->setIcon(QIcon(":/tools/imgs/upgrade.png"));
    connect(upgrade, &QAction::triggered, this, [=]()
            {
        upgrade_window->change_state(); });
    this->addAction(upgrade);

    save_status = new QAction(this);
    save_status->setText("Save");
    save_status->setIcon(QIcon(":/tools/imgs/save.png"));
    connect(save_status, &QAction::triggered, this, [=]()
            {emit save_file(); });

    this->addAction(save_status);
}

void Tools::start_place_tool(QWidget *parent)
//set the status for placing machinery
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action && action->isEnabled())
    {
        mouseFollower = QPixmap(tool_paths[action->text().toInt()]);
        mouseFollower = mouseFollower.scaled(30, 30);
        Map::chose_tool(action->text().toInt());
        parent->setCursor(QCursor(mouseFollower));
    }
}

Tools::~Tools()
{
    for (int i = 0; i < TOOL_NUM; ++i)
    {
        delete tool_buttons[i];
    }
}
