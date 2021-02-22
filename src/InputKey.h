#pragma once

#include <vector>

namespace input
{
enum class InputKey
{
    MouseLeft,
    ControlLeft,
    ShiftLeft,
    Escape
};

inline std::vector allKeys{InputKey::MouseLeft, InputKey::ControlLeft, InputKey::ShiftLeft, InputKey::Escape};

}
