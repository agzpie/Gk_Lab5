//////////////////////////////////////////////////////////////////////////////////////////
//
// Program wyswietlajacy szescian w rzucie perspektywicznym. Dlugosc boku szescianu
// moze byc interaktywnie zmieniana za pomoca klwiszy '+' i '-'.
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include "materials.h"

// Definicja stalych
#define X_OFFSET_SWIATLO    10
#define Y_OFFSET_SWIATLO    120
#define X_OFFSET_OBIEKT     10
#define ID_MENU_SWIATLA     0
#define LPOZ_MENU_SWIATLA   10
#define OBSERWATOR_FOV_Y    30.0
#define odlmin 20.0
#define odlmax 1000.0
#define DEG2RAD(x) ((float)(x)*M_PI/180.0)

// Zmienne globalne
int sIndeks;           // Wybrana pozycja w tablicy parametrow swiatla
int mIndeks;           // Wybrana pozycja w tablicy parametrow materialu
int change_material = 1;
int menu;              // Identyfikator wybranego menu (menu swiatla lub menu materialu)
int szerokoscOkna = 800;
int wysokoscOkna = 600;
//int h = 2;  //Wysokosc walca
int a = 5;  //Bok kwadratu pierscienia
int R = 15; //Promien pierscienia
char typ_materialu[100] = "Zloto";
char tryb_cieniowania[100] = "SMOOTH";
char tryb_modelowania[100] = "FILL";
char stan_swiatlo1[100] = "OFF";
char stan_swiatlo2[100] = "OFF";
char stan_wektory[100] = "OFF";
GLfloat rotObsZ = 0.0;
GLfloat rotObsY = 20.0;
GLfloat rotObsX = 20.0;
GLfloat odleglosc = 60.0;
GLdouble dAlfa = 360.0 / 90.0;
GLdouble N = 360.0 / 5.625;
int r = 30; //Promien orbity reflektora
GLdouble obrot_reflektora = 0.0;
GLdouble szybkosc = 1.0;
GLdouble nachylenie_refletkora_OX = 0.0;

// Zerowy parametr Emission
const GLfloat Emission[4] = {0.0, 0.0, 0.0, 0.0};

GLfloat param_materialu_1[5][4] = { //Brazowy blyszczacy
        {0.4,  0.2, 0.06, 1.0},  // [0] wspolczynnik odbicia swiatla otoczenia
        {0.4,  0.2, 0.06, 1.0},  // [1] wspolczynnik odbicia swiatla rozproszonego
        {1.0,  1.0, 1.0,  1.0},  // [2] wspolczynnik odbicia swiatla lustrzanego
        {80.0, 0.0, 0.0,  0.0}  // [3] polysk
};

GLfloat param_materialu_2[5][4] = { //Czerwony matowy
        {0.1,  0.0, 0.0, 1.0},  // [0] wspolczynnik odbicia swiatla otoczenia
        {1.0,  0.0, 0.0, 1.0},  // [1] wspolczynnik odbicia swiatla rozproszonego
        {0.0,  0.0, 0.0, 0.0},  // [2] wspolczynnik odbicia swiatla lustrzanego
        {10.0, 0.0, 0.0, 0.0}  // [3] polysk
};

// Tablica parametrow swiatla
GLfloat swiatlo1[10][4];

GLfloat pozycja_swiatla1[4] = {0.0, 0.0, 0.0, 0.0};

GLfloat skierowanie_swiatla1[4] = {0.0, 0.0, 0.0, 0.0};

GLfloat swiatlo2[10][4];

// Tablica parametrow materialu z jakiego wykonany jest walec
GLfloat material[6][4];

// Prototypy funkcji
void UstawDomyslneWartosciParametrow(void);

void RysujTekstRastrowy(void *font, char *tekst);

void WlaczOswietlenie(void);

void DefiniujMaterial(void);

void RysujNakladke(void);

void rysuj_figure();

void UstawParametryWidoku(int szer, int wys);

void WyswietlObraz(void);

