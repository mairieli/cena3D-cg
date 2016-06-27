#include "bibutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

#define SENS_ROT	5.0

// Vari�veis para controles de navega��o
GLfloat angle, fAspect;
// Vari�veis de rota��o
GLfloat rotY, rotY_ini;
// Vari�veis de transla��o
GLfloat transZ_banco = -6.5f, transX_banco = -0.2f, transY_corpo;
// Vari�veis que definem a posi��o do observador
GLfloat obsX, obsY, obsZ;
GLint x_inicial, botao;
// Habilitar/Desabilitar anima��o
GLboolean animacao = GL_FALSE;

// Apontador para objetos
OBJ *cabeca;
OBJ *corpo;
OBJ *banco;

// Fun��o respons�vel pela especifica��o dos par�metros de ilumina��o
void defineIluminacao(void) {
	GLfloat luzAmbiente[4] = {0.1f, 0.1f, 0.1f, 1.0f}; //"brilho"
	GLfloat direcao[4] = {0.0f, -1.0f, 0.0f, 1.0f};
	GLfloat posicaoLuzInfinita[4] = {0.0f, 0.0f, 100.0f, 0.0f};
	GLfloat posicaoLuz[4] = {0.0f, 8.0f, 5.0f, 1.0f};
	GLfloat branco[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat preto[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat azul[4] = {0.0f, 0.0f, 1.0f, 1.0f};

	// Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
	// Ativa o uso da posi�ao de vis�o corrente
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	// Define os par�metros da luz de n�mero 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, preto);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, branco);
	glLightfv(GL_LIGHT0, GL_SPECULAR, azul);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuzInfinita);

	// Define os par�metros da luz de n�mero 1
	glLightfv(GL_LIGHT1, GL_DIFFUSE, branco);
	glLightfv(GL_LIGHT1, GL_SPECULAR, azul);
	glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz);
	//Define a dire��o da luz
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direcao);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 40.0f);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

}

