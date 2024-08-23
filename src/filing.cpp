#include "filing.h"

// Filing::Filing()
// {
// }

Filing::Filing(Map *map, GameStatus *status, LayerMoving *moving)
{
    this->map = map;
    this->status = status;
    this->moving = moving;
}

void Filing::save_file()
{
    QString directoryPath ="./UserFile/";
    QDir().mkpath(directoryPath);
    QString fileName = QFileDialog::getSaveFileName(
        nullptr,
        "Save File",
        directoryPath,
        "Text Files (*.txt)"
        );
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Open file failed";
        return;
    }
    QTextStream out(&file);

    //save map data
    out << "Map" << Qt::endl;
    out << map->n_row << " * " << map->n_col << Qt::endl;
    out<<map->center_x<<","<<map->center_y<<","<<map->center_size<<Qt::endl;
    for (int i = 0; i < map->n_row; i++)
    {
        for (int j = 0; j < map->n_col; j++)
        {
            out  << map->map[i][j]->tool_type << "," << map->map[i][j]->tool_dir << "," << map->map[i][j]->tool_state << "," << map->map[i][j]->shape_type <<";";
        }
        out << Qt::endl;
    }
    out << Qt::endl;

    //save status data
    out << "Status" << Qt::endl;
    out << status->level << Qt::endl;
    out << status->money << Qt::endl;
    out << status->ani_rate << "," << status->cut_en << "," << status->ext_en<< Qt::endl;
    out <<status->enhanceable<<","<<status->enhanced[0]<<","<<status->enhanced[1]<<","<<status->enhanced[2]<<Qt::endl;
    out<<status->map_up_times[0]<<","<<status->map_up_times[1]<<","<<status->map_up_times[2]<<Qt::endl;
    out<<status->Levels.size()<<Qt::endl;
    for (int i = 0; i < status->Levels.size(); i++)
    {
        out<<status->Levels[i].size()<<Qt::endl;
        for (int j = 0; j < status->Levels[i].size(); j++)
        {
            out <<status->Levels[i][j].done<<",";
        }
        out << Qt::endl;
    }
    for (int i = 0; i < 5; i++)
    {
        out << status->shape_value[i]<<",";
    }
    out<<Qt::endl;
    out << Qt::endl;

    //save moving item data
    out << "Moving" << Qt::endl;
    out << moving->gen_items.size() << Qt::endl;
    for (int i = 0; i < moving->gen_items.size(); i++)
    {
        out << moving->gen_items[i].x << "," << moving->gen_items[i].y << "," << moving->gen_items[i].type << "," << moving->gen_items[i].dir << "," << moving->gen_items[i].gen_state << Qt::endl;
    }
    out << moving->moving_items.size() << Qt::endl;
    for (int i = 0; i < moving->moving_items.size(); i++)
    {
        out << moving->moving_items[i].x << "," << moving->moving_items[i].y << "," << moving->moving_items[i].type << "," << moving->moving_items[i].dir << Qt::endl;
    }
    for(int i=0;i<map->n_row;i++){
        for(int j=0;j<map->n_col;j++){
            out<<moving->map_moving[i][j]<<",";
        }
        out<<Qt::endl;
    }

    file.close();
}

void Filing::load_file(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cannot open file";
        return;
    }
    QTextStream in(&file);
    QString line;
    while (!in.atEnd())
    {
        line = in.readLine();
        if (line.startsWith("Map"))
        {
            line = in.readLine();
            QStringList mapsize = line.split("*");
            map->n_row = mapsize[0].toInt();
            map->n_col = mapsize[1].toInt();
            line = in.readLine();
            QStringList centerinfo = line.split(",");
            map->center_x = centerinfo[0].toInt();
            map->center_y = centerinfo[1].toInt();
            map->center_size = centerinfo[2].toInt();
            for (int i = 0; i < map->n_row; i++)
            {
                line = in.readLine();
                QStringList mapinfo = line.split(";");
                for (int j = 0; j < map->n_col; j++)
                {
                    QStringList toolinfo = mapinfo[j].split(",");
                    map->map[i][j]->tool_type = toolinfo[0].toInt();
                    map->map[i][j]->tool_dir = toolinfo[1].toInt();
                    map->map[i][j]->tool_state = toolinfo[2].toInt();
                    map->map[i][j]->shape_type = toolinfo[3].toInt();
                }
            }
        }
        else if (line.startsWith("Status"))
        {
            line = in.readLine();
            status->level = line.toInt();
            line = in.readLine();
            status->money = line.toInt();
            line = in.readLine();
            QStringList statusinfo = line.split(",");
            status->ani_rate = statusinfo[0].toInt();
            status->cut_en = statusinfo[1].toInt();
            status->ext_en = statusinfo[2].toInt();
            line = in.readLine();
            QStringList enhanceinfo = line.split(",");
            status->enhanceable = enhanceinfo[0].toInt();
            status->enhanced[0] = enhanceinfo[1].toInt();
            status->enhanced[1] = enhanceinfo[2].toInt();
            status->enhanced[2] = enhanceinfo[3].toInt();
            line = in.readLine();
            QStringList mapupinfo = line.split(",");
            status->map_up_times[0] = mapupinfo[0].toInt();
            status->map_up_times[1] = mapupinfo[1].toInt();
            status->map_up_times[2] = mapupinfo[2].toInt();
            line = in.readLine();
            int level_num = line.toInt();
            for (int i = 0; i <level_num ; i++)
            {
                line = in.readLine();
                int task_num = line.toInt();
                line = in.readLine();
                QStringList levelinfo = line.split(",");
                for(int j=0;j<task_num;j++){
                    status->Levels[i][j].done = levelinfo[j].toInt();
                }
            }
            line = in.readLine();
            QStringList shapeinfo = line.split(",");
            for (int i = 0; i < 5; i++)
            {
                status->shape_value[i] = shapeinfo[i].toInt();
            }
        }
        else if (line.startsWith("Moving"))
        {
            line = in.readLine();
            int gen_size = line.toInt();
            for (int i = 0; i < gen_size; i++)
            {
                line = in.readLine();
                QStringList geninfo = line.split(",");
                moving->add_gen_item(geninfo[0].toInt(),geninfo[1].toInt(),geninfo[2].toInt(),geninfo[3].toInt(),geninfo[4].toInt());
            }
            line = in.readLine();
            int moving_size = line.toInt();
            for (int i = 0; i < moving_size; i++)
            {
                line = in.readLine();
                QStringList movinginfo = line.split(",");
                moving->add_moving_item(movinginfo[0].toInt(),movinginfo[1].toInt(),movinginfo[2].toInt(),movinginfo[3].toInt());
            }
            for(int i=0;i<map->n_row;i++){
                line = in.readLine();
                QStringList movingmapinfo = line.split(",");
                for(int j=0;j<map->n_col;j++){
                    moving->map_moving[i][j] = movingmapinfo[j].toInt();
                }
            }
        }
    }
    file.close();
}