void ObslugaKlawiatury(unsigned char klawisz, int x, int y);

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja ustawiajaca domyslne parametry walca, materialu i zrodla swiatla
void UstawDomyslneWartosciParametrow(void) {

    // Tablica parametrow swiatla
    GLfloat param_swiatla_1[10][4] = {  //Reflektor
            {1.0, 1.0,  1.0, 1.0},  // [0] otoczenie
            {1.0, 1.0,  1.0, 1.0},  // [1] rozproszenie
            {1.0, 1.0,  1.0, 1.0},  // [2] lustrzane
            {0.0, 2.5,  0.0, 1.0},  // [3] polozenie x,z,y
            {-90, -2.5, 0.0, -1.0}}; // [4] kierunek swiecenia

    GLfloat param_swiatla_2[10][4] = {  //Kierunkowe
            {1.0,  1.0,  1.0,  0.5},  // [0] otoczenie
            {1.0,  1.0,  1.0,  1.0},  // [1] rozproszenie
            {1.0,  1.0,  1.0,  1.0},  // [2] lustrzane
            {10.0, 10.0, 10.0, 0.0}};  // [3] polozenie

    // Skopiowanie zawartosci tablic param_* do tablic globalnych
    memcpy(swiatlo1, param_swiatla_1, LPOZ_MENU_SWIATLA * 4 * sizeof(GLfloat));
    memcpy(swiatlo2, param_swiatla_2, LPOZ_MENU_SWIATLA * 4 * sizeof(GLfloat));

    // Skopiowanie W�a�ciwo�ci materia�u - z�oto
    memcpy(material[0], param_materialu_1[0], 4 * sizeof(GLfloat));
    memcpy(material[1], param_materialu_1[1], 4 * sizeof(GLfloat));
    memcpy(material[2], param_materialu_1[2], 4 * sizeof(GLfloat));
    memcpy(material[3], &param_materialu_1[3][0], 4 * sizeof(GLfloat));
    memcpy(material[4], Emission, 4 * sizeof(GLfloat));
    strcpy(typ_materialu, "Fioletowy blyszczacy");

    // Pozostale parametry
    sIndeks = 0;        // Wybrana pozycja w tablicy parametrow swiatla
    mIndeks = 0;        // Wybrana pozycja w tablicy parametrow materialu
    menu = ID_MENU_SWIATLA;

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja rysujaca tekst rastrowy <tekst> za pomca fontow bitmapowych <font>.Rysowanie
// tekstu wykonywane jest poczawszy od biezacej pozycji ekranu.
void RysujTekstRastrowy(void *font, char *tekst) {
    int i;

    for (i = 0; i < (int) strlen(tekst); i++)
        glutBitmapCharacter(font, tekst[i]);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Wlaczenie  oswietlenia sceny
void WlaczOswietlenie(void) {
    // Odblokowanie oswietlenia
    glEnable(GL_LIGHTING);

    pozycja_swiatla1[0] = r * cos(DEG2RAD(obrot_reflektora));
    pozycja_swiatla1[1] = a/2;
    pozycja_swiatla1[2] = r * sin(DEG2RAD(obrot_reflektora));
    skierowanie_swiatla1[0] = -r * cos(DEG2RAD(obrot_reflektora));
    skierowanie_swiatla1[1] = a/3;
    skierowanie_swiatla1[2] = -r * sin(DEG2RAD(obrot_reflektora));

    // Inicjowanie zrodla swiatla1
    glLightfv(GL_LIGHT0, GL_AMBIENT, swiatlo1[0]);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, swiatlo1[1]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, swiatlo1[2]);
    glLightfv(GL_LIGHT0, GL_POSITION, pozycja_swiatla1);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, skierowanie_swiatla1);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 25);


    glLightfv(GL_LIGHT1, GL_AMBIENT, swiatlo2[0]);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, swiatlo2[1]);
    glLightfv(GL_LIGHT1, GL_SPECULAR, swiatlo2[2]);
    glLightfv(GL_LIGHT1, GL_POSITION, swiatlo2[3]);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Zdefiniowanie walasciwosci materialu walca na podstawie zapisanych w tablcy 'material'
