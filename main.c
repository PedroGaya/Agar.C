#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "structs.h"
#include <math.h>

float sigmoid(float x) {

	return x / (1 + abs(x));
}

void must_init(bool test, const char *description) {
	if (test)
		return;

	printf("couldn't initialize %s\n", description);
	exit(1);
}

int main() {
	must_init(al_init(), "allegro");
	must_init(al_install_keyboard(), "keyboard");
	must_init(al_install_mouse(), "mouse");

	ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0); // Framerate (curr. 60 FPS)
	must_init(timer, "timer");

	ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
	must_init(queue, "queue");

	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST); // Screen size
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	ALLEGRO_DISPLAY *disp = al_create_display(1000, 600); // Screen size
	must_init(disp, "display");

	ALLEGRO_FONT *font = al_create_builtin_font();
	must_init(font, "font");

	must_init(al_init_primitives_addon(), "primitives");

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_mouse_event_source());

	bool done = false;
	bool redraw = true;
	ALLEGRO_EVENT event;

	al_grab_mouse(disp);

	OBJ loaded[100] = { }; // Array with all loaded objs.

	OBJ PlayerOBJ = { 'P', 500, 300, 0, 0, 30, 1, 0 }; // Placeholder player_obj.
	OBJ CameraOBJ = { 'C', 0, 0, 0, 0, 0, 0, 0 };     // Placeholder camera_obj.
	OBJ FoodOBJ1 = { 'f', 800, 200, 0, 0, 50, 1, 0 };  // Placeholder food_objs.
	OBJ FoodOBJ2 = { 'f', 900, 400, 0, 0, 20, 1, 0 };
	OBJ FoodOBJ3 = { 'f', 300, 500, 0, 0, 25, 1, 0 };
	OBJ FoodOBJ4 = { 'f', 100, 400, 0, 0, 15, 1, 0 };

	// Adding objs to be loaded to loaded_Array.
	loaded[0] = PlayerOBJ;
	loaded[1] = CameraOBJ;
	loaded[2] = FoodOBJ1;
	loaded[3] = FoodOBJ2;
	loaded[4] = FoodOBJ3;
	loaded[5] = FoodOBJ4;

	// Function to add objs to array.
//	void load_array(OBJ *array, int size) {
//		for (int i = 0; i < size; i++) {
//			switch (i) {
//			case 0:
//				//Player
//				break;
//			case 1:
//				//Camera
//				break;
////			case 2 ... 52:
//				//Food
//				break;
//			case 53 ... 100:
//				//stuff
//				break;
//			default:
//				//stuff
//
//			}
//		}
//	}

#define KEY_SEEN     1
#define KEY_RELEASED 2

	unsigned char key[ALLEGRO_KEY_MAX];
	memset(key, 0, sizeof(key));

	al_start_timer(timer);
	al_set_mouse_xy(disp, loaded[0].x, loaded[0].y);
	while (1) {
		al_wait_for_event(queue, &event);

		switch (event.type) {
		case ALLEGRO_EVENT_TIMER: // Things in this case run every frame
			if (key[ALLEGRO_KEY_ESCAPE])
				done = true;

			// Speed increment for camera
			loaded[1].x = loaded[0].x - 500;
			loaded[1].y = loaded[0].y - 300;

			// Speed increment for player
			loaded[0].x += loaded[0].dx;
			loaded[0].y += loaded[0].dy;

//                // Border Check
//                if(loaded[0].x < 0)
//                {
//                    loaded[0].x = 0;
//                    //loaded[0].dx *= -1;
//                }
//                if(loaded[0].x > 10000)
//                {
//                    loaded[0].x = 10000;
//                    //loaded[0].dx *= -1;
//                }
//                if(loaded[0].y < 0)
//                {
//                    loaded[0].y = 0;
//                    //loaded[0].dy *= -1;
//                }
//                if(loaded[0].y > 10000)
//                {
//                    loaded[0].y = 10000;
//                    //loaded[0].dy *= -1;
//                }

			// Stores what keys were pressed
			for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
				key[i] &= KEY_SEEN;

			// Collision check
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < 100; j++) {
					if (i != 1 && j != 1 && loaded[i].size > loaded[j].size
							&& loaded[i].render == 1 && loaded[j].render == 1
							&& abs(loaded[i].x - loaded[j].x) < loaded[i].size
							&& abs(loaded[i].y - loaded[j].y) < loaded[i].size
							&& i != j) {
						loaded[j].render = 0;
						loaded[i].size += 3;
						loaded[i].score += loaded[j].size * 10;
					}
				}
			}

			redraw = true;
			break;

		case ALLEGRO_EVENT_MOUSE_AXES: // Defines mouse movement for player.
			loaded[0].dx = (event.mouse.x - (loaded[0].x - loaded[1].x)) * 0.02;
			loaded[0].dy = (event.mouse.y - (loaded[0].y - loaded[1].y)) * 0.02;
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

		if (done)
			break;

		// Drawing function.
		if (redraw && al_is_event_queue_empty(queue)) {
			al_clear_to_color(al_map_rgb(255, 255, 255));

			for (int i = 0; i < 100; i++) { // Draws objects depending on their ID and position.
				switch (loaded[i].id) {
				case ('f'):
					if (loaded[i].render)
						al_draw_filled_circle(loaded[i].x - loaded[1].x,
								loaded[i].y - loaded[1].y, loaded[i].size,
								al_map_rgb(255, 0, 0));
					break;
				case ('P'):
					if (loaded[i].render)
						al_draw_filled_circle(loaded[i].x - loaded[1].x,
								loaded[i].y - loaded[1].y, loaded[i].size,
								al_map_rgb(0, 255, 255));
					break;
				}
			}

			al_draw_textf(font, al_map_rgb(0, 0, 0), 10, 0, 0, "Score: %d",
					loaded[0].score);
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
