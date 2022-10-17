#ifndef GRID_H
#define GRID_H

#include "graphics.h"

struct grid_pos
{
    int x;
    int y;
};

class grid
{
    private:
        int box_sz_x;
        int box_sz_y;
        int res_x;
        int res_y;
        int grid_sz_x;
        int grid_sz_y;

    public:
        grid(int box_sz_x, int box_sz_y, int res_x, int res_y);
        void set_pos(graphics_obj* obj, int x, int y);
        grid_pos* get_pos(graphics_obj* obj);
        bool pos_inside(int x, int y);
        int get_grid_sz_x();
        int get_grid_sz_y();
};

#endif

