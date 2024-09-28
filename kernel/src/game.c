#include "game.h"
#include "kernel_glbl.h"
#include "keyboard.h"
#include "print.h"
#include "random.h"
#include "keyboard_map.inl.h"

#define POSITION(X, Y) ((COLUMNS_IN_LINE * Y) + X)

#define WIDTH() 80
#define HEIGHT() 25

static int IsInPlay = 0;

static int random_number_not_zero_between(int begin, int end) {
  int number = Krnl_rand();
  const int range = end - begin;
  number %= range;

  number += begin;
  number >= 0 ? ++number : (void)0;
  return number;
}

// 80 x 25 screen, floats are impractical
struct Vec2int {
  int x;
  int y;
};

enum Direction {
  DIRECTION_UP,
  DIRECTION_RIGHT,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
};

struct Player {
  struct Vec2int position;
  struct Vec2int velocity;
  struct Vec2int size;
};
struct Ball {
  struct Vec2int position;
  struct Vec2int velocity;
  struct Vec2int size;
};
struct Control {
  enum Direction p1_input;
  enum Direction p2_input;
};
struct Game {
  struct Player p1;
  struct Player p2;
  struct Ball ball;
  struct Control control;
};

static void draw_player(struct Player const* player) {
  for (int i = player->position.y; i < player->position.y + player->size.y; ++i){
    Krnl_print_at(player->position.x, i, 'H');
  }
}

static void draw_ball(struct Ball const* ball) {
  Krnl_print_at(ball->position.x, ball->position.y, 'o');
}

static void render_game(struct Game const* game) {
  draw_player(&game->p1);
  draw_player(&game->p2);
  draw_ball(&game->ball);
}

#define SPEED() 1
static void update_player(struct Player* player) {
  player->position.x += player->velocity.x;
  player->position.y += player->velocity.y;
}

static void update_ball(struct Ball* ball) {}

static void player_to_ball_collision(struct Player* player, struct Ball* ball) {
  if ((((ball->position.x + ball->size.x) == player->position.x) ||
        (ball->position.x == player->position.x + player->size.x))
        && ((ball->position.y + ball->size.y >= player->position.y) &&
            (ball->position.y <= player->position.y + player->size.y))) {
    ball->velocity.x = -ball->velocity.x;
    ball->velocity.y = -ball->velocity.y;
  }
}

static void check_collision(struct Game* game) {
  player_to_ball_collision(&game->p1, &game->ball);
  player_to_ball_collision(&game->p2, &game->ball);
}

static void poll_input(struct Game* game) {
  struct Player* p1 = &game->p1;
  struct Player* p2 = &game->p2;

  int x = 0, y = 0;

  if (Krnl_is_key_pressed('w')) {
    y -= SPEED();
  }
  if (Krnl_is_key_pressed('s')) {
    y += SPEED();
  }

  p1->velocity.x = x;
  p1->velocity.y = y;

  x = y = 0;

  if (Krnl_is_key_pressed('i')) {
    y -= SPEED();
  }
  if (Krnl_is_key_pressed('k')) {
    y += SPEED();
  }

  p2->velocity.x = x;
  p2->velocity.y = y;
}

static void update_game(struct Game* game) {
  poll_input(game);

  update_player(&game->p1);
  update_player(&game->p2);

  check_collision(game);

  update_ball(&game->ball);
}

void Krnl_begin_game(void) {
  // Krnl_init_random_seed();

#define CREATE_PLAYER(X, Y) (struct Player) {        \
    .position = (struct Vec2int) {                   \
      .x = X,                                        \
      .y= Y,                                         \
    },                                               \
    .velocity = {                                    \
      .x = 0,                                        \
      .y = 0                                         \
    },                                               \
    .size = (struct Vec2int) {                       \
      .x = 1,                                        \
      .y = 3,                                        \
    },                                               \
  }

  // struct Player p1 = CREATE_PLAYER(0, Krnl_ScreenHeight / 2);
  // struct Player p2 = CREATE_PLAYER(Krnl_ScreenWidth - 1, Krnl_ScreenHeight / 2);
  // struct Ball ball = (struct Ball) {
  //   .x = Krnl_ScreenWidth / 2,
  //   .y = Krnl_ScreenHeight / 2,
  //   .velocity = (struct Vec2int) {
  //     .x = random_number_not_zero_between(-5, 5),
  //     .y = random_number_not_zero_between(-5, 5)
  //   }
  // };

  struct Game game = {
    .p1 = CREATE_PLAYER(1, HEIGHT() / 2),
    .p2 = CREATE_PLAYER(WIDTH() - 1, HEIGHT() / 2),
    .ball = (struct Ball) {
      .position = (struct Vec2int) {
        .x = Krnl_ScreenWidth / 2,
        .y = Krnl_ScreenHeight / 2,
      },
      .velocity = (struct Vec2int) {
        .x = random_number_not_zero_between(-5, 5),
        .y = random_number_not_zero_between(-5, 5)
      },
      .size = (struct Vec2int) {
        .x = 1,
        .y = 1,
      },
    }
  };
  IsInPlay = 1;

  do {
    Krnl_clear_screen();
    update_game(&game);
    render_game(&game);
  } while (IsInPlay);
}
