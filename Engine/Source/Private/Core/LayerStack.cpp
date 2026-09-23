#include "Core/LayerStack.h"

#include "Core/Layer.h"
#include "Logger/Logger.h"

DECLARE_LOG_CATEGORY(LayerStack)

namespace Engine
{
CLayerStack::~CLayerStack()
{
    for (CLayer* layer : m_Layers)
    {
        delete layer;
    }
}

void CLayerStack::PushLayer(CLayer* layer)
{
    m_LayerEnd = m_Layers.emplace(m_LayerEnd, layer);
    LOG(LogLayerStack, Trace, "Pushed Layer {}", layer->GetName());
}

void CLayerStack::PopLayer(CLayer* layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end())
    {
        m_Layers.erase(it);
        m_LayerEnd--;
        LOG(LogLayerStack, Trace, "Popped Layer {}", layer->GetName());
    }
}

void CLayerStack::PopOverlay(CLayer* layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end())
    {
        m_Layers.erase(it);
        LOG(LogLayerStack, Trace, "Popped Overlay {}", layer->GetName());
    }
}

void CLayerStack::PushOverlay(CLayer* layer)
{
    m_Layers.emplace_back(layer);
    LOG(LogLayerStack, Trace, "Pushed Overlay {}", layer->GetName());
}

void CLayerStack::TraceLayerStack() const
{
    LOG(LogLayerStack, Trace, "Layer Stack:");
    for (const CLayer* layer : m_Layers)
    {
        LOG(LogLayerStack, Trace, " - {}", layer->GetName());
    }
}
};