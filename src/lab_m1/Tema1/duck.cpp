#include "duck.h"
#include <iostream>

#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/transform2D.h"

using namespace std;
using namespace m1;

Duck::Duck(float posX, float posY, float currentSpeed) : Objects(posX, posY, 2.2, 2), currentSpeed(currentSpeed), maxSpeed(currentSpeed) {
    glm::mat3 matrix;
    trigRot = true;
    angularSpeed = initialAngularSpeed = M_PI / 3;
    wingRot = false;
    currentState = State::ACTIVE;

    duckAngle = 0;
    up = 1;
    srand(time(NULL));
    generateNewDirection();

    // Meshes used for duck
    Mesh* duckPos = object2D::CreateIsosTriangle("duckPos", glm::vec3(0, 0, 0), M_SQRT2, M_SQRT2 / 2, glm::vec3(0.1, 0.3, 0.18), true);
    Mesh* duckLeg = object2D::CreateIsosTriangle("duckLeg", glm::vec3(0, 0, 0), M_SQRT2, M_SQRT2 / 2, glm::vec3(1, 0.62, 0.16), true);
    Mesh* duckBody = object2D::CreateIsosTriangle("duckBody", glm::vec3(0, 0, 0), M_SQRT2, M_SQRT2 / 2, glm::vec3(0.1, 0.3, 0.18), true);
    Mesh* duckNeck = object2D::CreatePolygon("duckNeck", glm::vec3(0, 0, 0), M_SQRT2 / 2, 3, glm::vec3(1, 0.62, 0.16), true);
    Mesh* duckHead = object2D::CreatePolygon("duckHead", glm::vec3(0, 0, 0), 0.5, 50, glm::vec3(1, 0.62, 0.16), true);
    Mesh* duckWings = object2D::CreateIsosTriangle("duckWings", glm::vec3(0, 0, 0), 1, sqrt(3) / 2, glm::vec3(0.99, 0.95, 0.89), true);

    // Tail component
    componentMeshes.push_back(duckPos);
    matrix = glm::mat3(1) * transform2D::Translate(posX + 1, posY - 1) * transform2D::Rotate(M_PI - M_PI_4);
    matrixComp.push_back(matrix);

    //  Leg component
    componentMeshes.push_back(duckLeg);
    matrix = glm::mat3(1) * transform2D::Translate(posX - M_SQRT2 / 2 * 0.45, posY - 1 - M_SQRT2 / 2 * 0.45) * transform2D::Scale(0.45, 0.45);
    matrixComp.push_back(matrix);

    // Body component
    componentMeshes.push_back(duckBody);
    matrix = glm::mat3(1) * transform2D::Translate(posX, posY - 1) * transform2D::Shear(0, -0.3) * transform2D::Rotate(M_PI - M_PI_4);
    matrixComp.push_back(matrix);

    matrix = glm::mat3(1) * transform2D::Translate(posX, posY - 1) * transform2D::Shear(0, -0.3) * transform2D::Rotate(-M_PI_4) *
                transform2D::Translate(-M_PI / 2.2, 0);
    matrixComp.push_back(matrix);

    // Neck component
    componentMeshes.push_back(duckNeck);
    matrix = glm::mat3(1) * transform2D::Translate(posX - 1.4, posY + 0.9) * transform2D::Scale(0.5, 0.5) * transform2D::Rotate(-M_PI / 6 - M_PI / 12 + M_PI);
    matrixComp.push_back(matrix);

    componentMeshes.push_back(duckHead);
    matrix = glm::mat3(1) * transform2D::Translate(posX - 1, posY + 0.5);
    matrixComp.push_back(matrix);

    // Wings component
    componentMeshes.push_back(duckWings);
    matrix = glm::mat3(1) * transform2D::Translate(posX, posY) * transform2D::Rotate(-M_PI_4 - wingRot) * transform2D::Scale(M_PI_2, M_PI_2);
    matrixComp.push_back(matrix);

    matrix = glm::mat3(1) * transform2D::Translate(posX, posY) * transform2D::Rotate(M_PI_4 * 2 / 3 + wingRot) * transform2D::Scale(M_PI_2, M_PI_2);
    matrixComp.push_back(matrix);
}

Duck::Duck(float posX, float posY) : Duck(posX, posY, 0) {};

Duck::Duck() : Duck(0, 0) {};

void Duck::setPosition(float logicX, float logicY, float deltaTimeSeconds)
{
    if (trigRot) {
        wingRot += angularSpeed * deltaTimeSeconds;
        if (wingRot > M_PI_4)
            trigRot = false;
    }

    else {
        wingRot -= angularSpeed * deltaTimeSeconds;
        if (wingRot < 0) {
            trigRot = true;
        }
    }

    duckAngle += M_PI_4 * deltaTimeSeconds;

    posX += cos(dirAngle) * currentSpeed * right * deltaTimeSeconds;
    if (posX > logicX) {
        right = -1;
        mirrorOy = glm::mat3(1);
    }
    if (posX < 0) {
        right = 1;
       mirrorOy = transform2D::MirrorOy();
    }

    switch (currentState)
    {
        case State::ACTIVE: activeState(logicY, deltaTimeSeconds); break;
        case State::DEAD: deadState(logicY, deltaTimeSeconds); break;
        case State::FLEE: fleeState(logicY, deltaTimeSeconds); break;
        default: posX = -5; posY = -5;
    }
    
    updatePosition();
}

