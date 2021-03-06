/* 
Shipeng Xu
billhsu.x@gmail.com
*/
#pragma once
#include <iostream>
#include <vector>
#include "cashew/math/Vectors.h"
#include "cashew/scene/Plane.h"
#include "cashew/scene/LineSegment.h"
#include "cashew/math/Matrices.h"
#include "cashew/texture/Texture.h"

class StateIdle;
class StateDraw;
class StateSelectPlane;
class StateDeleteLine;
class State;
class Camera;
class UI;
class UINode;
class UIButton;
class UILabel;
class UIRadioButton;
class lua_State;
class Texture;

class Controller
{
public:
    static Controller& getInstance()
    {
        static Controller instance;
        std::cout <<"Controller getInstance()"<<std::endl;
        return instance;
    }

    
    void init();
    void MouseButton(int button, int state, int x, int y);
    void MouseMotion(int x, int y);
    void PassiveMotion(int x, int y);
    void Keyboard(unsigned char key, int x, int y);
    void update(float timeDelta);

    static void resize(int _width, int _heigth);
    static void UIButtonCallback(UINode* sender);

    enum {OPERATION_ADD_LINE = 1, OPERATION_DELETE_LINE};
    struct LineOperation
    {
        int lineID;
        int operation;
    };

    static std::vector<LineSegment> sketchLines;
    static std::vector<LineSegment> deletedLines;
    static std::vector<LineOperation> lineOperations;
    static void addLine(LineSegment l)
    {
        static int IDCounter = 0;
        l.ID = IDCounter++;
        LineOperation lineOp;
        lineOp.lineID = l.ID;
        lineOp.operation = OPERATION_ADD_LINE;
        lineOperations.push_back(lineOp);
        sketchLines.push_back(l);
    }
    static void delLine(LineSegment& l)
    {
        for(int i = 0; i < sketchLines.size(); ++i)
        {
            if(sketchLines[i].ID == l.ID)
            {
                LineOperation lineOp;
                lineOp.lineID = l.ID;
                lineOp.operation = OPERATION_DELETE_LINE;
                lineOperations.push_back(lineOp);
                deletedLines.push_back(sketchLines[i]);
                sketchLines.erase(Controller::sketchLines.begin()+i);
                break;
            }
        }
    }

    static Plane currPlane; // Plane to draw
    static Vector3 currPoint;
    static bool bCurrPoint;

    int status;

    static int width,height;
    static Matrix4 modelView;
    static Matrix4 projection;
    static int originWidth,originHeight;
    static int mouseX,mouseY;
    static int mouseButton,mouseState; // mouse status
    static int lastMouseX,lastMouseY; // last mouse position

    static int uiHold;

    StateIdle* sIdle;
    StateDeleteLine* sDelLine;
    StateSelectPlane* sSelectPlane;
    StateDraw* sDraw;
    Camera * camera;

    static cashew::Texture* texture;
    static UI* GUI;
    static UIButton *btnSelectVerticalPlane, *btnSelectHorizontalPlane;
    static UIButton *btnConfirmPlane, *btnCancelPlane;
    static UIButton *btnDeleteLine, *btnStandardView;
    static UIButton *btnUndo;
    static UIButton *btnDrawPlaneDone;
    static UIButton *btnDeleteLineDone;
    static UIButton *btnDocNew, *btnDocOpen, *btnDocSave;
    static UIButton *btnMirror;
    static UILabel  *lbFPS;
    static UIRadioButton *rbtnSelect;

    enum {BTN_ID_SELECT_VERTICAL=100, BTN_ID_SELECT_HORIZONTAL,
          BTN_ID_DOC_NEW, BTN_ID_DOC_OPEN, BTN_ID_DOC_SAVE,
          BTN_ID_CONFIRM_PLANE, BTN_ID_CANCEL_PLANE,
          BTN_ID_DRAW_PLANE_DONE,
          BTN_ID_DELETE_LINE,
          BTN_ID_DELETE_LINE_DONE,
          BTN_ID_STANDARD_VIEW,
          BTN_ID_UNDO,
          BTN_ID_MIRROR,
          LBL_FPS};
    enum {EVENT_BTN_CLICKED=100};
    
    static bool enableLight;
    static Vector3 rotate;
    static lua_State *luaState;

private:
    Controller();
    ~Controller();
    Controller(Controller const&);
    void operator=(Controller const&);
};
