#include "bibutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

#define SENS_ROT	5.0
#define SENS_OBS	15.0

// Vari�veis para controles de navega��o
GLfloat angle, fAspect;
GLfloat rotX, rotY, rotX_ini, rotY_ini, transZ_banco=-6.5f, transX_banco=-0.2f, transY_corpo;
GLfloat obsX, obsY, obsZ, obsX_ini, obsY_ini, obsZ_ini;
GLboolean animacao = GL_FALSE;
int x_ini,y_ini,bot;

// Apontador para objetos
OBJ *cabeca;
OBJ* corpo;
OBJ* banco;

// Fun��o respons�vel pela especifica��o dos par�metros de ilumina��o
void defineIluminacao(void) {
	GLfloat luzAmbiente[4]={0.1,0.1,0.1,1.0};	// "brilho"
	float direction[4] = {0.0, -1.0, 0.0, 1.0};
	GLfloat posicaoLuzInfinita[4]={0.0, 0.0, 100.0, 0.0};
	GLfloat posicaoLuz[4]={0.0, 8.0, 5.0, 1.0};
	float white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    float red[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    float blue[4] = {0.0f, 0.0f, 1.0f, 1.0f};


	// Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	// Define os par�metros da luz de n�mero 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, black);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, blue);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuzInfinita);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, red);
	glLightfv(GL_LIGHT1, GL_SPECULAR, red);
	glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction); //direcao da luz: para baixo
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 40.0f);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

}

// Fun��o callback de redesenho da janela de visualiza��o
void desenha(void) {
	// Limpa a janela de visualiza��o com a cor
	// de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Define que a matriz � a de modelo
	glMatrixMode(GL_MODELVIEW);

	// Altera a cor do desenho
	glColor3f(0.50f, 0.50f, 0.50f);

	GLfloat ks_cabeca[4]={0.0,0.0,0.0,0.0};
	GLfloat kd_cabeca[4]={1.0,1.0,1.0,0.0};
	GLint ns_cabeca = 90;

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, kd_cabeca);
	// Define a reflet�ncia do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, ks_cabeca);
	// Define a concentra��o do brilho
	glMateriali(GL_FRONT,GL_SHININESS, ns_cabeca);
	//Desenha cabe�a
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
	// Define a reflet�ncia do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, ks_corpo);
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, kd_corpo);
	// Define a concentra��o do brilho
	glMateriali(GL_FRONT,GL_SHININESS, ns_corpo);
    //Desenha corpo
    glPushMatrix();
    glRotatef(rotX,1,0,0);
	glRotatef(rotY,0,1,0);
	glScalef(1.3f, 1.3f, 1.3f);
	glTranslatef(0, transY_corpo, 0);
	DesenhaObjeto(corpo);
    glPopMatrix();

    GLfloat ks_banco[4]={0.0,0.0,0.0,0.0};
    GLfloat kd_banco[4]={0.0,0.0,0.0,0.0};
	GLint ns_banco = 800;
	// Define a reflet�ncia do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, ks_banco);
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, kd_banco);
	// Define a concentra��o do brilho
	glMateriali(GL_FRONT,GL_SHININESS, ns_banco);
    //Desenha banco
    glPushMatrix();
    glColor3f(0.91f, 0.51f, 0.23f);
    glRotatef(0,1,0,0);
	glRotatef(0,0,1,0);
	glScalef(0.8f, 0.8f,0.8f);
	glTranslatef(transX_banco, -7.8f, transZ_banco);
	DesenhaObjeto(banco);
    glPopMatrix();

    GLfloat ks_parede[4]={0.0,0.0,0.0,0.0};
    GLfloat kd_parede[4]={0.0,0.0,0.0,0.0};
	GLint ns_parede = 60;
	// Define a reflet�ncia do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, ks_parede);
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, kd_parede);
	// Define a concentra��o do brilho
	glMateriali(GL_FRONT,GL_SHININESS, ns_parede);

    //teto
    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(0, 10, 0);
    glScalef(20, 0.1f, 20);
    glutSolidCube(1.0f);
    glPopMatrix();

    //ch�o
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

// Fun��o usada para especificar a posi��o do observador virtual
void posicionaObservador(void){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();//carrega a matriz de identidade
    gluLookAt(obsX,obsY,obsZ,//posi��o da c�mera
              0.0,0.0,0.0,//para onde a c�mera aponta
              0.0,1.0,0.0);//vetor view-up//
}

// Fun��o usada para especificar o volume de visualiza��o
void especificaParametrosVisualizacao(void) {

	// Define que a matriz � a matriz de proje�ao
	glMatrixMode(GL_PROJECTION);
	// Carrega a matriz de identidade
	glLoadIdentity();

	// Especifica a proje��o perspectiva(angulo,aspecto,dnear,dfar)
	gluPerspective(angle, fAspect, 0.01, 1200);

	// Chama as fun��es que especificam os par�metros da c�mera e os par�metros de ilumina��o
	posicionaObservador();
	defineIluminacao();

}

