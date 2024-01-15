// File ID: Desktop.cpp
// Title: Reading on the desktop
// Author: Li_Linfeng
#define FREEGLUT_STATIC
#define GLUT_DISABLE_ATEXIT_HACK
#define PI 3.14159265
#include <GL/freeglut.h>
#include <math.h>
#include <stdlib.h> 
#include <stdio.h>
#include <vector>

using namespace std;

GLint w = 1600, h = 900;
float binterval = 0,bout1 = 0, bout2 = 0,bout3 = 0,bout4 = 0,bout5 = 0 ,speed = 1, time_interval = 16;
float viewx = 0, viewy = 0, viewz = 0;
bool viewmodel1 = false, viewmodel2 = true, viewmodel3 = false, outmodel = false, inmodel = false, readmode = false, putmode = false, light = false;
vector<GLubyte*> p;
GLuint texture[9];

struct image {
    GLint imageWidth;
    GLint imageHeight;
    GLint pixelLength;
};
image loadTexture[7];

void GL_LIGHT(void);
void ReadImage(const char path[256], GLint& imagewidth, GLint& imageheight, GLint& pixellength);
void GL_LOADTEXTURE(void);
void Viewc(GLfloat w, GLfloat h);
void BRIDGE_PIECE(void);
void GL_BACKGROUND(GLfloat angle);
void myinit();
void ReadImage(const char path[256], GLint& imagewidth, GLint& imageheight, GLint& pixellength);
void display(void);
void when_in_mainloop();
void desktop();
void drawCuboid();
void Lamp();
void Book(float x, float y, float z, float size, float w, int i);
void BookCover(float x, float y, float z, float s, float w, int i);
void Bookpage(float x, float y, float z, float s, float w);
void Shelff();
void Shelf(float h);
void drawPinapple();
void OnTimer(int value);
void keyboard_input(unsigned char key, int x, int y);
void mouse_input(int button, int state, int x, int y);




int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(w, h);
	glutCreateWindow("Reading books!");
    GL_LIGHT();
    GL_LOADTEXTURE();
	glutDisplayFunc(display);
    glutTimerFunc(time_interval, OnTimer, 1);
	glutKeyboardFunc(keyboard_input);
	glutMouseFunc(mouse_input);

    glutMainLoop();
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    Viewc(w,h);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glPushMatrix();
    glScalef(1, 1, 0.5);
    glRotatef(90.0, 0, 0, 1);
    glRotatef(90.0, 1, 0, 0);
    glTranslatef(700 , 200 ,130);
    
    drawPinapple();
    glPopMatrix();
    
    
	desktop();

    glColor3f(0.75,0.75,0.75);
    Lamp();
    Shelff();
    
    
	// Swap the buffers.
	glutSwapBuffers();
	glLoadIdentity();
	glFlush();

}

void when_in_mainloop()
{
    glutPostRedisplay(); // force OpenGL to redraw the current window
}
void GL_LIGHT() {
    GLfloat lmodel_ambient[] = { 0.45, 0.45, 0.45, 0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat light_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat light_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat light_specular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat light_position[] = { 300, 800, 280, 1.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    

    

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    // Specify the clear color
    glClearColor(0.5, 0.5, 0.5, 0.5);
};
void ReadImage(const char path[256], GLint& imagewidth, GLint& imageheight, GLint& pixellength) {
    GLubyte* pixeldata;
    FILE* pfile;
    fopen_s(&pfile, path, "rb");
    if (pfile == 0) exit(0);
    fseek(pfile, 0x0012, SEEK_SET);
    fread(&imagewidth, sizeof(imagewidth), 1, pfile);
    fread(&imageheight, sizeof(imageheight), 1, pfile);
    pixellength = imagewidth * 3;
    while (pixellength % 4 != 0)
        pixellength++;
    pixellength *= imageheight;
    pixeldata = (GLubyte*)malloc(pixellength);
    if (pixeldata == 0)
        exit(0);
    fseek(pfile, 54, SEEK_SET);
    fread(pixeldata, pixellength, 1, pfile);
    p.push_back(pixeldata);
    fclose(pfile);
}
void GL_LOADTEXTURE(void)
{
    const char* pictures[7] = {  // Define a char pointer for the file name to be used 
        "Wood.bmp",
        "Lamp.bmp",
        "Cover.bmp",
        "Page.bmp",
        "HeadPhone Girl.bmp",
        "Millon.bmp",
        "art.bmp"
    };
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_FLAT);
    glEnable(GL_TEXTURE_2D);
    for (int i = 0; i < 7; i++) {
        ReadImage(pictures[i], loadTexture[i].imageWidth, loadTexture[i].imageHeight, loadTexture[i].pixelLength);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &texture[i]);     // Creat texture for the number i texture
        glBindTexture(GL_TEXTURE_2D, texture[i]);    // bind the coresponding texture to the unique ID
        glTexImage2D(GL_TEXTURE_2D, 0, 3, loadTexture[i].imageWidth, loadTexture[i].imageHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[i]);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);    // Apply GL_MODULATE to combine the texture with material color
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    glDisable(GL_TEXTURE_2D);
}

