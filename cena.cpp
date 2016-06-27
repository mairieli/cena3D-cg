#include "bibutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

#define SENS_ROT	5.0

// Variáveis para controles de navegação
GLfloat angle, fAspect;
// Variáveis de rotação
GLfloat rotY, rotY_ini;
// Variáveis de translação
GLfloat transZ_banco = -6.5f, transX_banco = -0.2f, transY_corpo;
// Variáveis que definem a posição do observador
GLfloat obsX, obsY, obsZ;
GLint x_inicial, botao;
// Habilitar/Desabilitar animação
GLboolean animacao = GL_FALSE;

// Apontador para objetos
OBJ *cabeca;
OBJ *corpo;
OBJ *banco;

// Função responsável pela especificação dos parâmetros de iluminação
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
	// Ativa o uso da posiçao de visão corrente
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, preto);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, branco);
	glLightfv(GL_LIGHT0, GL_SPECULAR, azul);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuzInfinita);

	// Define os parâmetros da luz de número 1
	glLightfv(GL_LIGHT1, GL_DIFFUSE, branco);
	glLightfv(GL_LIGHT1, GL_SPECULAR, azul);
	glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz);
	//Define a direção da luz
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direcao);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 40.0f);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

}

// Função callback de redesenho da janela de visualização
void desenha(void) {
	// Limpa a janela de visualização com a cor de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Define que a matriz é a de modelo
	glMatrixMode(GL_MODELVIEW);

	GLfloat ks_corpo[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    GLfloat kd_corpo[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	GLint ns_corpo = 90;

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, kd_corpo);
	// Define a refletância do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, ks_corpo);
	// Define a concentração do brilho
	glMateriali(GL_FRONT, GL_SHININESS, ns_corpo);
	//Desenha cabeça
	glPushMatrix();
	// Altera a cor do desenho
	glColor3f(0.50f, 0.50f, 0.50f);
	glRotatef(rotY, 0, 1, 0);
	glScalef(0.8f, 0.8f, 0.8f);
	glTranslatef(0, 5, 0);
	DesenhaObjeto(cabeca);
    glPopMatrix();

	// Define a refletância do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, ks_corpo);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, kd_corpo);
	// Define a concentração do brilho
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
	// Define a refletância do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, ks_banco);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, kd_banco);
	// Define a concentração do brilho
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
	// Define a refletância do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, ks_parede);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, kd_parede);
	// Define a concentração do brilho
	glMateriali(GL_FRONT, GL_SHININESS, ns_parede);

    //Desenha teto
    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(0, 10, 0);
    glScalef(20.0f, 0.1f, 20.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Desenha chão
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

// Função usada para especificar a posição do observador virtual
void posicionaObservador(void){
    glMatrixMode(GL_MODELVIEW);
    //carrega a matriz de identidade
    glLoadIdentity();
    // Posiciona observador
    gluLookAt(obsX, obsY, obsZ,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);
}

// Função usada para especificar o volume de visualização
void especificaParametrosVisualizacao(void) {
	// Define que a matriz é a matriz de projeçao
	glMatrixMode(GL_PROJECTION);
	// Carrega a matriz de identidade
	glLoadIdentity();
	// Especifica a projeção perspectiva(angulo,aspecto,dnear,dfar)
	gluPerspective(angle, fAspect, 0.01, 30);
	// Chama as funções que especificam os parâmetros da câmera e os parâmetros de iluminação
	posicionaObservador();
	defineIluminacao();
}

// Função callback chamada quando o tamanho da janela é alterado
void alteraTamanhoJanela(GLsizei w, GLsizei h) {
	// Para previnir uma divisão por zero
	if (h == 0) h = 1;
	// Especifica as dimensões da viewport
	glViewport(0, 0, w, h);
	// Calcula a correção de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;
	especificaParametrosVisualizacao();
}

// Função callback chamada para gerenciar eventos de teclas normais (ESC)
void teclas(unsigned char tecla, int x, int y) {
	if(tecla == 27) {
		// Libera memória e finaliza programa
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

// Função callback para eventos de botões do mouse
void gerenciaMouse(int button, int state, int x, int y) {
	if(state == GLUT_DOWN) {
		// Salva os parâmetros atuais
		x_inicial = x;
		rotY_ini = rotY;
		botao = button;
	}
	else botao = -1;
}

// Função callback para eventos de movimento do mouse
void gerenciaMovimentoMouse(int x, int y) {
	if(botao == GLUT_LEFT_BUTTON) {
		// Calcula diferenças
		int deltax = x_inicial - x;
		// E modifica ângulo y
		rotY = rotY_ini - deltax/SENS_ROT;
	}
	glutPostRedisplay();
}

// Função responsável por inicializar parâmetros e variáveis
void inicializa(void) {
	// Define a cor de fundo da janela de visualização como branca
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	// Remoção de superfície oculta
	glEnable(GL_DEPTH_TEST);
	// Habilita o modelo de tonalização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Inicializa a variável que especifica o ângulo da projeção perspectiva
	angle = 55;

	// Inicializa as variáveis usadas para alterar a posição do observador virtual
	obsX = obsY = 0;
	obsZ = 20;

	cabeca = CarregaObjeto("objetos/cabeca.obj",true);
    printf("Objeto carregado!");

	// E calcula o vetor normal em cada face
	if(cabeca->normais) {
		// Se já existirem normais no arquivo, apaga elas
		free(cabeca->normais);
		cabeca->normais_por_vertice = false;
	}
	CalculaNormaisPorFace(cabeca);

	corpo = CarregaObjeto("objetos/corpo.obj",true);
    printf("Objeto carregado!");

	// E calcula o vetor normal em cada face
	if(corpo->normais) {
		// Se já existirem normais no arquivo, apaga elas
		free(corpo->normais);
		corpo->normais_por_vertice = false;
	}
	CalculaNormaisPorFace(corpo);

	banco = CarregaObjeto("objetos/stool.obj",true);
    printf("Objeto carregado!");

	// E calcula o vetor normal em cada face
	if(banco->normais) {
		// Se já existirem normais no arquivo, apaga elas
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
	// Define do modo de operação da GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// Especifica a posição inicial da janela GLUT
	glutInitWindowPosition(100, 100);
	// Especifica o tamanho inicial em pixels da janela GLUT
	glutInitWindowSize(800, 600);
	// Cria a janela passando como argumento o título
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
	glutMotionFunc(gerenciaMovimentoMouse);
	// Registra a função callback que executa a animação
	glutIdleFunc(cria_animacao);
	// Chama a função responsável por fazer as inicializações
	inicializa();
    // Inicia o processamento e aguarda interações do usuário
	glutMainLoop();

	return 0;
}