// Fun��o callback de redesenho da janela de visualiza��o
void desenha(void) {
	// Limpa a janela de visualiza��o com a cor de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Define que a matriz � a de modelo
	glMatrixMode(GL_MODELVIEW);

	GLfloat ks_corpo[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    GLfloat kd_corpo[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	GLint ns_corpo = 90;

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, kd_corpo);
	// Define a reflet�ncia do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, ks_corpo);
	// Define a concentra��o do brilho
	glMateriali(GL_FRONT, GL_SHININESS, ns_corpo);
	//Desenha cabe�a
	glPushMatrix();
	// Altera a cor do desenho
	glColor3f(0.50f, 0.50f, 0.50f);
	glRotatef(rotY, 0, 1, 0);
	glScalef(0.8f, 0.8f, 0.8f);
	glTranslatef(0, 5, 0);
	DesenhaObjeto(cabeca);
    glPopMatrix();

	// Define a reflet�ncia do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, ks_corpo);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, kd_corpo);
	// Define a concentra��o do brilho
	glMateriali(GL_FRONT, GL_SHININESS, ns_corpo);
    //Desenha corpo
    glPushMatrix();
	glRotatef(rotY, 0, 1, 0);
	glScalef(1.3f, 1.3f, 1.3f);
	glTranslatef(0, transY_corpo, 0);
	DesenhaObjeto(corpo);
    glPopMatrix();

    GLfloat ks_banco[4] = {0.6f, 1.0f, 0.0f, 0.0f};
    GLfloat kd_banco[4] = {0.7f, 0.8f, 1.0f, 0.0f};
	GLint ns_banco = 80;
	// Define a reflet�ncia do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, ks_banco);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, kd_banco);
	// Define a concentra��o do brilho
	glMateriali(GL_FRONT, GL_SHININESS, ns_banco);
    //Desenha banco
    glPushMatrix();
    glColor3f(0.91f, 0.51f, 0.23f);
	glScalef(0.8f, 0.8f, 0.8f);
	glTranslatef(transX_banco, -7.8f, transZ_banco);
	DesenhaObjeto(banco);
    glPopMatrix();

    GLfloat ks_parede[4] = {0.3f, 0.3f, 0.0f, 0.0f};
    GLfloat kd_parede[4] = {0.2f, 0.3f, 0.0f, 0.0f};
	GLint ns_parede = 90;
	// Define a reflet�ncia do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, ks_parede);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, kd_parede);
	// Define a concentra��o do brilho
	glMateriali(GL_FRONT, GL_SHININESS, ns_parede);

    //Desenha teto
    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(0, 10, 0);
    glScalef(20.0f, 0.1f, 20.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Desenha ch�o
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(0, -10, 0);
    glScalef(20.0f, 0.1f, 20.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Desenha esquerda
    glPushMatrix();
    glColor3f(1, 0, 1);
    glRotatef(90, 0, 0, 1);
    glTranslatef(0, -10, 0);
    glScalef(20.0f, 0.1f, 20.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Desenha direita
    glPushMatrix();
    glColor3f(0, 1, 1);
    glRotatef(90, 0, 0, 1);
    glTranslatef(0, 10, 0);
    glScalef(20.0f, 0.1f, 20.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Desenha tras
    glPushMatrix();
    glColor3f(0, 0, 0);
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, -10, 0);
    glScalef(20.0f, 0.1f, 20.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

	// Executa os comandos OpenGL
	glutSwapBuffers();
}

// Fun��o usada para especificar a posi��o do observador virtual
void posicionaObservador(void){
    glMatrixMode(GL_MODELVIEW);
    //carrega a matriz de identidade
    glLoadIdentity();
    // Posiciona observador
    gluLookAt(obsX, obsY, obsZ,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);
}

// Fun��o usada para especificar o volume de visualiza��o
void especificaParametrosVisualizacao(void) {
	// Define que a matriz � a matriz de proje�ao
	glMatrixMode(GL_PROJECTION);
	// Carrega a matriz de identidade
	glLoadIdentity();
	// Especifica a proje��o perspectiva(angulo,aspecto,dnear,dfar)
	gluPerspective(angle, fAspect, 0.01, 30);
	// Chama as fun��es que especificam os par�metros da c�mera e os par�metros de ilumina��o
	posicionaObservador();
	defineIluminacao();
}

// Fun��o callback chamada quando o tamanho da janela � alterado
void alteraTamanhoJanela(GLsizei w, GLsizei h) {
	// Para previnir uma divis�o por zero
	if (h == 0) h = 1;
	// Especifica as dimens�es da viewport
	glViewport(0, 0, w, h);
	// Calcula a corre��o de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;
	especificaParametrosVisualizacao();
}

// Fun��o callback chamada para gerenciar eventos de teclas normais (ESC)
void teclas(unsigned char tecla, int x, int y) {
	if(tecla == 27) {
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
        transZ_banco = -6.5f;
        transX_banco = -0.2f;
        transY_corpo = 0;
        rotY = 0;
	}
	glutPostRedisplay();
}

// Fun��o callback para eventos de bot�es do mouse
void gerenciaMouse(int button, int state, int x, int y) {
	if(state == GLUT_DOWN) {
		// Salva os par�metros atuais
		x_inicial = x;
		rotY_ini = rotY;
		botao = button;
	}
	else botao = -1;
}

// Fun��o callback para eventos de movimento do mouse
void gerenciaMovimentoMouse(int x, int y) {
	if(botao == GLUT_LEFT_BUTTON) {
		// Calcula diferen�as
		int deltax = x_inicial - x;
		// E modifica �ngulo y
		rotY = rotY_ini - deltax/SENS_ROT;
	}
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
	angle = 55;

	// Inicializa as vari�veis usadas para alterar a posi��o do observador virtual
	obsX = obsY = 0;
	obsZ = 20;

	cabeca = CarregaObjeto("objetos/cabeca.obj",true);
    printf("Objeto carregado!");

	// E calcula o vetor normal em cada face
	if(cabeca->normais) {
		// Se j� existirem normais no arquivo, apaga elas
		free(cabeca->normais);
		cabeca->normais_por_vertice = false;
	}
	CalculaNormaisPorFace(cabeca);

	corpo = CarregaObjeto("objetos/corpo.obj",true);
    printf("Objeto carregado!");

	// E calcula o vetor normal em cada face
	if(corpo->normais) {
		// Se j� existirem normais no arquivo, apaga elas
		free(corpo->normais);
		corpo->normais_por_vertice = false;
	}
	CalculaNormaisPorFace(corpo);

	banco = CarregaObjeto("objetos/stool.obj",true);
    printf("Objeto carregado!");

	// E calcula o vetor normal em cada face
	if(banco->normais) {
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
    }
    glutPostRedisplay();
}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
	// Define do modo de opera��o da GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// Especifica a posi��o inicial da janela GLUT
	glutInitWindowPosition(100, 100);
	// Especifica o tamanho inicial em pixels da janela GLUT
	glutInitWindowSize(800, 600);
	// Cria a janela passando como argumento o t�tulo
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
	glutMotionFunc(gerenciaMovimentoMouse);
	// Registra a fun��o callback que executa a anima��o
	glutIdleFunc(cria_animacao);
	// Chama a fun��o respons�vel por fazer as inicializa��es
	inicializa();
    // Inicia o processamento e aguarda intera��es do usu�rio
	glutMainLoop();

	return 0;
}
