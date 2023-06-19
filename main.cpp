#include <stdio.h>
#include <GL/glut.h>
#include "chip8.h"

// Display
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
uint8_t screenData[SCREEN_HEIGHT][SCREEN_WIDTH][3];

// Window
int modifier = 10;
int display_width = SCREEN_WIDTH * modifier;
int display_height = SCREEN_HEIGHT * modifier;

void display();
void reshapeWindow(GLsizei w, GLsizei h);
void keyboardUp(unsigned char key, int x, int y);
void keyboardDown(unsigned char key, int x, int y);
void setupTexture();

chip8 myChip8;

int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Usage: myChip8.out chip8application\n\n");
        return 1;
    }
    
    // Load game
    if(!myChip8.loadApplication(argv[1]))
        return 1;
    
    // Setup OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    
    glutInitWindowSize(display_width, display_height);
    glutInitWindowPosition(320, 320);
    glutCreateWindow("myChip8");
    
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshapeWindow);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    
    setupTexture();
    glutMainLoop();
    
    return 0;
}

// Setup Texture
void setupTexture() {
    // Clear screen
    for(int y = 0; y < SCREEN_HEIGHT; ++y)
        for(int x = 0; x < SCREEN_WIDTH; ++x)
            screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0;
    
    // Create a texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screenData);
    
    // Set up the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    // Enable textures
    glEnable(GL_TEXTURE_2D);
}

void updateTexture(const chip8& c8) {
    // Update pixels
    for(int y = 0; y < 32; ++y)
        for(int x = 0; x < 64; ++x)
            if(c8.gfx[(y * 64) + x] == 0)
                screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0;    // Disabled
            else
                screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 255;  // Enabled
    
    // Update Texture
    glTexSubImage2D(GL_TEXTURE_2D, 0 ,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screenData);
    
    glBegin( GL_QUADS );
        glTexCoord2d(0.0, 0.0);     glVertex2d(0.0,           0.0);
        glTexCoord2d(1.0, 0.0);     glVertex2d(display_width, 0.0);
        glTexCoord2d(1.0, 1.0);     glVertex2d(display_width, display_height);
        glTexCoord2d(0.0, 1.0);     glVertex2d(0.0,           display_height);
    glEnd();
}

void display() {
    myChip8.emulateCycle();
    
    if(myChip8.drawFlag) {
        // Clear framebuffer
        glClear(GL_COLOR_BUFFER_BIT);
        
        updateTexture(myChip8);
        
        // Swap buffers
        glutSwapBuffers();
        
        // Processed frame
        myChip8.drawFlag = false;
    }
}

void reshapeWindow(GLsizei w, GLsizei h) {
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
    
    // Resize quad
    display_width = w;
    display_height = h;
}

void keyboardDown(unsigned char key, int x, int y) {
    if(key == 27)    // esc
        exit(0);
    
    if(key == '1')      myChip8.key[0x1] = 1;
    else if(key == '2') myChip8.key[0x2] = 1;
    else if(key == '3') myChip8.key[0x3] = 1;
    else if(key == '4') myChip8.key[0xC] = 1;
    
    else if(key == 'q') myChip8.key[0x4] = 1;
    else if(key == 'w') myChip8.key[0x5] = 1;
    else if(key == 'e') myChip8.key[0x6] = 1;
    else if(key == 'r') myChip8.key[0xD] = 1;
    
    else if(key == 'a') myChip8.key[0x7] = 1;
    else if(key == 's') myChip8.key[0x8] = 1;
    else if(key == 'd') myChip8.key[0x9] = 1;
    else if(key == 'f') myChip8.key[0xE] = 1;
    
    else if(key == 'z') myChip8.key[0xA] = 1;
    else if(key == 'x') myChip8.key[0x0] = 1;
    else if(key == 'c') myChip8.key[0xB] = 1;
    else if(key == 'v') myChip8.key[0xF] = 1;
}

void keyboardUp(unsigned char key, int x, int y) {
    if(key == '1')      myChip8.key[0x1] = 0;
    else if(key == '2') myChip8.key[0x2] = 0;
    else if(key == '3') myChip8.key[0x3] = 0;
    else if(key == '4') myChip8.key[0xC] = 0;
    
    else if(key == 'q') myChip8.key[0x4] = 0;
    else if(key == 'w') myChip8.key[0x5] = 0;
    else if(key == 'e') myChip8.key[0x6] = 0;
    else if(key == 'r') myChip8.key[0xD] = 0;
    
    else if(key == 'a') myChip8.key[0x7] = 0;
    else if(key == 's') myChip8.key[0x8] = 0;
    else if(key == 'd') myChip8.key[0x9] = 0;
    else if(key == 'f') myChip8.key[0xE] = 0;
    
    else if(key == 'z') myChip8.key[0xA] = 0;
    else if(key == 'x') myChip8.key[0x0] = 0;
    else if(key == 'c') myChip8.key[0xB] = 0;
    else if(key == 'v') myChip8.key[0xF] = 0;
}
