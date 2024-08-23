#ifndef TOOLS_H
#define TOOLS_H

#include <QToolBar>
#include <QAction>
#include <vector>
#include <QToolButton>
#include <QGraphicsEffect>
#include <QMessageBox>
#include <QLabel>
#include <QAction>
#include <QMouseEvent>
#include "upgrade.h"
#include "guide.h"
#include "game_status.h"
#include "create_map.h"

#define TOOL_NUM 5

class Tools : public QToolBar
//manages the tools that the user can use
{
    Q_OBJECT
private:
    std::vector<bool> type;
    std::vector<QString> tool_paths = {
        ":/tools/imgs/conveyor.png",
        ":/tools/imgs/miner.png",
        ":/tools/imgs/trash.png",
        ":/tools/imgs/cutter.png",
        ":/tools/imgs/spinner.png",
    };
    QAction *tool_buttons[TOOL_NUM];
    QAction *guide;
    QAction *upgrade;
    QAction *save_status;
    QPixmap mouseFollower;
    int tool_owned = 5;
    GameStatus *status;


public:
    Tools(GameStatus *status, QWidget *parent = nullptr);
    ~Tools();
    Guide *guide_window;
    Upgrade *upgrade_window;
    void display_tools(QWidget *parent = nullptr);
private slots:
    void start_place_tool(QWidget *parent);

signals:
    void save_file();
};

#endif // TOOLS_H
