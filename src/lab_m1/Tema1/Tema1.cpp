#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    window->HidePointer();

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 10;   // logic width
    logicSpace.height = 10;  // logic height

    // Init Aim
    aim = Aim(5,2);
    for (int i = 0; i < aim.getComponentMeshes().size(); i++) {
        AddMeshToList(aim.getComponentMeshes()[i]);
    }

    // Init Duck
    poseXDuck = 8;
    poseYDuck = 0.001f;
    speedDuck = initialSpeed = 2;
    pastDucks = 0;
    currentTTL = maxTTL = 10;
    duck = Duck(poseXDuck, poseYDuck, speedDuck);
    for (int i = 0; i < duck.getComponentMeshes().size(); i++) {
        AddMeshToList(duck.getComponentMeshes()[i]);
    }

    // Init borders
    grassHeight = 2;
    grassWidth = 20;
    Mesh* grass = object2D::CreateRectangle("grass", glm::vec3(0, 0, 0), grassWidth, grassHeight, glm::vec3(0, 1, 0), true);
    AddMeshToList(grass);

    // Init Bullet
    bullet = Bullet(0, 0);
    maxBullets = currentBullets = 3;
    for (int i = 0; i < bullet.getComponentMeshes().size(); i++) {
        AddMeshToList(bullet.getComponentMeshes()[i]);
    }

    // Init Heart
    heart = Heart(0, 0);
    maxHearts = currentHearts = 3;
    for (int i = 0; i < heart.getComponentMeshes().size(); i++) {
        AddMeshToList(heart.getComponentMeshes()[i]);
    }

    // Init Score
    currentScore = 0;
    maxScore = 50;
    scoreHeight = 0.40f; scoreWidth = 2.5f;
    Mesh* scoreBorder = object2D::CreateRectangle("scoreBorder", glm::vec3(0, 0, 0), scoreWidth, scoreHeight, glm::vec3(0, 0, 1));
    Mesh* scoreSolid = object2D::CreateRectangle("scoreSolid", glm::vec3(0, 0, 0), scoreWidth, scoreHeight, glm::vec3(0, 0, 1), true);
    AddMeshToList(scoreBorder);
    AddMeshToList(scoreSolid);

    // Init background
    background = Background();
    for (int i = 0; i < background.getComponentMeshes().size(); i++) {
        AddMeshToList(background.getComponentMeshes()[i]);
    }
}


// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;

    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    //tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    //ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

void Tema1::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    // Compute uniform 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    currentTTL -= deltaTimeSeconds;
    if (currentTTL < 0 && duck.getState() == Duck::State::ACTIVE) {
        duck.setState(Duck::State::FLEE);
        currentHearts--;
    }

    checkDuck();
    DrawScene(visMatrix, deltaTimeSeconds);
}


void Tema1::FrameEnd()
{
}

void Tema1::checkDuck() {

    if (duck.getState() == Duck::State::RESET) {
        pastDucks++;
        currentTTL = maxTTL;
        speedDuck = initialSpeed + initialSpeed * (pastDucks / 5) / 5;

        float angularSpeed;
        float initialAngularSpeed = duck.getInitialAngularSpeed();
        angularSpeed = initialAngularSpeed + initialAngularSpeed * (pastDucks / 5) / 5;

        duck.resetState(poseXDuck - 5 + rand() % 11, poseYDuck, speedDuck, angularSpeed);
        currentBullets = maxBullets;

        if (currentHearts < 0) {
            cout << "Game over =))\n";
            exit(1);
        }
    }
}


