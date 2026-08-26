#include <stb_image.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

const char* vertexShaderSource = "#version 410 core\n"
    "layout (location = 0) in vec3 pos;\n"
    "layout (location = 1) in vec3 color;\n"
    "layout (location = 2) in vec2 textureCord;\n"
    "out vec3 vertexColor;\n"
    "out vec2 textCoord;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(pos, 1.0f);\n"
    "vertexColor = color;\n"
    "textCoord = textureCord;\n"
    "}\0";

const char* fragShaderSource = "#version 410 core\n"
    "in vec3 vertexColor;\n"
    "in vec2 textCoord;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D textureSamp;\n"
    "void main()\n"
    "{\n"
    "FragColor = texture(textureSamp, textCoord);\n"
    "}\0";

int main() {
    constexpr int width = 800;
    constexpr int height = 600;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Monitor: Used for full screen mode
    // Share: Context to share data with
    GLFWwindow* window = glfwCreateWindow(width, height, "Cody LearnOpenGL", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Set the callback for when the window is resized
    // Called on initial window display
    // Callbacks must happen after window creation and before rendering
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //glEnable(GL_FRAMEBUFFER_SRGB);

    // Define and compile the vertex and frag shaders
    const unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertShader, 512, nullptr, infoLog);
        std::cerr << "Failed to compile vertex shader" << std::endl << infoLog << std::endl;
        return -1;
    }

    const unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, nullptr);
    glCompileShader(fragShader);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
        std::cerr << "Failed to compile fragment shader" << std::endl << infoLog << std::endl;
        return -1;
    }

    // A program is the final output linking multiple shaders together
    const unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    // Linking the program links the output of shaders to the input of the next shader in the program
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Failed to link shader program" << std::endl << infoLog << std::endl;
        return -1;
    }
    // Once the program is defined we don't need the shaders anymore
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    // constexpr float vertices[] = {
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.0f, 0.5f, 0.0f,
    // };

    // constexpr float verts[] = {
    //     -0.75f, -0.75f, 0.0f, 1.0f, 0.0f, 0.0f,
    //     0.0f, -0.75f, 0.0f, 0.0f, 1.0f, 0.0f,
    //     -0.37f, 0.25f, 0.0f, 0.0f, 0.0f, 1.0f
    // };
    //
    // constexpr float verts2[] = {
    //     0.0f, -0.75f, 0.0f, 1.0f, 0.0f, 0.0f,
    //     0.75f, -0.75f, 0.0f, 0.0f, 1.0f, 0.0f,
    //     0.37f, 0.25f, 0.0f, 0.0f, 0.0f, 1.0f
    // };

    // constexpr float twoVerts[] = {
    //     -0.75f, -0.75f, 0.0f,
    //     0.0f, -0.75f, 0.0f,
    //     -0.37f, 0.25f, 0.0f,
    //     0.0f, -0.75f, 0.0f,
    //     0.75f, -0.75f, 0.0f,
    //     0.37f, 0.25f, 0.0f,
    // };

    constexpr float vertices[] = {
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

    const unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    // VAOs allow us to bind vertex and attributes to be reused
    // You bind the VAO, do all your vertex work, and rebind when you want to use it
    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Generate 1 vertex buffer and bind it to the array buffer
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Pass the triangle vertices to the array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element buffers allow us to efficiently render more complex objects that have identical vertices using
    // indices. They're bound in the same way but a VAO can only have 1 EBO.
    // unsigned int ebo;
    // glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Tell OpenGL how to interpret vertex data
    // index is 0 from the layout in the vertex shader
    // Since we're tightly packed, the stride is just the size of the 3D coords
    // Lastly enable the vertex attributes which are disabled by default
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int textureWidth, textureHeight, nbrChannels;
    unsigned char* textureData = stbi_load("assets/container.jpg", &textureWidth, &textureHeight, &nbrChannels, 0);

    if (textureData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }

    stbi_image_free(textureData);

    // VAO and VBO are setup, we can unbind to avoid future conflicts
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // glBindVertexArray(vao[1]);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    //
    // glBufferData(GL_ARRAY_BUFFER, sizeof(verts2), verts2, GL_STATIC_DRAW);

    // Setup render loop
    while (!glfwWindowShouldClose(window))
    {
        // Process input
        process_input(window);

        // Render logic
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the defined program when rendering geometry
        glUseProgram(shaderProgram);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        // Poll and swap buffers for next frame
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}

/**
 * Callback used by OpenGL on initial window display and resizing
 * @param window
 * @param width
 * @param height
 */
void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, const int width, const int height)
{
    // x,y are for the bottom left of the window/viewport
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
