#pragma once

#include <vector>

#include "components/simple_scene.h"


namespace m1
{
    class tema2 : public gfxc::SimpleScene
    {
     public:
        tema2();
        ~tema2();

        void Init() override;

     private:
        void CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        glm::mat4 modelMatrix;
        float translateX = 0;
        float translateY = 0;
        float translateZ = 0;
        float angularStepOX = 0;
        float angularStepOY = 0;
        float angularStepOY_2 = 0;
        float angularStepOZ = 0;
        GLenum cullFace;
        GLenum polygonMode;
    };
}   // namespace m1