// parametrow (material obowiazuje tylko do scian skierowanych przodem do obserwatora)
void DefiniujMaterial(void) {
    switch (change_material) {
        case 1:
            memcpy(material[0], param_materialu_1[0], 4 * sizeof(GLfloat));
            memcpy(material[1], param_materialu_1[1], 4 * sizeof(GLfloat));
            memcpy(material[2], param_materialu_1[2], 4 * sizeof(GLfloat));
            memcpy(material[3], &param_materialu_1[3][0], 4 * sizeof(GLfloat));
            strcpy(typ_materialu, "Brazowy blyszczacy");
            break;
        case 2:
            memcpy(material[0], param_materialu_2[0], 4 * sizeof(GLfloat));
            memcpy(material[1], param_materialu_2[1], 4 * sizeof(GLfloat));
            memcpy(material[2], param_materialu_2[2], 4 * sizeof(GLfloat));
            memcpy(material[3], &param_materialu_2[3][0], 4 * sizeof(GLfloat));
            strcpy(typ_materialu, "Czerwony matowy");
            break;
        case 3:
            memcpy(material[0], PolishedBronzeAmbient, 4 * sizeof(GLfloat));
            memcpy(material[1], PolishedBronzeDiffuse, 4 * sizeof(GLfloat));
            memcpy(material[2], PolishedBronzeSpecular, 4 * sizeof(GLfloat));
            memcpy(material[3], &PolishedBronzeShininess, 4 * sizeof(GLfloat));
            strcpy(typ_materialu, "Polerowany braz");
            break;
        default:
            exit(1);
    }
    glMaterialfv(GL_FRONT, GL_AMBIENT, material[0]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material[1]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material[2]);
    glMaterialfv(GL_FRONT, GL_SHININESS, material[3]);
    glMaterialfv(GL_FRONT, GL_EMISSION, material[4]);
}

