#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_primitives.h>
#define width 600
#define height 920
#define FPS 90.0
#define SPEED 6
typedef struct
{
	ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR rami = al_map_rgb(122, 113, 143);
	ALLEGRO_COLOR windowsblue = al_map_rgb(31, 52, 69);
	ALLEGRO_COLOR white = al_map_rgb(240, 255, 250);
}COLOURS;
typedef struct
{
	int xpos;
	int ypos;
	int radius;
	int velox;
	int veloy;
	float accex;
	float accey;
	int score;
}HERO;
enum KEYS { UP, DOWN, LEFT, RIGHT, SHIFT };

void allegro_init();
void draw_hero(HERO &hero, COLOURS renk);
void init_hero_p1(HERO &hero);
void init_hero_p2(HERO &hero);
void register_sources(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer);
void move_right(HERO &player1, bool keys[]);
void move_left(HERO &player1, bool keys[]);
void move_down(HERO &player1, bool keys[]);
void move_up(HERO &player1, bool keys[]);
void draw_area(COLOURS color);

int main()
{
	allegro_init();
	ALLEGRO_DISPLAY *display = al_create_display(width, height);
	COLOURS color;
	ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
	ALLEGRO_TIMER *timer = al_create_timer(1 / FPS);
	HERO player1; init_hero_p1(player1);
	HERO player2; init_hero_p2(player2);
	register_sources(queue, timer);
	bool keysP1[5] = { false,false, false, false, false };
	bool gameover = false;
	al_start_timer(timer);
	while (!gameover)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(queue, &ev);
		al_clear_to_color(color.white);
		draw_area(color);
		draw_hero(player1, color);
		al_flip_display();
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keysP1[UP] = true; break;
			case ALLEGRO_KEY_DOWN:
				keysP1[DOWN] = true; break;
			case ALLEGRO_KEY_LEFT:
				keysP1[LEFT] = true; break;
			case ALLEGRO_KEY_RIGHT:
				keysP1[RIGHT] = true; break;
			case ALLEGRO_KEY_LSHIFT:
				keysP1[SHIFT] = true; break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keysP1[UP] = false; break;
			case ALLEGRO_KEY_DOWN:
				keysP1[DOWN] = false; break;
			case ALLEGRO_KEY_LEFT:
				keysP1[LEFT] = false; break;
			case ALLEGRO_KEY_RIGHT:
				keysP1[RIGHT] = false; break;
			case ALLEGRO_KEY_LSHIFT:
				keysP1[SHIFT] = false; break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{

			if (keysP1[UP]) { move_up(player1, keysP1); }
			if (keysP1[DOWN]) { move_down(player1, keysP1); }
			if (keysP1[LEFT]) { move_left(player1, keysP1); }
			if (keysP1[RIGHT]) { move_right(player1, keysP1); }
			if (keysP1[SHIFT])
			{
				player1.velox = 10;
				player1.veloy = 10;
			}
			else if (!keysP1[SHIFT])
			{
				player1.velox = SPEED;
				player1.veloy = SPEED;
			}
		}
	}
}

void allegro_init()
{
	al_init();
	al_init_primitives_addon();
	al_install_keyboard();
}
void draw_hero(HERO &hero, COLOURS renk)
{
	al_draw_filled_circle(hero.xpos, hero.ypos, hero.radius, renk.windowsblue);
}
void init_hero_p1(HERO &hero)
{
	hero.xpos = width / 2;
	hero.ypos = height *0.75;
	hero.radius = 40;
	hero.velox = SPEED;
	hero.veloy = SPEED;
	hero.accex = 0;
	hero.accey = 0;
	hero.score = 0;
}
void init_hero_p2(HERO &hero)
{
	hero.xpos = width / 2;
	hero.ypos = height *0.25;
	hero.radius = 40;
	hero.velox = SPEED;
	hero.veloy = SPEED;
	hero.accex = 0;
	hero.accey = 0;
	hero.score = 0;
}
void register_sources(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer)
{
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));
}
void move_up(HERO &player1, bool keys[])
{
	player1.ypos -= keys[UP] * player1.veloy;
	if (player1.ypos <= height / 2) { player1.ypos = height / 2; }
}
void move_down(HERO &player1, bool keys[])
{
	player1.ypos += keys[DOWN] * player1.veloy;
	if (player1.ypos >= height - player1.radius) { player1.ypos = height - player1.radius; }
}
void move_left(HERO &player1, bool keys[])
{
	player1.xpos -= keys[LEFT] * player1.velox;
	if (player1.xpos <= player1.radius) { player1.xpos = player1.radius; }
}
void move_right(HERO &player1, bool keys[])
{
	player1.xpos += keys[RIGHT] * player1.velox;
	if (player1.xpos >= width - player1.radius) { player1.xpos = width - player1.radius; }
}
void draw_area(COLOURS color)
{
	al_draw_line(0, height / 2, width, height / 2, color.rami, 5);
	al_draw_circle(width / 2, height / 2, 50, color.rami, 5);
	al_draw_circle(width / 2, height, 250, color.rami, 5);
	al_draw_circle(width / 2, 0, 250, color.rami, 5);
	al_draw_rectangle(width / 2 - 120, height - 15, width / 2 + 120, height + 5, color.rami, 5);
	al_draw_rectangle(width / 2 - 120, 15, width / 2 + 120, -5, color.rami, 5);
}