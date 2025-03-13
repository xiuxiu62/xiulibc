#pragma once

#include "math/matrix.hpp"
#include "math/rotor.hpp"
#include "math/vector.hpp"

struct Transform {
    Vec2 translation, scale;
    Rot2 rotation;

    static constexpr Transform DEFAULT() {
        return {Vec2::ZERO(), Vec2::ONE(), Rot2::IDENTITY()};
    }

    static Transform from_translation(Vec2 translation);
    static Transform from_scale(Vec2 scale);
    static Transform from_rotation(Rot2 rotation);

    Mat4 to_mat4() const;
};
