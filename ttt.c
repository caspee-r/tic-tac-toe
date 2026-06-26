#include <stdlib.h>
#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>


#define CELL_OUTLINE_COLOR GOLD
#define HOVER_COLOR SKYBLUE
#define RING_THICKNESS 30

// GAME FINAL STATES
enum {
	DRAW         = 0,
	COMPUTER_WIN = 1,
	USER_WIN     = -1,
};

// GAME TURNS
enum {
	TTT_MAX = 69, // this is the computer
	TTT_MIN = 420,
};

typedef struct {
	int x, y;
	float vx, vy;
	float lifetime;
	Color color;
}Particle;

#define MAX_PARTICLES 20
#define PARTICLE_RADIUS 3
static Particle particles[MAX_PARTICLES] = {0};
static int particles_count = 0;

void spawn_particles(int x, int y)
{
	particles_count = 0;
	float speed = 150;
	for (int i = 0; i < MAX_PARTICLES; ++i){
		Particle p = {0};
		p.x = x;
		p.y = y;
		float angle = GetRandomValue(0, 360) * DEG2RAD;
		p.vx = cosf(angle) * speed;
		p.vy = sinf(angle) * speed;
		p.lifetime = 0.5f;
		particles[particles_count++] = p;
	}
}

void draw_o_shape(int x, int y, float radius, float thickness, Color color)
{
	Vector2 pos = (Vector2){.x = x, .y = y};
	DrawRing(pos, radius, radius - thickness, 0, 360, 0, color);
}

void draw_x_shape(int x, int y, float radius, float thickness, Color color)
{
	Vector2 topLeft     = { x - radius, y - radius };
	Vector2 bottomRight = { x + radius, y + radius };
	Vector2 topRight    = { x + radius, y - radius };
	Vector2 bottomLeft  = { x - radius, y + radius };
	DrawLineEx(topLeft, bottomRight, thickness, color);
	DrawLineEx(topRight, bottomLeft, thickness, color);
}

bool is_game_ended(int board[], int *final_value)
{

	int final_value1 = TTT_MAX * 3;
	int final_value2 = TTT_MIN * 3;
	int row1 = board[0] + board[1] + board[2];
	int row2 = board[3] + board[4] + board[5];
	int row3 = board[6] + board[7] + board[8];
	int col1 = board[0] + board[3] + board[6];
	int col2 = board[1] + board[4] + board[7];
	int col3 = board[2] + board[5] + board[8];
	int diag1 = board[0] + board[4] + board[8];
	int diag2 = board[2] + board[4] + board[6];
	if ((row1 == final_value1) ||
		(row2 == final_value1) ||
		(row3 == final_value1) ||
		(col1 == final_value1) ||
		(col2 == final_value1) ||
		(col3 == final_value1) ||
		(diag1 == final_value1) ||
		(diag2 == final_value1)) {
		*final_value = COMPUTER_WIN;
		return true;
	}else if ((row1 == final_value2) ||
			  (row2 == final_value2) ||
			  (row3 == final_value2) ||
			  (col1 == final_value2) ||
			  (col2 == final_value2) ||
			  (col3 == final_value2) ||
			  (diag1 == final_value2) ||
			  (diag2 == final_value2)){
		*final_value = USER_WIN;
		return true;
	 }
	for (int i = 0; i < 9; ++i){
		if (board[i] == 0) return false;
	}
	*final_value = DRAW;
	return true;
}

// minimax
int play_best_move(int board[], int turn, int* box)
{
	int final_value;
	int dumb_box;
	int temp_board[9] = {0};
	memcpy(temp_board, board, 9*sizeof(int));
	if (is_game_ended(temp_board, &final_value)){
		return final_value;
	}
	if (turn == TTT_MAX){
		int value = -100;
		for (int i = 0; i < 9; ++i){
			if (temp_board[i] == 0){
				temp_board[i] = TTT_MAX;
				int res = play_best_move(temp_board, TTT_MIN, &dumb_box);
				if (value < res){
					value = res;
					*box  = i;
				}
				temp_board[i] = 0;
			}
		}
		return value;
	}else {
		int value = 100;
		for (int i = 0; i < 9; ++i){
			if (temp_board[i] == 0){
				temp_board[i] = TTT_MIN;
				int res = play_best_move(temp_board, TTT_MAX, &dumb_box);
				temp_board[i] = 0;
				value = (value >= res) ? res : value;
			}
		}
		return value;
	}
}

