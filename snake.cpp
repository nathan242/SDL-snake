#include "graphics.h"
#include "grid.h"
#include <SDL/SDL.h>

#define RES_X 800
#define RES_Y 600
#define BPP 32

#define BOX_SZ 10
#define MAX_SNAKE_SEGMENTS 4800
#define INITIAL_SNAKE_SEGMENTS 5
#define SNAKE_START_X 40
#define SNAKE_START_Y 30

#define SNAKE_DIRECTION_STOPPED 0
#define SNAKE_DIRECTION_DOWN 1
#define SNAKE_DIRECTION_LEFT 2
#define SNAKE_DIRECTION_RIGHT 3
#define SNAKE_DIRECTION_UP 4

SDL_Event input;

struct snake_list
{
    graphics_obj* obj;
    snake_list* next;
};

snake_list* snake_head;
snake_list* snake_tail;

void add_snake_part(graphics_obj* obj)
{
    snake_list* part = new snake_list;
    snake_list* current = new snake_list;

    part->obj = obj;
    part->next = NULL;

    if (snake_head == NULL) {
        snake_head = part;
        snake_tail = part;
    } else {
        current = snake_tail;
        while (current->next != NULL) {
            current = current->next;
        }

        current->next = part;
        snake_head = part;
    }
}

void move_snake(graphics_obj* obj)
{
    snake_list* current = snake_tail;

    while (current->next != NULL) {
        current->obj = current->next->obj;
        current = current->next;
    }

    snake_head->obj = obj;
}

void snake()
{
    // bool vars for control directions and quit event
    bool quit = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;

    int snake_direction = SNAKE_DIRECTION_DOWN;

    graphics *window = new graphics("SDL SNAKE", RES_X, RES_Y, BPP);
    grid *grid = new class grid(BOX_SZ, BOX_SZ, RES_X, RES_Y);
    grid_pos *grid_pos = NULL;

    // Game objects
    graphics_obj *snake_parts[MAX_SNAKE_SEGMENTS];

    for (int i = 0; i < INITIAL_SNAKE_SEGMENTS; i++) {
        snake_parts[i] = new graphics_obj;
    }

    // Snake
    for (int i = 0; i < INITIAL_SNAKE_SEGMENTS; i++) {
        snake_parts[i]->sprite = SDL_CreateRGBSurface(0, BOX_SZ, BOX_SZ, 32, 0, 0, 0, 0);
        snake_parts[i]->draw_pos_x = 0;
        snake_parts[i]->draw_pos_y = 0;
        snake_parts[i]->draw_active = 1;
        snake_parts[i]->pos_x = &snake_parts[i]->draw_pos_x;
        snake_parts[i]->pos_y = &snake_parts[i]->draw_pos_y;
        snake_parts[i]->active = &snake_parts[i]->draw_active;
        SDL_FillRect(snake_parts[i]->sprite, NULL, SDL_MapRGB(window->screen->format, 255, 0, 0));

        window->add_object(snake_parts[i]);

        grid->set_pos(snake_parts[i], SNAKE_START_X, SNAKE_START_Y+i);

        add_snake_part(snake_parts[i]);
    }

    // Main loop
    while (quit==false)
    {
        // Read inputs
        while (SDL_PollEvent(&input))
        {
            switch (input.type)
            {
                case SDL_KEYDOWN:
                    switch (input.key.keysym.sym)
                        {
                            case SDLK_LEFT:
                                left = true;
                                break;
                            case SDLK_RIGHT:
                                right = true;
                                break;
                            case SDLK_UP:
                                up = true;
                                break;
                            case SDLK_DOWN:
                                down = true;
                                break;
                            case SDLK_q:
                                quit = true;
                                break;
                        }
                        break;
                case SDL_KEYUP:
                    switch (input.key.keysym.sym)
                        {
                            case SDLK_LEFT:
                                left = false;
                                break;
                            case SDLK_RIGHT:
                                right = false;
                                break;
                            case SDLK_UP:
                                up = false;
                                break;
                            case SDLK_DOWN:
                                down = false;
                                break;
                        }
            }
        }

        if (snake_direction != SNAKE_DIRECTION_STOPPED) {
            if (down && snake_direction != SNAKE_DIRECTION_UP) { snake_direction = SNAKE_DIRECTION_DOWN; }
            if (left && snake_direction != SNAKE_DIRECTION_RIGHT) { snake_direction = SNAKE_DIRECTION_LEFT; }
            if (right && snake_direction != SNAKE_DIRECTION_LEFT) { snake_direction = SNAKE_DIRECTION_RIGHT; }
            if (up && snake_direction != SNAKE_DIRECTION_DOWN) { snake_direction = SNAKE_DIRECTION_UP; }

            grid_pos = grid->get_pos(snake_head->obj);
        }

        switch (snake_direction) {
            case SNAKE_DIRECTION_DOWN:
                //grid->set_pos(snake_tail->obj, grid_pos->x, grid_pos->y+1); 
                grid_pos->y++;
                break;

            case SNAKE_DIRECTION_LEFT:
                //grid->set_pos(snake_tail->obj, grid_pos->x-1, grid_pos->y);
                grid_pos->x--;
                break;

            case SNAKE_DIRECTION_RIGHT:
                //grid->set_pos(snake_tail->obj, grid_pos->x+1, grid_pos->y);
                grid_pos->x++;
                break;

            case SNAKE_DIRECTION_UP:
                //grid->set_pos(snake_tail->obj, grid_pos->x, grid_pos->y-1);
                grid_pos->y--;
                break;

        }

        if (grid->pos_inside(grid_pos->x, grid_pos->y)) {
            grid->set_pos(snake_tail->obj, grid_pos->x, grid_pos->y);
            move_snake(snake_tail->obj);
        } else {
            snake_direction = SNAKE_DIRECTION_STOPPED;
        }

        // Redraw screen
        window->draw(200);
    }

    SDL_Quit();

    delete window;

    return;
}

int main (int argc, char *argv[])
{
    snake();
    return 0;
}

