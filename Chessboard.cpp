
#include "gl/glew.h"
#include "gl/freeglut.h"
#include "gl/GLSL.h"

GLuint vBufferId = 0;				// GPU vert buffer, valid > 0
GLuint programId = 0;				// GLSL program, valid if > 0

const char *vertexShader = "\
	#version 130								\n\
	in vec2 point;								\n\
	void main() {								\n\
		gl_Position = vec4(point, 0, 1);		\n\
	}											\n";

const char *pixelShader = "\
	#version 130								\n\
	out vec4 fColor;							\n\
	bool odd(int num){							\n\
		if(mod(num,2) < 1)	{					\n\
			return false;}						\n\
		else {return true;}						\n\
	}											\n\
	void main()	{								\n\
		float x = gl_FragCoord.x;				\n\
		float y = gl_FragCoord.y;				\n\
		int i,j; //limits						\n\
		for(i=1; i<=8; i++) {					\n\
			for(j=1;j<=8;j++) {					\n\
			if(odd(i) != odd(j)) {				\n\
				if(x > ((i-1)*50)-(i+1) && x < (i*50)-(i+1) && y > ((j-1)*50) -(j+1)&& y < (j*50)-(j+1)) {	\n\
					fColor = vec4(1,1,1,1); }	\n\
		}										\n\
		}										\n\
		}										\n\
	}											\n";

void Display() {
	// called whenever application displayed
	glUseProgram(programId);
	GLSL::VertexAttribPointer(programId, "point", 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	glDrawArrays(GL_QUADS, 0, 4);	// display entire window
    glFlush();						// flush GL ops complete
}

void InitVertexBuffer() {
	float pts[][2] = {{-1,-1}, {-1,1}, {1,1}, {1,-1}};
	int ptSize = sizeof(pts);
    // create GPU buffer for 4 verts, bind, allocate/copy
    glGenBuffers(1, &vBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vBufferId);
	glBufferData(GL_ARRAY_BUFFER, ptSize, pts, GL_STATIC_DRAW);
}

void main(int argc, char **argv) {	// application entry
    glutInit(&argc, argv);			// init app toolkit
    glutInitWindowSize(400, 400);	// set window size
    glutCreateWindow("Chessboard");		// create named window
    glewInit();						// wrangle GL extensions
	InitVertexBuffer();				// set GPU vertex memory
	programId = GLSL::LinkProgramViaCode(vertexShader, pixelShader);
	glutDisplayFunc(Display);		// GLUT display callback
    glutMainLoop();					// enter GLUT event loop
}
