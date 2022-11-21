#include<Windows.h>
#include<GLFW/glfw3.h>
#include<gl/GLU.h>
#include<iostream>
#include<vector>
#include<cstdlib>
#include<cmath>
#include "Cores.h"
#include "Primitivas.h"
#include "cg.h"

static float angulo = 5.0;
static float lastMousePos = 0.0;
static bool firstTimeMouse = true;

//INICIO CAMERA...

//Camera.h...
class Camera
{
private:
	vec3 m_pos;
	vec3 m_dir;
	vec3 m_left;
	vec3 m_up;
	vec3 m_veloc;
	float m_scl = 0.25;
	float m_yaw;
	float m_PI = 3.14159265;

public:
	Camera(vec3 pos);
	~Camera();
	void ativar();

	void forward();
	void back();
	void left();
	void right();
	void updateYaw(float dYaw);
	void update();

private:
	float cvtToRad(float ang);
};
//...

//Camera.cpp... 
Camera::Camera(vec3 pos)
{
	m_pos = vec3(pos);
	m_dir = vec3(0, 0, -1);
	m_left = vec3(-1, 0, 0);
	m_up = vec3(0, 1, 0);
	m_veloc = vec3(0, 0, 0);
}

Camera::~Camera()
{
}

void Camera::ativar()
{
	vec3 look = m_pos + m_dir;
	gluLookAt(m_pos.x, m_pos.y, m_pos.z, look.x, look.y, look.z, m_up.x, m_up.y, m_up.z);
}

void Camera::forward()
{
	m_veloc = m_dir * m_scl;
	m_pos = m_pos + m_veloc;
}

void Camera::back()
{
	m_veloc = m_dir * (-m_scl);
	m_pos = m_pos + m_veloc;
}

void Camera::left()
{
	m_veloc = m_left * m_scl;
	m_pos = m_pos + m_veloc;
}

void Camera::right()
{
	m_veloc = m_left * (-m_scl);
	m_pos = m_pos + m_veloc;
}

void Camera::updateYaw(float dYaw)
{
	m_yaw += dYaw;
}

void Camera::update()
{
	float ang = cvtToRad(m_yaw);
	m_dir.x = sin(ang);
	m_dir.z = -cos(ang);
	m_dir.normaliza();
	m_left = m_up.prodVetorial(m_dir);
}

float Camera::cvtToRad(float ang)
{
	return (ang * m_PI) / 180.0;
}
//...

Camera camera(vec3(-10, 0, 0));
//FIM CAMERA...

// Display List
unsigned int esferaID[3];
unsigned int cuboID[3];

void redimensiona(int w, int h)
{
	glViewport(0, 0, w, h);

	float aspect = (float)w / (float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 500.0);

	glMatrixMode(GL_MODELVIEW);
}

void desenha()
{
	glLoadIdentity();

	//Cria a matriz
	camera.ativar();
	
	//Passando parametros da Esfera
	glPushMatrix();
	glTranslatef(-30.0, 0.0, -80.0);
	glRotatef(angulo, 1.0, 1.0, 0.0);
	glCallList(esferaID[0]);
	glPopMatrix();

	//Passando parametros do Cubo
	glPushMatrix();
	glTranslatef(-0.0, 0.0, -80.0);
	glRotatef(angulo, 1.0, 1.0, 0.0);
	glCallList(cuboID[0]);
	glPopMatrix();

	angulo += 1.0;
	if (angulo >= 360.0)
		angulo = 0.0;
}

//Declarando a funcao que pega o comando do teclado
void teclado_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// Forward
		camera.forward();
	}
	else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// Back
		camera.back();

	}
	else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// Left
		camera.left();

	}
	else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// Right
		camera.right();
	}
}

//Declarando a funcao que pega o comando do mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float dx;

	if (firstTimeMouse)
	{
		dx = 0;
		lastMousePos = xpos;
		firstTimeMouse = false;
	}
	dx = xpos - lastMousePos;
	lastMousePos = xpos;
	camera.updateYaw(dx);
	camera.update();
}


void init(GLFWwindow* window)
{
	//Declarando a callback do mouse
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Declarando a callback do teclado
	glfwSetKeyCallback(window, teclado_callback);

	//glClearColor(0.0, 0.15, 0.25, 1.0); // cor de fundo
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);

	esferaID[0] = glGenLists(3);
	desenhaEsfera(esferaID[0], verde, 10.0, 20.0, 20.0);
	
	cuboID[0] = glGenLists(3);
	desenhaCubo(cuboID[0], 10.0);
}

int main(void)
{
	const int LARGURA = 800;
	const int ALTURA = 600;

	/* Initialize the library */
	glfwInit();

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(LARGURA, ALTURA, "Desenha Esfera", NULL, NULL);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// -- inicio
	init(window);

	float valor = 0.0;
	float lastTime = 0.0;
	float FPS = 60.0;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		float now;
		float dt;
		do {
			now = (float)glfwGetTime();
			dt = now - lastTime;

		} while (dt < 1.0 / FPS);
		lastTime = now;

		/* Poll for and process events */
		glfwPollEvents();
		
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int largura, altura;
		glfwGetFramebufferSize(window, &largura, &altura);

		redimensiona(largura, altura);

		desenha();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
	}

	glDeleteLists(esferaID[0], 3);
	glDeleteLists(cuboID[0], 3);
	glfwTerminate();
	return 0;
}