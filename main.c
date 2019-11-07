#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "structs.h"

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");


    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");


    //al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    //al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    //al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY* disp = al_create_display(1000, 600);
    must_init(disp, "display");

    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    OBJ PlayerOBJ = {0, 500, 300, 0, 0};
    OBJ *player = &PlayerOBJ;

    al_grab_mouse(disp);


    #define KEY_SEEN     1
    #define KEY_RELEASED 2

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));


    al_start_timer(timer);
    al_set_mouse_xy(disp, player->x, player->y);
    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                if(key[ALLEGRO_KEY_ESCAPE])
                    done = true;

                player->x += player->dx;
                player->y += player->dy;

                if(player->x < 0)
                {
                    player->x *= -1;
                    player->dx *= -1;
                }
                if(player->x > 1000)
                {
                    player->x -= (player->x - 1000) * 2;
                    player->dx *= -1;
                }
                if(player->y < 0)
                {
                    player->y *= -1;
                    player->dy *= -1;
                }
                if(player->y > 600)
                {
                    player->y -= (player->y - 600) * 2;
                    player->dy *= -1;
                }

                player->dx *= 0.95;
                player->dy *= 0.95;

                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= KEY_SEEN;

                redraw = true;
                break;

            case ALLEGRO_EVENT_MOUSE_AXES:
                player->dx += event.mouse.dx * 0.05;
                player->dy += event.mouse.dy * 0.05;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(255, 255, 255));
            al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "player->x: %.1f player->y: %.1f", (double)player->x, (double)player->y);
            al_draw_filled_circle(player->x, player->y, 30, al_map_rgb(0, 255, 255));

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
