#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
using namespace std;

/* 
   Can Yarar 200401069 @2022
   freeGLUT kutuphanesi ile yazilmistir
   1. oyuncu "w, s" tuslari
   2. oyuncu "yukari, asagi" ok tuslari ile oyunu kontrol eder
*/


// Pencere cozunurlugu
short int GENISLIK = 800;
short int YUKSEKLIK = 400;

// Topun her seferinde kac birim ilerleyecegini belirler
// UYARI: Bu degerin degistirilmesi oyun fizigini bozabilir
unsigned char TOPHIZ = 1;

// Oyun fiziginin kac ms'de bir calisacagini belirler
// UYARI: Dusuk degerler icin daha fazla performans gerekir
unsigned char OYUNHIZ = 5;

// Oyuncularin baslangic yukseklik degerlerini
// NOT: Oynanabilir alan [GENISLIK/32, YUKSEKLIK - GENISLIK/32]'dir
short int Oyuncu1Yt = YUKSEKLIK/2 - 25;
short int Oyuncu2Yt = YUKSEKLIK/2 - 25;

// Oyuncularin bir tus basiminda hareket ettigi birim sayisi
unsigned char OYUNCUHAREKET = 10;

// Oyuncunun uzunlugunu belirler
short int OYUNCUBOY = 100;

// Topun ilk konumunu tanimlar
short int Topust = YUKSEKLIK/2 + 10;
short int Topalt = YUKSEKLIK/2 - 10;
short int Topsol = GENISLIK/2 - 10;
short int Topsag = GENISLIK/2 + 10;

// Top ve kare rengi
char mavi = 1;
char kirmizi = 0;

// Topun hangi yonlere gidecegini belirler
char yonx = 1; // "1" saga, "-1" sola
char yony = 1; // "1" yukari, "-1" asagi

// Oyuncularin gol sayisini tutar
// 9. golu atan ilk oyuncu kazanir
unsigned char SKOR[] = "0:0";

// Oyun bitim mesaji
unsigned char SONUC[] = " . OYUNCU KAZANDI!";

// 1. oyuncu icin klavye kontrolleri
void Tus_Al(unsigned char key, int x, int y) {
	switch (key) {
		case 119: // "w" tusu, oyuncuyu yukari ilerletir
			Oyuncu1Yt += OYUNCUHAREKET;
			glutPostRedisplay();
			break;

		case 87: // "W" tusu
			Oyuncu1Yt += OYUNCUHAREKET;
			glutPostRedisplay();
			break;

		case 115: // "s" tusu, oyuncuyu asagi ilerletir
			Oyuncu1Yt -= OYUNCUHAREKET;
			glutPostRedisplay();
			break;

		case 83: // "S" tusu
			Oyuncu1Yt -= OYUNCUHAREKET;
			glutPostRedisplay();
			break;
	}
}

// 2. oyuncu icin klavye kontrolleri
void Tus_Al2(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP: // yukari yon tusu, oyuncuyu asagi ilerletir
			Oyuncu2Yt += OYUNCUHAREKET;
			glutPostRedisplay();
			break;

		case GLUT_KEY_DOWN: // asagi yon tusu, oyuncuyu asagi ilerletir
			Oyuncu2Yt -= OYUNCUHAREKET;
			glutPostRedisplay();
			break;
	}
}

