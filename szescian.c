//////////////////////////////////////////////////////////////////////////////////////////
//
// Program wyswietlajacy szescian w rzucie perspektywicznym. Dlugosc boku szescianu
// moze byc interaktywnie zmieniana za pomoca klwiszy '+' i '-'.
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <GL/glut.h>
#include <math.h>
#include "materials.h"



// Definicja stalych
#define DLUGOSC_BOKU            5.0
#define OBSERWATOR_ODLEGLOSC    130.0
#define OBSERWATOR_OBROT_X      20.0
#define OBSERWATOR_OBROT_Y      20.0
#define OBSERWATOR_FOV_Y        30.0  
#define OBSERWATOR_OBROT_Z      0

// Definicja stalych
#define X_OFFSET_SWIATLO    10
#define Y_OFFSET_SWIATLO    120
#define X_OFFSET_OBIEKT     10
#define ID_MENU_SWIATLA     0
#define LPOZ_MENU_SWIATLA   10
#define OBSERWATOR_FOV_Y        30.0
#define odlmin 20.0
#define odlmax 1000.0
#define DEG2RAD(x) ((float)(x)*M_PI/180.0)

//definiowanie N
#define N 8

//definiowanie zmiennych podstawowych
#define M_PI 3.1415926535
#define DEG2RAD(x) ((float)(x)*M_PI/180.0)

//definiowanie zmiennych do stozka
#define dAlfa (360/en)
#define PROMIEN 4
#define WYSOKOSC 4

//zmienne do pierscienia
#define H_PIERSCIENIA			2.59807621135
#define R_WEW_PIERSCIENIA		15
#define R_ZEW_PIERSCIENIA		18

int sIndeks;           // Wybrana pozycja w tablicy parametrow swiatla
int mIndeks;           // Wybrana pozycja w tablicy parametrow materialu
int change_material = 1;
int menu;              // Identyfikator wybranego menu (menu swiatla lub menu materialu)

//zestaw 10

// Zmienne globalne
double  bok = DLUGOSC_BOKU; // Dlugosc boku szescianu
int     szerokoscOkna = 800;
int     wysokoscOkna = 600;

double rWewPie = R_WEW_PIERSCIENIA;
double rZewPie = R_ZEW_PIERSCIENIA;
double hPie = H_PIERSCIENIA;

//zmkienne do ogleg³osci
float odlmin = 200;
float odlmax = -200;

//zmienne potrzebne do oblugi klawiszy
double os_x = OBSERWATOR_OBROT_X;
double os_y = OBSERWATOR_OBROT_Y;
double os_z = OBSERWATOR_OBROT_Z;
double os_o = OBSERWATOR_ODLEGLOSC;

//zmienne do stozka
double h = WYSOKOSC;
double r = PROMIEN;
double en = N;

int i;

// Prototypy funkcji
void RysujSzescian(double a);
void UstawParametryWidoku(int szer, int wys);
void WyswietlObraz(void);
void ObslugaKlawiatury(unsigned char klawisz, int x, int y);

void Stozek(double r);



//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja rysujaca szescian o boku "a" w trybie GL_QUAD_STRIP.
// Srodek szescianu znajduje siê w punkcie (0,0,0).
/*
void RysujSzescian(double a)
{
  // Sciany boczne
  glBegin(GL_QUAD_STRIP);
	glVertex3f(a/2.0, a/2.0, a/2.0);
	glVertex3f(a/2.0, -a/2.0, a/2.0);
	glVertex3f(a/2.0, a/2.0, -a/2.0);
	glVertex3f(a/2.0, -a/2.0, -a/2.0);

	glVertex3f(-a/2.0, a/2.0, -a/2.0);
	glVertex3f(-a/2.0, -a/2.0, -a/2.0);

	glVertex3f(-a/2.0, a/2.0, a/2.0);
	glVertex3f(-a/2.0, -a/2.0, a/2.0);

	glVertex3f(a/2.0, a/2.0, a/2.0);
	glVertex3f(a/2.0, -a/2.0, a/2.0);
  glEnd();

  // Gorna sciana
  glBegin(GL_QUAD_STRIP);
	glVertex3f(-a/2.0, a/2.0, a/2.0);
	glVertex3f(a/2.0, a/2.0, a/2.0);
	glVertex3f(-a/2.0, a/2.0, -a/2.0);
	glVertex3f(a/2.0, a/2.0, -a/2.0);
  glEnd();

  // Dolna sciana
  glBegin(GL_QUAD_STRIP);
	glVertex3f(-a/2.0, -a/2.0, a/2.0);
	glVertex3f(a/2.0, -a/2.0, a/2.0);
	glVertex3f(-a/2.0, -a/2.0, -a/2.0);
	glVertex3f(a/2.0, -a/2.0, -a/2.0);
  glEnd();
}
*/