//////////////////////////////////////////////////////////////////////////////////////////
void rysuj_figure() {
    int i;
    glDisable(GL_LIGHTING);

    // Pocztaek tworzenia ukladu wspolrzednych
    glBegin(GL_LINES);

    // Os X
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-20.0, 0.0, 0.0);
    glVertex3f(20.0, 0.0, 0.0);

    // Os Y
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, -20.0, 0.0);
    glVertex3f(0.0, 20.0, 0.0);

    // Os Z
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, -20.0);
    glVertex3f(0.0, 0.0, 20.0);

    // Koniec tworzenia ukladu wspolrzednych
    glEnd();

    glEnable(GL_LIGHTING);

    /*
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
    glTranslatef(10.0,0.0,0.0);
    */
    /*
    //Walec
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLE_FAN);   //Dolna podstawa
    glVertex3f(0.0, 0.0, 0.0);
    for (i = 0; i * dAlfa <= 360.0; i++) {
        glVertex3f(r * cos(DEG2RAD(i * dAlfa)),
                   0.0, r * sin(DEG2RAD(i * dAlfa)));
    }
    glEnd();

    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_TRIANGLE_STRIP); //Boki
    for (i = 0; i * dAlfa <= 360.0; i++) {
        glVertex3f(r * cos(DEG2RAD((i + 1) * dAlfa)),
                   0.0, r * sin(DEG2RAD((i + 1) * dAlfa)));   //V0
        glVertex3f(r * cos(DEG2RAD(i * dAlfa)),
                   0.0, r * sin(DEG2RAD(i * dAlfa)));   //V1
        glVertex3f(r * cos(DEG2RAD((i + 1) * dAlfa)),
                   h, r * sin(DEG2RAD((i + 1) * dAlfa))); //V2
        glVertex3f(r * cos(DEG2RAD(i * dAlfa)),
                   h, r * sin(DEG2RAD(i * dAlfa))); //V3
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, h, 0.0);    //Gorna podstawa
    for (i = 0; i * dAlfa <= 360.0; i++) {
        glVertex3f(r * cos(DEG2RAD(i * dAlfa)),
                   h, r * sin(DEG2RAD(i * dAlfa)));
    }
    glEnd();
    glPopMatrix();
     */

    for (i = 0; i * dAlfa <= 360.0; i++) {
        glBegin(GL_QUADS);
        glVertex3f(R * cos(DEG2RAD(i * dAlfa)), //Sciana wewnetrzna
                   0.0, R * sin(DEG2RAD(i * dAlfa)));   //V0
        glVertex3f(R * cos(DEG2RAD(i * dAlfa)),
                   a, R * sin(DEG2RAD(i * dAlfa)));   //V1
        glVertex3f(R * cos(DEG2RAD((i + 1) * dAlfa)),
                   a, R * sin(DEG2RAD((i + 1) * dAlfa))); //V2
        glVertex3f(R * cos(DEG2RAD((i + 1) * dAlfa)),
                   0.0, R * sin(DEG2RAD((i + 1) * dAlfa))); //V3

        glVertex3f((R + a) * cos(DEG2RAD(i * dAlfa)), //Sciana zewnetrzna
                   0.0, (R + a) * sin(DEG2RAD(i * dAlfa)));
        glVertex3f((R + a) * cos(DEG2RAD(i * dAlfa)),
                   a, (R + a) * sin(DEG2RAD(i * dAlfa)));
        glVertex3f((R + a) * cos(DEG2RAD((i + 1) * dAlfa)),
                   a, (R + a) * sin(DEG2RAD((i + 1) * dAlfa)));
        glVertex3f((R + a) * cos(DEG2RAD((i + 1) * dAlfa)),
                   0.0, (R + a) * sin(DEG2RAD((i + 1) * dAlfa)));

        glVertex3f(R * cos(DEG2RAD(i * dAlfa)), //Dolna podstawa
                   0.0, R * sin(DEG2RAD(i * dAlfa)));
        glVertex3f(R * cos(DEG2RAD((i + 1) * dAlfa)),
                   0.0, R * sin(DEG2RAD((i + 1) * dAlfa)));
        glVertex3f((R + a) * cos(DEG2RAD((i + 1) * dAlfa)),
                   0.0, (R + a) * sin(DEG2RAD((i + 1) * dAlfa)));
        glVertex3f((R + a) * cos(DEG2RAD(i * dAlfa)),
                   0.0, (R + a) * sin(DEG2RAD(i * dAlfa)));

        glVertex3f(R * cos(DEG2RAD(i * dAlfa)), //Gorna podstawa
                   a, R * sin(DEG2RAD(i * dAlfa)));
        glVertex3f(R * cos(DEG2RAD((i + 1) * dAlfa)),
                   a, R * sin(DEG2RAD((i + 1) * dAlfa)));
        glVertex3f((R + a) * cos(DEG2RAD((i + 1) * dAlfa)),
                   a, (R + a) * sin(DEG2RAD((i + 1) * dAlfa)));
        glVertex3f((R + a) * cos(DEG2RAD(i * dAlfa)),
                   a, (R + a) * sin(DEG2RAD(i * dAlfa)));
        glEnd();

        glNormal3f(-cos(DEG2RAD(i * dAlfa)), 0, -sin(DEG2RAD(i * dAlfa)));
        glNormal3f(0, 1, 0);
        glNormal3f(cos(DEG2RAD(i * dAlfa)), 0, sin(DEG2RAD(i * dAlfa)));
        glNormal3f(0, -1, 0);

        if (strcmp(stan_wektory, "ON") == 0) {
            glBegin(GL_LINES);
            glColor3f(0.0, 0.0, 0.0);
            glVertex3f(R * cos(DEG2RAD(i * dAlfa)),
                       a, R * sin(DEG2RAD(i * dAlfa)));   //Pierwszy punkt wektora normalnego sciany wewnetrznej
            glVertex3f(R * cos(DEG2RAD(i * dAlfa)) - 2 * cos(DEG2RAD(i * dAlfa)),
                       a, R * sin(DEG2RAD(i * dAlfa)) - 2 * sin(DEG2RAD(i * dAlfa)));
            glColor3f(0.0, 0.0, 0.0);
            glVertex3f((R + a) * cos(DEG2RAD(i * dAlfa)), //Pierwszy punkt wektora normalnego sciany zewnetrznej
                       0.0, (R + a) * sin(DEG2RAD(i * dAlfa)));
            glVertex3f((R + a) * cos(DEG2RAD(i * dAlfa)) + 2 * cos(DEG2RAD(i * dAlfa)),
                       0.0, (R + a) * sin(DEG2RAD(i * dAlfa)) + 2 * sin(DEG2RAD(i * dAlfa)));
            glColor3f(0.0, 0.0, 0.0);
            glVertex3f(R * cos(DEG2RAD(i * dAlfa)), //Pierwszy punkt wektora normalnego dolnej podstawy
                       0.0, R * sin(DEG2RAD(i * dAlfa)));
            glVertex3f(R * cos(DEG2RAD(i * dAlfa)),
                       0.0 - 2.0, R * sin(DEG2RAD(i * dAlfa)));
            glColor3f(0.0, 0.0, 0.0);
            glVertex3f(R * cos(DEG2RAD(i * dAlfa)), //Pierwszy punkt wektora normalnego gornej podstawy
                       a, R * sin(DEG2RAD(i * dAlfa)));
            glVertex3f(R * cos(DEG2RAD(i * dAlfa)),
                       a + 2.0, R * sin(DEG2RAD(i * dAlfa)));
            glEnd();
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja ustawiajaca parametry rzutu perspektywicznego i rozmiary viewportu. Powinna
// by� wywolywana kazdorazowo po zmianie rozmiarow okna programu.
void UstawParametryWidoku(int szer, int wys) {
    // Zapamietanie wielkosci widoku
    szerokoscOkna = szer;
    wysokoscOkna = wys;

    // Ustawienie parametrow viewportu
    glViewport(0, 0, szerokoscOkna, wysokoscOkna);

    // Przejscie w tryb modyfikacji macierzy rzutowania
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(OBSERWATOR_FOV_Y, (float) szerokoscOkna / (float) wysokoscOkna, 1.0, 1000.0);

    // Przejscie w tryb modyfikacji macierzy przeksztalcen geometrycznych
    glMatrixMode(GL_MODELVIEW);

    // Zmiana macierzy znajdujacej sie na wierzcholku stosu na macierz jednostkowa
    glLoadIdentity();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja rysujaca na wygenerowanym obrazie walca nkladke z tekstem opisujacym
// aktualne parametry zrodla swiatla i materialu.
void RysujNakladke(void) {
    char buf[255];

    // Zmiana typu rzutu z perspektywicznego na ortogonalny
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, szerokoscOkna, 0.0, wysokoscOkna, -100.0, 100.0);

    // Modelowanie sceny 2D (zawartosci nakladki)
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Zablokowanie oswietlenia (mialoby ono wplyw na kolor tekstu)
    glDisable(GL_LIGHTING);

    // Okreslenie koloru tekstu
    glColor3f(1.0, 1.0, 1.0);

    sprintf(buf, "i - powoduje wlaczenie/wylaczenie wektorow normalnych %s", stan_wektory);
    glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO + 45);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, "o - powoduje wlaczenie/wylaczenie swiatla nr 1 Stan swiatla1 %s", stan_swiatlo1);
    glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO + 30);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, "p - powoduje wlaczenie/wylaczenie swiatla nr 2 Stan swiatla2 %s", stan_swiatlo2);
    glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO + 15);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, "a i d powoduje rotacje wzgledem osi 0Y");
    glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, "w i s powoduje rotacje wzgledem osi 0X");
    glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 15);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, "q i e powoduje rotacje wzgledem osi 0Z");
    glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 30);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, "3 zmniejsza liczbe podzialow a 4 zwieksza liczbe podzialow");
    glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 45);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, "1 oddala a 2 przybliza obserwatora");
    glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 60);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, "m zmienia model geometryczny - %s", tryb_modelowania);
    glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 75);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, "n zmienia tryb cieniowania - %s", tryb_cieniowania);
    glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 90);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    glColor3f(1.0, 1.0, 1.0);
    sprintf(buf, "[ i ] zmienia material - %s", typ_materialu);
    glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 105);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);


    // RYSOWANIE MENU PARAMETROW WALCA

    glColor3f(1.0, 1.0, 1.0);

    sprintf(buf, "Pierscien:");
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_SWIATLO + 105);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, " - promien (%d)", R);
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_SWIATLO + 90);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, " - wysokosc (%d)", a);
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_SWIATLO + 75);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, " - podzialow pionowych (%f)", N);
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_SWIATLO + 60);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    // Przywrocenie macierzy sprzed wywolania funkcji
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // Odblokowanie oswietlenia
    glEnable(GL_LIGHTING);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja wyswietlajaca pojedyncza klatke animacji
