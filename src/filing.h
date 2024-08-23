#ifndef FILING_H
#define FILING_H

#include <QFile>
#include <QObject>
#include <QDir>
#include <QFileDialog>
#include "create_map.h"
#include "game_status.h"
#include "layer_moving.h"

class Filing : public QObject
//to save and load the user files
{
    Q_OBJECT
public:
    //Filing();
    Filing(Map *map, GameStatus *status, LayerMoving *moving);
public slots:
    void save_file();
    void load_file(QString fileName);

private:
    Map *map;
    GameStatus *status;
    LayerMoving *moving;
};

#endif // FILING_H
