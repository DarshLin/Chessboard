
#include "gl/glew.h"
#include "gl/freeglut.h"
#include "gl/GLSL.h"
#include <math.h>

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
	const float pi = 3.14152f;						\n\
	bool odd(float num){							\n\
		if(mod(num,2) < 1)	{					\n\
			return false;}						\n\
		else {return true;}						\n\
	}											\n\
	void main()	{								\n\
	vec2 pos = mod(gl_FragCoord.xy, vec2(400)) - vec2(200); \n\
	float x = 2;									\n\
	while(x < 10) {								\n\
	float dist_squared = dot(pos, pos);	\n\
	if ((dist_squared > 575.0 *x) || (dist_squared < 100.0))	\n\
	discard;									\n\
	gl_FragColor = mix(vec4(.90, .90, .90, 1), vec4(.20, .20, .40, 1),	\n\
	smoothstep(380.25, 420.25, dist_squared));	\n\
	x+=1.0;										\n\
	}											\n\
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
