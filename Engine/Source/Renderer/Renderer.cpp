#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"

#include "Logger/Logger.h"

#include "gtc/matrix_transform.hpp"
#include <iostream>

namespace Engine
{

struct QuadVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
};

struct SLineVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
};

struct SRendererData
{
    static const uint32_t MaxQuads = 20000;
    static const uint32_t MaxVertices = MaxQuads * 4;
    static const uint32_t MaxIndices = MaxQuads * 6;
    static const uint32_t MaxTextureSlots = 32;

    std::shared_ptr<CVertexArray> QuadVertexArray;
    std::shared_ptr<CVertexBuffer> QuadVertexBuffer;
    std::shared_ptr<CShader> QuadShader;
    std::shared_ptr<CTexture>WhiteTexture;

    std::shared_ptr<CVertexArray> LineVertexArray;
    std::shared_ptr<CVertexBuffer> LineVertexBuffer;
    std::shared_ptr<CShader> LineShader;


    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    uint32_t LineVertexCount = 0;
    SLineVertex* LineVertexBufferBase = nullptr;
    SLineVertex* LineVertexBufferPtr = nullptr;

    float LineWidth = 2.0f;

    std::array<std::shared_ptr<CTexture>, MaxTextureSlots> TextureSlots;
    uint32_t TextureSlotIndex = 1; // 0 = base texture

    glm::vec4 QuadVertexPositions[4];

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
}

void Renderer::Init()
{
    // fwd declare quadIB here to be used in both quad and tex setup
    /// Quad Setup
    {
    // Only one VAO per primitive type 
        s_Data.QuadVertexArray = std::make_shared<CVertexArray>();

        s_Data.QuadVertexBuffer = std::make_shared<CVertexBuffer>(s_Data.MaxVertices * sizeof(QuadVertex));

        CVertexBufferLayout layout;
        layout.Push<float>(3); // a_Position
        layout.Push<float>(4); // a_Color
        layout.Push<float>(2); // a_TexCoord
        layout.Push<float>(1); // a_TexIndex

        s_Data.QuadVertexBuffer->SetLayout(layout);
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        // allocate memory for all the Vertex Buffer we might use in a single draw call
        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

        for (uint32_t i = 0, offset = 0; i < s_Data.MaxIndices; )
        {
            // First Triangle Indices
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            // Second Triangle Indices
            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            // One quad has 4 vertices so +4 = next quad
            offset += 4;
            // 6 indices per quad
            i += 6;
        }

        std::shared_ptr<CIndexBuffer> quadIB = std::make_shared<CIndexBuffer>(quadIndices, s_Data.MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadIB); // this step binds IB to VA, its weird but its OpenGL
        delete[] quadIndices;
    }

    /// Line Setup
    {
        s_Data.LineVertexArray = std::make_shared<CVertexArray>();

        s_Data.LineVertexBuffer = std::make_shared<CVertexBuffer>(s_Data.MaxVertices * sizeof(SLineVertex));

        CVertexBufferLayout layout;
        layout.Push<float>(3); // a_Position
        layout.Push<float>(4); // a_Color

        s_Data.LineVertexBuffer->SetLayout(layout);
        s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);

        s_Data.LineVertexBufferBase = new SLineVertex[s_Data.MaxVertices];
    }

    s_Data.WhiteTexture = std::make_shared<CTexture>();
    uint32_t whiteTextureData = 0xffffffff;
    s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

    // Set white texture to slot 0
    s_Data.TextureSlots[0] = s_Data.WhiteTexture;

    /// Shader Setup
    {
        s_Data.QuadShader = std::make_shared<CShader>("res/shader/vertex/Quad2D.glsl", "res/shader/fragment/Quad2D.glsl");
        s_Data.LineShader = std::make_shared<CShader>("res/shader/vertex/Line.glsl", "res/shader/fragment/Line.glsl");
    }

    s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
    s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
    s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };


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

void Renderer::SetClearColor(const glm::vec4& Color)
{
    glClearColor(Color.r, Color.g, Color.b, Color.a);
}

void Renderer::DrawIndexed(const std::shared_ptr<CVertexArray>& VA, uint32_t indexCount)
{
    VA->Bind();
    uint32_t count = indexCount ? indexCount : VA->GetIndexBuffer()->GetCount();

    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
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

void Renderer::DrawSprite(const glm::vec3& Position, const glm::vec2& Size, const std::filesystem::path& SpriteSource)
{
    glm::mat4 Transform = glm::translate(glm::mat4(1.0f), Position) * glm::scale(glm::mat4(1.0f), { Size.x, Size.y, 1.0f });

    DrawQuad(Transform, std::make_shared<CTexture>(SpriteSource));
}

void Renderer::DrawQuad(const glm::mat4& Transform, const std::shared_ptr<CTexture>& Texture, const glm::vec4& TintColor)
{
    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    float textureIndex = 0.0f;
    // Find the correct texture slot for the texture
    for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
    {
        if (s_Data.TextureSlots[i] == Texture)
        {
            textureIndex = (float)i;
            break;
        }
    }

        // If texture not found, add it to the next available slot
    if (textureIndex < 1.0f && Texture != nullptr)
    {
        if (s_Data.TextureSlotIndex >= SRendererData::MaxTextureSlots)
        {
            // Flush and reset if we've run out of texture slots
            Flush();
            StartDraw();
        }

        textureIndex = (float)s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = Texture;
        s_Data.TextureSlotIndex++;
    }

    for (size_t i = 0; i < quadVertexCount; i++)
    {
        s_Data.QuadVertexBufferPtr->Position = Transform * s_Data.QuadVertexPositions[i];
        s_Data.QuadVertexBufferPtr->Color = TintColor;
        s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;
}

void Renderer::StartDraw()
{
    s_Data.LineVertexCount = 0;
    s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    s_Data.TextureSlotIndex = 1;
}

void Renderer::Flush()
{
    // Lines
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

    if (s_Data.QuadIndexCount)
    {
        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

        // Bind textures
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
            s_Data.TextureSlots[i]->Bind(i);

        s_Data.QuadShader->Bind();
        Renderer::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

        // Reset data
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.QuadIndexCount = 0;
    }
}

} // namespace Engine