// Fun��o callback chamada quando o tamanho da janela � alterado
void alteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divis�o por zero
	if ( h == 0 ) h = 1;

	// Especifica as dimens�es da viewport
	glViewport(0, 0, w, h);

	// Calcula a corre��o de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;

	especificaParametrosVisualizacao();
}

// Fun��o callback chamada para gerenciar eventos de teclas normais (ESC)
void teclas(unsigned char tecla, int x, int y) {
	if(tecla==27) {
		// Libera mem�ria e finaliza programa
		LiberaObjeto(cabeca);
		LiberaObjeto(corpo);
		LiberaObjeto(banco);
		exit(0);
	} else if (tecla == 'w'){
        animacao = GL_TRUE;
	} else if (tecla == 'a'){
        animacao = GL_FALSE;
	} else if (tecla == 's'){
        animacao =  GL_FALSE;
        transZ_banco=-6.5f;
        transX_banco=-0.2f;
        transY_corpo = 0;
        rotX=0;
        rotY=0;
	}
	glutPostRedisplay();
}

// Fun��o callback para eventos de bot�es do mouse
void gerenciaMouse(int button, int state, int x, int y) {
	if(state==GLUT_DOWN) {
		// Salva os par�metros atuais
		x_ini = x;
		y_ini = y;
		obsZ_ini = obsZ;
		rotY_ini = rotY;
		bot = button;
	}
	else bot = -1;
}

// Fun��o callback para eventos de movimento do mouse
void gerenciaMovimento(int x, int y) {
	// Bot�o esquerdo
	if(bot==GLUT_LEFT_BUTTON)
	{
		// Calcula diferen�as
		int deltax = x_ini - x;
		// E modifica �ngulo y
		rotY = rotY_ini - deltax/SENS_ROT;
	}
	// Bot�o do meio
	posicionaObservador();
	glutPostRedisplay();
}

// Fun��o respons�vel por inicializar par�metros e vari�veis
void inicializa(void) {

	// Define a cor de fundo da janela de visualiza��o como branca
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Habilita a defini��o da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	// Remo��o de superf�cie oculta
	glEnable(GL_DEPTH_TEST);

	// Habilita o modelo de tonaliza��o de Gouraud
	glShadeModel(GL_SMOOTH);

	// Inicializa a vari�vel que especifica o �ngulo da proje��o perspectiva
	angle=55;

	// Inicializa as vari�veis usadas para alterar a posi��o do observador virtual
	obsX = obsY = 0;
	obsZ = 20;

	// Carrega o objeto 3D
	cabeca = CarregaObjeto("objetos/cabeca.obj",true);
    printf("Objeto carregado!");

	// E calcula o vetor normal em cada face
	if(cabeca->normais)
	{
		// Se j� existirem normais no arquivo, apaga elas
		free(cabeca->normais);
		cabeca->normais_por_vertice = false;
	}
	CalculaNormaisPorFace(cabeca);

	corpo = CarregaObjeto("objetos/corpo.obj",true);
    printf("Objeto carregado!");

	// E calcula o vetor normal em cada face
	if(corpo->normais)
	{
		// Se j� existirem normais no arquivo, apaga elas
		free(corpo->normais);
		corpo->normais_por_vertice = false;
	}
	CalculaNormaisPorFace(corpo);

	banco = CarregaObjeto("objetos/stool.obj",true);
    printf("Objeto carregado!");

	// E calcula o vetor normal em cada face
	if(banco->normais)
	{
		// Se j� existirem normais no arquivo, apaga elas
		free(banco->normais);
		banco->normais_por_vertice = false;
	}
	CalculaNormaisPorFace(banco);
}

void cria_animacao(){
    if(animacao){
            if(transX_banco < 5){
                transX_banco += 1;
                transZ_banco += 1;
            } else {
                if(transY_corpo > -3){
                    transY_corpo -= 0.5;
                } else {
                    rotY += 15;
                }
            }

        glutPostRedisplay();
    }
    glutPostRedisplay();
}


// Programa Principal
int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
	// Define do modo de opera��o da GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// Especifica a posi��o inicial da janela GLUT
	glutInitWindowPosition(100, 100);
	// Especifica o tamanho inicial em pixels da janela GLUT
	glutInitWindowSize(800,600);
	// Cria a janela passando como argumento o t�tulo da mesma
	glutCreateWindow("Quarto do Terror");
	// Registra a fun��o callback de redesenho da janela de visualiza��o
	glutDisplayFunc(desenha);
	// Registra a fun��o callback de redimensionamento da janela de visualiza��o
	glutReshapeFunc(alteraTamanhoJanela);
	// Registra a fun��o callback para tratamento das teclas normais
	glutKeyboardFunc(teclas);
	// Registra a fun��o callback para eventos de bot�es do mouse
	glutMouseFunc(gerenciaMouse);
	// Registra a fun��o callback para eventos de movimento do mouse
	glutMotionFunc(gerenciaMovimento);
	glutIdleFunc(cria_animacao);
	// Chama a fun��o respons�vel por fazer as inicializa��es
	inicializa();
    // Inicia o processamento e aguarda intera��es do usu�rio
	glutMainLoop();

	return 0;
}