void Ekran() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f(0, 0, 0); // Orta kare
	glBegin(GL_QUADS);
	glVertex2i(GENISLIK / 32, GENISLIK / 32); // sol alt
	glVertex2i(GENISLIK - GENISLIK / 32, GENISLIK / 32); // sag alt
	glVertex2i(GENISLIK - GENISLIK / 32, YUKSEKLIK - GENISLIK / 32); // sag ust
	glVertex2i(GENISLIK / 32, YUKSEKLIK - GENISLIK / 32); // sol ust
	glEnd();
	
	
	glColor3f(kirmizi, 0, mavi); // Sol alt kare
	glBegin(GL_QUADS);
	glVertex2i(0, 0); // sol alt
	glVertex2i(GENISLIK/32, 0); // sag alt
	glVertex2i(GENISLIK/32, GENISLIK/32); // sag ust
	glVertex2i(0, GENISLIK/32); // sol alt
	glEnd();

	glColor3f(kirmizi, 0, mavi); // Sol ust kare
	glBegin(GL_QUADS);
	glVertex2i(0, YUKSEKLIK);
	glVertex2i(GENISLIK / 32, YUKSEKLIK);
	glVertex2i(GENISLIK / 32, YUKSEKLIK - GENISLIK / 32);
	glVertex2i(0, YUKSEKLIK - GENISLIK / 32);
	glEnd();

	glColor3f(kirmizi, 0, mavi); // Sag ust kare
	glBegin(GL_QUADS);
	glVertex2i(GENISLIK, YUKSEKLIK);
	glVertex2i(GENISLIK - GENISLIK / 32, YUKSEKLIK);
	glVertex2i(GENISLIK - GENISLIK / 32, YUKSEKLIK - GENISLIK / 32);
	glVertex2i(GENISLIK, YUKSEKLIK - GENISLIK / 32);
	glEnd();

	glColor3f(kirmizi, 0, mavi); // Sag alt kare
	glBegin(GL_QUADS);
	glVertex2i(GENISLIK, 0); // sag alt
	glVertex2i(GENISLIK, GENISLIK / 32); // sag ust
	glVertex2i(GENISLIK - GENISLIK / 32, GENISLIK / 32); // sol ust
	glVertex2i(GENISLIK - GENISLIK / 32, 0); // sol alt
	glEnd();

	glColor3f(1, 1, 1); // Orta cizgi
	glBegin(GL_LINE_STRIP);
	glVertex2i(GENISLIK/2, GENISLIK/32); // ust
	glVertex2i(GENISLIK/2, YUKSEKLIK - GENISLIK / 32); // alt
	glEnd();

	glColor3f(kirmizi, 0, mavi); // Top
	glBegin(GL_QUADS);
	glVertex2i(Topsol, Topust); // sol ust
	glVertex2i(Topsol, Topalt); // sol alt
	glVertex2i(Topsag, Topalt); // sag alt
	glVertex2i(Topsag, Topust); // sag ust
	glEnd();

	// Oyuncu1
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex2i(GENISLIK/32 + 10, Oyuncu1Yt + OYUNCUBOY); // sol ust
	glVertex2i(GENISLIK/32 + 10, Oyuncu1Yt); // sol alt
	glVertex2i(GENISLIK/32 + 20, Oyuncu1Yt); // sag alt
	glVertex2i(GENISLIK/32 + 20, Oyuncu1Yt + OYUNCUBOY); // sag ust
	glEnd();

	// Oyuncu2
	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex2i(GENISLIK - GENISLIK/32 - 20, Oyuncu2Yt + OYUNCUBOY); // sol ust
	glVertex2i(GENISLIK - GENISLIK/32 - 20, Oyuncu2Yt); // sol alt
	glVertex2i(GENISLIK - GENISLIK/32 - 10, Oyuncu2Yt); // sag alt
	glVertex2i(GENISLIK - GENISLIK/32 - 10, Oyuncu2Yt + OYUNCUBOY); // sag ust
	glEnd();

	// Skor
	glRasterPos2i(GENISLIK/2 - 13, YUKSEKLIK - 19);
	glColor3f(0,1,0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, SKOR);

	glutPostRedisplay();
	glutSwapBuffers();
}

void Top_Sifirlama() {
	Sleep(250); // Gol oldugunda aniden yeniden baslamasini engeller
	char x = (rand() % 51) - 25; // Her seferinde ayni konumda baslamasini engeller
	Topust = YUKSEKLIK / 2 + 10 + x;
	Topalt = YUKSEKLIK / 2 - 10 + x;
	Topsol = GENISLIK / 2 - 10 + x;
	Topsag = GENISLIK / 2 + 10 + x;
	return;
}

void Kazanma_Mesaji() {
	// Kazanani ekrana yazdirdiktan sonra 5 saniye bekler
	glRasterPos2i(GENISLIK / 2 - 104, YUKSEKLIK - 80);
	glColor3f(0, 1, 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, SONUC);
	glutPostRedisplay();
	glutSwapBuffers();
	Sleep(5000);
	// Skoru sifirlar
	SKOR[0] = 48;
	SKOR[2] = 48;
}

