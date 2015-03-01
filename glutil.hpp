#include <vector>

GLuint GLCreateShader(GLenum shaderType, const char* shaderSource)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        
        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
        
        const char *strShaderType = NULL;
        switch(shaderType)
        {
            case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
            case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
            case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
        }
        
        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        delete[] strInfoLog;

        return 0;
    }

    return shader;
}

GLuint GLCreateProgram(std::vector<GLuint> shaders)
{
    GLuint program = glCreateProgram();

    for (auto iter = shaders.begin(); iter < shaders.end(); ++iter)
    {
        glAttachShader(program, *iter);
    }

    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        // TODO: error
        fprintf(stderr, "error linking shader program!\n");
    }

    // see stuff about detaching, here:
    // http://www.arcsynthesis.org/gltut/Basics/Tut01%20Making%20Shaders.html

    return program;
}

GLuint GLCreateProgram(const char *vertexShaderSource, const char *fragmentShaderSource)
{
    std::vector<GLuint> shaderList;

    shaderList.push_back(GLCreateShader(GL_VERTEX_SHADER, vertexShaderSource));
    shaderList.push_back(GLCreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource));

    GLuint program = GLCreateProgram(shaderList);

    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

    return program;
}

GLuint GLCreateDebugProgram()
{
    static const char* vss =
        "#version 330\n"
        "layout(location = 0) in vec4 aPsition;\n"
        "layout(location = 1) in vec4 aColor;\n"
        "out vec4 vertexColor;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = aPosition;\n"
        "}\n";

    static const char* fss =
        "#version 330\n"
        "in vec4 vertexColor;\n"
        "out vec4 fragmentColor;\n"
        "void main()\n"
        "{\n"
        "    fragmentColor = vertexColor;\n"
        "}\n";

    return GLCreateProgram(vss, fss);
}