void WyswietlObraz(void) {
    // Wyczyszczenie bufora koloru i bufora glebokosci
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Okreslenie wielkosci widoku, wlaczenie rzutowania perspektywicznego
    // i zainicjowanie stosu macierzy modelowania
    UstawParametryWidoku(szerokoscOkna, wysokoscOkna);

    // Ustawienie oswietlenia sceny (polozenie zrodla swiatla wyznaczane jest w ukladzie
    // wspolrzednych obserwatora)
    WlaczOswietlenie();

    // Ustalenie polozenia obserwatora
    glTranslatef(0, 0, -odleglosc);
    glRotatef(rotObsX, 1, 0, 0);
    glRotatef(rotObsY, 0, 1, 0);
    glRotatef(rotObsZ, 0, 0, 1);

    // Zdefiniowanie materialu walca
    DefiniujMaterial();

    glPushMatrix();
    glDisable(GL_LIGHTING);
    glTranslatef(pozycja_swiatla1[0], pozycja_swiatla1[1], pozycja_swiatla1[2]);
    glColor3f(0.0, 0.0, 0.0);
    glutSolidSphere(1.0, 20.0, 20.0);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_POSITION, pozycja_swiatla1);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, skierowanie_swiatla1);
    glPopMatrix();

    // Narysowanie szescianu
    rysuj_figure();

    obrot_reflektora += szybkosc;

    // Narysowanie tekstow z opisem parametrow oswietlenia i materialu
    RysujNakladke();

    // Przelaczenie buforow ramki
    glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja obslugi klawiatury