//WALEC

/*
void Stozek(double r) {
	//dol
	glColor3f(0, 1, 1);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);
	for (i = 0; i*dAlfa <= 360; i++)
	{
		glVertex3f(r*cos(DEG2RAD(i*dAlfa)), 0, r*sin(DEG2RAD(i*dAlfa)));
	}
	glEnd();

	//srodek
	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLE_STRIP);
	for (i = 0; i*dAlfa <= 360.0; i++)
	{
		glVertex3f(r*cos(DEG2RAD(i*dAlfa)), 0, r*sin(DEG2RAD(i*dAlfa)));
		glVertex3f(r*cos(DEG2RAD(i*dAlfa)), h, r*sin(DEG2RAD(i*dAlfa)));
	}
	glEnd();

	//drugi dol
	glColor3f(0, 1, 1);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, h, 0);
	for (i = 0; i*dAlfa <= 360; i++)
	{
		glVertex3f(r*cos(DEG2RAD(i*dAlfa)), h, r*sin(DEG2RAD(i*dAlfa)));
	}
	glEnd();
}
*/


//PIERSCIEN
void Pierscien(double l) {


	//podstawa
	glColor3f(0, 1, 0);
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i*dAlfa <= 360.0; i++)
	{
		glVertex3f(l*cos(DEG2RAD(i*dAlfa)), 0, l*sin(DEG2RAD(i*dAlfa)));
		glVertex3f((l+3)*cos(DEG2RAD(i*dAlfa)), 0, (l+3)*sin(DEG2RAD(i*dAlfa)));
	}
	glEnd();

		//wektor podstawy
		glColor3f(1, 1, 0);
		glBegin(GL_LINES);
		for (i = 0; i*dAlfa <= 360.0; i++)
		{
			glVertex3f(l*cos(DEG2RAD(i*dAlfa)), 0, l*sin(DEG2RAD(i*dAlfa)));
			glVertex3f(l*cos(DEG2RAD(i*dAlfa)), -1, l*sin(DEG2RAD(i*dAlfa)));
		}
		glEnd();

	//sciana wew
	glColor3f(0, 1, 1);
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i*dAlfa <= 360.0; i++)
	{
		glVertex3f(l*cos(DEG2RAD(i*dAlfa)), 0, l*sin(DEG2RAD(i*dAlfa)));
		glVertex3f((l+1.5)*cos(DEG2RAD(i*dAlfa)), hPie, (l+1.5)*sin(DEG2RAD(i*dAlfa)));
	}
	glEnd();

		//wektor sciany wew
		glColor3f(0, 1, 0.5);
		glBegin(GL_LINES);
		for (i = 0; i*dAlfa <= 360.0; i++)
		{
			glVertex3f((l + 1.5)*cos(DEG2RAD(i*dAlfa)), hPie, (l + 1.5)*sin(DEG2RAD(i*dAlfa)));
			glVertex3f((l+3)*cos(DEG2RAD(i*dAlfa)), 3/cos(DEG2RAD(30)) , (l + 3)*sin(DEG2RAD(i*dAlfa)));
		}
		glEnd();
	
	//sciana zew
	glColor3f(1, 0, 0);
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i*dAlfa <= 360.0; i++)
	{
		glVertex3f((l+3)*cos(DEG2RAD(i*dAlfa)), 0, (l+3)*sin(DEG2RAD(i*dAlfa)));
		glVertex3f((l+1.5)*cos(DEG2RAD(i*dAlfa)), hPie, (l+1.5)*sin(DEG2RAD(i*dAlfa)));
	}
	glEnd();

		//wektor sciany zew
		glColor3f(0.5, 0.5, 0.8);
		glBegin(GL_LINES);
		for (i = 0; i*dAlfa <= 360.0; i++)
		{
			glVertex3f((l + 1.5)*cos(DEG2RAD(i*dAlfa)), hPie, (l + 1.5)*sin(DEG2RAD(i*dAlfa)));
			glVertex3f(l*cos(DEG2RAD(i*dAlfa)), 3 / cos(DEG2RAD(30)), l*sin(DEG2RAD(i*dAlfa)));
		}
		glEnd();
	
}


