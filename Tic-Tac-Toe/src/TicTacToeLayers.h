#pragma once
#include <Engine.h>
#include "TicTacToe.h"
#include "TicTacToeTypes.h"

namespace Game
{

struct SCellPosition;

class CTicTacToeLayer : public Engine::CLayer
{
public:
    CTicTacToeLayer(std::string_view Name = "TicTacToeLayer") : CLayer(Name) {}

    virtual ~CTicTacToeLayer() = default;

    virtual void OnAttach() override;

    virtual void OnUpdate(float deltaTime) override;
    virtual void OnEvent(Engine::Events::CEventBase& event) override;
protected:
    std::shared_ptr<Engine::CTexture> m_XTexture;
    std::shared_ptr<Engine::CTexture> m_OTexture;
};

class CTicTacToeOverlayLayer : public CTicTacToeLayer
{
public:
    CTicTacToeOverlayLayer(std::string_view Name = "OverlayLayer") : CTicTacToeLayer(Name)
    {
    }

    virtual void OnAttach() override;

    virtual void OnUpdate(float deltaTime) override;
};

class CWaitingForNetwork_OverlayLayer : public Engine::CLayer
{
public:
    CWaitingForNetwork_OverlayLayer() : CLayer("WaitingForNetwork") {}
    virtual ~CWaitingForNetwork_OverlayLayer() = default;

    virtual void OnAttach() override;

    virtual void OnUpdate(float deltaTime) override;
    virtual void OnEvent(Engine::Events::CEventBase& event) override;

protected:
    std::shared_ptr<Engine::CTexture> m_WaitingTexture;
};


class CBoardLayer : public Engine::CLayer
{
    struct SGrid
    {
        struct SLine
        {
            glm::vec3 Start;
            glm::vec3 End;
        } H1{}, H2{}, V1{}, V2{};

        float LineWidth{ 2.0f };
        glm::vec4 Color{ 1.0f,1.0f,1.0f,1.0f };
    };

public:
    CBoardLayer() : CLayer("BoardLayer") {}
    virtual ~CBoardLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(float deltaTime) override;
    virtual void OnEvent(Engine::Events::CEventBase& event) override;

    Point3D<float> CellPositionToNDC(const SCellPosition& cellPos) const;
    SCellPosition NDCToCellPosition(const Point2D<float>& NDCPos) const;
private:
    void DrawBoard();
    void DrawGrid();
    void DrawCell(int row, int col, enum ECellState state);
    void DrawX(int row, int col);
    void DrawO(int row, int col);

    bool OnMouseMoved(Engine::Events::InputEvents::CMouseMovedEvent& event);
    bool OnMouseButtonPressed(Engine::Events::InputEvents::CMouseButtonPressedEvent& event);

private:
    std::shared_ptr<Engine::CTexture> m_XTexture;
    std::shared_ptr<Engine::CTexture> m_OTexture;
    Point2D<double> m_MousePosition{};
    const float m_GridSize = 0.7f;
    const float m_CellSize = m_GridSize / 3.0f;
    SGrid m_Grid;
};
} // namespace Game