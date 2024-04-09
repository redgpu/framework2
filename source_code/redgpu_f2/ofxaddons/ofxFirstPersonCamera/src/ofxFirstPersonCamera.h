#pragma once

#include "ofMain.h"

#include <GLFW/glfw3.h>

class ofxFirstPersonCamera : public ofCamera
{
  public:

    ofxFirstPersonCamera();
   ~ofxFirstPersonCamera();

    bool isControlled();
    void toggleControl();
    void enableControl();
    void disableControl();

    int keyUp;
    int keyDown;
    int keyLeft;
    int keyRight;
    int keyForward;
    int keyBackward;
    int keyRollLeft;
    int keyRollRight;
    int keyRollReset;

    float movespeed;
    float rollspeed;
    float sensitivity;

    ofVec3f upvector;

  protected:

    void update(ofEventArgs&);

    void keyPressed(ofKeyEventArgs&);
    void keyReleased(ofKeyEventArgs&);
    void mouseMoved(ofMouseEventArgs&);
    void mouseDragged(ofMouseEventArgs&);

  private:

    GLFWwindow* m_glfwWindow;
    
    struct Actions {
      char Up;
      char Down;
      char Left;
      char Right;
      char Forward;
      char Backward;
      char RollLeft;
      char RollRight;
      char RollReset;
    } m_keyState;

    bool m_isControlled;
    int  m_previousMouseX;
    int  m_previousMouseY;
};
