#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#define width 800
#define height 600

enum KEYS { UP, DOWN, LEFT, RIGHT };

int main()
{
	al_init();
	ALLEGRO_DISPLAY *display = al_create_display(width, height);
	ALLEGRO_FONT *font31 = al_load_font("roboto.ttf", 31, 0);
	ALLEGRO_EVENT_QUEUE *kuyruk = al_create_event_queue(); 
	bool done = false;
	int pos_x = width / 2, pos_y = height / 2;
	bool keys[4] = { false, false, false ,false };


	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();
	al_register_event_source(kuyruk, al_get_keyboard_event_source());
	al_register_event_source(kuyruk, al_get_display_event_source(display));
	al_register_event_source(kuyruk, al_get_mouse_event_source());

	/*al_clear_to_color(al_map_rgb(31, 52, 69));
	al_draw_text(font31, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "salam");*/

	al_hide_mouse_cursor(display);

	while (!done)
	{
		ALLEGRO_EVENT olay;
		al_wait_for_event(kuyruk, &olay);

		if (olay.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (olay.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			}
		}
		else if (olay.type == ALLEGRO_EVENT_KEY_UP)
		{
			if (olay.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				done = true;
			}
			switch (olay.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			}
		}
		else if (olay.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (olay.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (olay.mouse.button & 2)
			{
				done = true;
			}
		}
		else if (olay.type == ALLEGRO_EVENT_MOUSE_AXES);
		{
			pos_x = olay.mouse.x;
			pos_y = olay.mouse.y;
		}
		

		pos_y -= keys[UP] * 10;
		pos_y += keys[DOWN] * 10;
		pos_x -= keys[LEFT] * 10;
		pos_x += keys[RIGHT] * 10;


		al_clear_to_color(al_map_rgb(31, 52, 69));
		al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, al_map_rgb(255, 255, 255));
		al_flip_display();
	}



	
	al_destroy_display(display);
	return 0;
}