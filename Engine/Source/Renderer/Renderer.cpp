#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"

#include "Logger/Logger.h"
#include <iostream>

namespace Engine
{

struct SLineVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
};

struct STexVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
};

struct SRendererData
{
    static const uint32_t MaxQuads = 20000;
    static const uint32_t MaxVertices = MaxQuads * 4;
    static const uint32_t MaxIndices = MaxQuads * 6;
    static const uint32_t MaxTextureSlots = 32;

    //std::shared_ptr<CVertexArray> QuadVertexArray;
    //std::shared_ptr<CVertexBuffer> QuadVertexBuffer;
    //std::shared_ptr<CShader> QuadShader;
    std::shared_ptr<CTexture> WhiteTexture;

    std::shared_ptr<CVertexArray> LineVertexArray;
    std::shared_ptr<CVertexBuffer> LineVertexBuffer;
    std::shared_ptr<CShader> LineShader;

    std::shared_ptr<CVertexArray> TexVertexArray;
    std::shared_ptr<CVertexBuffer> TexVertexBuffer;
    std::shared_ptr<CShader> TexShader;


    uint32_t LineVertexCount = 0;
    SLineVertex* LineVertexBufferBase = nullptr;
    SLineVertex* LineVertexBufferPtr = nullptr;

    uint32_t TexIndexCount = 0;
    STexVertex* TexVertexBufferBase = nullptr;
    STexVertex* TexVertexBufferPtr = nullptr;

    float LineWidth = 2.0f;

    std::array<std::shared_ptr<CTexture>, MaxTextureSlots> TextureSlots;
    uint32_t TextureSlotIndex = 1; // 0 = white texture

    //glm::vec4 QuadVertexPositions[4];

    glm::mat4 ViewProjection;
};

static SRendererData s_Data;

DECLARE_LOG_CATEGORY(Renderer)

void OpenGLMessageCallback(
    unsigned source,
    unsigned type,
    unsigned id,
    unsigned severity,
    int length,
    const char* message,
    const void* userParam)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         LOG(Renderer, ERROR, "{}", message); return;
    case GL_DEBUG_SEVERITY_MEDIUM:       LOG(Renderer, WARN, "{}", message); return;
    case GL_DEBUG_SEVERITY_LOW:          LOG(Renderer, INFO, "{}", message); return;
    case GL_DEBUG_SEVERITY_NOTIFICATION: LOG(Renderer, TRACE, "{}", message); return;
    default:                             LOG(Renderer, ERROR, "Unknown severity level!"); return;
    }

    //ASSERT(false, "Unknown severity level!");
}


void Renderer::Init()
{
    // Only one VAO per draw type 
    s_Data.LineVertexArray = std::make_shared<CVertexArray>();

    s_Data.LineVertexBuffer = std::make_shared<CVertexBuffer>(s_Data.MaxVertices * sizeof(SLineVertex));

    CVertexBufferLayout layout;
    layout.Push<EShaderDataType::FLOAT>(3); // a_Position
    layout.Push<EShaderDataType::FLOAT>(4); // a_Color

    s_Data.LineVertexBuffer->SetLayout(layout);

    s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
    /// QUESTION: WHAT?
    s_Data.LineVertexBufferBase = new SLineVertex[s_Data.MaxVertices];

    s_Data.LineShader.reset(new CShader("res/shader/vertex/Line.glsl", "res/shader/fragment/Line.glsl"));


#ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    LOG(Renderer, TRACE, "Renderer Initialized");
}

void Renderer::InitGLContext(GLFWwindow* window)
{
    glfwMakeContextCurrent(window);
    LOG(Renderer, TRACE, "GLFW CONTEXT INIT START");

    // glad should only be initialized after a valid context has been created
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG(Renderer, FATAL, "GLAD LoadGlLoader Failed");
    }
    LOG(Renderer, TRACE, "GLFW CONTEXT Initialized");
}


void Renderer::LineMode(bool drawInLineMode = false)
{
    if (drawInLineMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}


void Renderer::Clear()
{
/* Clean and assign new color to back buffer*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetClearColor(const glm::vec3& Color)
{
    glClearColor(Color.r, Color.g, Color.b, 1.0f);
}

void Renderer::Draw(const CVertexArray& VA, const CIndexBuffer& IBO,
    const CShader& shader)
{
/* Color of background of window */
    //glClearColor(0.2f, 0.39f, 0.32f, 1.0f);

    shader.Bind();
    VA.Bind();
    IBO.Bind();

    ///
    /// Primitive(shape) we want to use
    /// How many indices we want to draw
    /// Data type of indices
    /// Index of indices
    ///

    glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::DrawLine(const glm::vec3& Start, const glm::vec3& End, const glm::vec4& Color, float Width)
{
    s_Data.LineVertexBufferPtr->Position = Start;
    s_Data.LineVertexBufferPtr->Color = Color;
    s_Data.LineVertexBufferPtr++;

    s_Data.LineVertexBufferPtr->Position = End;
    s_Data.LineVertexBufferPtr->Color = Color;
    s_Data.LineVertexBufferPtr++;

    s_Data.LineVertexCount += 2;

    s_Data.LineWidth = Width;

}

void Renderer::StartDraw()
{
    s_Data.LineVertexCount = 0;
    s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;


    s_Data.TextureSlotIndex = 1;
}

void Renderer::Flush()
{
    if (s_Data.LineVertexCount)
    {
        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);

        s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

        s_Data.LineShader->Bind();

        glLineWidth(s_Data.LineWidth);

        s_Data.LineVertexArray->Bind();
        glDrawArrays(GL_LINES, 0, s_Data.LineVertexCount);

        // Reset data
        s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;
        s_Data.LineVertexCount = 0;
    }
}

} // namespace Engine