void Tema1::DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds)
{
    // Draw Aim
    glm::mat3 reverseVisMatrix = glm::inverse(visMatrix);
    realMatrix = reverseVisMatrix * transform2D::Translate(aimX_real, viewSpace.height - aimY_real);
    aim.setPosition(realMatrix[2][0], realMatrix[2][1], deltaTimeSeconds);

    std::vector<glm::mat3> matrixComp = aim.getMatrixComp();

    modelMatrix = visMatrix * matrixComp[0];
    RenderMesh2D(meshes["squareAim1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * matrixComp[1];
    RenderMesh2D(meshes["squareAim2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * matrixComp[2];
    RenderMesh2D(meshes["circleAim"], shaders["VertexColor"], modelMatrix);

    // Draw environment
    realMatrix = reverseVisMatrix * transform2D::Translate(window->GetResolution().x, window->GetResolution().y);
    float logicX = realMatrix[2][0];
    float logicY = realMatrix[2][1];
    float scaleX_factor = logicX / grassWidth; // for responsive width bottom/ upper border
    float scaleY_factor = logicY / grassWidth; // for responsive height right/ left border

    // bottom border
    modelMatrix = visMatrix * transform2D::Translate(0, 0) * transform2D::Scale(scaleX_factor, 1);
    RenderMesh2D(meshes["grass"], shaders["VertexColor"], modelMatrix);

    // upper border
    modelMatrix = visMatrix * transform2D::Translate(0, logicY - 0.25) * transform2D::Scale(scaleX_factor, 1);
    RenderMesh2D(meshes["grass"], shaders["VertexColor"], modelMatrix);

    // left border (bottom border rotated with PI / 2)
    modelMatrix = visMatrix * transform2D::Translate(0.25, 0) * transform2D::Scale(1, scaleY_factor) * transform2D::Rotate(M_PI_2);
    RenderMesh2D(meshes["grass"], shaders["VertexColor"], modelMatrix);

    // right border (bottom border rotated with PI / 2)
    modelMatrix = visMatrix * transform2D::Translate(logicX + grassHeight - 0.25, 0) * transform2D::Scale(1, scaleY_factor) * transform2D::Rotate(M_PI_2);
    RenderMesh2D(meshes["grass"], shaders["VertexColor"], modelMatrix);

    // Draw Score
    modelMatrix = visMatrix * transform2D::Translate(logicX - 3.2, logicY - 1);
    RenderMesh2D(meshes["scoreBorder"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(logicX - 3.2, logicY - 1) * transform2D::Scale(currentScore / maxScore, 1);
    RenderMesh2D(meshes["scoreSolid"], shaders["VertexColor"], modelMatrix);

    // Draw Bullet
    for (int i = 0; i < maxBullets; i++) {
        modelMatrix = visMatrix * transform2D::Translate(1 + 0.4 * i, logicY - 1.2);
        if (i < currentBullets) {
            RenderMesh2D(meshes["squareBulletFull"], shaders["VertexColor"], modelMatrix);
        }
        else {
            RenderMesh2D(meshes["squareBulletEmpty"], shaders["VertexColor"], modelMatrix);
        }
    }

    // Draw Heart
    for (int i = 0; i < maxHearts; i++) {
        modelMatrix = visMatrix * transform2D::Translate(1 + 0.4 * i, logicY - 0.8);
        if (i < currentHearts) {
            RenderMesh2D(meshes["heartFull"], shaders["VertexColor"], modelMatrix);
        }
        else {
            RenderMesh2D(meshes["heartEmpty"], shaders["VertexColor"], modelMatrix);
        }
    }

    // Draw duck
    duck.setPosition(logicX, logicY, deltaTimeSeconds);
    matrixComp = duck.getMatrixComp();
    DrawDuck(visMatrix, matrixComp);

    // Draw background
    background.setPosition(logicX / 2.f, logicY / 2.f, deltaTimeSeconds);
    matrixComp = background.getMatrixComp();
    modelMatrix = visMatrix * matrixComp[0];
    RenderMesh2D(meshes["bgSquare1"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * matrixComp[1];
    RenderMesh2D(meshes["bgSquare2"], shaders["VertexColor"], modelMatrix);
    
}

void Tema1::DrawDuck(glm::mat3 visMatrix, std::vector<glm::mat3> matrixComp) {
    glm::mat3 modelMatrix;
    // Wing front
    modelMatrix = visMatrix * matrixComp[6];
    RenderMesh2D(meshes["duckWings"], shaders["VertexColor"], modelMatrix);
    // Tail
    modelMatrix = visMatrix * matrixComp[0];
    RenderMesh2D(meshes["duckPos"], shaders["VertexColor"], modelMatrix);
    // Leg
    modelMatrix = visMatrix * matrixComp[1];
    RenderMesh2D(meshes["duckLeg"], shaders["VertexColor"], modelMatrix);
    // Body
    modelMatrix = visMatrix * matrixComp[5];
    RenderMesh2D(meshes["duckHead"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * matrixComp[2];
    RenderMesh2D(meshes["duckBody"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * matrixComp[3];
    RenderMesh2D(meshes["duckBody"], shaders["VertexColor"], modelMatrix);
    // Neck
    modelMatrix = visMatrix * matrixComp[4];
    RenderMesh2D(meshes["duckNeck"], shaders["VertexColor"], modelMatrix);
    // Wing back
    modelMatrix = visMatrix * matrixComp[7];
    RenderMesh2D(meshes["duckWings"], shaders["VertexColor"], modelMatrix);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    
}


void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_H) {
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // save mouse position for translate aim later in update
    aimX_real = mouseX;
    aimY_real = mouseY;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Bullet Logic
    if (button == 1) {
        Duck::State currentState = duck.getState();

        currentBullets--;
        if (currentBullets < 0)
            currentBullets = 0;

        if (currentBullets == 0 && currentState == Duck::State::ACTIVE) {
            duck.setState(Duck::State::FLEE);

            currentHearts--;
        }

        if (currentState == Duck::State::ACTIVE && aim.BoundingBoxCollision(duck) == true) {
            currentScore += 1;
            if (currentScore > maxScore) {
                currentScore = maxScore;
            }
            duck.setState(Duck::State::DEAD);
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}