void Kazanma_Kontrol() {
	if (SKOR[0] == 57) {
		SONUC[0] = 49;
		Kazanma_Mesaji();
	}
	if (SKOR[2] == 57) {
		SONUC[0] = 50;
		Kazanma_Mesaji();
	}
}

void Gol_Kontrol() {
	if (Topsol == GENISLIK / 32) {
		yonx = -yonx;
		SKOR[2]++;
		Top_Sifirlama();
		mavi = 1, kirmizi = 0;
		Kazanma_Kontrol();
	}
	else if (Topsag == GENISLIK - GENISLIK / 32) {
		yonx = -yonx;
		SKOR[0]++;
		Top_Sifirlama();
		mavi = 0, kirmizi = 1;
		Kazanma_Kontrol();
	}
}

void Oyuncu_Carpma_Kontrol() {
	if (Oyuncu1Yt < GENISLIK / 32) {
		Oyuncu1Yt = GENISLIK / 32; 
	}
	else if (Oyuncu1Yt + OYUNCUBOY > YUKSEKLIK - GENISLIK / 32) {
		Oyuncu1Yt = YUKSEKLIK - GENISLIK / 32 - OYUNCUBOY;
	}
	if (Oyuncu2Yt < GENISLIK / 32) {
		Oyuncu2Yt = GENISLIK / 32;
	}
	else if (Oyuncu2Yt + OYUNCUBOY > YUKSEKLIK - GENISLIK / 32) {
		Oyuncu2Yt = YUKSEKLIK - GENISLIK / 32 - OYUNCUBOY;
	}
}

void Oyuncuya_Carpma_Kontrol() {
	if (Topsol == GENISLIK / 32 + 20) {
		if ((Oyuncu1Yt + OYUNCUBOY > Topalt && Topalt > Oyuncu1Yt) || (Oyuncu1Yt + OYUNCUBOY > Topust && Topust > Oyuncu1Yt)) {
			yonx = -yonx;
			yony = -yony;
			return;
		}
		return;
	}
	if (Topsag == GENISLIK - GENISLIK / 32 - 20) {
		if ((Oyuncu2Yt + OYUNCUBOY > Topalt && Topalt > Oyuncu2Yt) || (Oyuncu2Yt + OYUNCUBOY > Topust && Topust > Oyuncu2Yt)) {
			yonx = -yonx;
			yony = -yony;
			return;
		}
	}
}

void Kenar_Carpma_Kontrol() {
	if (Topust == YUKSEKLIK - GENISLIK / 32 || Topalt == GENISLIK / 32) {
		yony = -yony;
	}
}

void Top_Hareketi() {
	Topsol += yonx;
	Topsag += yonx;
	Topust += yony;
	Topalt += yony;
}

void Oyun_Mantigi(int x) {
	// Gol olup olmadiginin kontrol eder
	// Kazanan varsa ekrana yazar ve oyunu sifirlar
	// Sadece gol varsa topun rengini ve kutucuklari ataninkine degistirir ve konumunu sifirlar
	Gol_Kontrol();

	// Oyuncunun duvara carpip carpmadiginin kontrolu
	Oyuncu_Carpma_Kontrol();

	// Oyunculara carpip carpmadiginin kontrolu
	Oyuncuya_Carpma_Kontrol();

	// Tavan ya da tabana carpma kontrolu
	Kenar_Carpma_Kontrol();

	// Top hareketi
	Top_Hareketi();

	glutPostRedisplay();
	glutTimerFunc(OYUNHIZ, Oyun_Mantigi, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(-1, -1); //Olusturulan ekranin pencere konumu
	glutInitWindowSize(GENISLIK, YUKSEKLIK); //Olusturulan ekranin cozunurlugu
	glutCreateWindow("Ping-Pong");

	glutKeyboardFunc(Tus_Al);
	glutSpecialFunc(Tus_Al2);
	glutTimerFunc(OYUNHIZ, Oyun_Mantigi, 0);
	glutDisplayFunc(Ekran);
	glClearColor(0, 1, 1, 1);
	gluOrtho2D(0.0, GENISLIK, 0.0, YUKSEKLIK);

	glutMainLoop();
	return 0;
}