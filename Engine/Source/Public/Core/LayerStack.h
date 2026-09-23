#pragma once

#include "Core/Layer.h"

#include <vector>

namespace Engine
{
class CLayerStack
{
public:
    CLayerStack() = default;
    ~CLayerStack();

    void PushLayer(CLayer* layer);
    void PopLayer(CLayer* layer);

    void PushOverlay(CLayer* overlay);
    void PopOverlay(CLayer* overlay);


    auto begin() { return m_Layers.begin(); }
    auto end() { return m_Layers.end(); }

    auto begin() const { return m_Layers.begin(); }
    auto end() const { return m_Layers.end(); }

    void TraceLayerStack() const;
protected:
    std::vector<CLayer*> m_Layers;
    std::vector<CLayer*>::iterator m_LayerEnd{ m_Layers.begin() };
};
}