void Uklad()
{
	// Pocztaek tworzenia ukladu wspolrzednych
	glBegin(GL_LINES);

	// Os X
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-180.0, 0.0, 0.0);
	glVertex3f(180.0, 0.0, 0.0);

	// Os Y
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -180.0, 0.0);
	glVertex3f(0.0, 180.0, 0.0);

	// Os Z
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -180.0);
	glVertex3f(0.0, 0.0, 180.0);

	// Koniec tworzenia ukladu wspolrzednych


	glEnd();

}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja ustawiajaca parametry rzutu perspektywicznego i rozmiary viewportu. Powinna
// byæ wywolywana kazdorazowo po zmianie rozmiarow okna programu.
void UstawParametryWidoku(int szer, int wys)
{
	// Zapamietanie wielkosci widoku
	szerokoscOkna = szer;
	wysokoscOkna = wys;

	// Ustawienie parametrow viewportu
	glViewport(0, 0, szerokoscOkna, wysokoscOkna);

	// Przejscie w tryb modyfikacji macierzy rzutowania
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(OBSERWATOR_FOV_Y, (float)szerokoscOkna / (float)wysokoscOkna, 1.0, 1000.0);
}


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja wyswietlajaca pojedyncza klatke animacji
void WyswietlObraz(void)
{
	// Wyczyszczenie bufora koloru i bufora glebokosci
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Przejscie w tryb modyfikacji macierzy przeksztalcen geometrycznych
	glMatrixMode(GL_MODELVIEW);

	// Zastapienie aktywnej macierzy macierza jednostkowa
	glLoadIdentity();


	// Ustalenie polozenia obserwatora
	glTranslatef(0, 0, -os_o);
	glRotatef(os_x, 1, 0, 0);
	glRotatef(os_y, 0, 1, 0);
	glRotatef(os_z, 0, 0, 1);


	//rysowanie ukladu
	Uklad();

	// Narysowanie szescianu
   // RysujSzescian(bok);

	//rysowanie stozka 
	//Stozek(r);

	Pierscien(rWewPie);




	// Przelaczenie buforow ramki
	glutSwapBuffers();
}




//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja obslugi klawiatury
void ObslugaKlawiatury(unsigned char klawisz, int x, int y)
{
	//MODE
	if (klawisz == 'f')
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (klawisz == 'l')
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (klawisz == '+')
		bok *= 2.0;
	else if (klawisz == '-')
		bok /= 2.0;
	else if (klawisz == 27)
		exit(0);

	//obroty osi
	if (klawisz == 'a')
		os_y--;
	if (klawisz == 'd')
		os_y++;
	if (klawisz == 's')
		os_x++;
	if (klawisz == 'w')
		os_x--;
	if (klawisz == 'z')
		os_z++;
	if (klawisz == 'x')
		os_z--;

	//przyblizanie
	if (klawisz == '-')
		os_o = (os_o < odlmin) ? os_o + 3.0 : os_o;
	if (klawisz == '+')
		os_o = (os_o > odlmax) ? os_o - 3.0 : os_o;

	/* 
	//N dla stozka
	if (klawisz == 'q')
		if (en < 64)
			en = en + 1;
	if (klawisz == 'e')
		if (en > 4)
			en = en - 1;
	*/
		//N dla pierscienia
	if (klawisz == 'q')
		if (en < 64)
			en = en + 2;
	if (klawisz == 'e')
		if (en > 4)
			en = en - 2;

}


//////////////////////////////////////////////////////////////////////////////////////////
// Glowna funkcja programu
int  main(int argc, char **argv)
{
	// Zainicjowanie biblioteki GLUT
	glutInit(&argc, argv);

	// Ustawienie trybu wyswietlania
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Ustawienie polozenia dolenego lewego rogu okna
	glutInitWindowPosition(100, 100);

	// Ustawienie rozmiarow okna
	glutInitWindowSize(szerokoscOkna, wysokoscOkna);

	// Utworzenie okna
	glutCreateWindow("Szescian");

	// Odblokowanie bufora glebokosci
	glEnable(GL_DEPTH_TEST);

	// Ustawienie wartosci czyszczacej zawartosc bufora glebokosci
	glClearDepth(1000.0);

	// Ustawienie koloru czyszczenia bufora ramki
	glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

	// Wlaczenie wyswietlania wielokatow w postaci obrysow (przydatne w celach diagnostycznych).
	glPolygonMode(GL_FRONT_AND_BACK, GL_SMOOTH);

	// Zarejestrowanie funkcji (callback) odpowiedzialnej za 
	glutDisplayFunc(WyswietlObraz);

	// Zarejestrowanie funkcji (callback) wywolywanej za kazdym razem kiedy
	// zmieniane sa rozmiary okna
	glutReshapeFunc(UstawParametryWidoku);

	// Zarejestrowanie funkcji wykonywanej gdy okno nie obsluguje
	// zadnych zadan
	glutIdleFunc(WyswietlObraz);

	// Zarejestrowanie funkcji obslugi klawiatury
	glutKeyboardFunc(ObslugaKlawiatury);

	// Obsluga glownej petli programu (wywolywanie zarejestrowanych callbackow
	// w odpowiedzi na odbierane zdarzenia lub obsluga stanu bezczynnosci)
	glutMainLoop();

	return 0;
}