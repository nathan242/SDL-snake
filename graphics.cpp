#include "graphics.h"

graphics::graphics(const char* caption, int res_x, int res_y, int bpp)
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(res_x, res_y, bpp, SDL_HWSURFACE);
    SDL_WM_SetCaption(caption, NULL);

    // Set screen clearing colour
    clear_colour = SDL_MapRGB(screen->format, 0, 0, 0);

    list_len = 0;
    list_head = NULL;
}

int graphics::add_object(graphics_obj *obj)
{
    obj_list *list = new obj_list;
    list->obj = obj;
    list->next = NULL;

    // First
    if (list_head == NULL) {
        list_head = list;
    } else {
        // Append to list
        obj_list *list_item = list_head;
        while (list_item->next != NULL) {
            list_item = list_item->next;
        }
        list_item->next = list;
    }

    return ++list_len;
}

void graphics::draw(int delay) {
    obj_list *list = NULL;
    graphics_obj *obj = NULL;

    list = list_head;

    // Clear screen
    SDL_FillRect(screen, NULL, clear_colour);

    while (list != NULL) {
        // Get object
        obj = list->obj;

        if (*obj->active) {
            offset.x = *obj->pos_x;
            offset.y = *obj->pos_y;

            SDL_BlitSurface(obj->sprite, NULL, screen, &offset);
        }

        // Get next
        list = list->next;
    }

    SDL_Flip(screen);
    SDL_Delay(delay);
}

graphics::~graphics()
{
    if (list_head != NULL) {
        obj_list *list = NULL;
        obj_list *prev = NULL;

        list = list_head;

        while (list != NULL) {
            prev = list;
            list = list->next;
            delete prev;
        }
    }
}

