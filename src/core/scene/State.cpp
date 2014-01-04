#include "State.h"
#include "Controller.h"
#include "Camera.h"
#include "Scene.h"
#include "Plane.h"
#include "Quaternion.h"

State::State()
{
    ctrl = &Controller::getInstance();
    camera = &Camera::getInstance();
    std::cout<<"State State()"<<std::endl;
}

// State Idle

void StateIdle::MouseButton(int button, int state, int x, int y)
{    
    if(state==GLUT_DOWN)
    {
        if(button == GLUT_LEFT_BUTTON)
        {
            stateSelectPlane->selectedPoints.clear();
            Vector3 v;
            camera->getPoint(v);
            std::cout<<v<<std::endl;
            stateSelectPlane->selectedPoints.push_back(v);
            Plane::buildPlane(
                stateSelectPlane->selectedPoints, Controller::currPlane);
            Quaternion q = Quaternion::fromVector(Controller::currPlane.N, 
                Quaternion::Z_NEG_AXIS);
            camera->setCamCenterTo(v);
            camera->rotateCamTo(q);
            enterState(stateSelectPlane);
        }

        if(button==GLUT_RIGHT_BUTTON)
        {
            Controller::lastMouseX=x;
            Controller::lastMouseY=y;
        }
        if(button == 3)
        {
            float dist = camera->distance;
            camera->setCamDist(dist-2);
        }
        if(button == 4)
        {
            float dist = camera->distance;
            camera->setCamDist(dist+2);
        }
    }
}

void StateIdle::MouseMotion(int x, int y)
{
    int dx,dy;

    dx = x - Controller::lastMouseX;
    dy = y - Controller::lastMouseY;
    Controller::lastMouseX = x;
    Controller::lastMouseY = y;

    if(Controller::mouseState==GLUT_DOWN)
    {
        if(Controller::mouseButton==GLUT_RIGHT_BUTTON)
        {
            Controller::rotate.x -=dy;
            Controller::rotate.y +=dx;
            camera->rotateCam(Controller::rotate);
        }
    }
}

void StateIdle::PassiveMotion(int x, int y)
{

}

void StateIdle::Keyboard(unsigned char key, int x, int y)
{

}

void StateIdle::render(float timeDelta)
{
    
}

// State Select Plane
void StateSelectPlane::MouseButton(int button, int state, int x, int y)
{
    if(state==GLUT_DOWN)
    {
        if(button == GLUT_LEFT_BUTTON)
        {
            Vector3 v;
            camera->getPoint(v);
            std::cout<<v<<std::endl;
            selectedPoints.push_back(v);
            Plane::buildPlane(selectedPoints, Controller::currPlane);
            Quaternion q = Quaternion::fromVector(Controller::currPlane.N, 
                Quaternion::Z_NEG_AXIS);
            camera->setCamCenter((v+selectedPoints[0])/2);
            camera->rotateCamTo(q);
            if(selectedPoints.size()==3) enterState(stateDraw);
        }
        if(button == GLUT_RIGHT_BUTTON)
        {
            Controller::lastMouseX=x;
            Controller::lastMouseY=y;
        }
    }
}

void StateSelectPlane::MouseMotion(int x, int y)
{
    int dx,dy;

    dx = x - Controller::lastMouseX;
    dy = y - Controller::lastMouseY;
    Controller::lastMouseX = x;
    Controller::lastMouseY = y;

    if(Controller::mouseState==GLUT_DOWN)
    {
        if(Controller::mouseButton==GLUT_RIGHT_BUTTON)
        {
            Controller::rotate.x -=dy;
            Controller::rotate.y +=dx;
            camera->rotateCam(Controller::rotate);
        }
    }
}

void StateSelectPlane::PassiveMotion(int x, int y)
{

}

void StateSelectPlane::Keyboard(unsigned char key, int x, int y)
{
    if(key == 'x')
    {
        enterState(stateDraw);
    }
}

void StateSelectPlane::render(float timeDelta)
{
    Controller::currPlane.drawPlane(selectedPoints[0], 10);
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(1,1,0);
    for(int i = 0; i<selectedPoints.size(); ++i)
    {
        glVertex3fv(selectedPoints[i].cell);
    }
    glEnd();
    glPointSize(1);
}

// State Draw
void StateDraw::MouseButton(int button, int state, int x, int y)
{

}

void StateDraw::MouseMotion(int x, int y)
{

}

void StateDraw::PassiveMotion(int x, int y)
{

}

void StateDraw::Keyboard(unsigned char key, int x, int y)
{
    if(key == 27) exit(1);
}

void StateDraw::render(float timeDelta)
{
    
}
