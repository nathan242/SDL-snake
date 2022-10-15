#include "grid.h"

grid::grid(int box_sz_x, int box_sz_y, int res_x, int res_y)
{
    this->box_sz_x = box_sz_x;
    this->box_sz_y = box_sz_y;
    this->res_x = res_x;
    this->res_y = res_y;

    this->grid_sz_x = res_x/box_sz_x;
    this->grid_sz_y = res_y/box_sz_y;
}

void grid::set_pos(graphics_obj* obj, int x, int y)
{
    *obj->pos_x = x*box_sz_x;
    *obj->pos_y = y*box_sz_y;
}

grid_pos* grid::get_pos(graphics_obj* obj)
{
    grid_pos * pos = new grid_pos;

    pos->x = *obj->pos_x/box_sz_x;
    pos->y = *obj->pos_y/box_sz_y;

    return pos;
}

bool grid::pos_inside(int x, int y)
{
    return x >= 0 && x < grid_sz_x && y >= 0 && y < grid_sz_y;
}