// Desktop is consist of a bottom base and a bookshelf.
void desktop() {
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);  glVertex3f(-5000, -5000, 100);
    glTexCoord2f(0, 12); glVertex3f(-5000, 5000, 100);
    glTexCoord2f(12, 10); glVertex3f(5000, 5000, 100);
    glTexCoord2f(12, 0); glVertex3f(5000, -5000.0, 100);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
    /*drawCuboid();*/
    
}
void drawCuboid() {
    // Draw a 3D cuboid with dimensions 1000x1000x100
    glBegin(GL_QUADS);

    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1000.0, 0.0, 0.0);
    glVertex3f(1000.0, 1000.0, 0.0);
    glVertex3f(0.0, 1000.0, 0.0);

    glVertex3f(0.0, 0.0, 100.0);
    glVertex3f(1000.0, 0.0, 100.0);
    glVertex3f(1000.0, 1000.0, 100.0);
    glVertex3f(0.0, 1000.0, 100.0);

    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1000.0, 0.0);
    glVertex3f(0.0, 1000.0, 100.0);
    glVertex3f(0.0, 0.0, 100.0);

    glVertex3f(1000.0, 0.0, 0.0);
    glVertex3f(1000.0, 1000.0, 0.0);
    glVertex3f(1000.0, 1000.0, 100.0);
    glVertex3f(1000.0, 0.0, 100.0);

    glVertex3f(0.0, 1000.0, 0.0);
    glVertex3f(1000.0, 1000.0, 0.0);
    glVertex3f(1000.0, 1000.0, 100.0);
    glVertex3f(0.0, 1000.0, 100.0);

    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1000.0, 0.0, 0.0);
    glVertex3f(1000.0, 0.0, 100.0);
    glVertex3f(0.0, 0.0, 100.0);

    glEnd();
}



void Lamp() {
    glPushMatrix();
    glTranslatef(0, 200, 0);
    float scaleFactor = 0.2;
    float Rotation = 45.0;

    // The base of the lamp
    glPushMatrix(); // Save the current transformation matrix
    glTranslatef(300.0, 650.0, 100.0);
    glRotatef(Rotation, 0.0, 0.0, 1.0); // Rotate around the y-axis
    glScalef(scaleFactor, scaleFactor, scaleFactor); // Scale the cuboid
    drawCuboid();
    glPushMatrix();
    glTranslatef(400.0, 500.0, 0);
    glScalef(scaleFactor, scaleFactor*0.5, 14.0);
    
    drawCuboid();
    
    glPopMatrix();
    glPushMatrix();
    glTranslatef(400.0,-400.0, 1400);
    glScalef(scaleFactor, 1, 1);
    drawCuboid();

    // Light source
    glPushMatrix();

    glTranslatef(0, 20, -40);
    glScalef(0.2, 0.2, 0.5);

    drawCuboid();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    if (light == true) {
        glEnable(GL_LIGHT1);
    }
    if (light == false) {
        glDisable(GL_LIGHT1);
    }
    GLfloat light1_ambient_diffuse[] = { 0.5f, 0.5f, 0.5f, 0.5f };
    GLfloat light1_specular[] = { 0.5f, 0.5f, 0.5f, 0.5f };
    GLfloat spot1_position[] = { 1000, 300, 150 };
    GLfloat spot1_direction[] = { 0, 0, -1};
    GLfloat attenuation1[] = { 0.1, 0.01, 0.001 };
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient_diffuse);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_ambient_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot1_position);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot1_direction);
    glLightfv(GL_LIGHT1, GL_LINEAR_ATTENUATION, attenuation1);
}

