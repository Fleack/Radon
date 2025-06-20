#include "MenuSceneAnimator.hpp"

#include <Urho3D/Core/CoreEvents.h>

namespace Radon
{

MenuSceneAnimator::MenuSceneAnimator(Context* context)
    : Object(context)
{
}

void MenuSceneAnimator::Setup(Scene* scene, Node* cameraNode)
{
    scene_ = scene;
    cameraNode_ = cameraNode;

    if (cameraNode_)
    {
        Quaternion rotation = cameraNode_->GetRotation();
        cameraYaw_ = rotation.YawAngle();
    }

    // Subscribe to update events for animation
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(MenuSceneAnimator, HandleUpdate));
}

void MenuSceneAnimator::Update(float timeStep)
{
    time_ += timeStep;

    AnimateCamera(timeStep);
    AnimateObjects(timeStep);
}

void MenuSceneAnimator::HandleUpdate(StringHash, VariantMap& eventData)
{
    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();
    Update(timeStep);
}

void MenuSceneAnimator::AnimateCamera(float timeStep)
{
    if (!cameraNode_)
        return;

    // Slowly orbit the camera around the scene
    cameraYaw_ += timeStep * rotationSpeed_;
    if (cameraYaw_ >= 360.0f)
        cameraYaw_ -= 360.0f;

    float radius = 15.0f;
    float height = 5.0f + Sin(time_ * 10.0f) * 0.5f; // Slight up and down motion

    Vector3 newPosition(
        radius * Cos(cameraYaw_),
        height,
        radius * Sin(cameraYaw_)
    );

    cameraNode_->SetPosition(newPosition);
    cameraNode_->LookAt(Vector3(0.0f, 1.0f, 0.0f)); // Look at a point slightly above the floor
}

void MenuSceneAnimator::AnimateObjects(float timeStep)
{
    if (!scene_)
        return;

    // Animate boxes in the menu scene
    for (int i = 0; i < 3; ++i)
    {
        Node* boxNode = scene_->GetChild("MenuBox" + i);
        if (boxNode)
        {
            // Add slight rotation to each box
            float rotSpeed = 5.0f - i * 1.0f; // Each box rotates at different speed
            boxNode->Rotate(Quaternion(0.0f, timeStep * rotSpeed, 0.0f));

            // Add slight bobbing motion to top box only
            if (i == 2) // top box
            {
                Vector3 pos = boxNode->GetPosition();
                pos.y_ = 0.5f + i * 1.1f + Sin(time_ * 3.0f) * 0.1f;
                boxNode->SetPosition(pos);
            }
        }
    }

    // Animate other objects
    for (int i = 0; i < 4; ++i)
    {
        Node* objNode = scene_->GetChild("SceneObject" + i);
        if (objNode)
        {
            // Each object rotates on a different axis
            Quaternion rotationDelta;
            switch (i % 3)
            {
                case 0: rotationDelta = Quaternion(timeStep * 15.0f, 0.0f, 0.0f); break;
                case 1: rotationDelta = Quaternion(0.0f, timeStep * 15.0f, 0.0f); break;
                case 2: rotationDelta = Quaternion(0.0f, 0.0f, timeStep * 15.0f); break;
            }
            objNode->Rotate(rotationDelta);
        }
    }
}

} // namespace Radon
