#pragma once

#include <App.hpp>

namespace Anvil
{
    class Editor : public Smith::App
    {
    private:
        /* data */
    public:
        Editor(/* args */);
        ~Editor();

        void Init();
    };

} // namespace Anvil
