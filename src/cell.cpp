#include "cell.h"

Cell::Cell() : tool_type(0), tool_dir(0), shape_type(0),tool_state(0)
{
}

Cell::Cell(int tool_type, int tool_dir, int shape_type) : tool_type(tool_type), tool_dir(tool_dir), shape_type(shape_type)
{
    tool_state=0;
}

void Cell::update_cell(int tool_type, int tool_dir, int shape_type)
{
    this->tool_type = tool_type;
    this->tool_dir = tool_dir;
    this->shape_type = shape_type;
}
