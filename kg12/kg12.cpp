#include <iostream>
#include <gl/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
GLuint Program;
GLint Attrib_vertex;
GLuint VBO;

struct Vertex {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat r;
    GLfloat g;
    GLfloat b;
};

const char* VertexShaderSource = R"(
    #version 330 core
    in vec3 coord;
    in vec3 color;
    out vec3 vertexColor;
    void main() {
        gl_Position = vec4(coord, 1.0);
        vertexColor = color;
    }
)";

const char* FragShaderSource = R"(
    #version 330 core
    in vec3 vertexColor;
    out vec4 fragColor;
    void main() {
        fragColor = vec4(vertexColor, 1.0);
    }
)";

void Init();
void Draw();
void Release();

void ReleaseVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBO);
}

void ReleaseShader() {
    glUseProgram(0);
    glDeleteProgram(Program);
}

void Release() {
    ReleaseShader();
    ReleaseVBO();
}

GLfloat translationX = 0.0f;
GLfloat translationY = 0.0f;
GLfloat translationZ = 0.0f;

//void ProcessInput(sf::Window& window) {
//    sf::Event event;
//    while (window.pollEvent(event)) {
//        if (event.type == sf::Event::Closed)
//            window.close();
//        else if (event.type == sf::Event::KeyPressed) {
//            // Обработка нажатий клавиш
//            if (event.key.code == sf::Keyboard::Left) {
//                translationX -= 0.1f;
//            }
//            else if (event.key.code == sf::Keyboard::Right) {
//                translationX += 0.1f;
//            }
//        }
//    }
//}
void UpdateVBO(GLfloat translationX) {
    glGenBuffers(1, &VBO);
    Vertex cubeVertices1[9] = {
        // КУБ
        //// Передняя грань
        //{ -0.25f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f }, // левая низ
        //{ 0.5f, -0.25f, 0.5f, 1.0f, 1.0f, 0.0f }, // правая низ
        //{ 0.25f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f }, // правая вверх
        //{ -0.5f, 0.25f, 0.5f, 0.0f, 0.0f, 1.0f }, // левая вверх

        //// Правая грань
        //{ 0.5f, -0.25f, -0.5f, 1.0f, 1.0f, 0.0f  }, // левая низ
        //{ 0.25f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f  }, // левая вверх
        //{ 0.5f, 0.5f, -0.5f, 0.1f, 0.6f, 0.4f }, // правая вверх
        //{ 0.75f, -0.25f, -0.5f, 0.90f, 0.50f, 0.70f }, // правая низ

        //// Нижняя грань
        //{ -0.25f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f }, // левая низ 
        //{ 0.5f, -0.25f, 0.5f, 1.0f, 1.0f, 0.0f }, // левая вверх
        //{ 0.75f, -0.25f, -0.5f, 0.90f, 0.50f, 0.70f }, // правая вверх
        //{ 0.0f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f }, // правая низ

        // ТЕТРАЭДЕР (пирамитка)
        // Передняя грань
        { -0.5f + translationX, 0.0f + translationY, 0.5f, 1.0f, 0.0f, 0.0f }, // левая низ
        { 0.25f + translationX, 0.5f + translationY, 0.0f, 1.0f, 1.0f, 0.0f }, // правая вверх
        { 0.0f + translationX, 0.0f + translationY, 0.5f, 0.0f, 1.0f, 0.0f }, // центр (правая низ)

        // Правая грань
        { 0.25f + translationX, -0.5f + translationY, -0.5f, 0.9f, 0.9f, 0.1f }, // правая них
        { 0.0f + translationX, 0.0f + translationY, 0.5f, 1.0f, 1.0f, 1.0f }, // центр (правая низ)
        { 0.25f + translationX, 0.5f + translationY, 0.5f, 0.6f, 0.6f, 0.0f }, // правая вверх

        // Нижняя грань
        { 0.0f + translationX, 0.0f + translationY, 0.5f, 0.0f, 1.0f, 0.0f }, // центр (правая низ)
        { -0.5f + translationX, 0.0f + translationY, 0.5f, 1.0f, 0.0f, 0.0f }, // левая низ
        { 0.25f + translationX, -0.5f + translationY, 0.0f, 0.1f, 0.6f, 0.4f } // правая них
    };
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices1), cubeVertices1, GL_STATIC_DRAW);
}
int main() {
    sf::Window window(sf::VideoMode(700, 700), "My OpenGL window",
        sf::Style::Default, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(true);
    window.setActive(true);
    glewInit();
    Init();
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                // Обработка нажатий клавиш
                if (event.key.code == sf::Keyboard::Left) {
                    if (translationX > -0.5f) {
                        translationX -= 0.01f;  // влево
                        UpdateVBO(translationX);
                    }
                }
                else if (event.key.code == sf::Keyboard::Right) {
                    if (translationX < 0.75f) {
                        translationX += 0.01f;  // вправо
                        UpdateVBO(translationX);
                    }
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    if (translationY < 0.5f) {
                        translationY += 0.01f;  // вверх
                        UpdateVBO(translationX);
                    }
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    if (translationY > -0.5f) {
                        translationY -= 0.01f;  // вниз
                        UpdateVBO(translationX);
                    }
                }
            }
            else if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Draw();
        window.display();
    }
    Release();
    return 0;
}