void Duck::activeState(float logicY, float deltaTimeSeconds) {
    posY += sin(dirAngle) * currentSpeed * up * deltaTimeSeconds;

    if (posY > logicY) {
        up = -1;
        //mirrorOx = transform2D::MirrorOx();
    }
    if (posY < 1) {
        up = 1;
        //mirrorOx = glm::mat3(1);
    }
}

void Duck::deadState(float logicY, float deltaTimeSeconds) {
    currentSpeed += 0.1f;
    posY += sin(dirAngle) * currentSpeed * (-1) * deltaTimeSeconds;
    dirAngle = M_PI_2;

    //mirrorOx = transform2D::MirrorOx();

    if (posY < 0) {
        currentState = State::RESET;
    }
}

void Duck::fleeState(float logicY, float deltaTimeSeconds) {
    currentSpeed += 0.1f;
    posY += sin(dirAngle) * currentSpeed * deltaTimeSeconds;
    //mirrorOx = glm::mat3(1);

    if (posY > logicY + 1) {
        currentState = State::RESET;
    }
}

void Duck::resetState(float posX, float posY, float currentSpeed, float angularSpeed) {
    currentState = State::ACTIVE;
    this->posX = posX;
    this->posY = posY;
    this->currentSpeed = currentSpeed;
    this->angularSpeed = angularSpeed;

    duckAngle = 0;
    up = 1;
    generateNewDirection();

}

void Duck::updatePosition() {
    // Tail component
    matrixComp[0] = glm::mat3(1) * transform2D::Translate(posX + 1, posY - 1) *
                    transform2D::Translate(-1, 1) * mirrorOx * mirrorOy * transform2D::Rotate(dirAngle) *  transform2D::Translate(1, -1) *
                    transform2D::Rotate(M_PI - M_PI_4);

    //  Leg component
    matrixComp[1] = glm::mat3(1) * transform2D::Translate(posX - M_SQRT2 / 2 * 0.45, posY - 1 - M_SQRT2 / 2 * 0.45) *
                    transform2D::Translate(M_SQRT2 / 2 * 0.45, 1 + M_SQRT2 / 2 * 0.45) 
                    * mirrorOx * mirrorOy * transform2D::Rotate(dirAngle) *
                    transform2D::Translate(-M_SQRT2 / 2 * 0.45, -1 - M_SQRT2 / 2 * 0.45) *
                    transform2D::Scale(0.45, 0.45);

    // Body component
    matrixComp[2] = glm::mat3(1) * transform2D::Translate(posX + 0.01f, posY - 1) *
                    transform2D::Translate(-0.01f, 1) * mirrorOx * mirrorOy * transform2D::Rotate(dirAngle) * transform2D::Translate(0.01f, -1) *
                    transform2D::Shear(0, -0.3) * transform2D::Rotate(M_PI - M_PI_4);

    matrixComp[3] = glm::mat3(1) * transform2D::Translate(posX, posY - 1) *
                    transform2D::Translate(0, 1) * mirrorOx * mirrorOy * transform2D::Rotate(dirAngle) * transform2D::Translate(0, -1) *
                    transform2D::Shear(0, -0.3) * transform2D::Rotate(-M_PI_4) *
                    transform2D::Translate(-M_PI / 2.2, 0);

    // Neck component
    matrixComp[4] = glm::mat3(1) * transform2D::Translate(posX - 1.4, posY + 0.9) *
                    transform2D::Translate(1.4, -0.9) * mirrorOx * mirrorOy * transform2D::Rotate(dirAngle) * transform2D::Translate(-1.4, 0.9) *
                    transform2D::Scale(0.5, 0.5) * transform2D::Rotate(-M_PI / 6 - M_PI / 12 + M_PI);
    matrixComp[5] = glm::mat3(1) * transform2D::Translate(posX - 1, posY + 0.5) *
                    transform2D::Translate(1, -0.5) * mirrorOx * mirrorOy * transform2D::Rotate(dirAngle) * transform2D::Translate(-1, 0.5);

    // Wings component
    matrixComp[6] = glm::mat3(1) * transform2D::Translate(posX, posY) *
                    mirrorOx * mirrorOy * transform2D::Rotate(dirAngle) *
                    transform2D::Rotate(-M_PI_4 - wingRot) * transform2D::Scale(M_PI_2, M_PI_2);
    matrixComp[7] = glm::mat3(1) * transform2D::Translate(posX, posY) *
                    mirrorOx * mirrorOy * transform2D::Rotate(dirAngle) *
                    transform2D::Rotate(M_PI_4 * 2 / 3 + wingRot) * transform2D::Scale(M_PI_2, M_PI_2);
}

void Duck::generateNewDirection() {
    mirrorOx = glm::mat3(1);
    if (rand() % 2 == 0) {
        right = 1;
        mirrorOy = transform2D::MirrorOy();
    }
    else {
        right = -1;
        mirrorOy = glm::mat3(1);
    }
    dirAngle = 30 + rand() % 30;
    dirAngle = RADIANS(dirAngle);
}

void Duck::setState(State currentState) {
    this->currentState = currentState;
}

Duck::State Duck::getState() {
    return currentState;
}

void Duck::setSpeed(float currentSpeed) {
    this->currentSpeed = currentSpeed;
}

void Duck::setStartPosition(float posX, float posY) {
    this->posX = posX;
    this->posY = posY;
}

float Duck::getInitialAngularSpeed() {
    return initialAngularSpeed;
}