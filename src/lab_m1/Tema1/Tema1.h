#pragma once

#include "lab_m1/Tema1/aim.h"
#include "lab_m1/Tema1/duck.h"
#include "lab_m1/Tema1/bullet.h"
#include "lab_m1/Tema1/heart.h"
#include "lab_m1/Tema1/background.h"
#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

     public:
         Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds = 1);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

        // Sets the logic space and view space
        // logicSpace: { x, y, width, height }
        // viewSpace: { x, y, width, height }
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace &logicSpace, const ViewportSpace &viewSpace);

        void SetViewportArea(const ViewportSpace &viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);
        void DrawDuck(glm::mat3 visMatrix, std::vector<glm::mat3> matrixComp);
        void checkDuck();

     protected:
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix, realMatrix;

        // Aim variables
        Aim aim;
        float aimX_real;
        float aimY_real;

        // Duck variables
        Duck duck;
        int pastDucks;
        float currentTTL;
        float maxTTL;
        float poseXDuck;
        float poseYDuck;
        float initialSpeed;
        float speedDuck;

        // Environment variables
        float grassWidth;
        float grassHeight;

        // Bullet variables
        Bullet bullet;
        int maxBullets;
        int currentBullets;

        // Heart variables
        Heart heart;
        int maxHearts;
        int currentHearts;

        // Score variables
        float currentScore;
        float maxScore;
        float scoreHeight, scoreWidth;

        // Background variables
        Background background;
    };
}   // namespace m1
