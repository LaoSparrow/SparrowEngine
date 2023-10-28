#include <utility>

#include "sparrow_engine/components/mesh.hpp"

namespace SparrowEngine::Components {

    void Mesh::allocate_gl_objects() {
        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glCreateBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    }

    Mesh::Mesh(std::weak_ptr<GameObject> obj)
    : Behavior(std::move(obj)) { }

    Mesh::Mesh(std::weak_ptr<GameObject> obj, std::initializer_list<MeshData> mesh_data)
    : Behavior(std::move(obj)), mesh_data(mesh_data) { }

    Mesh::Mesh(std::weak_ptr<GameObject> obj, std::vector<MeshData>&& mesh_data)
    : Behavior(std::move(obj)), mesh_data(mesh_data) { }

    Mesh::~Mesh() {
        glDeleteVertexArrays(1, &vao);
        glBindVertexArray(0);
    }

    void Mesh::start() {
        allocate_gl_objects();
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, sizeof(MeshData) * mesh_data.size(), mesh_data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshData), (void*)offsetof(MeshData, position));
        glEnableVertexAttribArray(0);

        const char *vertexShaderSource = "#version 330 core\n"
                                         "layout (location = 0) in vec3 aPos;\n"
                                         "void main()\n"
                                         "{\n"
                                         "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                         "}\0";
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        const char *fragmentShaderSource = "#version 330 core\n"
                                           "out vec4 FragColor;\n"
                                           "\n"
                                           "void main()\n"
                                           "{\n"
                                           "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                           "} ";
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glUseProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        shader_program = shaderProgram;

        glBindVertexArray(0);
        Behavior::start();
    }

    void Mesh::render() {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, mesh_data.size());
        glBindVertexArray(0);

        Behavior::render();
    }

}