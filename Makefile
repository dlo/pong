GLEW_INCLUDE = /usr/local/opt/include
GLEW_LIB = /usr/local/opt

pong: pong.o
	gcc -o pong $^ -framework GLUT -framework OpenGL -L$(GLEW_LIB) -lGLEW

.c.o:
	gcc -c -o $@ $< -I$(GLEW_INCLUDE)

clean:
	rm -f pong pong.o
