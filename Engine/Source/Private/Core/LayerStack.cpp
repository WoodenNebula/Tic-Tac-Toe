#include "Core/LayerStack.h"

#include "Core/Layer.h"
#include "Logger/Logger.h"

DECLARE_LOG_CATEGORY(LayerStack)

namespace Engine
{
LayerStack::~LayerStack()
{
    for (Layer* layer : m_Layers)
    {
        delete layer;
    }
}

void LayerStack::PushLayer(Layer* layer)
{
    m_LayerEnd = m_Layers.emplace(m_LayerEnd, layer);
    LOG(LayerStack, TRACE, "Pushed Layer {}", layer->GetName());
}

void LayerStack::PopLayer(Layer* layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end())
    {
        m_Layers.erase(it);
        m_LayerEnd--;
        LOG(LayerStack, TRACE, "Popped Layer {}", layer->GetName());
    }
}

void LayerStack::PopOverlay(Layer* layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end())
    {
        m_Layers.erase(it);
        LOG(LayerStack, TRACE, "Popped Overlay {}", layer->GetName());
    }
}

void LayerStack::PushOverlay(Layer* layer)
{
    m_Layers.emplace_back(layer);
    LOG(LayerStack, TRACE, "Pushed Overlay {}", layer->GetName());
}

void LayerStack::Trace() const
{
    LOG(LayerStack, TRACE, "Layer Stack:");
    for (const Layer* layer : m_Layers)
    {
        LOG(LayerStack, TRACE, " - {}", layer->GetName());
    }
}
};