void draw_winer_line(int is_filled[], Rectangle* board, int winer, Color color)
{
	// check the center box
	int x1,y1,x2,y2;
	if (is_filled[4] == winer){
		if (is_filled[0] == winer  && is_filled[8] == winer){
			x1 = board[0].x;
			y1 = board[0].y;
			x2 = board[8].x+board[8].width;
			y2 = board[8].y+board[8].height;
		}else if (is_filled[2] == winer  && is_filled[6] == winer){
			x1 = board[2].x+board[2].width;
			y1 = board[2].y;
			x2 = board[6].x;
			y2 = board[6].y+board[6].height;
		}else if (is_filled[1] == winer  && is_filled[7] == winer){
			x1 = board[1].x+board[1].width/2;
			y1 = board[1].y;
			x2 = board[7].x+board[7].width/2;
			y2 = board[7].y+board[7].height;
		}else {
			x1 = board[3].x;
			y1 = board[3].y+board[3].height/2;
			x2 = board[5].x + board[5].width;
			y2 = board[5].y+board[5].height/2;
		}
	}else {
		if (is_filled[0] == winer){
			if (is_filled[1] == winer && is_filled[2] == winer){
				x1 = board[0].x;
				y1 = board[0].y + board[0].height/2;
				x2 = board[2].x + board[2].width;
				y2 = y1;
			}else {
				x1 = board[0].x + board[0].width/2;
				y1 = board[0].y;
				x2 = board[6].x + board[6].width/2;
				y2 = board[6].y + board[6].height;
			}
		}else {
			if (is_filled[2] == winer){
				x1 = board[0].x + board[0].width/2;
				y1 = board[0].y;
				x2 = board[6].x + board[6].width/2;
				y2 = board[6].y + board[6].height;
			}else {
				x1 = board[6].x;
				y1 = board[6].y + board[6].height/2;
				x2 = board[8].x + board[8].width;
				y2 = y1;
			}
		}
	}
	DrawLineEx((Vector2){x1, y1}, (Vector2){x2, y2}, 10, color);
}

