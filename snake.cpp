#include "graphics.h"
#include "grid.h"
#include <SDL/SDL.h>
#include <time.h>

#define RES_X 800
#define RES_Y 600
#define BPP 32

#define BOX_SZ 10
#define MAX_SNAKE_SEGMENTS 4800
#define INITIAL_SNAKE_SEGMENTS 3
#define SNAKE_START_X 40
#define SNAKE_START_Y 30
#define SNAKE_MOVE_DELAY 20

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

grid *grid = new class grid(BOX_SZ, BOX_SZ, RES_X, RES_Y);
int snake_part_count = 0;
int snake_delay_count = 0;

void add_snake_part(graphics_obj* obj)
{
    snake_list* part = new snake_list;
    snake_list* current;

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

    snake_part_count++;
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

bool snake_is_at(int x, int y, snake_list* start = NULL)
{
    grid_pos *pos;
    snake_list* current;

    if (start != NULL) {
        current = start;
    } else {
        current = snake_tail;
    }

    do {
        pos = grid->get_pos(current->obj);
        if (pos->x == x && pos->y == y) { return true; }

        current = current->next;
    } while (current != NULL);

    return false;
}

void init_snake(graphics *window, graphics_obj *snake_parts[], SDL_Surface* sprite, int length, int start_x, int start_y)
{
    for (int i = 0; i < length; i++) {
        snake_parts[i] = new graphics_obj;
        snake_parts[i]->sprite = sprite;
        snake_parts[i]->draw_pos_x = 0;
        snake_parts[i]->draw_pos_y = 0;
        snake_parts[i]->draw_active = 1;
        snake_parts[i]->pos_x = &snake_parts[i]->draw_pos_x;
        snake_parts[i]->pos_y = &snake_parts[i]->draw_pos_y;
        snake_parts[i]->active = &snake_parts[i]->draw_active;

        window->add_object(snake_parts[i]);

        grid->set_pos(snake_parts[i], start_x, start_y+i);

        add_snake_part(snake_parts[i]);
    }
}

graphics_obj *add_snake_food(graphics *window, graphics_obj *snake_parts[], SDL_Surface* sprite, int pos_x, int pos_y)
{
    snake_parts[snake_part_count] = new graphics_obj;
    snake_parts[snake_part_count]->sprite = sprite;
    snake_parts[snake_part_count]->draw_pos_x = 0;
    snake_parts[snake_part_count]->draw_pos_y = 0;
    snake_parts[snake_part_count]->draw_active = 1;
    snake_parts[snake_part_count]->pos_x = &snake_parts[snake_part_count]->draw_pos_x;
    snake_parts[snake_part_count]->pos_y = &snake_parts[snake_part_count]->draw_pos_y;
    snake_parts[snake_part_count]->active = &snake_parts[snake_part_count]->draw_active;

    window->add_object(snake_parts[snake_part_count]);

    grid->set_pos(snake_parts[snake_part_count], pos_x, pos_y);

    return snake_parts[snake_part_count++];
}

graphics_obj *add_rand_snake_food(graphics *window, graphics_obj *snake_parts[], SDL_Surface* sprite)
{
    return add_snake_food(window, snake_parts, sprite, rand() % grid->get_grid_sz_x(), rand() % grid->get_grid_sz_y());
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
    grid_pos *pos;
    grid_pos *food_pos;

    // Game objects
    graphics_obj *snake_parts[MAX_SNAKE_SEGMENTS];
    graphics_obj *snake_food;

    SDL_Surface *snake_sprite = SDL_CreateRGBSurface(0, BOX_SZ, BOX_SZ, 32, 0, 0, 0, 0);
    SDL_Surface *snake_food_sprite = SDL_CreateRGBSurface(0, BOX_SZ, BOX_SZ, 32, 0, 0, 0, 0);
    SDL_FillRect(snake_sprite, NULL, SDL_MapRGB(window->screen->format, 0, 255, 0));
    SDL_FillRect(snake_food_sprite, NULL, SDL_MapRGB(window->screen->format, 255, 0, 0));

    // Snake
    init_snake(window, snake_parts, snake_sprite, INITIAL_SNAKE_SEGMENTS, SNAKE_START_X, SNAKE_START_Y);

    // Init random number generator
    srand(time(NULL));

    // Add first snake food
    snake_food = add_rand_snake_food(window, snake_parts, snake_food_sprite);
    food_pos = grid->get_pos(snake_food);

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

            pos = grid->get_pos(snake_head->obj);
        }

        if (snake_delay_count == SNAKE_MOVE_DELAY) {
            snake_delay_count = 0;

            switch (snake_direction) {
                case SNAKE_DIRECTION_DOWN:
                    pos->y++;
                    break;

                case SNAKE_DIRECTION_LEFT:
                    pos->x--;
                    break;

                case SNAKE_DIRECTION_RIGHT:
                    pos->x++;
                    break;

                case SNAKE_DIRECTION_UP:
                    pos->y--;
                    break;
            }

            if (pos->x == food_pos->x && pos->y == food_pos->y) {
                snake_food->sprite = snake_sprite;
                add_snake_part(snake_food);
                snake_food = add_rand_snake_food(window, snake_parts, snake_food_sprite);
                food_pos = grid->get_pos(snake_food);
            } else if (grid->pos_inside(pos->x, pos->y) && !snake_is_at(pos->x, pos->y, snake_tail->next)) {
                grid->set_pos(snake_tail->obj, pos->x, pos->y);
                move_snake(snake_tail->obj);
            } else {
                snake_direction = SNAKE_DIRECTION_STOPPED;
            }
        } else {
            snake_delay_count++;
        }

        // Redraw screen
        window->draw(2);
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

