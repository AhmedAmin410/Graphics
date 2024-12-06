#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <cmath>

// Vector class to represent 3D coordinates (x, y, z)
class Vector {
public:
    float x, y, z;

    Vector() : x(0), y(0), z(0) {}
    Vector(float x, float y, float z) : x(x), y(y), z(z) {}

    void Normalize() {
        float length = std::sqrt(x * x + y * y + z * z);
        if (length != 0) {
            x /= length;
            y /= length;
            z /= length;
        }
    }

    Vector operator+(const Vector& v) const {
        return Vector(x + v.x, y + v.y, z + v.z);
    }

    Vector operator-(const Vector& v) const {
        return Vector(x - v.x, y - v.y, z - v.z);
    }

    Vector operator*(float scalar) const {
        return Vector(x * scalar, y * scalar, z * scalar);
    }

    float Dot(const Vector& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector Cross(const Vector& v) const {
        return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
};

// Constants for game setup
int WIDTH = 1280, HEIGHT = 720;
GLuint tex;
char title[] = "6th Yard - 3D Football Game";
GLdouble fovy = 45.0, aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear = 0.1, zFar = 100.0;

// Models and Textures
Model_3DS model_Stadium, model_ball, model_player, model_goalkeeper;
Model_3DS model_cone1, model_barrier, model_collectible, model_target, model_defensiveplayer, model_goal;
GLTexture tex_ground, tex_cons, tex_messi;

// Player position
Vector player_position(0.0f, 0.0f, 0.0f); // Initialize player position at (0,0,0)
Vector Eye(20, 5, 20), At(0, 0, 0), Up(0, 1, 0);
int cameraZoom = 0, firstPerson = 0; // 0 - third person, 1 - first person

// Lighting Setup
void InitLightSource() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void InitMaterial() {
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    GLfloat shininess[] = { 96.0f };
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

// Camera logic
void UpdateCamera() {
    if (firstPerson) {
        gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
    }
    else {
        Eye = Vector(player_position.x + 5, player_position.y + 5, player_position.z + 10);
        gluLookAt(Eye.x, Eye.y, Eye.z, player_position.x, player_position.y, player_position.z, Up.x, Up.y, Up.z);
    }
}

// Keyboard input
void myKeyboard(unsigned char button, int x, int y) {
    switch (button) {
    case 'w': player_position.z -= 0.1f; break;
    case 's': player_position.z += 0.1f; break;
    case 'a': player_position.x -= 0.1f; break;
    case 'd': player_position.x += 0.1f; break;
    case 'q': cameraZoom++; break;
    case 'e': cameraZoom--; break;
    case 'r': firstPerson = !firstPerson; break;
    default: break;
    }
    UpdateCamera();
    glutPostRedisplay();
}

// Load assets
void LoadAssets() {
    model_Stadium.Load("Models/field/Field.3DS");
    model_ball.Load("Models/ball/Ball 3DS.3DS");
    model_player.Load("Models/Messi/model.3ds");
    model_goal.Load("Models/Goal/Gates.3ds");
    model_defensiveplayer.Load("Models/Messi/model.3ds");
}

// Rendering function
void RenderObjects() {
    // Render stadium
    model_Stadium.Draw();
    // Render player model
    glPushMatrix();
    glTranslatef(player_position.x, player_position.y, player_position.z);
    model_player.Draw();
    glPopMatrix();

    // Render ball
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.0f); // Ball floating above the ground
    model_ball.Draw();
    glPopMatrix();
}

// Main display function
void myDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat lightIntensity[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);

    RenderObjects();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow(title);

    glEnable(GL_DEPTH_TEST);

    InitLightSource();
    LoadAssets();

    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(myKeyboard);

    glutMainLoop();
    return 0;
}