int main(void)
{
	int width  = 800;
	int height = 600;
	SetRandomSeed(time(NULL));
	SetTargetFPS(60);
	InitWindow(width, height, "Tic Tac Toe");
	Rectangle board[9] = {0};
	int is_filled[9]  = {0};
	int x_start = (float)width*1/20;
	int y_start = (float)height*1/5;
	float rect_width = (float)width * 1/5;
	float rect_height = (float)height * 1/4;
	float rect_thikness = 5.0f;
	int   rect_padding  = 15;
	int y_pos = y_start;
	// setup the board
	for (int y = 0; y < 3; ++y){
		int x_pos = x_start;
		for (int x = 0; x < 3; ++x){
			board[y*3+x] = (Rectangle){
				.x = x_pos,
				.y = y_pos,
				.width = rect_width,
				.height = rect_height
			};
			x_pos += rect_width-rect_thikness;
		}
		y_pos += rect_height - rect_thikness;
	}
	Vector2 oshape_padding = (Vector2){.x = 10, .y = 40};
	float space = width - x_start - (rect_width*3 + rect_thikness * 3) - oshape_padding.x;
	float oshape_radius = space*0.4f;
	Vector2 oshape_center = (Vector2){
		.x = x_start + (3*rect_width + 3*rect_thikness) + oshape_padding.x + oshape_radius,
		.y = y_start + (2*rect_height + 2*rect_thikness ) + oshape_padding.y
	};
	int box_padding = 20;
	// for hover effect
	Rectangle oshape_box = (Rectangle){
		.x      = oshape_center.x - oshape_radius - box_padding,
		.y      = oshape_center.y - oshape_radius - box_padding,
		.width  = oshape_radius*2 + box_padding*2,
		.height = oshape_radius*2 + box_padding*2
	};
	// X
	Vector2 xshape_padding =  {.x = 10, .y = 50};
	Vector2 xshape_center  =  {
		.x = oshape_center.x,
		.y = y_start + rect_height - xshape_padding.y
	};

	float xshape_radius = oshape_radius - 10;

	int xshape_box_padding = 30;
	Vector2 topLeft     = { xshape_center.x - xshape_radius, xshape_center.y - xshape_radius };
	Vector2 bottomRight = { xshape_center.x + xshape_radius, xshape_center.y + xshape_radius };
	Vector2 topRight    = { xshape_center.x + xshape_radius, xshape_center.y - xshape_radius };
	/* Vector2 bottomLeft  = { xshape_center.x - xshape_radius, xshape_center.y + xshape_radius }; */
	Rectangle xshape_box = (Rectangle){
		.x      = topLeft.x - xshape_box_padding,
		.y      = topLeft.y - xshape_box_padding,
		.width  = (topRight.x - topLeft.x) + xshape_box_padding*2,
		.height = (bottomRight.y - topRight.y) + xshape_box_padding*2
	};
	char user_symbol;
	char computer_symbol;
	bool game_started = false;
	bool game_ended = false;
	while (!WindowShouldClose()){
		Vector2 mouse_pos = GetMousePosition();
		float dt = GetFrameTime();
		// update !!
		for (int i = 0; i < particles_count; ++i){
			Particle* p = &particles[i];
			if (p->lifetime <= 0){
				particles[i] = particles[i+1];
				particles_count--;
				continue;
			}
			p->x -= p->vx * dt;
			p->y -= p->vy * dt;
			p->lifetime -= dt;
		}
		BeginDrawing();
		ClearBackground(DARKGRAY);
		for (int i = 0; i < 9; ++i){
			DrawRectangleLinesEx(board[i], rect_thikness, CELL_OUTLINE_COLOR);
		}
		if (!game_started){
			if (CheckCollisionPointRec(mouse_pos, oshape_box)){
				DrawRectangleRec(oshape_box, HOVER_COLOR);
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
					user_symbol = 'o';
					computer_symbol = 'x';
					game_started = true;
				};
			}
			draw_o_shape(oshape_center.x, oshape_center.y, oshape_radius, RING_THICKNESS, GREEN );
			if (CheckCollisionPointRec(mouse_pos, xshape_box)){
				// hover effect
				DrawRectangleRec(xshape_box, HOVER_COLOR);
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
					user_symbol = 'x';
					computer_symbol = 'o';
					game_started = true;
				};
			}
			draw_x_shape(xshape_center.x, xshape_center.y, xshape_radius, RING_THICKNESS, RED);
		}else {
			// draw x's and o's
			for (int i = 0; i < 9; ++i){
				if (is_filled[i] != 0){
					int x = board[i].x + board[i].width/2;
					int y = board[i].y + board[i].height/2;
					char symbol_to_draw = 0;
					if (is_filled[i] == TTT_MIN){
						symbol_to_draw = user_symbol;

					}else if (is_filled[i] == TTT_MAX){
						symbol_to_draw = computer_symbol;
					}
					if (symbol_to_draw == 'x'){
						draw_x_shape(x, y, rect_width/2 - rect_padding, 10, RED);
					}else if (symbol_to_draw == 'o'){
						draw_o_shape(x, y, board[i].width/2 - rect_padding, 10, GREEN);
					}
				}
			}
			if (!game_ended){
				int turn = TTT_MIN;
				for (int i = 0; i < 9; ++i){
					if (CheckCollisionPointRec(mouse_pos, board[i])){
						if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
							(is_filled[i] == 0)){
							is_filled[i] = TTT_MIN;
							spawn_particles(board[i].x+rect_width/2, board[i].y+rect_height/2);
							turn = TTT_MAX;
						}
					}
				}
				int dumb;
				if (!is_game_ended(is_filled, &dumb)){
					// computer move
					if (turn == TTT_MAX){
						int box = 0;
						play_best_move(is_filled, TTT_MAX, &box);
						is_filled[box] = TTT_MAX;
					}
				}

				for (int i = 0; i < particles_count; ++i){
					DrawCircle(particles[i].x, particles[i].y, PARTICLE_RADIUS, PURPLE);
				}
			}else {
				// play again ?
				int text_widht = MeasureText("Play Again ?", x_start);
				Rectangle text_rect = {
					.x = x_start + rect_width*3 + box_padding,
					.y = y_start + rect_height*1.5f,
					.width = text_widht,
					.height = x_start};
				if (CheckCollisionPointRec(mouse_pos, text_rect)){
					DrawRectangle(x_start + rect_width*3 + box_padding, y_start + rect_height*1.5f, text_widht, x_start, RED);
					if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
						game_started = false;
						game_ended = false;
						memset(is_filled, 0, 9*sizeof(int));
					}
				}
				DrawText("Play Again ?", x_start + rect_width*3 + box_padding, y_start + rect_height*1.5f, x_start, YELLOW);

			}

			int res;
			if (is_game_ended(is_filled, &res)){
				if (res == COMPUTER_WIN){
					draw_winer_line(is_filled, board, TTT_MAX, DARKBLUE);
					DrawText("Computer Won", x_start + rect_width, y_start - rect_height/2, x_start, YELLOW);
				}else {
					DrawText("DRAW!!", x_start + rect_width, y_start - rect_height/3, x_start, YELLOW);
				}
				game_ended = true;
			}
		}
		EndDrawing();
	}
	return 0;
}
