#pragma once

#include "camera.h"

class game;

class freefly_camera : public camera {
public:
    void init(game&);
    void tick(game&, double);

    [[nodiscard]] glm::mat4x4 get_view_matrix() const override;

private:
    glm::mat4x4 local_trans = glm::mat4x4(1.0f);

    double cam_x = 0.0;
    double cam_y = 0.0;
};