void ObslugaKlawiatury(unsigned char klawisz, int x, int y) {
    switch (klawisz) {
        case '3':   //Poczatek wlasnego kodu
            if (N <= 4.0)
                break;
            N--;
            dAlfa = 360 / N;
            break;
        case '4':
            if (N >= 64.0)
                break;
            N++;
            dAlfa = 360 / N;
            break;
        case 'w':
            rotObsX = rotObsX + 1.0;
            break;
        case 's':
            rotObsX = rotObsX - 1.0;
            break;
        case 'a':
            rotObsY = rotObsY - 1.0;
            break;
        case 'd':
            rotObsY = rotObsY + 1.0;
            break;
        case 'q':
            rotObsZ = rotObsZ - 1.0;
            break;
        case 'e':
            rotObsZ = rotObsZ + 1.0;
            break;
        case '1':
            if (odleglosc + 1.0 > odlmax)
                break;
            else
                odleglosc++;
            break;
        case '2':
            if (odleglosc - 1.0 < odlmin)
                break;
            else
                odleglosc--;
            break;  //Koniec wlasnego kodu
        case '[':
            if (change_material - 1 > 0)
                change_material--;
            break;
        case ']':
            if (change_material + 1 < 4)
                change_material++;
            break;
        case 'n':
            if (strcmp(tryb_cieniowania, "SMOOTH") == 0) {
                glShadeModel(GL_FLAT);
                strcpy(tryb_cieniowania, "FLAT");
            } else {
                glShadeModel(GL_SMOOTH);
                strcpy(tryb_cieniowania, "SMOOTH");
            }
            break;
        case 'm':
            if (strcmp(tryb_modelowania, "FILL") == 0) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                strcpy(tryb_modelowania, "LINE");
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                strcpy(tryb_modelowania, "FILL");
            }
            break;
        case 'o':
            if (strcmp(stan_swiatlo1, "ON") == 0) {
                glDisable(GL_LIGHT0);
                strcpy(stan_swiatlo1, "OFF");
            } else {
                glEnable(GL_LIGHT0);
                strcpy(stan_swiatlo1, "ON");
            }
            break;
        case 'p':
            if (strcmp(stan_swiatlo2, "ON") == 0) {
                glDisable(GL_LIGHT1);
                strcpy(stan_swiatlo2, "OFF");
            } else {
                glEnable(GL_LIGHT1);
                strcpy(stan_swiatlo2, "ON");
            }
            break;
        case 'i':
            if (strcmp(stan_wektory, "ON") == 0) {
                strcpy(stan_wektory, "OFF");
            } else {
                strcpy(stan_wektory, "ON");
            }
            break;
        case 27: //Escape
            exit(0);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Glowna funkcja programu
int main(int argc, char **argv) {
    // Zainicjowanie biblioteki GLUT
    glutInit(&argc, argv);

    // Ustawienie trybu wyswietlania
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    // Ustawienie polozenia dolenego lewego rogu okna
    glutInitWindowPosition(100, 100);

    // Ustawienie rozmiarow okna
    glutInitWindowSize(szerokoscOkna, wysokoscOkna);

    // Utworzenie okna
    glutCreateWindow("Oswietlony pierscien");

    // Odblokowanie bufora glebokosci
    glEnable(GL_DEPTH_TEST);

    // Ustawienie funkcji wykonywanej na danych w buforze glebokosci
    glDepthFunc(GL_LEQUAL);

    // Ustawienie wartosci czyszczacej zawartosc bufora glebokosci
    glClearDepth(1000.0);

    // Odblokowanie wykonywania operacji na skladowych "alfa"
    glEnable(GL_BLEND);

    // Wybor funkcji mieszajacej kolory
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Ustawienie koloru czyszczenia bufora ramki
    glClearColor(0.0, 0.3, 0.0, 0.0);

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

    // Ustawienie domyslnych wartosci parametrow
    UstawDomyslneWartosciParametrow();

    // Obsluga glownej petli programu (wywolywanie zarejestrowanych callbackow
    // w odpowiedzi na odbierane zdarzenia lub obsluga stanu bezczynnosci)
    glutMainLoop();

    return 0;
}