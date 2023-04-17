#ifndef PAD_H
#define PAD_H

#define MAX_SPEED 1

typedef struct {
	float width, height, x, y, velocity;
} PAD;

PAD* init_paddle(float width, float height, float x, float y, float velocity);

void free_paddle(PAD* p);

void update_paddle(PAD* p, float dts);

void go_left(PAD* p);
void go_right(PAD* p);

#endif
