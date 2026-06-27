#pragma once

class IPanel
{
    public:
        virtual void render() = 0;
        virtual ~IPanel() = default;
};
