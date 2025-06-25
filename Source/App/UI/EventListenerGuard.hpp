#pragma once

#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/EventListener.h>

namespace Radon::UI
{

struct ListenerGuard
{
    ListenerGuard(Rml::Element* elem, Rml::EventId ev, Rml::SharedPtr<Rml::EventListener> listener)
        : element_(elem), event_(ev), listener_(std::move(listener))
    {
        element_->AddEventListener(event_, listener_.get());
    }

    ~ListenerGuard()
    {
        if (element_)
            element_->RemoveEventListener(event_, listener_.get());
    }

    ListenerGuard(ListenerGuard const&) = delete;
    ListenerGuard& operator=(ListenerGuard const&) = delete;

    ListenerGuard(ListenerGuard&& other) noexcept
        : element_(other.element_), event_(other.event_), listener_(std::move(other.listener_))
    {
        other.element_ = nullptr;
    }

    ListenerGuard& operator=(ListenerGuard&& other) noexcept
    {
        if (this == &other)
            return *this;

        if (element_)
            element_->RemoveEventListener(event_, listener_.get());
        element_ = other.element_;
        event_ = other.event_;
        listener_ = std::move(other.listener_);
        other.element_ = nullptr;
        return *this;
    }

private:
    Rml::Element* element_{};
    Rml::EventId event_{};
    Rml::SharedPtr<Rml::EventListener> listener_;
};

} // namespace Radon::UI
