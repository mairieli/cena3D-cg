#include "bibutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

#define SENS_ROT	5.0
#define SENS_OBS	15.0
#define SENS_TRANSL	20.0

// Variáveis para controles de navegação
GLfloat angle, fAspect;
GLfloat rotX, rotY, rotX_ini, rotY_ini;
GLfloat obsX, obsY, obsZ, obsX_ini, obsY_ini, obsZ_ini;
int x_ini,y_ini,bot;

// Apontador para objetos
OBJ *cabeca;
OBJ* corpo;
OBJ* banco;

// Função responsável pela especificação dos parâmetros de iluminação
void defineIluminacao(void) {
	GLfloat luzAmbiente[4]={0.1,0.1,0.1,1.0};
	GLfloat luzDifusa[4]={1.0,1.0,1.0,1.0};	   	// "cor"
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};	// "brilho"
	GLfloat posicaoLuz[4]={0.0, 0.0, 100.0, 1.0};

	// Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);
}

// Função callback de redesenho da janela de visualização
void desenha(void) {
	// Limpa a janela de visualização com a cor
	// de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Define que a matriz é a de modelo
	glMatrixMode(GL_MODELVIEW);

	// Altera a cor do desenho
	glColor3f(0.50f, 0.50f, 0.50f);

	GLfloat ks_cabeca[4]={0.0,0.0,0.0,0.0};
	GLfloat kd_cabeca[4]={1.0,1.0,1.0,0.0};
	GLint ns_cabeca = 90;

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, kd_cabeca);
	// Define a refletância do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, ks_cabeca);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS, ns_cabeca);
	//Desenha cabeça
	glPushMatrix();
    glRotatef(rotX,1,0,0);
	glRotatef(rotY,0,1,0);
	glScalef(0.8f, 0.8f, 0.8f);
	glTranslatef(0, 5, 0);
	DesenhaObjeto(cabeca);
    glPopMatrix();

    GLfloat ks_corpo[4]={0.0,1.0,0.0,1.0};
    GLfloat kd_corpo[4]={0.0,0.0,0.0,0.0};
	GLint ns_corpo = 90;
	// Define a refletância do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, ks_corpo);
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, kd_corpo);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS, ns_corpo);
    //Desenha corpo
    glPushMatrix();
    glRotatef(rotX,1,0,0);
	glRotatef(rotY,0,1,0);
	glScalef(1.3f, 1.3f, 1.3f);
	DesenhaObjeto(corpo);
    glPopMatrix();

    GLfloat ks_banco[4]={0.0,0.0,0.0,0.0};
    GLfloat kd_banco[4]={0.0,0.0,0.0,0.0};
	GLint ns_banco = 800;
	// Define a refletância do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, ks_banco);
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, kd_banco);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS, ns_banco);
    //Desenha banco
    glPushMatrix();
    glColor3f(0.91f, 0.51f, 0.23f);
    glRotatef(0,1,0,0);
	glRotatef(0,0,1,0);
	glScalef(0.8f, 0.8f,0.8f);
	glTranslatef(-0.2f, -7.8f, -6.5f);
	DesenhaObjeto(banco);
    glPopMatrix();

    GLfloat ks_parede[4]={0.0,0.0,0.0,0.0};
    GLfloat kd_parede[4]={0.0,0.0,0.0,0.0};
	GLint ns_parede = 60;
	// Define a refletância do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, ks_parede);
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, kd_parede);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS, ns_parede);

    //teto
    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(0, 10, 0);
    glScalef(20, 0.1f, 20);
    glutSolidCube(1.0f);
    glPopMatrix();

    //chão
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(0, -10, 0);
    glScalef(20, 0.1f, 20);
    glutSolidCube(1.0f);
    glPopMatrix();

    //esquerda
    glPushMatrix();
    glColor3f(1, 0, 1);
    glRotatef(90, 0, 0, 1);
    glTranslatef(0, -10, 0);
    glScalef(20, 0.1f, 20);
    glutSolidCube(1.0f);
    glPopMatrix();

    //direita
    glPushMatrix();
    glColor3f(0, 1, 1);
    glRotatef(90, 0, 0, 1);
    glTranslatef(0, 10, 0);
    glScalef(20, 0.1f, 20);
    glutSolidCube(1.0f);
    glPopMatrix();

    //tras
    glPushMatrix();
    glColor3f(0, 0, 0);
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, -10, 0);
    glScalef(20, 0.1f, 20);
    glutSolidCube(1.0f);
    glPopMatrix();

	// Executa os comandos OpenGL
	glutSwapBuffers();
}

// Função usada para especificar a posição do observador virtual
void posicionaObservador(void){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();//carrega a matriz de identidade
    gluLookAt(obsX,obsY,obsZ,//posição da câmera
              0.0,0.0,0.0,//para onde a câmera aponta
              0.0,1.0,0.0);//vetor view-up//
}

