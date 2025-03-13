#include "math/transform.hpp"

Transform Transform::from_translation(Vec2 translation) {
    Transform t;
    t.translation = translation;
    t.scale = Vec2::ONE();
    t.rotation = Rot2::IDENTITY();
    return t;
}

Transform Transform::from_scale(Vec2 scale) {
    Transform t;
    t.translation = Vec2::ZERO();
    t.scale = scale;
    t.rotation = Rot2::IDENTITY();
    return t;
}

Transform Transform::from_rotation(Rot2 rotation) {
    Transform t;
    t.translation = Vec2::ZERO();
    t.scale = Vec2::ONE();
    t.rotation = rotation;
    return t;
}

Mat4 Transform::to_mat4() const {
    Mat4 result = Mat4::IDENTITY();
    result.x.x = scale.x;
    result.y.y = scale.y;

    Vec2 rotated_x = rotation.rotate({1, 0});
    Vec2 rotated_y = rotation.rotate({0, 1});

    result.x.x = rotated_x.x * scale.x;
    result.x.y = rotated_x.y * scale.x;
    result.y.x = rotated_y.x * scale.y;
    result.y.y = rotated_y.y * scale.y;

    result.w.x = translation.x;
    result.w.y = translation.y;

    return result;
}
