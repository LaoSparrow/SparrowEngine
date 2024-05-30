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

}