#include <math.h>
#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#define width 600
#define height 920
#define FPS 90.0
#define SPEED 6
typedef struct
{
	ALLEGRO_COLOR black ;
	ALLEGRO_COLOR rami ;
	ALLEGRO_COLOR windowsblue ;
	ALLEGRO_COLOR white ;
	ALLEGRO_COLOR ball;
}COLOURS;
typedef struct
{
	double xpos;
	double ypos;
	double radius;
	double velox;
	double veloy;
	double accex;
	double accey;
	int score;
}HERO;
typedef struct
{
	double xpos;
	double ypos;
	double radius;
	double velox;
	double veloy;
	double collisionx;
	double collisiony;
	double accex;
	double accey;
}BALL;
enum KEYS { UP, DOWN, LEFT, RIGHT, SHIFT };

void allegro_init();
void color_init(COLOURS *renk); //done
void draw_hero(HERO &hero, COLOURS renk);
void init_hero_p1(HERO &hero);
void init_hero_p2(HERO &hero);
void init_puck(BALL &puck);
void register_sources(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer);
void move_right(HERO &player1, bool keys[]);
void move_left(HERO &player1, bool keys[]);
void move_down(HERO &player1, bool keys[]);
void move_up(HERO &player1, bool keys[]);
void draw_area(COLOURS color);
void player2_bot(HERO *player, BALL *puck);//done
void make_collision(HERO *player1, BALL *puck);//done
void main_menu(bool& menu, ALLEGRO_FONT *font, COLOURS color, ALLEGRO_EVENT_QUEUE *queue, bool& stop, bool& gameover);//done
void score_table(ALLEGRO_FONT *font, HERO *player1, HERO *player2);//done
void scored(BALL *puck);
int main()
{
	allegro_init();
	ALLEGRO_DISPLAY *display = al_create_display(width, height);
RESTART:
	COLOURS color;
	color_init(&color);
	ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
	ALLEGRO_TIMER *timer = al_create_timer(1 / FPS);
	HERO player1; init_hero_p1(player1);
	HERO player2; init_hero_p2(player2);
	BALL puck; init_puck(puck);
	register_sources(queue, timer);
	bool keysP1[5] = { false,false, false, false, false };
	bool gameover = false;
	bool menu = true;
	bool stop = false;
	al_start_timer(timer);


	ALLEGRO_FONT *font = al_load_font("font.ttf", 50, 0);
	ALLEGRO_FONT *font_small = al_load_font("font.ttf", 40, 0);

	while (!gameover)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(queue, &ev);
		al_clear_to_color(color.white);
		if (player1.score == 6)
		{
			al_draw_text(font, color.rami, 300, 460, 1, "PLAYER 1 WINS!!!");
			al_flip_display();
			al_rest(5.00);
			goto RESTART;
		}
		else if (player2.score == 6)
		{
			al_draw_text(font, color.rami, 300, 460, 1, "PLAYER 2 WINS!!!");
			al_flip_display();
			al_rest(5.00);
			goto RESTART;
		}
		main_menu(menu, font, color, queue, stop, gameover);
		draw_area(color);
		draw_hero(player1, color);
		draw_hero(player2, color);
		al_draw_filled_circle(puck.xpos, puck.ypos, puck.radius, color.ball);
		score_table(font_small, &player1, &player2);
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
			case ALLEGRO_KEY_ESCAPE:
			{
				menu = true;
				stop = true;
				al_stop_timer(timer);
				main_menu(menu, font, color, queue, stop, gameover);
				if (!stop) { goto RESTART; }
				al_resume_timer(timer);
			}

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
		make_collision(&player1, &puck);
		make_collision(&player2, &puck);
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			if (puck.xpos >= width - puck.radius || puck.xpos <= puck.radius)
			{
				if (puck.xpos > width / 2) { puck.xpos = width - puck.radius; }
				else if (puck.xpos < width / 2) { puck.xpos = puck.radius; }
				puck.velox *= -1;
			}
			if (puck.ypos <= puck.radius || puck.ypos >= height - puck.radius)
			{
				if (puck.ypos > height / 2) { puck.ypos = height - puck.radius; }
				else if (puck.ypos < height / 2) { puck.ypos = puck.radius; }
				puck.veloy *= -1;
			}
			puck.xpos += puck.velox;
			puck.ypos += puck.veloy;
			if (puck.veloy > 0) { puck.veloy -= puck.accey; }
			else if (puck.veloy < 0) { puck.veloy += puck.accey; }
			if (puck.velox < 0) { puck.velox += puck.accex; }
			else if (puck.velox > 0) { puck.velox -= puck.accex; }
			player2_bot(&player2, &puck); //player 2 bot
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
			if (puck.xpos > width / 2 - 120 && puck.ypos > height - puck.radius && puck.xpos < width / 2 + 120) { player2.score++; scored(&puck); }
			if (puck.xpos > width / 2 - 120 && puck.ypos < puck.radius && puck.xpos < width / 2 + 120) { player1.score++; scored(&puck); }
		}
	}
}

