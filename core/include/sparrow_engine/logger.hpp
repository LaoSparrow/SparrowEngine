#pragma once

#include "glad/glad.h"

namespace SparrowEngine {

    class Logger {
    private:
        static void GLAPIENTRY gl_message_callback(GLenum source,
                                                   GLenum type,
                                                   GLuint id,
                                                   GLenum severity,
                                                   GLsizei length,
                                                   const GLchar *message,
                                                   const void *userParam);

    public:
        static void Initialize();
    };

    void Logger::gl_message_callback(GLenum source,
                                     GLenum type,
                                     GLuint id,
                                     GLenum severity,
                                     GLsizei length,
                                     const GLchar *message,
                                     const void *userParam) {
        if (type == GL_DEBUG_TYPE_ERROR) {
            fprintf(stderr, "GL CALLBACK: ** GL ERROR ** type = 0x%x, id = %u, severity = 0x%x, message = %s\n",
                    type, id, severity, message);
        }
        else {
//            fprintf(stdout, "GL CALLBACK: type = 0x%x, id = %u, severity = 0x%x, message = %s\n",
//                    type, id, severity, message);
        }
    }

    void Logger::Initialize() {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(gl_message_callback, nullptr);
    }

}