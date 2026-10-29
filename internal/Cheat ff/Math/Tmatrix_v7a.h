#pragma once

#include <cstdint>
#include "FrameWork/Math/Vectors/Vector4D.hpp"
#include "Quaternion.hpp"
#include <internal/Cheat ff/Memory/Memory.hpp>
struct TMatrix2 {
    Vector4 position;
    Quaternion rotation;
    Vector4 scale;
};

class TransformUtils2 {
public:
    static bool GetPosition(uint32_t transform, Vector3& pos) {
        pos = Vector3::Zero();

        if (transform == 0 || transform == (uint32_t)-1) return false;

        uint32_t transformObjValue = 0;
        if (!Mem.Read(transform + 0x8, transformObjValue)) return false;
        if (transformObjValue == 0 || transformObjValue == (uint32_t)-1) return false;

        uint32_t indexValue = 0;
        if (!Mem.Read(transformObjValue + 0x24, indexValue)) return false;
        if (indexValue == (uint32_t)-1) return false;

        uint32_t matrixValue = 0;
        if (!Mem.Read(transformObjValue + 0x20, matrixValue)) return false;
        if (matrixValue == 0 || matrixValue == (uint32_t)-1) return false;

        uint32_t matrixListValue = 0;
        if (!Mem.Read(matrixValue + 0x18, matrixListValue)) return false;
        if (matrixListValue == 0 || matrixListValue == (uint32_t)-1) return false;

        uint32_t matrixIndicesValue = 0;
        if (!Mem.Read(matrixValue + 0x1C, matrixIndicesValue)) return false;
        if (matrixIndicesValue == 0 || matrixIndicesValue == (uint32_t)-1) return false;

        Vector3 resultValue = Vector3::Zero();
        uint32_t readAddr = indexValue * 0x30 + matrixListValue;
        if (readAddr == 0 || readAddr == (uint32_t)-1) return false;

        if (!Mem.Read(readAddr, resultValue)) return false;

        int maxTries = 50;
        int tries = 0;
        int transformIndexValue = 0;

        uint32_t indicesAddr = (uint32_t)((indexValue * 0x4) + matrixIndicesValue);
        if (indicesAddr == 0 || indicesAddr == (uint32_t)-1) return false;

        if (!Mem.Read(indicesAddr, transformIndexValue)) return false;

        while (transformIndexValue >= 0 && tries < maxTries) {
            tries++;

            uint32_t tMatrixAddr = (uint32_t)(0x30 * transformIndexValue + matrixListValue);
            if (tMatrixAddr == 0 || tMatrixAddr == (uint32_t)-1) break;

            TMatrix2 tMatrixValue = {};
            if (!Mem.Read(tMatrixAddr, tMatrixValue)) break;

            float rotX = tMatrixValue.rotation.X;
            float rotY = tMatrixValue.rotation.Y;
            float rotZ = tMatrixValue.rotation.Z;
            float rotW = tMatrixValue.rotation.W;

            if (std::isnan(rotX) || std::isnan(rotY) || std::isnan(rotZ) || std::isnan(rotW)) break;
            if (std::isinf(rotX) || std::isinf(rotY) || std::isinf(rotZ) || std::isinf(rotW)) break;

            float scaleX = resultValue.X * tMatrixValue.scale.x;
            float scaleY = resultValue.Y * tMatrixValue.scale.y;
            float scaleZ = resultValue.Z * tMatrixValue.scale.z;

            resultValue.X = tMatrixValue.position.x + scaleX +
                (scaleX * ((rotY * rotY * -2.0f) - (rotZ * rotZ * 2.0f))) +
                (scaleY * ((rotW * rotZ * -2.0f) - (rotY * rotX * -2.0f))) +
                (scaleZ * ((rotZ * rotX * 2.0f) - (rotW * rotY * -2.0f)));

            resultValue.Y = tMatrixValue.position.y + scaleY +
                (scaleX * ((rotX * rotY * 2.0f) - (rotW * rotZ * -2.0f))) +
                (scaleY * ((rotZ * rotZ * -2.0f) - (rotX * rotX * 2.0f))) +
                (scaleZ * ((rotW * rotX * -2.0f) - (rotZ * rotY * -2.0f)));

            resultValue.Z = tMatrixValue.position.z + scaleZ +
                (scaleX * ((rotW * rotY * -2.0f) - (rotX * rotZ * -2.0f))) +
                (scaleY * ((rotY * rotZ * 2.0f) - (rotW * rotX * -2.0f))) +
                (scaleZ * ((rotX * rotX * -2.0f) - (rotY * rotY * 2.0f)));

            if (std::isnan(resultValue.X) || std::isnan(resultValue.Y) || std::isnan(resultValue.Z)) break;
            if (std::isinf(resultValue.X) || std::isinf(resultValue.Y) || std::isinf(resultValue.Z)) break;

            uint32_t nextAddr = (uint32_t)(transformIndexValue * 0x4 + matrixIndicesValue);
            if (nextAddr == 0 || nextAddr == (uint32_t)-1) break;

            if (!Mem.Read(nextAddr, transformIndexValue)) break;
        }

        if (tries >= maxTries) return false;

        if (std::isnan(resultValue.X) || std::isnan(resultValue.Y) || std::isnan(resultValue.Z)) return false;
        if (std::isinf(resultValue.X) || std::isinf(resultValue.Y) || std::isinf(resultValue.Z)) return false;

        pos = resultValue;
        return true;
    }

    static bool GetNodePosition(uint32_t nodeTransform, Vector3& result) {
        result = Vector3::Zero();

        if (nodeTransform == 0 || nodeTransform == (uint32_t)-1) return false;

        uint32_t transformValue = 0;
        if (!Mem.Read(nodeTransform + 0x8, transformValue)) return false;
        if (transformValue == 0 || transformValue == (uint32_t)-1) return false;

        return GetPosition(transformValue, result);
    }
};
