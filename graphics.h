#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL/SDL.h>

struct graphics_obj
{
    SDL_Surface *sprite;

    int *pos_x;
    int *pos_y;
    int draw_pos_x;
    int draw_pos_y;

    bool *active;
    bool draw_active;
};

class graphics
{
    private:
        SDL_Rect offset;
        Uint32 clear_colour;

        int list_len;

        struct obj_list
        {
            graphics_obj *obj;
            obj_list *next;
        };
        obj_list *list_head;

    public:
        SDL_Surface *screen;

        graphics(const char* caption, int res_x, int res_y, int bpp);
        int add_object(graphics_obj *obj);
        void draw(int delay);
        ~graphics();
};

#endif

