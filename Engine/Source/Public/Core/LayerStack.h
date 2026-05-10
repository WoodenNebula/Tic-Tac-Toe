#pragma once

#include "Core/Layer.h"

#include <vector>

namespace Engine
{
class LayerStack
{
public:
    LayerStack() = default;
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PopLayer(Layer* layer);

    void PushOverlay(Layer* overlay);
    void PopOverlay(Layer* overlay);


    auto begin() { return m_Layers.begin(); }
    auto end() { return m_Layers.end(); }

    auto begin() const { return m_Layers.begin(); }
    auto end() const { return m_Layers.end(); }

    void Trace() const;
protected:
    std::vector<Layer*> m_Layers;
    std::vector<Layer*>::iterator m_LayerEnd{ m_Layers.begin() };
};
}