void checkOpenGLerror() {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << err << std::endl;
    }
}

void ShaderLog(unsigned int shader) {
    int infologLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
    if (infologLen > 1) {
        int charsWritten = 0;
        std::vector<char> infoLog(infologLen);
        glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog.data());
        std::cout << "InfoLog: " << infoLog.data() << std::endl;
    }
}

void InitShader() {
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &VertexShaderSource, NULL);
    glCompileShader(vShader);
    std::cout << "vertex shader \n";
    ShaderLog(vShader);

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &FragShaderSource, NULL);
    glCompileShader(fShader);
    std::cout << "fragment shader \n";
    ShaderLog(fShader);

    Program = glCreateProgram();
    glAttachShader(Program, vShader);
    glAttachShader(Program, fShader);
    glLinkProgram(Program);

    int link_ok;
    glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        std::cout << "error attach shaders \n";
        return;
    }

    const char* attr_name = "coord";
    Attrib_vertex = glGetAttribLocation(Program, attr_name);
    if (Attrib_vertex == -1) {
        std::cout << "could not bind attrib " << attr_name << std::endl;
        return;
    }

    checkOpenGLerror();
}

void InitVBO() {
    glGenBuffers(1, &VBO);
    Vertex cubeVertices[9] = {
        // КУБ
        //// Передняя грань
        //{ -0.25f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f }, // левая низ
        //{ 0.5f, -0.25f, 0.5f, 1.0f, 1.0f, 0.0f }, // правая низ
        //{ 0.25f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f }, // правая вверх
        //{ -0.5f, 0.25f, 0.5f, 0.0f, 0.0f, 1.0f }, // левая вверх

        //// Правая грань
        //{ 0.5f, -0.25f, -0.5f, 1.0f, 1.0f, 0.0f  }, // левая низ
        //{ 0.25f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f  }, // левая вверх
        //{ 0.5f, 0.5f, -0.5f, 0.1f, 0.6f, 0.4f }, // правая вверх
        //{ 0.75f, -0.25f, -0.5f, 0.90f, 0.50f, 0.70f }, // правая низ

        //// Нижняя грань
        //{ -0.25f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f }, // левая низ 
        //{ 0.5f, -0.25f, 0.5f, 1.0f, 1.0f, 0.0f }, // левая вверх
        //{ 0.75f, -0.25f, -0.5f, 0.90f, 0.50f, 0.70f }, // правая вверх
        //{ 0.0f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f }, // правая низ

        // ТЕТРАЭДЕР (пирамитка)
        // Передняя грань
        { -0.5f + translationX, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f }, // левая низ
        { 0.25f + translationX, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f }, // правая вверх
        { 0.0f + translationX, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f }, // центр (правая низ)
       
        // Правая грань
        { 0.25f + translationX, -0.5f, -0.5f, 0.9f, 0.9f, 0.1f }, // правая них
        { 0.0f + translationX, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f }, // центр (правая низ)
        { 0.25f + translationX, 0.5f, 0.5f, 0.6f, 0.6f, 0.0f }, // правая вверх

        // Нижняя грань
        { 0.0f + translationX, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f }, // центр (правая низ)
        { -0.5f + translationX, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f }, // левая низ
        { 0.25f + translationX, -0.5f, 0.0f, 0.1f, 0.6f, 0.4f } // правая них
    };
    std::cout << cubeVertices[0].x;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    checkOpenGLerror();
}

void Draw() {
    glUseProgram(Program);
    glEnableVertexAttribArray(Attrib_vertex);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(Attrib_vertex, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    glDrawArrays(GL_QUADS, 0, 24);

    glDisableVertexAttribArray(Attrib_vertex);
    glDisableVertexAttribArray(1);

    glUseProgram(0);
    checkOpenGLerror();
}

void Init() {
    InitShader();
    InitVBO();
}
