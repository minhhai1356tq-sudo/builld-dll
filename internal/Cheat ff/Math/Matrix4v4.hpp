#pragma once
#include <cmath>

struct Matrix4x4 {
    union {
        struct {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;
        };
        float m[4][4];
    };

    Matrix4x4() {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                m[i][j] = 0.0f;
    }

    bool IsValid() const {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (std::isnan(m[i][j]) || std::isinf(m[i][j]))
                    return false;
            }
        }

        if (m[0][0] == 0 && m[0][1] == 0 && m[0][2] == 0 && m[0][3] == 0 &&
            m[1][0] == 0 && m[1][1] == 0 && m[1][2] == 0 && m[1][3] == 0 &&
            m[2][0] == 0 && m[2][1] == 0 && m[2][2] == 0 && m[2][3] == 0 &&
            m[3][0] == 0 && m[3][1] == 0 && m[3][2] == 0 && m[3][3] == 0) {
            return false;
        }

        return true;
    }

    void Reset() {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                m[i][j] = 0.0f;
    }
};