void allegro_init()
{
	al_init();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();
	al_init_font_addon();
	al_init_ttf_addon();
}
void color_init(COLOURS *renk)
{
	renk->black = al_map_rgb(0, 0, 0);
	renk->rami = al_map_rgb(122, 113, 143);
	renk->windowsblue = al_map_rgb(31, 52, 69);
	renk->white = al_map_rgb(240, 255, 250);
	renk->ball = al_map_rgb(6, 12, 255);
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
	hero.ypos = height *0.15;
	hero.radius = 40;
	hero.velox = 5;
	hero.veloy = 5;
	hero.accex = 0;
	hero.accey = 0;
	hero.score = 0;
}
void init_puck(BALL &puck)
{
	puck.xpos = width / 2;
	puck.ypos = height / 2;
	puck.velox = 0;
	puck.veloy = 0;
	puck.radius = 30;
	puck.collisionx = 0;
	puck.collisiony = 0;
	puck.accex = 0.01;
	puck.accey = 0.01;
}
void register_sources(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer)
{
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_mouse_event_source());
}
void move_up(HERO &player1, bool keys[])
{
	player1.ypos -= keys[UP] * player1.veloy;
	if (player1.ypos <= height / 2 + player1.radius) { player1.ypos = height / 2 + player1.radius; }
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
void player2_bot(HERO *player, BALL *puck)
{


	if (player->ypos > height / 2 - player->radius)
	{
		player->ypos = height / 2 - player->radius;
	}
	else if (pow(pow(player->xpos - puck->xpos, 2) + pow(player->ypos - puck->ypos, 2), 0.5) < 120 && puck->ypos > player->ypos)
	{
		player->ypos += player->veloy;
		if (player->ypos > height * 0.3)
		{
			player->ypos = height * 0.3;
		}
	}

	else
	{
		if (player->ypos <= 150)
		{
			player->ypos = 150;
		}
		else
		{
			player->ypos -= player->veloy;
		}
	}


	if (puck->xpos > player->xpos + 100 && player->ypos < puck->ypos + 5)
	{
		player->xpos += player->velox;
	}
	else if (puck->xpos < player->xpos - 100 && player->ypos < puck->ypos + 5)
	{
		player->xpos -= player->velox;
	}


}
void make_collision(HERO *player1, BALL *puck)
{
	if (pow(pow(player1->xpos - puck->xpos, 2) + pow(player1->ypos - puck->ypos, 2), 0.5) <= (player1->radius + puck->radius))
	{
		double collisionPointX = ((player1->xpos * puck->radius) + (puck->xpos * player1->radius)) / (player1->radius + puck->radius);
		double collisionPointY = ((player1->ypos * puck->radius) + (puck->ypos * player1->radius)) / (player1->radius + puck->radius);
		double vector = sqrt(pow(player1->velox, 2) + pow(player1->veloy, 2));
		double cosa = (player1->xpos - puck->xpos) / (player1->radius + puck->radius);
		double sina = (player1->ypos - puck->ypos) / (player1->radius + puck->radius);
		puck->velox = -vector*cosa;
		puck->veloy = -vector*sina;
		puck->xpos += puck->velox;
		puck->ypos += puck->veloy;
	}
}
void main_menu(bool& menu, ALLEGRO_FONT *font, COLOURS color, ALLEGRO_EVENT_QUEUE *queue, bool& stop, bool& gameover)
{
	while (menu)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(queue, &ev);
		al_clear_to_color(al_map_rgb(255, 255, 255));
		if (!stop)
		{
			al_draw_text(font, color.rami, width / 2, height / 2 - 200, ALLEGRO_ALIGN_CENTRE, "Start");
			al_draw_text(font, color.rami, width / 2, height / 2, ALLEGRO_ALIGN_CENTRE, "Options");
			al_draw_text(font, color.rami, width / 2, height / 2 + 200, ALLEGRO_ALIGN_CENTRE, "Exit");
		}
		else if (stop)
		{
			al_draw_text(font, color.rami, width / 2, height / 2 - 400, ALLEGRO_ALIGN_CENTRE, "New Game");
			al_draw_text(font, color.rami, width / 2, height / 2 - 200, ALLEGRO_ALIGN_CENTRE, "Resume Game");
			al_draw_text(font, color.rami, width / 2, height / 2, ALLEGRO_ALIGN_CENTRE, "Options");
			al_draw_text(font, color.rami, width / 2, height / 2 + 200, ALLEGRO_ALIGN_CENTRE, "Exit");
		}
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (ev.mouse.y >= height / 2 - 200 && ev.mouse.y < height / 2)
			{
				menu = false;
			}
			else if (ev.mouse.y >= height / 2 - 400 && ev.mouse.y < height / 2 - 200)
			{
				menu = false;
				stop = false;
			}
			else if (ev.mouse.y >= height / 2 - 200 && ev.mouse.y < height / 2)
			{
				//menu = false;
				//stop = false;
			}
			else if (ev.mouse.y >= height / 2 + 200 && ev.mouse.y < height / 2 + 400)
			{
				gameover = true;
				menu = false;
			}
		}
		al_flip_display();
	}
}
void score_table(ALLEGRO_FONT *font, HERO *player1, HERO *player2)
{
	al_draw_textf(font, al_map_rgb(162, 162, 162), 540, 410, 0, "%d ", player2->score);
	al_draw_textf(font, al_map_rgb(162, 162, 162), 540, 460, 0, "%d", player1->score);
}
void scored(BALL *puck)
{
	puck->xpos = width / 2;
	puck->ypos = height / 2;
	puck->velox = 0;
	puck->veloy = 0;
}