void Shelff() {
    glColor3f(1, 1, 1);
    // Draw the bookshelf
    Shelf(600.0);
    float y = 40;
    float up = 405;
    float down = 105;
    int n = 5;

    // Second row books
    glPushMatrix();
    glRotatef(-8.0, 0, 1, 0);
    glTranslatef(0, 0, -10);
    Book(100, y, up, 140, 50, n);
    glPopMatrix();
    glPushMatrix();
    glRotatef(-30.0, 0, 1, 0);
    glTranslatef(80, 0, -155);
    Book(300, y, up, 130, 50, n);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(75, 0, -450);
    glRotatef(-45.0, 0, 1, 0);
    Book(800, y, up, 120, 50, n);
    glPopMatrix();
    Book(890, y, up, 140, 80, n);
    
    // First row books, these books' warp is facing outside
    
    Book(30, y, down, 140, 70, 5);
    glPushMatrix();
    glTranslatef(binterval, 0, 0);

    glPushMatrix();
    glTranslatef(0, bout5, 0);
    Book(110, y, down, 140, 50, 6);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(binterval, 0, 0);

    glPushMatrix();
    glTranslatef(0, bout4, 0);
    Book(160, y, down, 140, 50, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(binterval, 0, 0);

    glPushMatrix();
    glTranslatef(0, bout3, 0);
    Book(210, y, down, 140, 50, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(binterval, 0, 0);

    glPushMatrix();
    glTranslatef(0, bout2, 0);
    Book(260, y, down, 140, 50, 4);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(binterval, 0, 0);
    
    glPushMatrix();
    glTranslatef(0, bout1, 0);
    Book(310, y, down, 140, 50, 6);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

void Shelf(float h) {

    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glBegin(GL_QUADS);

    // Front face
    glTexCoord2f(0, 0);  glVertex3f(1000, 0, 100);
    glTexCoord2f(1, 0); glVertex3f(1000, h / 2, 100);
    glTexCoord2f(1, 1); glVertex3f(1000, h / 2, 100 + h);
    glTexCoord2f(0, 1); glVertex3f(1000, 0, 100 + h);

    // Back face
    glTexCoord2f(0, 0); glVertex3f(0, 0, 100);
    glTexCoord2f(1, 0); glVertex3f(0, h / 2, 100);
    glTexCoord2f(1, 1); glVertex3f(0, h / 2, 100 + h);
    glTexCoord2f(0, 1); glVertex3f(0, 0, 100 + h);

    // Left face
    glTexCoord2f(0, 0); glVertex3f(1000, 0, 100 + h);
    glTexCoord2f(1, 0); glVertex3f(1000, 0, 100);
    glTexCoord2f(1, 1); glVertex3f(0, 0, 100);
    glTexCoord2f(0, 1); glVertex3f(0, 0, h + 100);

    // Upface
    glTexCoord2f(0, 0); glVertex3f(1000, 0, 100 + h);
    glTexCoord2f(1, 0); glVertex3f(1000, h / 2, 100 + h);
    glTexCoord2f(1, 1); glVertex3f(0, h / 2, 100 + h);
    glTexCoord2f(0, 1); glVertex3f(0, 0, 100 + h);

    // Shelf Interval
    glTexCoord2f(0, 0); glVertex3f(1000, 0, 100 + h / 2);
    glTexCoord2f(1, 0); glVertex3f(1000, h / 2, 100 + h / 2);
    glTexCoord2f(1, 1); glVertex3f(0, h / 2, 100 + h / 2);
    glTexCoord2f(0, 1); glVertex3f(0, 0, 100 + h / 2);
    glEnd();
}

void Book(float x, float y, float z, float s, float W, int i) {

    // The cover and back of the book
    Bookpage(x, y, z, s, W);
    BookCover(x, y, z, s, W, i);
}
void BookCover(float x, float y, float z, float s, float W, int i) {
    float w = s/ 50;
    glColor3f(0.75, 0.75, 0.75);
    glPushMatrix();
    glTranslatef(x + W / 2, y, z - w);
    glutSolidCylinder(W / 2, 2 * s + 2 * w, 60, 1);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[i]);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(x  + W + 1, y, z - w);
    glTexCoord2f(1, 0); glVertex3f(x + W + 1, y + s + w/2, z - w);
    glTexCoord2f(1, 1); glVertex3f(x + W + 1, y + s + w/2, z + 2 * s + w);
    glTexCoord2f(0, 1); glVertex3f(x + W + 1, y, z + 2 * s + w);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[i]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(x - 1, y, z - w);
    glTexCoord2f(1, 0); glVertex3f(x - 1, y + s + w/2, z - w);
    glTexCoord2f(1, 1); glVertex3f(x - 1, y + s + w/2, z + 2 * s + w);
    glTexCoord2f(0, 1); glVertex3f(x - 1, y, z + 2 * s + w);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void Bookpage(float x, float y, float z, float s, float W) {
    float w = s / 50;
    float position = 0;
    float interval = W / 50;
    
    while ( position < W ){
        position += interval;
        x += interval;
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);  glVertex3f(x, y, z);
        glTexCoord2f(1, 0); glVertex3f(x, y + s, z);
        glTexCoord2f(1, 1); glVertex3f(x, y + s, z + 2 * s);
        glTexCoord2f(0, 1); glVertex3f(x, y, z + 2 * s);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
    
}

void drawPinapple()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(0.808, 0.396, 0.271);
    glPushMatrix();
    glTranslatef(0, 60, 0);
    glScalef(40.0, 75.0, 40.0);
    glutSolidSphere(2, 20, 20);
    glLineWidth(2);
    glColor3f(0.718, 0.310, 0.227);
    glutWireSphere(2, 12, 18);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -53, 0);
    glScalef(0.6, 1.1, 0.8);
    glColor3f(0.565, 0.682, 0.262);
    glPushMatrix();
    glTranslatef(0, 300, 3);
    glScalef(0.85, 4.3, 0.85);
    glutSolidSphere(20, 20, 20);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(38, 300, 3);
    glRotatef(-20, 0, 0, 1);
    glScalef(0.85, 3.5, 0.85);
    glutSolidSphere(20, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-38, 300, 3);
    glRotatef(20, 0, 0, 1);
    glScalef(0.85, 3.5, 0.85);
    glutSolidSphere(20, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(60, 270, 0);
    glRotatef(-63, 0, 0, 1);
    glScalef(0.85, 3.5, 0.85);
    glutSolidSphere(20, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-60, 270, 0);
    glRotatef(63, 0, 0, 1);
    glScalef(0.85, 3.5, 0.85);
    glutSolidSphere(20, 20, 20);
    glPopMatrix();
    glPopMatrix();
}
void OnTimer(int value)
{
    float read = speed * 0.8;
    if (binterval <= 0) inmodel = false;
    if (binterval >= 120) outmodel = false;
    if (bout1 >= 400) readmode = false;
    if (bout5 <= 0) putmode = false;
    if (outmodel == true) {
        binterval += speed;
    }
    if (inmodel == true) {
        binterval -= speed;
    }
    if (readmode == true) {
        bout1 += speed;
        bout2 += speed;
        bout3 += speed;
        bout4 += speed;
        bout5 += speed;
    }
    if (putmode == true) {
        if (bout1 >0)
            bout1 -= read;
        if (bout1 <= 0 && bout2 > 0) {
            bout2 -= read;
        }
        if (bout1 <= 0 && bout2 <= 0 && bout3 > 0) {
            bout3 -= read;
        }
        if (bout1 <= 0 && bout2 <= 0 && bout3 <= 0 && bout4 > 0) {
            bout4 -= read;
        }
        if (bout1 <= 0 && bout2 <= 0 && bout3 <= 0 && bout4 <= 0 && bout5 > 0) {
            bout5 -= read;
        }
        
    }
    when_in_mainloop();
    glutTimerFunc(time_interval, OnTimer, 1);
}
// Set the model-view-projection matrix
void Viewc(GLfloat w, GLfloat h)
{
    if (viewmodel1 == true) {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // Set the vertical degree by the first factor
        gluPerspective(60, 1.7777, 10.0, 2000);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(1400 + viewx, 600 + viewy, 300 + viewz, 500, 500, 175, 0, 0, 1);
    }
    if (viewmodel2 == true) {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // Set the vertical degree by the first factor
        gluPerspective(60, 1.7777, 10.0, 5000);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(1000 + viewx, 1500 + viewy, 350 + viewz, 500, 500, 300, 0, 0, 1);
    }
    if (viewmodel3 == true) {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // Set the vertical degree by the first factor
        gluPerspective(60, 1.7777, 10.0, 5000);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(500 + viewx, 1500 + viewy,  300 + viewz, 500, 0, 300, 0, 0, 1);
    }
    
}
void keyboard_input(unsigned char key, int x, int y) 
{
    if (key == 'x') viewx += 100;
    if (key == 'X') viewx -= 100;
    if (key == 'y') viewy += 100;
    if (key == 'Y') viewy -= 100;
    if (key == 'z') viewz += 100;
    if (key == 'Z') viewz -= 100;
    if (key == '2') {
        viewmodel1 = false;
        viewmodel2 = true;
        viewmodel3 = false;
    }
    if (key == '1') {
        viewmodel2 = false;
        viewmodel1 = true;
        viewmodel3 = false;
    }
    if (key == '3') {
        viewmodel1 = false;
        viewmodel2 = false;
        viewmodel3 = true;
    }
    if (key == 't' && binterval >= 120) {
        readmode = true;
    }
    if (key == 'r' && bout1 >= 400) {
        putmode = true;
    }
    if (key == 'l' || key == 'L') {
        light = !light;
    }
};
void mouse_input(int button, int state, int x, int y) 
{
    if (button == GLUT_LEFT_BUTTON) {
        outmodel = true;
    }
    if (button == GLUT_RIGHT_BUTTON) {
        inmodel = true;
    }
};