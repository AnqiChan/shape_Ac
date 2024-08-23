#ifndef CELL_H
#define CELL_H

#include <QPushButton>

#define CELL_SIZE 40

class Cell
//store the information of each cell
{
private:
public:
    int tool_type;
    int tool_dir;
    int tool_state = 0;
    int shape_type;
    Cell();
    Cell(int tool_type, int tool_dir, int shape_type);
    void update_cell(int tool_type, int tool_dir, int shape_type);
};

#endif // CELL_H