// Função usada para especificar o volume de visualização
void especificaParametrosVisualizacao(void) {

	// Define que a matriz é a matriz de projeçao
	glMatrixMode(GL_PROJECTION);
	// Carrega a matriz de identidade
	glLoadIdentity();

	// Especifica a projeção perspectiva(angulo,aspecto,dnear,dfar)
	gluPerspective(angle, fAspect, 0.01, 1200);

	// Chama as funções que especificam os parâmetros da câmera e os parâmetros de iluminação
	posicionaObservador();
	defineIluminacao();

}

// Função callback chamada quando o tamanho da janela é alterado
void alteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if ( h == 0 ) h = 1;

	// Especifica as dimensões da viewport
	glViewport(0, 0, w, h);

	// Calcula a correção de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;

	especificaParametrosVisualizacao();
}

// Função callback chamada para gerenciar eventos de teclas normais (ESC)
void teclas(unsigned char tecla, int x, int y) {
	if(tecla==27) {
		// Libera memória e finaliza programa
		LiberaObjeto(cabeca);
		LiberaObjeto(corpo);
		LiberaObjeto(banco);
		exit(0);
	}
	glutPostRedisplay();
}

// Função callback para eventos de botões do mouse
void gerenciaMouse(int button, int state, int x, int y) {
	if(state==GLUT_DOWN) {
		// Salva os parâmetros atuais
		x_ini = x;
		y_ini = y;
		obsZ_ini = obsZ;
		rotY_ini = rotY;
		bot = button;
	}
	else bot = -1;
}

// Função callback para eventos de movimento do mouse
void gerenciaMovimento(int x, int y) {
	// Botão esquerdo
	if(bot==GLUT_LEFT_BUTTON)
	{
		// Calcula diferenças
		int deltax = x_ini - x;
		// E modifica ângulo y
		rotY = rotY_ini - deltax/SENS_ROT;
	}
	// Botão direito (zoom-in e zoom-out)
	else if(bot==GLUT_RIGHT_BUTTON){
		// Calcula diferença
		int deltaz = y_ini - y;
		if((obsZ_ini + deltaz/SENS_OBS) <= 20 && (obsZ_ini + deltaz/SENS_OBS) >= 10)
		// E modifica distância do observador
            obsZ = obsZ_ini + deltaz/SENS_OBS;
	}
	// Botão do meio
	posicionaObservador();
	glutPostRedisplay();
}

// Função responsável por inicializar parâmetros e variáveis
void inicializa(void) {

	// Define a cor de fundo da janela de visualização como branca
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Remoção de superfície oculta
	glEnable(GL_DEPTH_TEST);

	// Habilita o modelo de tonalização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Inicializa a variável que especifica o ângulo da projeção perspectiva
	angle=55;

	// Inicializa as variáveis usadas para alterar a posição do observador virtual
	obsX = obsY = 0;
	obsZ = 20;

	// Carrega o objeto 3D
	cabeca = CarregaObjeto("objetos/cabeca.obj",true);
    printf("Objeto carregado!");

	// E calcula o vetor normal em cada face
	if(cabeca->normais)
	{
		// Se já existirem normais no arquivo, apaga elas
		free(cabeca->normais);
		cabeca->normais_por_vertice = false;
	}
	CalculaNormaisPorFace(cabeca);

	corpo = CarregaObjeto("objetos/corpo.obj",true);
    printf("Objeto carregado!");

	// E calcula o vetor normal em cada face
	if(corpo->normais)
	{
		// Se já existirem normais no arquivo, apaga elas
		free(corpo->normais);
		corpo->normais_por_vertice = false;
	}
	CalculaNormaisPorFace(corpo);

	banco = CarregaObjeto("objetos/stool.obj",true);
    printf("Objeto carregado!");

	// E calcula o vetor normal em cada face
	if(banco->normais)
	{
		// Se já existirem normais no arquivo, apaga elas
		free(banco->normais);
		banco->normais_por_vertice = false;
	}
	CalculaNormaisPorFace(banco);
}

// Programa Principal
int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
	// Define do modo de operação da GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Especifica a posição inicial da janela GLUT
	glutInitWindowPosition(5,5);

	// Especifica o tamanho inicial em pixels da janela GLUT
	glutInitWindowSize(1200,600);

	// Cria a janela passando como argumento o título da mesma
	glutCreateWindow("Quarto do Terror");

	// Registra a função callback de redesenho da janela de visualização
	glutDisplayFunc(desenha);

	// Registra a função callback de redimensionamento da janela de visualização
	glutReshapeFunc(alteraTamanhoJanela);

	// Registra a função callback para tratamento das teclas normais
	glutKeyboardFunc(teclas);

	// Registra a função callback para eventos de botões do mouse
	glutMouseFunc(gerenciaMouse);

	// Registra a função callback para eventos de movimento do mouse
	glutMotionFunc(gerenciaMovimento);

	// Chama a função responsável por fazer as inicializações
	inicializa();

	// Inicia o processamento e aguarda interações do usuário
	glutMainLoop();

	return 0;
}
