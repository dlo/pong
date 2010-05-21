#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#include <math.h>
#include <stdio.h>

#define NO_COLLISION_DETECTED 0
#define BOTTOM_PADDLE_COLLISION 1
#define TOP_PADDLE_COLLISION 2

#define NOT_OUT_OF_BOUNDS 0
#define OUT_OF_BOUNDS_TOP 1
#define OUT_OF_BOUNDS_BOTTOM 2
#define OUT_OF_BOUNDS_LEFT 3
#define OUT_OF_BOUNDS_RIGHT 4

struct {
	struct {
		GLfloat x;
		GLint score;
		char *score_buf;
	} bottom_paddle;

	struct {
		GLfloat x;
		GLint score;
		char *score_buf;
	} top_paddle;

	struct {
		GLfloat x;
		GLfloat y;
		GLfloat dx;
		GLfloat dy;
	} ball;
} resources;

void setup(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	resources.bottom_paddle.x = 0.0f;
	resources.bottom_paddle.score = 0;
	resources.bottom_paddle.score_buf = (char *)malloc(sizeof(char) * 5);

	resources.top_paddle.x = 0.0f;
	resources.top_paddle.score = 0;
	resources.top_paddle.score_buf = (char *)malloc(sizeof(char) * 5);

	resources.ball.x = 0.0f;
	resources.ball.y = 0.0f;
	resources.ball.dx = 0.0f;
	resources.ball.dy = -0.05f;
}

void bitmapString(void *font, char *s, GLfloat x, GLfloat y) {
	glRasterPos2f(x, y);
	while (*s) glutBitmapCharacter(font, *s++);
}

void render(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 1.0f);
	glRectf(resources.bottom_paddle.x - 0.2f, -0.8f, resources.bottom_paddle.x + 0.2f, -0.7f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glRectf(resources.top_paddle.x - 0.2f, 0.7f, resources.top_paddle.x + 0.2f, 0.8f);

	// Draw the ball
	glColor3f(0.0f, 0.0f, 0.0f);
	glRectf(resources.ball.x - 0.05f, resources.ball.y, resources.ball.x + 0.05f, resources.ball.y + 0.1f);

	sprintf(resources.top_paddle.score_buf, "%d", resources.top_paddle.score);
	sprintf(resources.bottom_paddle.score_buf, "%d", resources.bottom_paddle.score);

	// Print scores
	bitmapString(GLUT_BITMAP_HELVETICA_18, resources.top_paddle.score_buf, -0.8f, 0.8f);
	bitmapString(GLUT_BITMAP_HELVETICA_18, resources.bottom_paddle.score_buf, -0.8f, -0.8f);

	glutSwapBuffers();
}

int detect_out_of_bounds() {
	if (resources.ball.y >= 1.0f) {
		// Bottom player gets a point
		resources.bottom_paddle.score++;
		return OUT_OF_BOUNDS_TOP;
	}
	else if (resources.ball.y <= -1.0f) {
		// Top player gets a point
		resources.top_paddle.score++;
		return OUT_OF_BOUNDS_BOTTOM;
	}
	else if (resources.ball.x < -1.0f) {
		return OUT_OF_BOUNDS_LEFT;
	}
	else if (resources.ball.x + 0.1f > 1.0f) {
		return OUT_OF_BOUNDS_RIGHT;
	}
	return NOT_OUT_OF_BOUNDS;
}

int detect_collision() {
	if (resources.ball.y <= -0.7f && resources.ball.x - 0.05f <= resources.bottom_paddle.x+0.2f && resources.ball.x +0.0f>= resources.bottom_paddle.x-0.2f)
		return BOTTOM_PADDLE_COLLISION;
	if (resources.ball.y >= 0.57f && resources.ball.x - 0.05f <= resources.top_paddle.x+0.2f && resources.ball.x +0.05f>= resources.top_paddle.x-0.2f)
		return TOP_PADDLE_COLLISION;
	return NO_COLLISION_DETECTED;
}

void keypress(unsigned char key, int x, int y) {
	switch (key) {
		case 97:
			resources.top_paddle.x-=0.05f;
			glutPostRedisplay();
			break;
		case 100:
			resources.top_paddle.x+=0.05f;
			glutPostRedisplay();
			break;
		default:
			break;
	}
}

void special_keypress(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT:
			resources.bottom_paddle.x-=0.05f;
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			resources.bottom_paddle.x+=0.05f;
			glutPostRedisplay();
			break;
		default:
			break;
	}
}

void update_ball_location(void) {
	// Check for collisions
	switch (detect_collision()) {
		case TOP_PADDLE_COLLISION:
			resources.ball.dy *= -1;
			resources.ball.dx += (resources.ball.x - resources.top_paddle.x) / 15.0f;
			resources.ball.y -= 0.02;
			break;
		case BOTTOM_PADDLE_COLLISION:
			resources.ball.dy *= -1;
			resources.ball.dx += (resources.ball.x - resources.bottom_paddle.x) / 15.0f;
			resources.ball.y += 0.02;
			break;
		case NO_COLLISION_DETECTED:
			break;
	}

	switch (detect_out_of_bounds()) {
		case OUT_OF_BOUNDS_TOP:
			// update score
			resources.top_paddle.x = 0.0f;
			resources.bottom_paddle.x = 0.0f;
			resources.ball.dy = 0.05f;
			resources.ball.dx = 0.0f;
			resources.ball.x = 0.0f;
			resources.ball.y = 0.0f;
			break;
		case OUT_OF_BOUNDS_BOTTOM:
			// update score
			resources.top_paddle.x = 0.0f;
			resources.bottom_paddle.x = 0.0f;
			resources.ball.dy = -0.05f;
			resources.ball.dx = 0.0f;
			resources.ball.x = 0.0f;
			resources.ball.y = 0.0f;
			break;
		case OUT_OF_BOUNDS_LEFT:
			resources.ball.dx *= -1;
			break;
		case OUT_OF_BOUNDS_RIGHT:
			resources.ball.dx *= -1;
			break;
		case NOT_OUT_OF_BOUNDS:
			break;
	}

	resources.ball.x += resources.ball.dx;
	resources.ball.y += resources.ball.dy;
}

void update(int n) {
	update_ball_location();
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Pong");
	setup();

	glutKeyboardFunc(&keypress);
	glutSpecialFunc(&special_keypress);
	glutDisplayFunc(&render);
	glutTimerFunc(25, update, 0);

	glewInit();
	if (!GLEW_VERSION_2_0) {
		fprintf(stderr, "OpenGL 2.0 not available\n");
		return 1;
	}

	glutMainLoop();
	return 0;
}
