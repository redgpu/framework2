#ifdef _WIN32
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "of_v0.12.0_vs_release/libs/cairo/lib/vs/x64/zlib.lib")
#pragma comment(lib, "of_v0.12.0_vs_release/libs/glew/lib/vs/x64/glew32s.lib")
#pragma comment(lib, "glfw-3.3.8.bin.WIN64/lib-vc2019/glfw3dll.lib")
#pragma comment(lib, "of_v0.12.0_vs_release/libs/uriparser/lib/vs/x64/uriparser.lib")
#pragma comment(lib, "of_v0.12.0_vs_release/libs/FreeImage/lib/vs/x64/FreeImage.lib")
#pragma comment(lib, "of_v0.12.0_vs_release/libs/freetype/lib/vs/x64/libfreetype.lib")
#pragma comment(lib, "of_v0.12.0_vs_release/libs/curl/lib/vs/x64/libcurl.lib")
#pragma comment(lib, "of_v0.12.0_vs_release/libs/openssl/lib/vs/x64/libssl.lib")
#pragma comment(lib, "of_v0.12.0_vs_release/libs/openssl/lib/vs/x64/libcrypto.lib")
#pragma comment(lib, "of_v0.12.0_vs_release/libs/fmod/lib/vs/x64/fmod64_vc.lib")
#pragma comment(lib, "of_v0.12.0_vs_release/libs/tess2/lib/vs/x64/tess2.lib")
#ifdef _DEBUG
#pragma comment(lib, "src/pugixml/lib/vs/x64/Debug/pugixml.lib")
#pragma comment(lib, "ofxaddons/ofxFBX/libs/libfbxsdk_2019.5/lib/vs/x64/Debug/libxml2-md.lib")
#pragma comment(lib, "ofxaddons/ofxFBX/libs/libfbxsdk_2019.5/lib/vs/x64/Debug/libfbxsdk-md.lib")
#pragma comment(lib, "addons/ofxAssimpModelLoader/libs/assimp/lib/vs/x64/Debug/assimp-vc142-mtd.lib")
#pragma comment(lib, "of_v0.12.0_vs_release/libs/openFrameworksCompiled/lib/vs/x64/openframeworksLib_debug.lib")
#else  // #ifdef _DEBUG
#pragma comment(lib, "src/pugixml/lib/vs/x64/Release/pugixml.lib")
#pragma comment(lib, "ofxaddons/ofxFBX/libs/libfbxsdk_2019.5/lib/vs/x64/Release/libxml2-md.lib")
#pragma comment(lib, "ofxaddons/ofxFBX/libs/libfbxsdk_2019.5/lib/vs/x64/Release/libfbxsdk-md.lib")
#pragma comment(lib, "addons/ofxAssimpModelLoader/libs/assimp/lib/vs/x64/Release/assimp-vc142-mt.lib")
#pragma comment(lib, "of_v0.12.0_vs_release/libs/openFrameworksCompiled/lib/vs/x64/openframeworksLib.lib")
#endif // #ifdef _DEBUG
#endif // #ifdef _WIN32

#ifdef _WIN32
#ifdef _DEBUG
#if 0
// https://codeberg.org/redgpu/tracing
#pragma comment(lib, "../tracing/tracingdll.lib")
#include "../tracing/tracing.h"
#else
static inline void tracingSet(const char * label) {}
static inline void tracingEnd(const char * label) {}
static inline void tracingPrint() {}
#endif
#endif // #ifdef _DEBUG
#endif // #ifdef _WIN32

#include "ofMain.h"

#ifndef _WIN32
// https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
// https://stackoverflow.com/questions/15615136/is-codecvt-not-a-std-header/28875347#28875347

#include <boost/locale/encoding_utf.hpp>

static std::wstring s2ws(const std::string & str) {
  return boost::locale::conv::utf_to_utf<wchar_t>(str.c_str(), str.c_str() + str.size());
}

static std::string ws2s(const std::wstring & wstr) {
  return boost::locale::conv::utf_to_utf<char>(wstr.c_str(), wstr.c_str() + wstr.size());
}
#endif

#ifdef _WIN32
#define REDGPU_F_DECLSPEC extern "C" __declspec(dllexport)
#endif
#ifdef __linux__
#define REDGPU_F_DECLSPEC extern "C" __attribute__((visibility("default")))
#endif
#include "redgpu_f2.h"

#include "addons/ofxAssimpModelLoader/src/ofxAssimpModelLoader.h"
#include "ofxaddons/ofxFirstPersonCamera/src/ofxFirstPersonCamera.h"
#include "ofxaddons/ofxManipulator/src/ofxManipulator.h"
#include "ofxaddons/ofxRaccoonImGui/src/ofxRaccoonImGui.hpp"
#include "ofxaddons/ofxFBX/src/ofxFBX.h"
#include "ofxaddons/ofxPostProcessing/src/ofxPostProcessing.h"
#include "ofxaddons/ofxPostProcessingExtra/GenericPass.h"

#ifdef _WIN32
#include <atlbase.h> // For CA2W, CW2A
#endif

RedFEvents __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events;

class ofApp : public ofBaseApp {
public:
  void setup           (void);
  void update          (void);
  void draw            (void);
  void exit            (void);

  void keyPressed      (ofKeyEventArgs &);
  void keyReleased     (ofKeyEventArgs &);

  void mouseMoved      (ofMouseEventArgs &);
  void mouseDragged    (ofMouseEventArgs &);
  void mousePressed    (ofMouseEventArgs &);
  void mouseReleased   (ofMouseEventArgs &);
  void mouseScrolled   (ofMouseEventArgs &);
  void mouseEntered    (ofMouseEventArgs &);
  void mouseExited     (ofMouseEventArgs &);

  void touchDown       (ofTouchEventArgs &);
  void touchMoved      (ofTouchEventArgs &);
  void touchUp         (ofTouchEventArgs &);
  void touchDoubleTap  (ofTouchEventArgs &);
  void touchCancelled  (ofTouchEventArgs &);

  void windowResized   (ofResizeEventArgs &);
  void dragged         (ofDragInfo &);
  void messageReceived (ofMessage &);
};

int ofAppMain2(int windowWidth, int windowHeight, RedFWindowMode windowMode, RedFBool32 useCustomOpenGLVersion, int glVersionMajor, int glVersionMinor, RedFBool32 windowIsDecorated, RedFBool32 windowIsResizable, int msaaSamples, uint64_t optionalSettingsCount, const void * optionalSettings) {
  ofGLFWWindowSettings settings;
  // https://forum.openframeworks.cc/t/setglversion-3-2-cant-use-oflinewidth/21664
  if (useCustomOpenGLVersion == 1) {
    settings.glVersionMajor = glVersionMajor;
    settings.glVersionMinor = glVersionMinor;
  }
  settings.setSize(windowWidth, windowHeight);
  settings.windowMode = (ofWindowMode)windowMode;
  settings.decorated  = windowIsDecorated;
  settings.resizable  = windowIsResizable;
  settings.numSamples = msaaSamples;

  std::shared_ptr<ofAppBaseWindow> window = ofCreateWindow(settings);
  std::shared_ptr<ofApp> app = make_shared<ofApp>();
  ofRunApp(window, app);

  return ofRunMainLoop();
}

void ofApp::setup(void) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.setup != NULL) {
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.setup();
  }
}

void ofApp::update(void) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.update != NULL) {
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.update();
  }
}

void ofApp::draw(void) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.draw != NULL) {
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.draw();
  }
}

void ofApp::exit(void) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.exit != NULL) {
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.exit();
  }
}

void ofApp::keyPressed(ofKeyEventArgs & key) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.keyPressed != NULL) {
    ofKeyEventArgs args = key;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.keyPressed((RedFHandleEventParametersKey)p);
  }
}

void ofApp::keyReleased(ofKeyEventArgs & key) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.keyReleased != NULL) {
    ofKeyEventArgs args = key;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.keyReleased((RedFHandleEventParametersKey)p);
  }
}

void ofApp::mouseMoved(ofMouseEventArgs & mouse) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.mouseMoved != NULL) {
    ofMouseEventArgs args = mouse;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.mouseMoved((RedFHandleEventParametersMouse)p);
  }
}

void ofApp::mouseDragged(ofMouseEventArgs & mouse) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.mouseDragged != NULL) {
    ofMouseEventArgs args = mouse;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.mouseDragged((RedFHandleEventParametersMouse)p);
  }
}

void ofApp::mousePressed(ofMouseEventArgs & mouse) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.mousePressed != NULL) {
    ofMouseEventArgs args = mouse;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.mousePressed((RedFHandleEventParametersMouse)p);
  }
}

void ofApp::mouseReleased(ofMouseEventArgs & mouse) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.mouseReleased != NULL) {
    ofMouseEventArgs args = mouse;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.mouseReleased((RedFHandleEventParametersMouse)p);
  }
}

void ofApp::mouseScrolled(ofMouseEventArgs & mouse) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.mouseScrolled != NULL) {
    ofMouseEventArgs args = mouse;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.mouseScrolled((RedFHandleEventParametersMouse)p);
  }
}

void ofApp::mouseEntered(ofMouseEventArgs & mouse) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.mouseEntered != NULL) {
    ofMouseEventArgs args = mouse;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.mouseEntered((RedFHandleEventParametersMouse)p);
  }
}

void ofApp::mouseExited(ofMouseEventArgs & mouse) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.mouseExited != NULL) {
    ofMouseEventArgs args = mouse;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.mouseExited((RedFHandleEventParametersMouse)p);
  }
}

void ofApp::touchDown(ofTouchEventArgs & touch) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.touchDown != NULL) {
    ofTouchEventArgs args = touch;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.touchDown(p);
  }
}

void ofApp::touchMoved(ofTouchEventArgs & touch) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.touchMoved != NULL) {
    ofTouchEventArgs args = touch;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.touchMoved(p);
  }
}

void ofApp::touchUp(ofTouchEventArgs & touch) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.touchUp != NULL) {
    ofTouchEventArgs args = touch;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.touchUp(p);
  }
}

void ofApp::touchDoubleTap(ofTouchEventArgs & touch) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.touchDoubleTap != NULL) {
    ofTouchEventArgs args = touch;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.touchDoubleTap(p);
  }
}

void ofApp::touchCancelled(ofTouchEventArgs & touch) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.touchCancelled != NULL) {
    ofTouchEventArgs args = touch;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.touchCancelled(p);
  }
}

void ofApp::windowResized(ofResizeEventArgs & window) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.windowResized != NULL) {
    ofResizeEventArgs args = window;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.windowResized((RedFHandleEventParametersResize)p);
  }
}

void ofApp::dragged(ofDragInfo & dragged) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.dragged != NULL) {
    ofDragInfo args = dragged;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.dragged((RedFHandleEventParametersDrag)p);
  }
}

void ofApp::messageReceived(ofMessage & message) {
  if (__REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.messageReceived != NULL) {
    ofMessage args = message;
    void * p = &args;
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events.messageReceived(p);
  }
}

REDGPU_F_DECLSPEC int redFMain(const RedFEvents * events, int windowWidth, int windowHeight, RedFWindowMode windowMode, RedFBool32 useOpenGL32, RedFBool32 windowIsDecorated, RedFBool32 windowIsResizable, int msaaSamples, uint64_t optionalSettingsCount, const void * optionalSettings) {
  __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events = {};
  if (events != NULL) {
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events = events[0];
  }
  return ofAppMain2(windowWidth, windowHeight, windowMode, useOpenGL32, 3, 2, windowIsDecorated, windowIsResizable, msaaSamples, optionalSettingsCount, optionalSettings);
}

#ifdef _WIN32
  #define REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(REDGPU_F_HANDLE_NAME, REDGPU_F_OF_NAME) \
    extern "C" __declspec(dllexport) RedFHandle##REDGPU_F_HANDLE_NAME * redFCreate##REDGPU_F_HANDLE_NAME(uint64_t count) { \
      if (count == 0) { return NULL; } \
      REDGPU_F_OF_NAME * allocation = new(std::nothrow) REDGPU_F_OF_NAME [count](); \
      if (allocation == NULL) { return NULL; } \
      uint64_t * handles = new(std::nothrow) uint64_t [1 + count]; \
      if (handles == NULL) { return NULL; } \
      handles[0] = (uint64_t)allocation; \
      for (uint64_t i = 0; i < count; i += 1) { handles[1 + i] = (uint64_t)(&allocation[i]); } \
      return (RedFHandle##REDGPU_F_HANDLE_NAME *)(&handles[1]); \
    } \
    extern "C" __declspec(dllexport) void redFDestroy##REDGPU_F_HANDLE_NAME(RedFHandle##REDGPU_F_HANDLE_NAME * handles) { \
      if (handles == NULL) { return; } \
      uint64_t * h = ((uint64_t *)handles) - 1; \
      REDGPU_F_OF_NAME * allocation = (REDGPU_F_OF_NAME *)(h[0]); \
      delete[] allocation; \
      delete[] h; \
    }
#endif
#ifdef __linux__
  #define REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(REDGPU_F_HANDLE_NAME, REDGPU_F_OF_NAME) \
    extern "C" __attribute__((visibility("default"))) RedFHandle##REDGPU_F_HANDLE_NAME * redFCreate##REDGPU_F_HANDLE_NAME(uint64_t count) { \
      if (count == 0) { return NULL; } \
      REDGPU_F_OF_NAME * allocation = new(std::nothrow) REDGPU_F_OF_NAME [count](); \
      if (allocation == NULL) { return NULL; } \
      uint64_t * handles = new(std::nothrow) uint64_t [1 + count]; \
      if (handles == NULL) { return NULL; } \
      handles[0] = (uint64_t)allocation; \
      for (uint64_t i = 0; i < count; i += 1) { handles[1 + i] = (uint64_t)(&allocation[i]); } \
      return (RedFHandle##REDGPU_F_HANDLE_NAME *)(&handles[1]); \
    } \
    extern "C" __attribute__((visibility("default"))) void redFDestroy##REDGPU_F_HANDLE_NAME(RedFHandle##REDGPU_F_HANDLE_NAME * handles) { \
      if (handles == NULL) { return; } \
      uint64_t * h = ((uint64_t *)handles) - 1; \
      REDGPU_F_OF_NAME * allocation = (REDGPU_F_OF_NAME *)(h[0]); \
      delete[] allocation; \
      delete[] h; \
    }
#endif

// https://openframeworks.cc/documentation/utils/ofThread/
class RedFThread : public ofThread {
public:
  void threadedFunction() {
    if (procedure != 0) {
      procedure();
    }
  }

  void (*procedure)(void) = 0;
};

REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(Node, ofNode)
REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(Camera, ofCamera)
REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(FirstPersonCamera, ofxFirstPersonCamera)
REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(Mesh, ofMesh)
REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(Image, ofImage)
REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(Fbo, ofFbo)
REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(Light, ofLight)
REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(Material, ofMaterial)
REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(Shader, ofShader)
REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(Manipulator, ofxManipulator)
REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(Assimp, ofxAssimpModelLoader)
REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(SoundPlayer, ofSoundPlayer)
REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(Directory, ofDirectory)
REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(Thread, RedFThread)
REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(ThreadChannel, ofThreadChannel<void *>)
REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(Fbx, ofxFBX)
REDGPU_F_DECLARE_CREATE_AND_DESTROY_PROCEDURES(PostProcessing, ofxPostProcessing)

REDGPU_F_DECLSPEC void redFSetEscapeQuitsApp(RedFBool32 quitOnEsc) {
  ofSetEscapeQuitsApp(quitOnEsc);
}

REDGPU_F_DECLSPEC void redFSetWindowShape(int width, int height) {
  ofSetWindowShape(width, height);
}

REDGPU_F_DECLSPEC void redFSetWindowPosition(int x, int y) {
  ofSetWindowPosition(x, y);
}

REDGPU_F_DECLSPEC void redFSetWindowTitle(const char * title) {
  std::string name = title;
  ofSetWindowTitle(name);
}

REDGPU_F_DECLSPEC void redFSetWindowTitleW(const wchar_t * title) {
  std::wstring wname = title;
  std::string name;
  if (!wname.empty()) {
#ifdef _WIN32
    name = CW2A(wname.c_str(), CP_UTF8);
#else
    name = ws2s(wname);
#endif
  }
  ofSetWindowTitle(name);
}

REDGPU_F_DECLSPEC void redFNodeCopyToNode(RedFHandleNode handle, RedFHandleNode to) {
  ofNode * source = (ofNode *)handle;
  ofNode * target = (ofNode *)to;
  const glm::vec3 s = source->getScale();
  const glm::quat r = source->getGlobalOrientation();
  const glm::vec3 t = source->getGlobalPosition();
  target->setScale(s);
  target->setGlobalOrientation(r);
  target->setGlobalPosition(t);
}

REDGPU_F_DECLSPEC void redFNodeCopyToManipulator(RedFHandleNode handle, RedFHandleManipulator to) {
  ofNode * source = (ofNode *)handle;
  ofxManipulator * target = (ofxManipulator *)to;
  const glm::vec3 s = source->getScale();
  const glm::quat r = source->getGlobalOrientation();
  const glm::vec3 t = source->getGlobalPosition();
  target->setScale(s);
  target->setRotation(r);
  target->setTranslation(t);
}

REDGPU_F_DECLSPEC void redFNodeDraw(RedFHandleNode handle) {
  ((ofNode *)handle)->draw();
}

REDGPU_F_DECLSPEC float redFNodeGetGlobalPositionX(RedFHandleNode handle) {
  return ((ofNode *)handle)->getGlobalPosition().x;
}

REDGPU_F_DECLSPEC float redFNodeGetGlobalPositionY(RedFHandleNode handle) {
  return ((ofNode *)handle)->getGlobalPosition().y;
}

REDGPU_F_DECLSPEC float redFNodeGetGlobalPositionZ(RedFHandleNode handle) {
  return ((ofNode *)handle)->getGlobalPosition().z;
}

REDGPU_F_DECLSPEC float redFNodeGetGlobalOrientationQuaternionX(RedFHandleNode handle) {
  return ((ofNode *)handle)->getGlobalOrientation().x;
}

REDGPU_F_DECLSPEC float redFNodeGetGlobalOrientationQuaternionY(RedFHandleNode handle) {
  return ((ofNode *)handle)->getGlobalOrientation().y;
}

REDGPU_F_DECLSPEC float redFNodeGetGlobalOrientationQuaternionZ(RedFHandleNode handle) {
  return ((ofNode *)handle)->getGlobalOrientation().z;
}

REDGPU_F_DECLSPEC float redFNodeGetGlobalOrientationQuaternionW(RedFHandleNode handle) {
  return ((ofNode *)handle)->getGlobalOrientation().w;
}

REDGPU_F_DECLSPEC float redFNodeGetScaleX(RedFHandleNode handle) {
  return ((ofNode *)handle)->getScale().x;
}

REDGPU_F_DECLSPEC float redFNodeGetScaleY(RedFHandleNode handle) {
  return ((ofNode *)handle)->getScale().y;
}

REDGPU_F_DECLSPEC float redFNodeGetScaleZ(RedFHandleNode handle) {
  return ((ofNode *)handle)->getScale().z;
}

REDGPU_F_DECLSPEC void redFNodeGetGlobalTransformMatrix(RedFHandleNode handle, void * outMat4) {
  glm::mat4 out = ((ofNode *)handle)->getGlobalTransformMatrix();
  float * outFloats = (float *)outMat4;
  float * p = glm::value_ptr(out);
  outFloats[0]  = p[0];
  outFloats[1]  = p[1];
  outFloats[2]  = p[2];
  outFloats[3]  = p[3];
  outFloats[4]  = p[4];
  outFloats[5]  = p[5];
  outFloats[6]  = p[6];
  outFloats[7]  = p[7];
  outFloats[8]  = p[8];
  outFloats[9]  = p[9];
  outFloats[10] = p[10];
  outFloats[11] = p[11];
  outFloats[12] = p[12];
  outFloats[13] = p[13];
  outFloats[14] = p[14];
  outFloats[15] = p[15];
}

REDGPU_F_DECLSPEC float redFNodeGetSideDirX(RedFHandleNode handle) {
  return ((ofNode *)handle)->getSideDir().x;
}

REDGPU_F_DECLSPEC float redFNodeGetSideDirY(RedFHandleNode handle) {
  return ((ofNode *)handle)->getSideDir().y;
}

REDGPU_F_DECLSPEC float redFNodeGetSideDirZ(RedFHandleNode handle) {
  return ((ofNode *)handle)->getSideDir().z;
}

REDGPU_F_DECLSPEC float redFNodeGetUpDirX(RedFHandleNode handle) {
  return ((ofNode *)handle)->getUpDir().x;
}

REDGPU_F_DECLSPEC float redFNodeGetUpDirY(RedFHandleNode handle) {
  return ((ofNode *)handle)->getUpDir().y;
}

REDGPU_F_DECLSPEC float redFNodeGetUpDirZ(RedFHandleNode handle) {
  return ((ofNode *)handle)->getUpDir().z;
}

REDGPU_F_DECLSPEC float redFNodeGetLookAtDirX(RedFHandleNode handle) {
  return ((ofNode *)handle)->getLookAtDir().x;
}

REDGPU_F_DECLSPEC float redFNodeGetLookAtDirY(RedFHandleNode handle) {
  return ((ofNode *)handle)->getLookAtDir().y;
}

REDGPU_F_DECLSPEC float redFNodeGetLookAtDirZ(RedFHandleNode handle) {
  return ((ofNode *)handle)->getLookAtDir().z;
}

REDGPU_F_DECLSPEC void redFNodeSetGlobalPosition(RedFHandleNode handle, float x, float y, float z) {
  ((ofNode *)handle)->setGlobalPosition(x, y, z);
}

REDGPU_F_DECLSPEC void redFNodeSetGlobalOrientationQuaternion(RedFHandleNode handle, float x, float y, float z, float w) {
  glm::quat q;
  q.x = x;
  q.y = y;
  q.z = z;
  q.w = w;
  ((ofNode *)handle)->setGlobalOrientation(q);
}

REDGPU_F_DECLSPEC void redFNodeSetScale(RedFHandleNode handle, float x, float y, float z) {
  ((ofNode *)handle)->setScale(x, y, z);
}

REDGPU_F_DECLSPEC void redFNodeLookAt(RedFHandleNode handle, float x, float y, float z, float upX, float upY, float upZ) {
  glm::vec3 p;
  p.x = x;
  p.y = y;
  p.z = z;
  glm::vec3 up;
  up.x = upX;
  up.y = upY;
  up.z = upZ;
  ((ofNode *)handle)->lookAt(p, up);
}

REDGPU_F_DECLSPEC void redFNodeRotateDeg(RedFHandleNode handle, float degrees, float axisX, float axisY, float axisZ) {
  if (axisX == 0 && axisY == 0 && axisZ == 0) {
    return;
  }
  ((ofNode *)handle)->rotateDeg(degrees, axisX, axisY, axisZ);
}

REDGPU_F_DECLSPEC RedFHandleNode redFCameraCastToNode(RedFHandleCamera handle) {
  return (RedFHandleNode)(void *)handle;
}

REDGPU_F_DECLSPEC void redFCameraSetFov(RedFHandleCamera handle, float fov) {
  ((ofCamera *)handle)->setFov(fov);
}

REDGPU_F_DECLSPEC void redFCameraSetNearClip(RedFHandleCamera handle, float nearDist) {
  ((ofCamera *)handle)->setNearClip(nearDist);
}

REDGPU_F_DECLSPEC void redFCameraSetFarClip(RedFHandleCamera handle, float farDist) {
  ((ofCamera *)handle)->setFarClip(farDist);
}

REDGPU_F_DECLSPEC void redFCameraSetLensOffset(RedFHandleCamera handle, float lensOffsetX, float lensOffsetY) {
  glm::vec2 lensOffset;
  lensOffset.x = lensOffsetX;
  lensOffset.y = lensOffsetY;
  ((ofCamera *)handle)->setLensOffset(lensOffset);
}

REDGPU_F_DECLSPEC void redFCameraSetAspectRatio(RedFHandleCamera handle, float aspectRatio) {
  ((ofCamera *)handle)->setAspectRatio(aspectRatio);
}

REDGPU_F_DECLSPEC void redFCameraSetForceAspectRatio(RedFHandleCamera handle, RedFBool32 forceAspectRatio) {
  ((ofCamera *)handle)->setForceAspectRatio(forceAspectRatio);
}

REDGPU_F_DECLSPEC float redFCameraGetFov(RedFHandleCamera handle) {
  return ((ofCamera *)handle)->getFov();
}

REDGPU_F_DECLSPEC float redFCameraGetNearClip(RedFHandleCamera handle) {
  return ((ofCamera *)handle)->getNearClip();
}

REDGPU_F_DECLSPEC float redFCameraGetFarClip(RedFHandleCamera handle) {
  return ((ofCamera *)handle)->getFarClip();
}

REDGPU_F_DECLSPEC void redFCameraGetLensOffset(RedFHandleCamera handle, void * outVec2) {
  glm::vec2 out = ((ofCamera *)handle)->getLensOffset();
  float * outFloats = (float *)outVec2;
  float * p = glm::value_ptr(out);
  outFloats[0] = p[0];
  outFloats[1] = p[1];
}

REDGPU_F_DECLSPEC RedFBool32 redFCameraGetForceAspectRatio(RedFHandleCamera handle) {
  return (RedFBool32)((ofCamera *)handle)->getForceAspectRatio();
}

REDGPU_F_DECLSPEC float redFCameraGetAspectRatio(RedFHandleCamera handle) {
  return ((ofCamera *)handle)->getAspectRatio();
}

REDGPU_F_DECLSPEC void redFCameraSetupPerspective(RedFHandleCamera handle, RedFBool32 vFlip, float fov, float nearDist, float farDist, float lensOffsetX, float lensOffsetY) {
  glm::vec2 lensOffset;
  lensOffset.x = lensOffsetX;
  lensOffset.y = lensOffsetY;
  ((ofCamera *)handle)->setupPerspective(vFlip, fov, nearDist, farDist, lensOffset);
}

REDGPU_F_DECLSPEC void redFCameraSetupOffAxisViewPortal(RedFHandleCamera handle, float topLeftX, float topLeftY, float topLeftZ, float bottomLeftX, float bottomLeftY, float bottomLeftZ, float bottomRightX, float bottomRightY, float bottomRightZ) {
  glm::vec3 topLeft;
  topLeft.x = topLeftX;
  topLeft.y = topLeftY;
  topLeft.z = topLeftZ;
  glm::vec3 bottomLeft;
  bottomLeft.x = bottomLeftX;
  bottomLeft.y = bottomLeftY;
  bottomLeft.z = bottomLeftZ;
  glm::vec3 bottomRight;
  bottomRight.x = bottomRightX;
  bottomRight.y = bottomRightY;
  bottomRight.z = bottomRightZ;
  ((ofCamera *)handle)->setupOffAxisViewPortal(topLeft, bottomLeft, bottomRight);
}

REDGPU_F_DECLSPEC void redFCameraSetVFlip(RedFHandleCamera handle, RedFBool32 vFlip) {
  ((ofCamera *)handle)->setVFlip(vFlip);
}

REDGPU_F_DECLSPEC RedFBool32 redFCameraIsVFlipped(RedFHandleCamera handle) {
  return (RedFBool32)((ofCamera *)handle)->isVFlipped();
}

REDGPU_F_DECLSPEC void redFCameraEnableOrtho(RedFHandleCamera handle) {
  ((ofCamera *)handle)->enableOrtho();
}

REDGPU_F_DECLSPEC void redFCameraDisableOrtho(RedFHandleCamera handle) {
  ((ofCamera *)handle)->disableOrtho();
}

REDGPU_F_DECLSPEC RedFBool32 redFCameraGetOrtho(RedFHandleCamera handle) {
  return (RedFBool32)((ofCamera *)handle)->getOrtho();
}

REDGPU_F_DECLSPEC float redFCameraGetImagePlaneDistance(RedFHandleCamera handle) {
  return ((ofCamera *)handle)->getImagePlaneDistance();
}

REDGPU_F_DECLSPEC void redFCameraBegin(RedFHandleCamera handle) {
  ((ofCamera *)handle)->begin();
}

REDGPU_F_DECLSPEC void redFCameraEnd(RedFHandleCamera handle) {
  ((ofCamera *)handle)->end();
}

REDGPU_F_DECLSPEC void redFCameraGetProjectionMatrix(RedFHandleCamera handle, void * outMat4) {
  glm::mat4 out = ((ofCamera *)handle)->getProjectionMatrix();
  float * outFloats = (float *)outMat4;
  float * p = glm::value_ptr(out);
  outFloats[0]  = p[0];
  outFloats[1]  = p[1];
  outFloats[2]  = p[2];
  outFloats[3]  = p[3];
  outFloats[4]  = p[4];
  outFloats[5]  = p[5];
  outFloats[6]  = p[6];
  outFloats[7]  = p[7];
  outFloats[8]  = p[8];
  outFloats[9]  = p[9];
  outFloats[10] = p[10];
  outFloats[11] = p[11];
  outFloats[12] = p[12];
  outFloats[13] = p[13];
  outFloats[14] = p[14];
  outFloats[15] = p[15];
}

REDGPU_F_DECLSPEC void redFCameraGetModelViewMatrix(RedFHandleCamera handle, void * outMat4) {
  glm::mat4 out = ((ofCamera *)handle)->getModelViewMatrix();
  float * outFloats = (float *)outMat4;
  float * p = glm::value_ptr(out);
  outFloats[0]  = p[0];
  outFloats[1]  = p[1];
  outFloats[2]  = p[2];
  outFloats[3]  = p[3];
  outFloats[4]  = p[4];
  outFloats[5]  = p[5];
  outFloats[6]  = p[6];
  outFloats[7]  = p[7];
  outFloats[8]  = p[8];
  outFloats[9]  = p[9];
  outFloats[10] = p[10];
  outFloats[11] = p[11];
  outFloats[12] = p[12];
  outFloats[13] = p[13];
  outFloats[14] = p[14];
  outFloats[15] = p[15];
}

REDGPU_F_DECLSPEC void redFCameraGetModelViewProjectionMatrix(RedFHandleCamera handle, void * outMat4) {
  glm::mat4 out = ((ofCamera *)handle)->getModelViewProjectionMatrix();
  float * outFloats = (float *)outMat4;
  float * p = glm::value_ptr(out);
  outFloats[0]  = p[0];
  outFloats[1]  = p[1];
  outFloats[2]  = p[2];
  outFloats[3]  = p[3];
  outFloats[4]  = p[4];
  outFloats[5]  = p[5];
  outFloats[6]  = p[6];
  outFloats[7]  = p[7];
  outFloats[8]  = p[8];
  outFloats[9]  = p[9];
  outFloats[10] = p[10];
  outFloats[11] = p[11];
  outFloats[12] = p[12];
  outFloats[13] = p[13];
  outFloats[14] = p[14];
  outFloats[15] = p[15];
}

REDGPU_F_DECLSPEC void redFCameraWorldToScreen(RedFHandleCamera handle, float worldX, float worldY, float worldZ, void * outVec3) {
  glm::vec3 world;
  world.x = worldX;
  world.y = worldY;
  world.z = worldZ;
  glm::vec3 out = ((ofCamera *)handle)->worldToScreen(world);
  float * outFloats = (float *)outVec3;
  float * p = glm::value_ptr(out);
  outFloats[0] = p[0];
  outFloats[1] = p[1];
  outFloats[2] = p[2];
}

REDGPU_F_DECLSPEC void redFCameraScreenToWorld(RedFHandleCamera handle, float screenX, float screenY, float screenZ, void * outVec3) {
  glm::vec3 screen;
  screen.x = screenX;
  screen.y = screenY;
  screen.z = screenZ;
  glm::vec3 out = ((ofCamera *)handle)->screenToWorld(screen);
  float * outFloats = (float *)outVec3;
  float * p = glm::value_ptr(out);
  outFloats[0] = p[0];
  outFloats[1] = p[1];
  outFloats[2] = p[2];
}

REDGPU_F_DECLSPEC void redFCameraWorldToCamera(RedFHandleCamera handle, float worldX, float worldY, float worldZ, void * outVec3) {
  glm::vec3 world;
  world.x = worldX;
  world.y = worldY;
  world.z = worldZ;
  glm::vec3 out = ((ofCamera *)handle)->worldToCamera(world);
  float * outFloats = (float *)outVec3;
  float * p = glm::value_ptr(out);
  outFloats[0] = p[0];
  outFloats[1] = p[1];
  outFloats[2] = p[2];
}

REDGPU_F_DECLSPEC void redFCameraCameraToWorld(RedFHandleCamera handle, float cameraX, float cameraY, float cameraZ, void * outVec3) {
  glm::vec3 camera;
  camera.x = cameraX;
  camera.y = cameraY;
  camera.z = cameraZ;
  glm::vec3 out = ((ofCamera *)handle)->cameraToWorld(camera);
  float * outFloats = (float *)outVec3;
  float * p = glm::value_ptr(out);
  outFloats[0] = p[0];
  outFloats[1] = p[1];
  outFloats[2] = p[2];
}

REDGPU_F_DECLSPEC void redFCameraDrawFrustum(RedFHandleCamera handle) {
  ((ofCamera *)handle)->drawFrustum();
}

REDGPU_F_DECLSPEC RedFHandleCamera redFFirstPersonCameraCastToCamera(RedFHandleFirstPersonCamera handle) {
  return (RedFHandleCamera)(void *)handle;
}

REDGPU_F_DECLSPEC RedFHandleNode redFFirstPersonCameraCastToNode(RedFHandleFirstPersonCamera handle) {
  return (RedFHandleNode)(void *)handle;
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraBegin(RedFHandleFirstPersonCamera handle) {
  ((ofxFirstPersonCamera *)handle)->begin();
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraEnd(RedFHandleFirstPersonCamera handle) {
  ((ofxFirstPersonCamera *)handle)->end();
}

REDGPU_F_DECLSPEC RedFBool32 redFFirstPersonCameraControlIsEnabled(RedFHandleFirstPersonCamera handle) {
  return ((ofxFirstPersonCamera *)handle)->isControlled() == true ? 1 : 0;
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraControlToggle(RedFHandleFirstPersonCamera handle) {
  ((ofxFirstPersonCamera *)handle)->toggleControl();
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraControlEnable(RedFHandleFirstPersonCamera handle) {
  ((ofxFirstPersonCamera *)handle)->enableControl();
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraControlDisable(RedFHandleFirstPersonCamera handle) {
  ((ofxFirstPersonCamera *)handle)->disableControl();
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraSetKeyUp(RedFHandleFirstPersonCamera handle, int key) {
  ((ofxFirstPersonCamera *)handle)->keyUp = key;
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraSetKeyDown(RedFHandleFirstPersonCamera handle, int key) {
  ((ofxFirstPersonCamera *)handle)->keyDown = key;
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraSetKeyLeft(RedFHandleFirstPersonCamera handle, int key) {
  ((ofxFirstPersonCamera *)handle)->keyLeft = key;
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraSetKeyRight(RedFHandleFirstPersonCamera handle, int key) {
  ((ofxFirstPersonCamera *)handle)->keyRight = key;
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraSetKeyForward(RedFHandleFirstPersonCamera handle, int key) {
  ((ofxFirstPersonCamera *)handle)->keyForward = key;
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraSetKeyBackward(RedFHandleFirstPersonCamera handle, int key) {
  ((ofxFirstPersonCamera *)handle)->keyBackward = key;
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraSetKeyRollLeft(RedFHandleFirstPersonCamera handle, int key) {
  ((ofxFirstPersonCamera *)handle)->keyRollLeft = key;
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraSetKeyRollRight(RedFHandleFirstPersonCamera handle, int key) {
  ((ofxFirstPersonCamera *)handle)->keyRollRight = key;
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraSetKeyRollReset(RedFHandleFirstPersonCamera handle, int key) {
  ((ofxFirstPersonCamera *)handle)->keyRollReset = key;
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraSetMoveSpeed(RedFHandleFirstPersonCamera handle, float moveSpeed) {
  ((ofxFirstPersonCamera *)handle)->movespeed = moveSpeed;
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraSetRollSpeed(RedFHandleFirstPersonCamera handle, float rollSpeed) {
  ((ofxFirstPersonCamera *)handle)->rollspeed = rollSpeed;
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraSetSensitivity(RedFHandleFirstPersonCamera handle, float sensitivity) {
  ((ofxFirstPersonCamera *)handle)->sensitivity = sensitivity;
}

REDGPU_F_DECLSPEC void redFFirstPersonCameraSetUpVector(RedFHandleFirstPersonCamera handle, float x, float y, float z) {
  ((ofxFirstPersonCamera *)handle)->upvector.set(x, y, z);
}

REDGPU_F_DECLSPEC void redFMeshSetMode(RedFHandleMesh handle, RedFPrimitiveMode mode) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->setMode((ofPrimitiveMode)mode);
}

REDGPU_F_DECLSPEC RedFPrimitiveMode redFMeshGetMode(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return (RedFPrimitiveMode)mesh->getMode();
}

REDGPU_F_DECLSPEC void redFMeshClear(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->clear();
}

// Mesh vertexes

REDGPU_F_DECLSPEC void redFMeshAddVertex(RedFHandleMesh handle, float vx, float vy, float vz) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->addVertex(glm::vec3(vx, vy, vz));
}

REDGPU_F_DECLSPEC void redFMeshAddVertices(RedFHandleMesh handle, void * vertVec3s, uint64_t vertVec3sCount) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->addVertices((glm::vec3 *)vertVec3s, vertVec3sCount);
}

REDGPU_F_DECLSPEC void redFMeshRemoveVertex(RedFHandleMesh handle, unsigned index) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->removeVertex((ofIndexType)index);
}

REDGPU_F_DECLSPEC void redFMeshClearVertices(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->clearVertices();
}

REDGPU_F_DECLSPEC void redFMeshSetVertex(RedFHandleMesh handle, unsigned index, float x, float y, float z) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->setVertex(index, glm::vec3(x, y, z));
}

REDGPU_F_DECLSPEC void redFMeshGetVertex(RedFHandleMesh handle, unsigned index, void * outVec3) {
  ofMesh * mesh = (ofMesh *)handle;
  glm::vec3 out = mesh->getVertex(index);
  float * outFloats = (float *)outVec3;
  float * p = glm::value_ptr(out);
  outFloats[0] = p[0];
  outFloats[1] = p[1];
  outFloats[2] = p[2];
}

REDGPU_F_DECLSPEC uint64_t redFMeshGetNumVertices(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return mesh->getNumVertices();
}

REDGPU_F_DECLSPEC void * redFMeshGetVerticesPointer(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return mesh->getVerticesPointer();
}

REDGPU_F_DECLSPEC RedFBool32 redFMeshHasVertices(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return (RedFBool32)mesh->hasVertices();
}

REDGPU_F_DECLSPEC RedFBool32 redFMeshHaveVertsChanged(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return 1; // (RedFBool32)mesh->haveVertsChanged();
}

REDGPU_F_DECLSPEC void redFMeshMergeDuplicateVertices(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->mergeDuplicateVertices();
}

REDGPU_F_DECLSPEC void redFMeshGetCentroid(RedFHandleMesh handle, void * outVec3) {
  ofMesh * mesh = (ofMesh *)handle;
  glm::vec3 out = mesh->getCentroid();
  float * outFloats = (float *)outVec3;
  float * p = glm::value_ptr(out);
  outFloats[0] = p[0];
  outFloats[1] = p[1];
  outFloats[2] = p[2];
}

// Mesh normals

REDGPU_F_DECLSPEC void redFMeshEnableNormals(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->enableNormals();
}

REDGPU_F_DECLSPEC void redFMeshDisableNormals(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->disableNormals();
}

REDGPU_F_DECLSPEC RedFBool32 redFMeshUsingNormals(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return (RedFBool32)mesh->usingNormals();
}

REDGPU_F_DECLSPEC void redFMeshAddNormal(RedFHandleMesh handle, float nx, float ny, float nz) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->addNormal(glm::vec3(nx, ny, nz));
}

REDGPU_F_DECLSPEC void redFMeshAddNormals(RedFHandleMesh handle, void * normVec3s, uint64_t normVec3sCount) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->addNormals((glm::vec3 *)normVec3s, normVec3sCount);
}

REDGPU_F_DECLSPEC void redFMeshRemoveNormal(RedFHandleMesh handle, unsigned index) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->removeNormal(index);
}

REDGPU_F_DECLSPEC void redFMeshClearNormals(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->clearNormals();
}

REDGPU_F_DECLSPEC void redFMeshSetNormal(RedFHandleMesh handle, unsigned index, float x, float y, float z) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->setNormal(index, glm::vec3(x, y, z));
}

REDGPU_F_DECLSPEC void redFMeshGetNormal(RedFHandleMesh handle, unsigned index, void * outVec3) {
  ofMesh * mesh = (ofMesh *)handle;
  glm::vec3 out = mesh->getNormal(index);
  float * outFloats = (float *)outVec3;
  float * p = glm::value_ptr(out);
  outFloats[0] = p[0];
  outFloats[1] = p[1];
  outFloats[2] = p[2];
}

REDGPU_F_DECLSPEC uint64_t redFMeshGetNumNormals(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return mesh->getNumNormals();
}

REDGPU_F_DECLSPEC void * redFMeshGetNormalsPointer(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return mesh->getNormalsPointer();
}

REDGPU_F_DECLSPEC RedFBool32 redFMeshHasNormals(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return (RedFBool32)mesh->hasNormals();
}

REDGPU_F_DECLSPEC RedFBool32 redFMeshHaveNormalsChanged(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return 1; // (RedFBool32)mesh->haveNormalsChanged();
}

REDGPU_F_DECLSPEC void redFMeshSmoothNormals(RedFHandleMesh handle, float angle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->smoothNormals(angle);
}

REDGPU_F_DECLSPEC void redFMeshFlatNormals(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->flatNormals();
}

// Mesh colors

REDGPU_F_DECLSPEC void redFMeshEnableColors(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->enableColors();
}

REDGPU_F_DECLSPEC void redFMeshDisableColors(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->disableColors();
}

REDGPU_F_DECLSPEC RedFBool32 redFMeshUsingColors(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return (RedFBool32)mesh->usingColors();
}

REDGPU_F_DECLSPEC void redFMeshAddColor(RedFHandleMesh handle, float cr, float cg, float cb, float ca) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->addColor(ofFloatColor(cr, cg, cb, ca));
}

REDGPU_F_DECLSPEC void redFMeshAddColors(RedFHandleMesh handle, void * colVec4s, uint64_t colVec4sCount) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->addColors((ofFloatColor *)colVec4s, colVec4sCount);
}

REDGPU_F_DECLSPEC void redFMeshRemoveColor(RedFHandleMesh handle, unsigned index) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->removeColor(index);
}

REDGPU_F_DECLSPEC void redFMeshClearColors(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->clearColors();
}

REDGPU_F_DECLSPEC void redFMeshSetColor(RedFHandleMesh handle, unsigned index, float r, float g, float b, float a) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->setColor(index, ofFloatColor(r, g, b, a));
}

REDGPU_F_DECLSPEC void redFMeshGetColor(RedFHandleMesh handle, unsigned index, void * outVec4) {
  ofMesh * mesh = (ofMesh *)handle;
  ofFloatColor out = mesh->getColor(index);
  float * outFloats = (float *)outVec4;
  outFloats[0] = out.r;
  outFloats[1] = out.g;
  outFloats[2] = out.b;
  outFloats[3] = out.a;
}

REDGPU_F_DECLSPEC uint64_t redFMeshGetNumColors(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return mesh->getNumColors();
}

REDGPU_F_DECLSPEC void * redFMeshGetColorsPointer(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return mesh->getColorsPointer();
}

REDGPU_F_DECLSPEC RedFBool32 redFMeshHasColors(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return (RedFBool32)mesh->hasColors();
}

REDGPU_F_DECLSPEC RedFBool32 redFMeshHaveColorsChanged(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return 1; // (RedFBool32)mesh->haveColorsChanged();
}

// Mesh texture coordinates

REDGPU_F_DECLSPEC void redFMeshEnableTextures(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->enableTextures();
}

REDGPU_F_DECLSPEC void redFMeshDisableTextures(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->disableTextures();
}

REDGPU_F_DECLSPEC RedFBool32 redFMeshUsingTextures(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return (RedFBool32)mesh->usingTextures();
}

REDGPU_F_DECLSPEC void redFMeshAddTexCoord(RedFHandleMesh handle, float tu, float tv) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->addTexCoord(glm::vec2(tu, tv));
}

REDGPU_F_DECLSPEC void redFMeshAddTexCoords(RedFHandleMesh handle, void * texVec2s, uint64_t texVec2sCount) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->addTexCoords((glm::vec2 *)texVec2s, texVec2sCount);
}

REDGPU_F_DECLSPEC void redFMeshRemoveTexCoord(RedFHandleMesh handle, unsigned index) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->removeTexCoord(index);
}

REDGPU_F_DECLSPEC void redFMeshClearTexCoords(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->clearTexCoords();
}

REDGPU_F_DECLSPEC void redFMeshSetTexCoord(RedFHandleMesh handle, unsigned index, float u, float v) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->setTexCoord(index, glm::vec2(u, v));
}

REDGPU_F_DECLSPEC void redFMeshGetTexCoord(RedFHandleMesh handle, unsigned index, void * outVec2) {
  ofMesh * mesh = (ofMesh *)handle;
  glm::vec2 out = mesh->getTexCoord(index);
  float * outFloats = (float *)outVec2;
  float * p = glm::value_ptr(out);
  outFloats[0] = p[0];
  outFloats[1] = p[1];
}

REDGPU_F_DECLSPEC uint64_t redFMeshGetNumTexCoords(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return mesh->getNumTexCoords();
}

REDGPU_F_DECLSPEC void * redFMeshGetTexCoordsPointer(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return mesh->getTexCoordsPointer();
}

REDGPU_F_DECLSPEC RedFBool32 redFMeshHasTexCoords(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return (RedFBool32)mesh->hasTexCoords();
}

REDGPU_F_DECLSPEC RedFBool32 redFMeshHaveTexCoordsChanged(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return 1; // (RedFBool32)mesh->haveTexCoordsChanged();
}

// Mesh indexes

REDGPU_F_DECLSPEC void redFMeshEnableIndices(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->enableIndices();
}

REDGPU_F_DECLSPEC void redFMeshDisableIndices(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->disableIndices();
}

REDGPU_F_DECLSPEC RedFBool32 redFMeshUsingIndices(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return (RedFBool32)mesh->usingIndices();
}

REDGPU_F_DECLSPEC void redFMeshAddIndex(RedFHandleMesh handle, unsigned index) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->addIndex((ofIndexType)index);
}

REDGPU_F_DECLSPEC void redFMeshAddIndices(RedFHandleMesh handle, unsigned * indexes, uint64_t indexesCount) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->addIndices((ofIndexType *)indexes, indexesCount);
}

REDGPU_F_DECLSPEC void redFMeshRemoveIndex(RedFHandleMesh handle, unsigned index) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->removeIndex(index);
}

REDGPU_F_DECLSPEC void redFMeshClearIndices(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->clearIndices();
}

REDGPU_F_DECLSPEC void redFMeshSetIndex(RedFHandleMesh handle, unsigned index, unsigned value) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->setIndex(index, (ofIndexType)value);
}

REDGPU_F_DECLSPEC unsigned redFMeshGetIndex(RedFHandleMesh handle, unsigned index) {
  ofMesh * mesh = (ofMesh *)handle;
  return (unsigned)(ofIndexType)mesh->getIndex(index);
}

REDGPU_F_DECLSPEC uint64_t redFMeshGetNumIndices(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return mesh->getNumIndices();
}

REDGPU_F_DECLSPEC unsigned * redFMeshGetIndexPointer(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return (unsigned *)(ofIndexType *)mesh->getIndexPointer();
}

REDGPU_F_DECLSPEC RedFBool32 redFMeshHasIndices(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return (RedFBool32)mesh->hasIndices();
}

REDGPU_F_DECLSPEC RedFBool32 redFMeshHaveIndicesChanged(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  return 1; // (RedFBool32)mesh->haveIndicesChanged();
}

REDGPU_F_DECLSPEC void redFMeshSetupIndicesAuto(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->setupIndicesAuto();
}

REDGPU_F_DECLSPEC void redFMeshAddTriangle(RedFHandleMesh handle, unsigned index1, unsigned index2, unsigned index3) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->addTriangle((ofIndexType)index1, (ofIndexType)index2, (ofIndexType)index3);
}

REDGPU_F_DECLSPEC void redFMeshSetColorForIndices(RedFHandleMesh handle, unsigned startIndex, unsigned endIndex, float r, float g, float b, float a) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->setColorForIndices((ofIndexType)startIndex, (ofIndexType)endIndex, ofFloatColor(r, g, b, a));
}

// Mesh draw

REDGPU_F_DECLSPEC void redFMeshDrawVertices(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->drawVertices();
}

REDGPU_F_DECLSPEC void redFMeshDrawWireframe(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->drawWireframe();
}

REDGPU_F_DECLSPEC void redFMeshDraw(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->draw();
}

REDGPU_F_DECLSPEC void redFMeshDrawWithRenderMode(RedFHandleMesh handle, RedFPolyRenderMode renderMode) {
  ofMesh * mesh = (ofMesh *)handle;
  mesh->draw((ofPolyRenderMode)renderMode);
}

REDGPU_F_DECLSPEC void redFImageAllocate(RedFHandleImage handle, int w, int h, RedFImageType type) {
  ofImage * image = (ofImage *)handle;
  image->allocate(w, h, (ofImageType)type);
}

REDGPU_F_DECLSPEC RedFBool32 redFImageIsAllocated(RedFHandleImage handle) {
  ofImage * image = (ofImage *)handle;
  return (RedFBool32)image->isAllocated();
}

REDGPU_F_DECLSPEC int redFImageGetWidth(RedFHandleImage handle) {
  ofImage * image = (ofImage *)handle;
  return (int)image->getWidth();
}

REDGPU_F_DECLSPEC int redFImageGetHeight(RedFHandleImage handle) {
  ofImage * image = (ofImage *)handle;
  return (int)image->getHeight();
}

REDGPU_F_DECLSPEC int redFImageGetBitsPerPixel(RedFHandleImage handle) {
  ofImage * image = (ofImage *)handle;
  return (int)image->getPixels().getBitsPerPixel();
}

REDGPU_F_DECLSPEC RedFImageType redFImageGetImageType(RedFHandleImage handle) {
  ofImage * image = (ofImage *)handle;
  return (RedFImageType)image->getImageType();
}

REDGPU_F_DECLSPEC void * redFImageGetPixelsPointer(RedFHandleImage handle) {
  ofImage * image = (ofImage *)handle;
  return (void *)image->getPixels().getData();
}

REDGPU_F_DECLSPEC void redFImageClear(RedFHandleImage handle) {
  ofImage * image = (ofImage *)handle;
  image->clear();
}

REDGPU_F_DECLSPEC RedFBool32 redFImageLoad(RedFHandleImage handle, const char * fileName) {
  ofImage * image = (ofImage *)handle;
  std::string name = fileName;
#ifdef __linux__
#warning
  std::string path = name;
#else
  std::filesystem::path path = name;
#endif
  return image->load(path);
}

REDGPU_F_DECLSPEC RedFBool32 redFImageLoadFromMemory(RedFHandleImage handle, uint64_t imageMemoryBytesCount, const void * imageMemory) {
  ofImage * image = (ofImage *)handle;
  ofBuffer buffer;
  buffer.set((const char *)imageMemory, imageMemoryBytesCount);
  return image->load(buffer);
}

REDGPU_F_DECLSPEC void redFImageSave(RedFHandleImage handle, const char * fileName) {
  ofImage * image = (ofImage *)handle;
  std::string name = fileName;
#ifdef __linux__
#warning
  std::string path = name;
#else
  std::filesystem::path path = name;
#endif
  image->save(path);
}

REDGPU_F_DECLSPEC void redFImageGrabScreen(RedFHandleImage handle, int x, int y, int w, int h) {
  ofImage * image = (ofImage *)handle;
  image->grabScreen(x, y, w, h);
}

REDGPU_F_DECLSPEC void redFImageSetColor(RedFHandleImage handle, int x, int y, int r, int g, int b, int a) {
  ofImage * image = (ofImage *)handle;
  image->setColor(x, y, ofColor(r, g, b, a));
}

REDGPU_F_DECLSPEC void redFImageGetColor(RedFHandleImage handle, int x, int y, unsigned char * outR, unsigned char * outG, unsigned char * outB, unsigned char * outA) {
  ofImage * image = (ofImage *)handle;
  ofColor color = image->getColor(x, y);
  if (outR != NULL) { outR[0] = color.r; }
  if (outG != NULL) { outG[0] = color.g; }
  if (outB != NULL) { outB[0] = color.b; }
  if (outA != NULL) { outA[0] = color.a; }
}

REDGPU_F_DECLSPEC void redFImageResize(RedFHandleImage handle, int newWidth, int newHeight) {
  ofImage * image = (ofImage *)handle;
  image->resize(newWidth, newHeight);
}

REDGPU_F_DECLSPEC void redFImageCrop(RedFHandleImage handle, int x, int y, int w, int h) {
  ofImage * image = (ofImage *)handle;
  image->crop(x, y, w, h);
}

REDGPU_F_DECLSPEC void redFImageRotate90(RedFHandleImage handle, int rotation) {
  ofImage * image = (ofImage *)handle;
  image->rotate90(rotation);
}

REDGPU_F_DECLSPEC void redFImageMirror(RedFHandleImage handle, RedFBool32 vertical, RedFBool32 horizontal) {
  ofImage * image = (ofImage *)handle;
  image->mirror(vertical, horizontal);
}

REDGPU_F_DECLSPEC void redFImageSetTextureWrap(RedFHandleImage handle, int wrapModeHorizontal, int wrapModeVertical) {
  ofImage * image = (ofImage *)handle;
  image->getTexture().setTextureWrap(wrapModeHorizontal, wrapModeVertical);
}

REDGPU_F_DECLSPEC void redFImageSetTextureMinMagFilter(RedFHandleImage handle, int minFilter, int magFilter) {
  ofImage * image = (ofImage *)handle;
  image->getTexture().setTextureMinMagFilter(minFilter, magFilter);
}

REDGPU_F_DECLSPEC void redFImageSetTextureCompareModeFunc(RedFHandleImage handle, int compareMode, int compareFunc) {
  ofImage * image = (ofImage *)handle;
  unsigned textureTarget = image->getTexture().getTextureData().textureTarget;
  unsigned textureId     = image->getTexture().getTextureData().textureID;
  glBindTexture(textureTarget, textureId);
  glTexParameteri(textureTarget, GL_TEXTURE_COMPARE_MODE, compareMode);
  glTexParameteri(textureTarget, GL_TEXTURE_COMPARE_FUNC, compareFunc);
  glBindTexture(textureTarget, 0);
}

REDGPU_F_DECLSPEC void redFImageSetTextureMaxAnisotropy(RedFHandleImage handle, int maxAnisotropy) {
  ofImage * image = (ofImage *)handle;
  unsigned textureTarget = image->getTexture().getTextureData().textureTarget;
  unsigned textureId     = image->getTexture().getTextureData().textureID;
  glBindTexture(textureTarget, textureId);
  glTexParameteri(textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
  glBindTexture(textureTarget, 0);
}

REDGPU_F_DECLSPEC void redFImageSetAnchorPercent(RedFHandleImage handle, float x, float y) {
  ofImage * image = (ofImage *)handle;
  image->setAnchorPercent(x, y);
}

REDGPU_F_DECLSPEC void redFImageSetAnchorPoint(RedFHandleImage handle, float x, float y) {
  ofImage * image = (ofImage *)handle;
  image->setAnchorPoint(x, y);
}

REDGPU_F_DECLSPEC void redFImageResetAnchor(RedFHandleImage handle) {
  ofImage * image = (ofImage *)handle;
  image->resetAnchor();
}

REDGPU_F_DECLSPEC RedFBool32 redFImageHasMipmap(RedFHandleImage handle) {
  ofImage * image = (ofImage *)handle;
  return (RedFBool32)image->getTexture().hasMipmap();
}

REDGPU_F_DECLSPEC void redFImageMipmapDisable(RedFHandleImage handle) {
  ofImage * image = (ofImage *)handle;
  image->getTexture().disableMipmap();
}

REDGPU_F_DECLSPEC void redFImageMipmapEnable(RedFHandleImage handle) {
  ofImage * image = (ofImage *)handle;
  image->getTexture().enableMipmap();
}

REDGPU_F_DECLSPEC void redFImageMipmapGenerate(RedFHandleImage handle) {
  ofImage * image = (ofImage *)handle;
  image->getTexture().generateMipmap();
}

REDGPU_F_DECLSPEC void redFImageUpdate(RedFHandleImage handle) {
  ofImage * image = (ofImage *)handle;
  image->update();
}

REDGPU_F_DECLSPEC void redFImageBind(RedFHandleImage handle) {
  ofImage * image = (ofImage *)handle;
  image->bind();
}

REDGPU_F_DECLSPEC void redFImageUnbind(RedFHandleImage handle) {
  ofImage * image = (ofImage *)handle;
  image->unbind();
}

REDGPU_F_DECLSPEC void redFImageDraw(RedFHandleImage handle, float x, float y, float z, float w, float h) {
  ofImage * image = (ofImage *)handle;
  image->draw(x, y, z, w, h);
}

REDGPU_F_DECLSPEC void redFImageDrawSubsection(RedFHandleImage handle, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) {
  ofImage * image = (ofImage *)handle;
  image->drawSubsection(x, y, z, w, h, sx, sy, sw, sh);
}

REDGPU_F_DECLSPEC void redFFboAllocate(RedFHandleFbo handle, int width, int height, int numSamples, const RedFFboSettings * optionalSettings) {
  ofFbo * fbo = (ofFbo *)handle;
  if (optionalSettings == NULL) {
    fbo->allocate(width, height, 0x1908, numSamples);
  } else {
    ofFbo::Settings fboSettings;
    fboSettings.width                      = optionalSettings->width;
    fboSettings.height                     = optionalSettings->height;
    fboSettings.numColorbuffers            = optionalSettings->numColorbuffers;
    for (int i = 0; i < fboSettings.numColorbuffers; i += 1) {
      fboSettings.colorFormats.push_back(optionalSettings->colorFormats[i]);
    }
    fboSettings.useDepth                   = optionalSettings->useDepth;
    fboSettings.useStencil                 = optionalSettings->useStencil;
    fboSettings.depthStencilAsTexture      = optionalSettings->depthStencilAsTexture;
    fboSettings.textureTarget              = optionalSettings->textureTarget;
    fboSettings.internalformat             = optionalSettings->internalformat;
    fboSettings.depthStencilInternalFormat = optionalSettings->depthStencilInternalFormat;
    fboSettings.wrapModeHorizontal         = optionalSettings->wrapModeHorizontal;
    fboSettings.wrapModeVertical           = optionalSettings->wrapModeVertical;
    fboSettings.minFilter                  = optionalSettings->minFilter;
    fboSettings.maxFilter                  = optionalSettings->maxFilter;
    fboSettings.numSamples                 = optionalSettings->numSamples;
    fbo->allocate(fboSettings);
  }
}

REDGPU_F_DECLSPEC RedFBool32 redFFboIsAllocated(RedFHandleFbo handle) {
  ofFbo * fbo = (ofFbo *)handle;
  return (RedFBool32)fbo->isAllocated();
}

REDGPU_F_DECLSPEC void redFFboBegin(RedFHandleFbo handle, RedFBool32 setupScreenDefaultIs1) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->begin(setupScreenDefaultIs1 == 1 ? (OF_FBOMODE_PERSPECTIVE | OF_FBOMODE_MATRIXFLIP) : OF_FBOMODE_NODEFAULTS); // NOTE(Constantine): https://openframeworks.cc/documentation/gl/ofFbo/#show_begin
}

REDGPU_F_DECLSPEC void redFFboActivateAllDrawBuffers(RedFHandleFbo handle) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->activateAllDrawBuffers();
}

REDGPU_F_DECLSPEC void redFFboEnd(RedFHandleFbo handle) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->end();
}

REDGPU_F_DECLSPEC void redFFboSetTextureWrap(RedFHandleFbo handle, int fboAttachmentPoint, int wrapModeHorizontal, int wrapModeVertical) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->getTexture(fboAttachmentPoint).setTextureWrap(wrapModeHorizontal, wrapModeVertical);
}

REDGPU_F_DECLSPEC void redFFboSetTextureMinMagFilter(RedFHandleFbo handle, int fboAttachmentPoint, int minFilter, int magFilter) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->getTexture(fboAttachmentPoint).setTextureMinMagFilter(minFilter, magFilter);
}

REDGPU_F_DECLSPEC void redFFboSetTextureCompareModeFunc(RedFHandleFbo handle, int fboAttachmentPoint, int compareMode, int compareFunc) {
  ofFbo * fbo = (ofFbo *)handle;
  unsigned textureTarget = fbo->getTexture(fboAttachmentPoint).getTextureData().textureTarget;
  unsigned textureId     = fbo->getTexture(fboAttachmentPoint).getTextureData().textureID;
  glBindTexture(textureTarget, textureId);
  glTexParameteri(textureTarget, GL_TEXTURE_COMPARE_MODE, compareMode);
  glTexParameteri(textureTarget, GL_TEXTURE_COMPARE_FUNC, compareFunc);
  glBindTexture(textureTarget, 0);
}

REDGPU_F_DECLSPEC void redFFboSetDepthTextureWrap(RedFHandleFbo handle, int wrapModeHorizontal, int wrapModeVertical) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->getDepthTexture().setTextureWrap(wrapModeHorizontal, wrapModeVertical);
}

REDGPU_F_DECLSPEC void redFFboSetDepthTextureMinMagFilter(RedFHandleFbo handle, int minFilter, int magFilter) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->getDepthTexture().setTextureMinMagFilter(minFilter, magFilter);
}

REDGPU_F_DECLSPEC void redFFboSetDepthTextureCompareModeFunc(RedFHandleFbo handle, int compareMode, int compareFunc) {
  ofFbo * fbo = (ofFbo *)handle;
  unsigned textureTarget = fbo->getDepthTexture().getTextureData().textureTarget;
  unsigned textureId     = fbo->getDepthTexture().getTextureData().textureID;
  glBindTexture(textureTarget, textureId);
  glTexParameteri(textureTarget, GL_TEXTURE_COMPARE_MODE, compareMode);
  glTexParameteri(textureTarget, GL_TEXTURE_COMPARE_FUNC, compareFunc);
  glBindTexture(textureTarget, 0);
}

REDGPU_F_DECLSPEC void redFFboSetDepthTextureRGToRGBASwizzles(RedFHandleFbo handle, RedFBool32 rToRGBSwizzles) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->getDepthTexture().setRGToRGBASwizzles(rToRGBSwizzles);
}

REDGPU_F_DECLSPEC void redFFboBind(RedFHandleFbo handle, int fboAttachmentPoint) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->getTexture(fboAttachmentPoint).bind();
  // NOTE(Constantine): For correct FBO binding, drawing and reading, don't forget to push and pop styles!
}

REDGPU_F_DECLSPEC void redFFboUnbind(RedFHandleFbo handle, int fboAttachmentPoint) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->getTexture(fboAttachmentPoint).unbind();
}

REDGPU_F_DECLSPEC void redFFboDraw(RedFHandleFbo handle, int fboAttachmentPoint, float x, float y, float width, float height) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->getTexture(fboAttachmentPoint).draw(x, y, width, height);
}

REDGPU_F_DECLSPEC void redFFboDepthDraw(RedFHandleFbo handle, float x, float y, float width, float height) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->getDepthTexture().draw(x, y, width, height);
}

REDGPU_F_DECLSPEC void redFFboReadToImage(RedFHandleFbo handle, int fboAttachmentPoint, RedFHandleImage image) {
  ofFbo *   fbo = (ofFbo *)handle;
  ofImage * img = (ofImage *)image;
  ofPixels pixels;
  fbo->readToPixels(pixels, fboAttachmentPoint);
  img->setFromPixels(pixels);
}

REDGPU_F_DECLSPEC void redFFboSave(RedFHandleFbo handle, int fboAttachmentPoint, const char * fileName) {
  ofFbo * fbo = (ofFbo *)handle;
  ofImage img;
  ofPixels pixels;
  fbo->readToPixels(pixels, fboAttachmentPoint);
  img.setFromPixels(pixels);
  std::string name = fileName;
#ifdef __linux__
#warning
  std::string path = name;
#else
  std::filesystem::path path = name;
#endif
  img.save(path);
}

REDGPU_F_DECLSPEC void redFFboClear(RedFHandleFbo handle) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->clear();
}

REDGPU_F_DECLSPEC void redFFboClearColorBuffer(RedFHandleFbo handle, int bufferIndex, float r, float g, float b, float a) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->clearColorBuffer(bufferIndex, ofFloatColor(r, g, b, a));
}

REDGPU_F_DECLSPEC void redFFboClearDepthBuffer(RedFHandleFbo handle, float depth) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->clearDepthBuffer(depth);
}

REDGPU_F_DECLSPEC void redFFboClearStencilBuffer(RedFHandleFbo handle, int stencil) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->clearStencilBuffer(stencil);
}

REDGPU_F_DECLSPEC void redFFboClearDepthStencilBuffer(RedFHandleFbo handle, float depth, int stencil) {
  ofFbo * fbo = (ofFbo *)handle;
  fbo->clearDepthStencilBuffer(depth, stencil);
}

REDGPU_F_DECLSPEC RedFHandleNode redFLightCastToNode(RedFHandleLight handle) {
  return (RedFHandleNode)(void *)handle;
}

REDGPU_F_DECLSPEC void redFLightSetup(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  light->setup();
}

REDGPU_F_DECLSPEC void redFLightEnable(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  light->enable();
}

REDGPU_F_DECLSPEC void redFLightDisable(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  light->disable();
}

REDGPU_F_DECLSPEC RedFBool32 redFLightGetIsEnabled(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  return (RedFBool32)light->getIsEnabled();
}

REDGPU_F_DECLSPEC void redFLightSetDirectional(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  light->setDirectional();
}

REDGPU_F_DECLSPEC RedFBool32 redFLightGetIsDirectional(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  return (RedFBool32)light->getIsDirectional();
}

REDGPU_F_DECLSPEC void redFLightSetSpotlight(RedFHandleLight handle, float spotCutOffDefaultIs45, float exponentDefaultIs0) {
  ofLight * light = (ofLight *)handle;
  light->setSpotlight(spotCutOffDefaultIs45, exponentDefaultIs0);
}

REDGPU_F_DECLSPEC RedFBool32 redFLightGetIsSpotlight(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  return (RedFBool32)light->getIsSpotlight();
}

REDGPU_F_DECLSPEC void redFLightSetSpotlightCutOff(RedFHandleLight handle, float spotCutOff) {
  ofLight * light = (ofLight *)handle;
  light->setSpotlightCutOff(spotCutOff);
}

REDGPU_F_DECLSPEC float redFLightGetSpotlightCutOff(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  return light->getSpotlightCutOff();
}

REDGPU_F_DECLSPEC void redFLightSetSpotConcentration(RedFHandleLight handle, float exponent) {
  ofLight * light = (ofLight *)handle;
  light->setSpotConcentration(exponent);
}

REDGPU_F_DECLSPEC float redFLightGetSpotConcentration(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  return light->getSpotConcentration();
}

REDGPU_F_DECLSPEC void redFLightSetPointLight(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  return light->setPointLight();
}

REDGPU_F_DECLSPEC RedFBool32 redFLightGetIsPointLight(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  return (RedFBool32)light->getIsPointLight();
}

REDGPU_F_DECLSPEC void redFLightSetAttenuation(RedFHandleLight handle, float constantDefaultIs1, float linearDefaultIs0, float quadraticDefaultIs0) {
  ofLight * light = (ofLight *)handle;
  light->setAttenuation(constantDefaultIs1, linearDefaultIs0, quadraticDefaultIs0);
}

REDGPU_F_DECLSPEC float redFLightGetAttenuationConstant(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  return light->getAttenuationConstant();
}

REDGPU_F_DECLSPEC float redFLightGetAttenuationLinear(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  return light->getAttenuationLinear();
}

REDGPU_F_DECLSPEC float redFLightGetAttenuationQuadratic(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  return light->getAttenuationQuadratic();
}

REDGPU_F_DECLSPEC void redFLightSetAreaLight(RedFHandleLight handle, float width, float height) {
  ofLight * light = (ofLight *)handle;
  light->setAreaLight(width, height);
}

REDGPU_F_DECLSPEC RedFBool32 redFLightGetIsAreaLight(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  return (RedFBool32)light->getIsAreaLight();
}

REDGPU_F_DECLSPEC RedFLightType redFLightGetType(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  return (RedFLightType)light->getType();
}

REDGPU_F_DECLSPEC void redFLightSetAmbientColor(RedFHandleLight handle, float r, float g, float b, float a) {
  ofLight * light = (ofLight *)handle;
  light->setAmbientColor(ofFloatColor(r, g, b, a));
}

REDGPU_F_DECLSPEC void redFLightSetDiffuseColor(RedFHandleLight handle, float r, float g, float b, float a) {
  ofLight * light = (ofLight *)handle;
  light->setDiffuseColor(ofFloatColor(r, g, b, a));
}

REDGPU_F_DECLSPEC void redFLightSetSpecularColor(RedFHandleLight handle, float r, float g, float b, float a) {
  ofLight * light = (ofLight *)handle;
  light->setSpecularColor(ofFloatColor(r, g, b, a));
}

REDGPU_F_DECLSPEC void redFLightGetAmbientColor(RedFHandleLight handle, void * outVec4) {
  ofLight * light = (ofLight *)handle;
  ofFloatColor out = light->getAmbientColor();
  float * outFloats = (float *)outVec4;
  outFloats[0] = out.r;
  outFloats[1] = out.g;
  outFloats[2] = out.b;
  outFloats[3] = out.a;
}

REDGPU_F_DECLSPEC void redFLightGetDiffuseColor(RedFHandleLight handle, void * outVec4) {
  ofLight * light = (ofLight *)handle;
  ofFloatColor out = light->getDiffuseColor();
  float * outFloats = (float *)outVec4;
  outFloats[0] = out.r;
  outFloats[1] = out.g;
  outFloats[2] = out.b;
  outFloats[3] = out.a;
}

REDGPU_F_DECLSPEC void redFLightGetSpecularColor(RedFHandleLight handle, void * outVec4) {
  ofLight * light = (ofLight *)handle;
  ofFloatColor out = light->getSpecularColor();
  float * outFloats = (float *)outVec4;
  outFloats[0] = out.r;
  outFloats[1] = out.g;
  outFloats[2] = out.b;
  outFloats[3] = out.a;
}

REDGPU_F_DECLSPEC int redFLightGetLightID(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  return light->getLightID();
}

REDGPU_F_DECLSPEC void redFMaterialSetup(RedFHandleMaterial handle, const RedFMaterialSettings * settings) {
  ofMaterial * material = (ofMaterial *)handle;
  ofMaterialSettings s;
  if (settings != NULL) {
    s.diffuse.r  = settings->diffuse[0];
    s.diffuse.g  = settings->diffuse[1];
    s.diffuse.b  = settings->diffuse[2];
    s.diffuse.a  = settings->diffuse[3];
    s.ambient.r  = settings->ambient[0];
    s.ambient.g  = settings->ambient[1];
    s.ambient.b  = settings->ambient[2];
    s.ambient.a  = settings->ambient[3];
    s.specular.r = settings->specular[0];
    s.specular.g = settings->specular[1];
    s.specular.b = settings->specular[2];
    s.specular.a = settings->specular[3];
    s.emissive.r = settings->emissive[0];
    s.emissive.g = settings->emissive[1];
    s.emissive.b = settings->emissive[2];
    s.emissive.a = settings->emissive[3];
    s.shininess  = settings->shininess;
    std::string postFragment   = settings->postFragment   != NULL ? settings->postFragment   : "";
    std::string customUniforms = settings->customUniforms != NULL ? settings->customUniforms : "";
    s.postFragment   = postFragment;
    s.customUniforms = customUniforms;
  }
  material->setup(s);
}

REDGPU_F_DECLSPEC void redFMaterialSetDiffuseColor(RedFHandleMaterial handle, float r, float g, float b, float a) {
  ofMaterial * material = (ofMaterial *)handle;
  material->setDiffuseColor(ofFloatColor(r, g, b, a));
}

REDGPU_F_DECLSPEC void redFMaterialSetAmbientColor(RedFHandleMaterial handle, float r, float g, float b, float a) {
  ofMaterial * material = (ofMaterial *)handle;
  material->setAmbientColor(ofFloatColor(r, g, b, a));
}

REDGPU_F_DECLSPEC void redFMaterialSetSpecularColor(RedFHandleMaterial handle, float r, float g, float b, float a) {
  ofMaterial * material = (ofMaterial *)handle;
  material->setSpecularColor(ofFloatColor(r, g, b, a));
}

REDGPU_F_DECLSPEC void redFMaterialSetEmissiveColor(RedFHandleMaterial handle, float r, float g, float b, float a) {
  ofMaterial * material = (ofMaterial *)handle;
  material->setEmissiveColor(ofFloatColor(r, g, b, a));
}

REDGPU_F_DECLSPEC void redFMaterialSetShininess(RedFHandleMaterial handle, float nShininess) {
  ofMaterial * material = (ofMaterial *)handle;
  material->setShininess(nShininess);
}

REDGPU_F_DECLSPEC void redFMaterialGetDiffuseColor(RedFHandleMaterial handle, void * outVec4) {
  ofMaterial * material = (ofMaterial *)handle;
  ofFloatColor out = material->getDiffuseColor();
  float * outFloats = (float *)outVec4;
  outFloats[0] = out.r;
  outFloats[1] = out.g;
  outFloats[2] = out.b;
  outFloats[3] = out.a;
}

REDGPU_F_DECLSPEC void redFMaterialGetAmbientColor(RedFHandleMaterial handle, void * outVec4) {
  ofMaterial * material = (ofMaterial *)handle;
  ofFloatColor out = material->getAmbientColor();
  float * outFloats = (float *)outVec4;
  outFloats[0] = out.r;
  outFloats[1] = out.g;
  outFloats[2] = out.b;
  outFloats[3] = out.a;
}

REDGPU_F_DECLSPEC void redFMaterialGetSpecularColor(RedFHandleMaterial handle, void * outVec4) {
  ofMaterial * material = (ofMaterial *)handle;
  ofFloatColor out = material->getSpecularColor();
  float * outFloats = (float *)outVec4;
  outFloats[0] = out.r;
  outFloats[1] = out.g;
  outFloats[2] = out.b;
  outFloats[3] = out.a;
}

REDGPU_F_DECLSPEC void redFMaterialGetEmissiveColor(RedFHandleMaterial handle, void * outVec4) {
  ofMaterial * material = (ofMaterial *)handle;
  ofFloatColor out = material->getEmissiveColor();
  float * outFloats = (float *)outVec4;
  outFloats[0] = out.r;
  outFloats[1] = out.g;
  outFloats[2] = out.b;
  outFloats[3] = out.a;
}

REDGPU_F_DECLSPEC float redFMaterialGetShininess(RedFHandleMaterial handle) {
  ofMaterial * material = (ofMaterial *)handle;
  return material->getShininess();
}

REDGPU_F_DECLSPEC void redFMaterialGetSettings(RedFHandleMaterial handle, RedFMaterialSettings * outSettings, char ** outPostFragment, char ** outCustomUniforms) {
  ofMaterial * material = (ofMaterial *)handle;
  ofMaterialSettings out = material->getSettings();
  outSettings->diffuse[0]  = out.diffuse.r;
  outSettings->diffuse[1]  = out.diffuse.g;
  outSettings->diffuse[2]  = out.diffuse.b;
  outSettings->diffuse[3]  = out.diffuse.a;
  outSettings->ambient[0]  = out.ambient.r;
  outSettings->ambient[1]  = out.ambient.g;
  outSettings->ambient[2]  = out.ambient.b;
  outSettings->ambient[3]  = out.ambient.a;
  outSettings->specular[0] = out.specular.r;
  outSettings->specular[1] = out.specular.g;
  outSettings->specular[2] = out.specular.b;
  outSettings->specular[3] = out.specular.a;
  outSettings->emissive[0] = out.emissive.r;
  outSettings->emissive[1] = out.emissive.g;
  outSettings->emissive[2] = out.emissive.b;
  outSettings->emissive[3] = out.emissive.a;
  outSettings->shininess   = out.shininess;
  if (out.postFragment == "") {
    outSettings->postFragment = NULL;
    if (outPostFragment != NULL) {
      outPostFragment[0] = NULL;
    }
  } else {
    char * postFragment = (char *)malloc(out.postFragment.size() + 1);
    if (postFragment != NULL) {
      std::copy(out.postFragment.begin(), out.postFragment.end(), postFragment);
      postFragment[out.postFragment.size()] = 0;
    }
    outSettings->postFragment = (const char *)postFragment;
    if (outPostFragment != NULL) {
      outPostFragment[0] = postFragment;
    }
  }
  if (out.customUniforms == "") {
    outSettings->customUniforms = NULL;
    if (outCustomUniforms != NULL) {
      outCustomUniforms[0] = NULL;
    }
  } else {
    char * customUniforms = (char *)malloc(out.customUniforms.size() + 1);
    if (customUniforms != NULL) {
      std::copy(out.customUniforms.begin(), out.customUniforms.end(), customUniforms);
      customUniforms[out.customUniforms.size()] = 0;
    }
    outSettings->customUniforms = (const char *)customUniforms;
    if (outCustomUniforms != NULL) {
      outCustomUniforms[0] = customUniforms;
    }
  }
}

REDGPU_F_DECLSPEC void redFMaterialSetCustomUniform4f(RedFHandleMaterial handle, const char * uniformName, float v0, float v1, float v2, float v3) {
  ofMaterial * material = (ofMaterial *)handle;
  std::string name = uniformName;
  material->setCustomUniform4f(name, glm::vec4(v0, v1, v2, v3));
}

REDGPU_F_DECLSPEC void redFMaterialSetCustomUniformMatrix4f(RedFHandleMaterial handle, const char * uniformName, void * mat4) {
  ofMaterial * material = (ofMaterial *)handle;
  std::string name = uniformName;
  glm::mat4 m = glm::make_mat4((float *)mat4);
  material->setCustomUniformMatrix4f(name, m);
}

REDGPU_F_DECLSPEC void redFMaterialSetCustomUniformImage(RedFHandleMaterial handle, const char * uniformName, RedFHandleImage image, int textureLocation) {
  ofMaterial * material = (ofMaterial *)handle;
  ofImage * _image = (ofImage *)image;
  std::string name = uniformName;
  material->setCustomUniformTexture(name, _image->getTexture(), textureLocation);
}

REDGPU_F_DECLSPEC void redFMaterialSetCustomUniformFbo(RedFHandleMaterial handle, const char * uniformName, RedFHandleFbo fbo, int fboAttachmentPoint, int textureLocation) {
  ofMaterial * material = (ofMaterial *)handle;
  ofFbo * _fbo = (ofFbo *)fbo;
  std::string name = uniformName;
  material->setCustomUniformTexture(name, _fbo->getTexture(fboAttachmentPoint), textureLocation);
}

REDGPU_F_DECLSPEC void redFMaterialSetCustomUniformFboDepth(RedFHandleMaterial handle, const char * uniformName, RedFHandleFbo fbo, int textureLocation) {
  ofMaterial * material = (ofMaterial *)handle;
  ofFbo * _fbo = (ofFbo *)fbo;
  std::string name = uniformName;
  material->setCustomUniformTexture(name, _fbo->getDepthTexture(), textureLocation);
}

REDGPU_F_DECLSPEC void redFMaterialBegin(RedFHandleMaterial handle) {
  ofMaterial * material = (ofMaterial *)handle;
  material->begin();
}

REDGPU_F_DECLSPEC void redFMaterialEnd(RedFHandleMaterial handle) {
  ofMaterial * material = (ofMaterial *)handle;
  material->end();
}

REDGPU_F_DECLSPEC RedFBool32 redFShaderLoad(RedFHandleShader handle, const char * vertexFileName, const char * fragmentFileName) {
  ofShader * shader = (ofShader *)handle;
  std::string vertName = vertexFileName;
#ifdef __linux__
#warning
  std::string vertPath = vertName;
#else
  std::filesystem::path vertPath = vertName;
#endif
  std::string fragName = fragmentFileName;
#ifdef __linux__
#warning
  std::string fragPath = fragName;
#else
  std::filesystem::path fragPath = fragName;
#endif
  return (RedFBool32)shader->load(vertPath, fragPath);
}

REDGPU_F_DECLSPEC void redFShaderBegin(RedFHandleShader handle) {
  ofShader * shader = (ofShader *)handle;
  shader->begin();
}

REDGPU_F_DECLSPEC void redFShaderSetUniform4f(RedFHandleShader handle, const char * uniformName, float v0, float v1, float v2, float v3) {
  ofShader * shader = (ofShader *)handle;
  std::string name = uniformName;
  shader->setUniform4f(name, v0, v1, v2, v3);
}

REDGPU_F_DECLSPEC void redFShaderSetUniform4fv(RedFHandleShader handle, const char * uniformName, void * v, int count) {
  ofShader * shader = (ofShader *)handle;
  std::string name = uniformName;
  shader->setUniform4fv(name, (const float *)v, count);
}

REDGPU_F_DECLSPEC void redFShaderSetUniformMatrix4f(RedFHandleShader handle, const char * uniformName, void * mat4) {
  ofShader * shader = (ofShader *)handle;
  std::string name = uniformName;
  glm::mat4 m = glm::make_mat4((float *)mat4);
  shader->setUniformMatrix4f(name, m);
}

REDGPU_F_DECLSPEC void redFShaderSetUniformImage(RedFHandleShader handle, const char * uniformName, RedFHandleImage image, int textureLocation) {
  ofShader * shader = (ofShader *)handle;
  ofImage * _image = (ofImage *)image;
  std::string name = uniformName;
  shader->setUniformTexture(name, _image->getTexture(), textureLocation);
}

REDGPU_F_DECLSPEC void redFShaderSetUniformFbo(RedFHandleShader handle, const char * uniformName, RedFHandleFbo fbo, int fboAttachmentPoint, int textureLocation) {
  ofShader * shader = (ofShader *)handle;
  ofFbo * _fbo = (ofFbo *)fbo;
  std::string name = uniformName;
  shader->setUniformTexture(name, _fbo->getTexture(fboAttachmentPoint), textureLocation);
}

REDGPU_F_DECLSPEC void redFShaderSetUniformFboDepth(RedFHandleShader handle, const char * uniformName, RedFHandleFbo fbo, int textureLocation) {
  ofShader * shader = (ofShader *)handle;
  ofFbo * _fbo = (ofFbo *)fbo;
  std::string name = uniformName;
  shader->setUniformTexture(name, _fbo->getDepthTexture(), textureLocation);
}

REDGPU_F_DECLSPEC void redFShaderEnd(RedFHandleShader handle) {
  ofShader * shader = (ofShader *)handle;
  shader->end();
}

REDGPU_F_DECLSPEC void redFManipulatorCopyToNode(RedFHandleManipulator handle, RedFHandleNode to) {
  ofxManipulator * source = (ofxManipulator *)handle;
  ofNode * target = (ofNode *)to;
  const glm::vec3 s = source->getScale();
  const glm::quat r = source->getRotation();
  const glm::vec3 t = source->getTranslation();
  target->setScale(s);
  target->setGlobalOrientation(r);
  target->setGlobalPosition(t);
}

REDGPU_F_DECLSPEC void redFManipulatorCopyToManipulator(RedFHandleManipulator handle, RedFHandleManipulator to) {
  ofxManipulator * source = (ofxManipulator *)handle;
  ofxManipulator * target = (ofxManipulator *)to;
  const glm::vec3 s = source->getScale();
  const glm::quat r = source->getRotation();
  const glm::vec3 t = source->getTranslation();
  target->setScale(s);
  target->setRotation(r);
  target->setTranslation(t);
}

REDGPU_F_DECLSPEC void redFManipulatorDraw(RedFHandleManipulator handle, RedFHandleCamera camera) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  ofCamera * cam = (ofCamera *)camera;
  manipulator->draw(cam[0]);
}

REDGPU_F_DECLSPEC void redFManipulatorGetMatrix(RedFHandleManipulator handle, void * outMat4) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  glm::mat4 out = manipulator->getMatrix();
  float * outFloats = (float *)outMat4;
  float * p = glm::value_ptr(out);
  outFloats[0]  = p[0];
  outFloats[1]  = p[1];
  outFloats[2]  = p[2];
  outFloats[3]  = p[3];
  outFloats[4]  = p[4];
  outFloats[5]  = p[5];
  outFloats[6]  = p[6];
  outFloats[7]  = p[7];
  outFloats[8]  = p[8];
  outFloats[9]  = p[9];
  outFloats[10] = p[10];
  outFloats[11] = p[11];
  outFloats[12] = p[12];
  outFloats[13] = p[13];
  outFloats[14] = p[14];
  outFloats[15] = p[15];
}

REDGPU_F_DECLSPEC void redFManipulatorGetScale(RedFHandleManipulator handle, void * outVec3) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  glm::vec3 out = manipulator->getScale();
  float * outFloats = (float *)outVec3;
  float * p = glm::value_ptr(out);
  outFloats[0] = p[0];
  outFloats[1] = p[1];
  outFloats[2] = p[2];
}

REDGPU_F_DECLSPEC void redFManipulatorGetRotationQuaternion(RedFHandleManipulator handle, void * outVec4) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  glm::quat out = manipulator->getRotation();
  float * outFloats = (float *)outVec4;
  float * p = glm::value_ptr(out);
  outFloats[0] = p[0];
  outFloats[1] = p[1];
  outFloats[2] = p[2];
  outFloats[3] = p[3];
}

REDGPU_F_DECLSPEC void redFManipulatorGetRotationAxisAngleDeg(RedFHandleManipulator handle, void * outVec4) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  float p[4] = {};
  manipulator->getRotation().getRotate(p[3], p[0], p[1], p[2]);
  float * outFloats = (float *)outVec4;
  outFloats[0] = p[0];
  outFloats[1] = p[1];
  outFloats[2] = p[2];
  outFloats[3] = p[3];
}

REDGPU_F_DECLSPEC void redFManipulatorGetTranslation(RedFHandleManipulator handle, void * outVec3) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  glm::vec3 out = manipulator->getTranslation();
  float * outFloats = (float *)outVec3;
  float * p = glm::value_ptr(out);
  outFloats[0] = p[0];
  outFloats[1] = p[1];
  outFloats[2] = p[2];
}

REDGPU_F_DECLSPEC void redFManipulatorSetScale(RedFHandleManipulator handle, float x, float y, float z) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  manipulator->setScale(glm::vec3(x, y, z));
}

REDGPU_F_DECLSPEC void redFManipulatorSetRotation(RedFHandleManipulator handle, float x, float y, float z, float w) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  glm::quat q;
  q.x = x;
  q.y = y;
  q.z = z;
  q.w = w;
  manipulator->setRotation(q);
}

REDGPU_F_DECLSPEC void redFManipulatorSetTranslation(RedFHandleManipulator handle, float x, float y, float z) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  manipulator->setTranslation(glm::vec3(x, y, z));
}

REDGPU_F_DECLSPEC void redFManipulatorScale(RedFHandleManipulator handle, float x, float y, float z) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  manipulator->scale(glm::vec3(x, y, z));
}

REDGPU_F_DECLSPEC void redFManipulatorRotate(RedFHandleManipulator handle, float x, float y, float z, float w) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  glm::quat q;
  q.x = x;
  q.y = y;
  q.z = z;
  q.w = w;
  manipulator->rotate(q);
}

REDGPU_F_DECLSPEC void redFManipulatorTranslate(RedFHandleManipulator handle, float x, float y, float z) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  manipulator->translate(glm::vec3(x, y, z));
}

REDGPU_F_DECLSPEC void redFManipulatorToggleScale(RedFHandleManipulator handle) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  manipulator->toggleScale();
}

REDGPU_F_DECLSPEC void redFManipulatorToggleRotation(RedFHandleManipulator handle) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  manipulator->toggleRotation();
}

REDGPU_F_DECLSPEC void redFManipulatorToggleTranslation(RedFHandleManipulator handle) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  manipulator->toggleTranslation();
}

REDGPU_F_DECLSPEC float redFManipulatorGetManipulatorScale(RedFHandleManipulator handle) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  return manipulator->getManipulatorScale();
}

REDGPU_F_DECLSPEC void redFManipulatorSetManipulatorScale(RedFHandleManipulator handle, float scale) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  manipulator->setManipulatorScale(scale);
}

REDGPU_F_DECLSPEC RedFManipulatorType redFManipulatorGetManipulatorType(RedFHandleManipulator handle) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  return (RedFManipulatorType)manipulator->getManipulatorType();
}

REDGPU_F_DECLSPEC void redFManipulatorSetManipulatorType(RedFHandleManipulator handle, RedFManipulatorType type) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  manipulator->setManipulatorType((ofxManipulator::MANIPULATOR_TYPE)type);
}

REDGPU_F_DECLSPEC void redFManipulatorSetManipulatorColors(RedFHandleManipulator handle, int xr, int xg, int xb, int xa, int yr, int yg, int yb, int ya, int zr, int zg, int zb, int za, int wr, int wg, int wb, int wa, int sr, int sg, int sb, int sa) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  manipulator->setManipulatorColors(ofColor(xr, xg, xb, xa), ofColor(yr, yg, yb, ya), ofColor(zr, zg, zb, za), ofColor(wr, wg, wb, wa), ofColor(sr, sg, sb, sa));
}

REDGPU_F_DECLSPEC RedFBool32 redFManipulatorIsEnabled(RedFHandleManipulator handle) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  return (RedFBool32)manipulator->isEnabled();
}

REDGPU_F_DECLSPEC void redFManipulatorEnable(RedFHandleManipulator handle) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  manipulator->enable();
}

REDGPU_F_DECLSPEC void redFManipulatorDisable(RedFHandleManipulator handle) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  manipulator->disable();
}

REDGPU_F_DECLSPEC RedFBool32 redFAssimpLoadModel(RedFHandleAssimp handle, const char * fileName, RedFBool32 optimize) {
  ofxAssimpModelLoader * assimp = (ofxAssimpModelLoader *)handle;
  std::string name = fileName;
  return (RedFBool32)assimp->load(name, optimize == 1 ? ofxAssimpModelLoader::Flags::OPTIMIZE_HIGH : ofxAssimpModelLoader::Flags::OPTIMIZE_DEFAULT);
}

REDGPU_F_DECLSPEC unsigned redFAssimpGetNumMeshes(RedFHandleAssimp handle) {
  ofxAssimpModelLoader * assimp = (ofxAssimpModelLoader *)handle;
  return assimp->getNumMeshes();
}

REDGPU_F_DECLSPEC void redFAssimpGetMeshName(RedFHandleAssimp handle, unsigned index, uint64_t * outCharsCountIncludingNullTerminator, char * outChars) {
  ofxAssimpModelLoader * assimp = (ofxAssimpModelLoader *)handle;
  const std::vector<std::string> names = assimp->getMeshNames();
  const std::string name = names[index];
  if (outCharsCountIncludingNullTerminator != NULL) {
    outCharsCountIncludingNullTerminator[0] = name.size() + 1;
  }
  if (outChars != NULL) {
    std::copy(name.begin(), name.end(), outChars);
    outChars[name.size()] = 0;
  }
}

REDGPU_F_DECLSPEC void redFAssimpGetMesh(RedFHandleAssimp handle, unsigned index, RedFHandleMesh replaceMesh) {
  ofxAssimpModelLoader * assimp = (ofxAssimpModelLoader *)handle;
  ofMesh * out = (ofMesh *)replaceMesh;
  out[0] = assimp->getMesh(index);
}

// EquiMap stubs for compatibility with REDGPU Framework 1

REDGPU_F_DECLSPEC RedFHandleEquiMap * redFCreateEquiMap(uint64_t count) {
  RedFHandleEquiMap * allocation = new(std::nothrow) RedFHandleEquiMap [count]();
  if (allocation == NULL) { return NULL; }
  for (uint64_t i = 0; i < count; i += 1) {
    allocation[i] = (RedFHandleEquiMap)(void *)1;
  }
  return allocation;
}

REDGPU_F_DECLSPEC void redFDestroyEquiMap(RedFHandleEquiMap * handles) {
  if (handles == NULL) { return; }
  delete[] handles;
}

REDGPU_F_DECLSPEC void redFEquiMapSetup(RedFHandleEquiMap handle, int size) {
  volatile int nil = 0;
}

REDGPU_F_DECLSPEC void redFEquiMapRender(RedFHandleEquiMap handle, void (*drawEquiScene)(void *), void * userData, float cubePosX, float cubePosY, float cubePosZ) {
  volatile int nil = 0;
}

REDGPU_F_DECLSPEC void redFEquiMapDraw(RedFHandleEquiMap handle, float x, float y, float w, float h) {
  volatile int nil = 0;
}

REDGPU_F_DECLSPEC RedFHandleImgui * redFCreateImgui(uint64_t count) {
  RedFHandleImgui * allocation = new(std::nothrow) RedFHandleImgui [count]();
  if (allocation == NULL) { return NULL; }
  for (uint64_t i = 0; i < count; i += 1) {
    allocation[i] = (RedFHandleImgui)(void *)1;
  }
  return allocation;
}

REDGPU_F_DECLSPEC void redFDestroyImgui(RedFHandleImgui * handles) {
  if (handles == NULL) { return; }
  delete[] handles;
}

REDGPU_F_DECLSPEC void redFImguiSetup(RedFHandleImgui handle) {
  ofxRaccoonImGui::initialize();
}

REDGPU_F_DECLSPEC void redFImguiBegin(RedFHandleImgui handle) {
  ofxRaccoonImGui::beginGui();
}

REDGPU_F_DECLSPEC void redFImguiEnd(RedFHandleImgui handle) {
  ofxRaccoonImGui::endGui();
}

REDGPU_F_DECLSPEC RedFBool32 redFSoundPlayerLoad(RedFHandleSoundPlayer handle, const char * fileName, RedFBool32 streamDefaultIs0) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  std::string name = fileName;
#ifdef __linux__
#warning
  std::string path = name;
#else
  std::filesystem::path path = name;
#endif
  return (RedFBool32)sound->load(path, streamDefaultIs0);
}

REDGPU_F_DECLSPEC void redFSoundPlayerUnload(RedFHandleSoundPlayer handle) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  sound->unload();
}

REDGPU_F_DECLSPEC void redFSoundPlayerPlay(RedFHandleSoundPlayer handle) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  sound->play();
}

REDGPU_F_DECLSPEC void redFSoundPlayerStop(RedFHandleSoundPlayer handle) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  sound->stop();
}

REDGPU_F_DECLSPEC void redFSoundPlayerSetVolume(RedFHandleSoundPlayer handle, float volume0to1) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  sound->setVolume(volume0to1);
}

REDGPU_F_DECLSPEC void redFSoundPlayerSetPan(RedFHandleSoundPlayer handle, float panMinus1to1) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  sound->setPan(panMinus1to1);
}

REDGPU_F_DECLSPEC void redFSoundPlayerSetSpeed(RedFHandleSoundPlayer handle, float speedDefaultIs1) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  sound->setSpeed(speedDefaultIs1);
}

REDGPU_F_DECLSPEC void redFSoundPlayerSetPaused(RedFHandleSoundPlayer handle, RedFBool32 paused) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  sound->setPaused(paused);
}

REDGPU_F_DECLSPEC void redFSoundPlayerSetLoop(RedFHandleSoundPlayer handle, RedFBool32 loop) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  sound->setLoop(loop);
}

REDGPU_F_DECLSPEC void redFSoundPlayerSetMultiPlay(RedFHandleSoundPlayer handle, RedFBool32 multiplay) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  sound->setMultiPlay(multiplay);
}

REDGPU_F_DECLSPEC void redFSoundPlayerSetPosition(RedFHandleSoundPlayer handle, float percent0to1) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  sound->setPosition(percent0to1);
}

REDGPU_F_DECLSPEC void redFSoundPlayerSetPositionMS(RedFHandleSoundPlayer handle, int milliseconds) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  sound->setPositionMS(milliseconds);
}

REDGPU_F_DECLSPEC int redFSoundPlayerGetPositionMS(RedFHandleSoundPlayer handle) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  return sound->getPositionMS();
}

REDGPU_F_DECLSPEC float redFSoundPlayerGetPosition(RedFHandleSoundPlayer handle) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  return sound->getPosition();
}

REDGPU_F_DECLSPEC RedFBool32 redFSoundPlayerIsPlaying(RedFHandleSoundPlayer handle) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  return (RedFBool32)sound->isPlaying();
}

REDGPU_F_DECLSPEC float redFSoundPlayerGetSpeed(RedFHandleSoundPlayer handle) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  return sound->getSpeed();
}

REDGPU_F_DECLSPEC float redFSoundPlayerGetPan(RedFHandleSoundPlayer handle) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  return sound->getPan();
}

REDGPU_F_DECLSPEC float redFSoundPlayerGetVolume(RedFHandleSoundPlayer handle) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  return sound->getVolume();
}

REDGPU_F_DECLSPEC RedFBool32 redFSoundPlayerIsLoaded(RedFHandleSoundPlayer handle) {
  ofSoundPlayer * sound = (ofSoundPlayer *)handle;
  return (RedFBool32)sound->isLoaded();
}

REDGPU_F_DECLSPEC void redFSoundSetVolume(float volume0to1) {
  ofSoundSetVolume(volume0to1);
}

REDGPU_F_DECLSPEC void redFSoundShutdown(void) {
  ofSoundShutdown();
}

REDGPU_F_DECLSPEC void redFSoundStopAll(void) {
  ofSoundStopAll();
}

REDGPU_F_DECLSPEC void redFSoundUpdate(void) {
  ofSoundUpdate();
}

REDGPU_F_DECLSPEC void redFDirectoryOpen(RedFHandleDirectory handle, const char * path) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string name = path;
#ifdef __linux__
#warning
  std::string p = name;
#else
  std::filesystem::path p = name;
#endif
  dir->open(p);
}

REDGPU_F_DECLSPEC void redFDirectoryOpenFromCurrentWorkingDirectory(RedFHandleDirectory handle, const char * path) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string name = path;
#ifdef __linux__
#warning
  std::string p = name;
#else
  std::filesystem::path p = name;
#endif
  dir->openFromCWD(p);
}

REDGPU_F_DECLSPEC void redFDirectoryClose(RedFHandleDirectory handle) {
  ofDirectory * dir = (ofDirectory *)handle;
  dir->close();
}

REDGPU_F_DECLSPEC uint64_t redFDirectoryListDir(RedFHandleDirectory handle) {
  ofDirectory * dir = (ofDirectory *)handle;
  return dir->listDir();
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryCreate(RedFHandleDirectory handle, RedFBool32 recursiveDefaultIs0) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->create(recursiveDefaultIs0);
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryExists(RedFHandleDirectory handle) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->exists();
}

REDGPU_F_DECLSPEC void redFDirectoryPath(RedFHandleDirectory handle, char ** outPath, uint64_t * outPathBytesCount) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string path = dir->path();
  {
    std::string s = path;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outPath[0] = p;
  }
  if (outPathBytesCount != NULL) {
    outPathBytesCount[0] = path.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFDirectoryGetAbsolutePath(RedFHandleDirectory handle, char ** outAbsolutePath, uint64_t * outAbsolutePathBytesCount) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string absolute = dir->getAbsolutePath();
  {
    std::string s = absolute;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outAbsolutePath[0] = p;
  }
  if (outAbsolutePathBytesCount != NULL) {
    outAbsolutePathBytesCount[0] = absolute.size() + 1;
  }
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryCanRead(RedFHandleDirectory handle) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->canRead();
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryCanWrite(RedFHandleDirectory handle) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->canWrite();
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryCanExecute(RedFHandleDirectory handle) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->canExecute();
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryIsDirectory(RedFHandleDirectory handle) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->isDirectory();
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryIsHidden(RedFHandleDirectory handle) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->isHidden();
}

REDGPU_F_DECLSPEC void redFDirectorySetWriteable(RedFHandleDirectory handle, RedFBool32 writeableDefaultIs1) {
  ofDirectory * dir = (ofDirectory *)handle;
  dir->setWriteable(writeableDefaultIs1);
}

REDGPU_F_DECLSPEC void redFDirectorySetReadable(RedFHandleDirectory handle, RedFBool32 readableDefaultIs1) {
  ofDirectory * dir = (ofDirectory *)handle;
  dir->setReadable(readableDefaultIs1);
}

REDGPU_F_DECLSPEC void redFDirectorySetExecutable(RedFHandleDirectory handle, RedFBool32 executableDefaultIs1) {
  ofDirectory * dir = (ofDirectory *)handle;
  dir->setExecutable(executableDefaultIs1);
}

REDGPU_F_DECLSPEC void redFDirectorySetShowHidden(RedFHandleDirectory handle, RedFBool32 showHidden) {
  ofDirectory * dir = (ofDirectory *)handle;
  dir->setShowHidden(showHidden);
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryCopyTo(RedFHandleDirectory handle, const char * path, RedFBool32 bRelativeToDataDefaultIs1, RedFBool32 overwriteDefaultIs0) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string name = path;
#ifdef __linux__
#warning
  std::string p = name;
#else
  std::filesystem::path p = name;
#endif
  return (RedFBool32)dir->copyTo(p, bRelativeToDataDefaultIs1, overwriteDefaultIs0);
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryMoveTo(RedFHandleDirectory handle, const char * path, RedFBool32 bRelativeToDataDefaultIs1, RedFBool32 overwriteDefaultIs0) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string name = path;
#ifdef __linux__
#warning
  std::string p = name;
#else
  std::filesystem::path p = name;
#endif
  return (RedFBool32)dir->moveTo(p, bRelativeToDataDefaultIs1, overwriteDefaultIs0);
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryRenameTo(RedFHandleDirectory handle, const char * path, RedFBool32 bRelativeToDataDefaultIs1, RedFBool32 overwriteDefaultIs0) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string name = path;
#ifdef __linux__
#warning
  std::string p = name;
#else
  std::filesystem::path p = name;
#endif
  return (RedFBool32)dir->renameTo(p, bRelativeToDataDefaultIs1, overwriteDefaultIs0);
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryRemove(RedFHandleDirectory handle, RedFBool32 recursive) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->remove(recursive);
}

REDGPU_F_DECLSPEC void redFDirectoryAllowExt(RedFHandleDirectory handle, const char * extension) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string ext = extension;
  dir->allowExt(ext);
}

REDGPU_F_DECLSPEC void redFDirectoryGetOriginalDirectory(RedFHandleDirectory handle, char ** outOriginalDirectory, uint64_t * outOriginalDirectoryBytesCount) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string name = dir->getOriginalDirectory();
  {
    std::string s = name;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outOriginalDirectory[0] = p;
  }
  if (outOriginalDirectoryBytesCount != NULL) {
    outOriginalDirectoryBytesCount[0] = name.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFDirectoryGetName(RedFHandleDirectory handle, uint64_t index, char ** outName, uint64_t * outNameBytesCount) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string name = dir->getName(index);
  {
    std::string s = name;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outName[0] = p;
  }
  if (outNameBytesCount != NULL) {
    outNameBytesCount[0] = name.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFDirectoryGetPath(RedFHandleDirectory handle, uint64_t index, char ** outPath, uint64_t * outPathBytesCount) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string name = dir->getPath(index);
  {
    std::string s = name;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outPath[0] = p;
  }
  if (outPathBytesCount != NULL) {
    outPathBytesCount[0] = name.size() + 1;
  }
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryGetShowHidden(RedFHandleDirectory handle) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->getShowHidden();
}

REDGPU_F_DECLSPEC void redFDirectoryReset(RedFHandleDirectory handle) {
  ofDirectory * dir = (ofDirectory *)handle;
  dir->reset();
}

REDGPU_F_DECLSPEC void redFDirectorySort(RedFHandleDirectory handle) {
  ofDirectory * dir = (ofDirectory *)handle;
  dir->sort();
}

REDGPU_F_DECLSPEC void redFDirectorySortByDate(RedFHandleDirectory handle) {
  ofDirectory * dir = (ofDirectory *)handle;
  dir->sortByDate();
}

REDGPU_F_DECLSPEC uint64_t redFDirectorySize(RedFHandleDirectory handle) {
  ofDirectory * dir = (ofDirectory *)handle;
  return dir->size();
}

REDGPU_F_DECLSPEC uint64_t redFDirectoryGetFileCount(RedFHandleDirectory handle) {
  ofDirectory * dir = (ofDirectory *)handle;
  return dir->getFiles().size();
}

REDGPU_F_DECLSPEC void redFDirectoryGetFilePath(RedFHandleDirectory handle, uint64_t index, char ** outPath, uint64_t * outPathBytesCount) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string name = dir->getFile(index).path();
  {
    std::string s = name;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outPath[0] = p;
  }
  if (outPathBytesCount != NULL) {
    outPathBytesCount[0] = name.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFDirectoryGetFileExtension(RedFHandleDirectory handle, uint64_t index, char ** outExtension, uint64_t * outExtensionBytesCount) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string name = dir->getFile(index).getExtension();
  {
    std::string s = name;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outExtension[0] = p;
  }
  if (outExtensionBytesCount != NULL) {
    outExtensionBytesCount[0] = name.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFDirectoryGetFileName(RedFHandleDirectory handle, uint64_t index, char ** outFileName, uint64_t * outFileNameBytesCount) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string name = dir->getFile(index).getFileName();
  {
    std::string s = name;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outFileName[0] = p;
  }
  if (outFileNameBytesCount != NULL) {
    outFileNameBytesCount[0] = name.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFDirectoryGetFileBaseName(RedFHandleDirectory handle, uint64_t index, char ** outBaseName, uint64_t * outBaseNameBytesCount) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string name = dir->getFile(index).getBaseName();
  {
    std::string s = name;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outBaseName[0] = p;
  }
  if (outBaseNameBytesCount != NULL) {
    outBaseNameBytesCount[0] = name.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFDirectoryGetFileEnclosingDirectory(RedFHandleDirectory handle, uint64_t index, char ** outEnclosingDirectory, uint64_t * outEnclosingDirectoryBytesCount) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string name = dir->getFile(index).getEnclosingDirectory();
  {
    std::string s = name;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outEnclosingDirectory[0] = p;
  }
  if (outEnclosingDirectoryBytesCount != NULL) {
    outEnclosingDirectoryBytesCount[0] = name.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFDirectoryGetFileAbsolutePath(RedFHandleDirectory handle, uint64_t index, char ** outAbsolutePath, uint64_t * outAbsolutePathBytesCount) {
  ofDirectory * dir = (ofDirectory *)handle;
  std::string name = dir->getFile(index).getAbsolutePath();
  {
    std::string s = name;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outAbsolutePath[0] = p;
  }
  if (outAbsolutePathBytesCount != NULL) {
    outAbsolutePathBytesCount[0] = name.size() + 1;
  }
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryGetFileCanRead(RedFHandleDirectory handle, uint64_t index) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->getFile(index).canRead();
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryGetFileCanWrite(RedFHandleDirectory handle, uint64_t index) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->getFile(index).canWrite();
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryGetFileCanExecute(RedFHandleDirectory handle, uint64_t index) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->getFile(index).canExecute();
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryGetFileIsFile(RedFHandleDirectory handle, uint64_t index) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->getFile(index).isFile();
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryGetFileIsLink(RedFHandleDirectory handle, uint64_t index) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->getFile(index).isLink();
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryGetFileIsDirectory(RedFHandleDirectory handle, uint64_t index) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->getFile(index).isDirectory();
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryGetFileIsDevice(RedFHandleDirectory handle, uint64_t index) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->getFile(index).isDevice();
}

REDGPU_F_DECLSPEC RedFBool32 redFDirectoryGetFileIsHidden(RedFHandleDirectory handle, uint64_t index) {
  ofDirectory * dir = (ofDirectory *)handle;
  return (RedFBool32)dir->getFile(index).isHidden();
}

REDGPU_F_DECLSPEC uint64_t redFDirectoryGetFileGetSize(RedFHandleDirectory handle, uint64_t index) {
  ofDirectory * dir = (ofDirectory *)handle;
  return dir->getFile(index).getSize();
}

REDGPU_F_DECLSPEC RedFBool32 redFCreateDirectoryPath(const char * dirPath, RedFBool32 bRelativeToDataDefaultIs1, RedFBool32 recursiveDefaultIs0) {
  std::string name = dirPath;
#ifdef __linux__
#warning
  std::string p = name;
#else
  std::filesystem::path p = name;
#endif
  return (RedFBool32)ofDirectory::createDirectory(p, bRelativeToDataDefaultIs1, recursiveDefaultIs0);
}

REDGPU_F_DECLSPEC RedFBool32 redFIsDirectoryEmpty(const char * dirPath, RedFBool32 bRelativeToDataDefaultIs1) {
  std::string name = dirPath;
#ifdef __linux__
#warning
  std::string p = name;
#else
  std::filesystem::path p = name;
#endif
  return (RedFBool32)ofDirectory::isDirectoryEmpty(p, bRelativeToDataDefaultIs1);
}

REDGPU_F_DECLSPEC RedFBool32 redFDoesDirectoryExist(const char * dirPath, RedFBool32 bRelativeToDataDefaultIs1) {
  std::string name = dirPath;
#ifdef __linux__
#warning
  std::string p = name;
#else
  std::filesystem::path p = name;
#endif
  return (RedFBool32)ofDirectory::doesDirectoryExist(p, bRelativeToDataDefaultIs1);
}

REDGPU_F_DECLSPEC RedFBool32 redFRemoveDirectory(const char * dirPath, RedFBool32 deleteIfNotEmpty, RedFBool32 bRelativeToDataDefaultIs1) {
  std::string name = dirPath;
#ifdef __linux__
#warning
  std::string p = name;
#else
  std::filesystem::path p = name;
#endif
  return (RedFBool32)ofDirectory::removeDirectory(p, deleteIfNotEmpty, bRelativeToDataDefaultIs1);
}

REDGPU_F_DECLSPEC RedFBool32 redFCopyFileFromTo(const char * pathSrc, const char * pathDst, RedFBool32 bRelativeToDataDefaultIs1, RedFBool32 overwriteDefaultIs0) {
  std::string nameSrc = pathSrc;
  std::string nameDst = pathDst;
#ifdef __linux__
#warning
  std::string src = nameSrc;
  std::string dst = nameDst;
#else
  std::filesystem::path src = nameSrc;
  std::filesystem::path dst = nameDst;
#endif
  return (RedFBool32)ofFile::copyFromTo(src, dst, bRelativeToDataDefaultIs1, overwriteDefaultIs0);
}

REDGPU_F_DECLSPEC RedFBool32 redFMoveFileFromTo(const char * pathSrc, const char * pathDst, RedFBool32 bRelativeToDataDefaultIs1, RedFBool32 overwriteDefaultIs0) {
  std::string nameSrc = pathSrc;
  std::string nameDst = pathDst;
#ifdef __linux__
#warning
  std::string src = nameSrc;
  std::string dst = nameDst;
#else
  std::filesystem::path src = nameSrc;
  std::filesystem::path dst = nameDst;
#endif
  return (RedFBool32)ofFile::moveFromTo(src, dst, bRelativeToDataDefaultIs1, overwriteDefaultIs0);
}

REDGPU_F_DECLSPEC RedFBool32 redFDoesFileExist(const char * path, RedFBool32 bRelativeToDataDefaultIs1) {
  std::string name = path;
#ifdef __linux__
#warning
  std::string p = name;
#else
  std::filesystem::path p = name;
#endif
  return (RedFBool32)ofFile::doesFileExist(p, bRelativeToDataDefaultIs1);
}

REDGPU_F_DECLSPEC RedFBool32 redFRemoveFile(const char * path, RedFBool32 bRelativeToDataDefaultIs1) {
  std::string name = path;
#ifdef __linux__
#warning
  std::string p = name;
#else
  std::filesystem::path p = name;
#endif
  return (RedFBool32)ofFile::removeFile(p, bRelativeToDataDefaultIs1);
}

REDGPU_F_DECLSPEC void redFGetCurrentWorkingDirectory(char ** outPath, uint64_t * outPathBytesCount) {
  std::string name = ofFilePath::getCurrentWorkingDirectory();
  {
    std::string s = name;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outPath[0] = p;
  }
  if (outPathBytesCount != NULL) {
    outPathBytesCount[0] = name.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFGetCurrentExeDir(char ** outPath, uint64_t * outPathBytesCount) {
  std::string name = ofFilePath::getCurrentExeDir();
  {
    std::string s = name;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outPath[0] = p;
  }
  if (outPathBytesCount != NULL) {
    outPathBytesCount[0] = name.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFGetCurrentExePath(char ** outPath, uint64_t * outPathBytesCount) {
  std::string name = ofFilePath::getCurrentExePath();
  {
    std::string s = name;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outPath[0] = p;
  }
  if (outPathBytesCount != NULL) {
    outPathBytesCount[0] = name.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFGetUserHomeDir(char ** outPath, uint64_t * outPathBytesCount) {
  std::string name = ofFilePath::getUserHomeDir();
  {
    std::string s = name;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outPath[0] = p;
  }
  if (outPathBytesCount != NULL) {
    outPathBytesCount[0] = name.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFThreadSetProcedure(RedFHandleThread handle, void (*procedure)(void)) {
  RedFThread * thread = (RedFThread *)handle;
  thread->procedure = procedure;
}

REDGPU_F_DECLSPEC RedFBool32 redFThreadIsRunning(RedFHandleThread handle) {
  RedFThread * thread = (RedFThread *)handle;
  return (RedFBool32)thread->isThreadRunning();
}

REDGPU_F_DECLSPEC void redFThreadGetName(RedFHandleThread handle, char ** outName, uint64_t * outNameBytesCount) {
  RedFThread * thread = (RedFThread *)handle;
  std::string name = thread->getThreadName();
  {
    std::string s = name;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outName[0] = p;
  }
  if (outNameBytesCount != NULL) {
    outNameBytesCount[0] = name.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFThreadSetName(RedFHandleThread handle, const char * name) {
  RedFThread * thread = (RedFThread *)handle;
  std::string n = name;
  thread->setThreadName(n);
}

REDGPU_F_DECLSPEC void redFThreadStart(RedFHandleThread handle) {
  RedFThread * thread = (RedFThread *)handle;
  thread->startThread();
}

REDGPU_F_DECLSPEC RedFBool32 redFThreadLock(RedFHandleThread handle) {
  RedFThread * thread = (RedFThread *)handle;
  return (RedFBool32)thread->lock();
}

REDGPU_F_DECLSPEC RedFBool32 redFThreadTryLock(RedFHandleThread handle) {
  RedFThread * thread = (RedFThread *)handle;
  return (RedFBool32)thread->tryLock();
}

REDGPU_F_DECLSPEC void redFThreadUnlock(RedFHandleThread handle) {
  RedFThread * thread = (RedFThread *)handle;
  thread->unlock();
}

REDGPU_F_DECLSPEC void redFThreadStop(RedFHandleThread handle) {
  RedFThread * thread = (RedFThread *)handle;
  thread->stopThread();
}

REDGPU_F_DECLSPEC void redFThreadWaitFor(RedFHandleThread handle, RedFBool32 callStopThreadDefaultIs1, long millisecondsDefaultIsMinus1ForInfiniteJoinTimeout) {
  RedFThread * thread = (RedFThread *)handle;
  thread->waitForThread(callStopThreadDefaultIs1, millisecondsDefaultIsMinus1ForInfiniteJoinTimeout);
}

REDGPU_F_DECLSPEC void redFThreadSleep(RedFHandleThread handle, long milliseconds) {
  RedFThread * thread = (RedFThread *)handle;
  thread->sleep(milliseconds);
}

REDGPU_F_DECLSPEC void redFThreadYield(RedFHandleThread handle) {
  RedFThread * thread = (RedFThread *)handle;
  thread->yield();
}

REDGPU_F_DECLSPEC RedFBool32 redFThreadIsCurrent(RedFHandleThread handle) {
  RedFThread * thread = (RedFThread *)handle;
  return (RedFBool32)thread->isCurrentThread();
}

REDGPU_F_DECLSPEC RedFBool32 redFThreadChannelSend(RedFHandleThreadChannel handle, void * value) {
  ofThreadChannel<void *> * channel = (ofThreadChannel<void *> *)handle;
  void * sends = value;
  return (RedFBool32)channel->send(sends);
}

REDGPU_F_DECLSPEC RedFBool32 redFThreadChannelReceive(RedFHandleThreadChannel handle, void ** outSentValue) {
  ofThreadChannel<void *> * channel = (ofThreadChannel<void *> *)handle;
  void * received = NULL;
  RedFBool32 b = (RedFBool32)channel->receive(received);
  outSentValue[0] = received;
  return b;
}

REDGPU_F_DECLSPEC RedFBool32 redFThreadChannelTryReceive(RedFHandleThreadChannel handle, void ** outSentValue) {
  ofThreadChannel<void *> * channel = (ofThreadChannel<void *> *)handle;
  void * received = NULL;
  RedFBool32 b = (RedFBool32)channel->tryReceive(received);
  outSentValue[0] = received;
  return b;
}

REDGPU_F_DECLSPEC RedFBool32 redFThreadChannelTryReceiveWithTimeout(RedFHandleThreadChannel handle, void ** outSentValue, int64_t timeoutMs) {
  ofThreadChannel<void *> * channel = (ofThreadChannel<void *> *)handle;
  void * received = NULL;
  RedFBool32 b = (RedFBool32)channel->tryReceive(received, timeoutMs);
  outSentValue[0] = received;
  return b;
}

REDGPU_F_DECLSPEC RedFBool32 redFThreadChannelEmpty(RedFHandleThreadChannel handle) {
  ofThreadChannel<void *> * channel = (ofThreadChannel<void *> *)handle;
  return (RedFBool32)channel->empty();
}

REDGPU_F_DECLSPEC void redFThreadChannelClose(RedFHandleThreadChannel handle) {
  ofThreadChannel<void *> * channel = (ofThreadChannel<void *> *)handle;
  channel->close();
}

REDGPU_F_DECLSPEC RedFBool32 redFEventParametersKeyHasModifier(RedFHandleEventParametersKey parameters, int modifier) {
  return (RedFBool32)((ofKeyEventArgs *)parameters)->hasModifier(modifier);
}

REDGPU_F_DECLSPEC unsigned redFEventParametersKeyGetCodepoint(RedFHandleEventParametersKey parameters) {
  return ((ofKeyEventArgs *)parameters)->codepoint;
}

REDGPU_F_DECLSPEC RedFBool32 redFEventParametersKeyIsRepeat(RedFHandleEventParametersKey parameters) {
  return (RedFBool32)((ofKeyEventArgs *)parameters)->isRepeat;
}

REDGPU_F_DECLSPEC int redFEventParametersKeyGetKey(RedFHandleEventParametersKey parameters) {
  return ((ofKeyEventArgs *)parameters)->key;
}

REDGPU_F_DECLSPEC int redFEventParametersKeyGetKeycode(RedFHandleEventParametersKey parameters) {
  return ((ofKeyEventArgs *)parameters)->keycode;
}

REDGPU_F_DECLSPEC int redFEventParametersKeyGetModifiers(RedFHandleEventParametersKey parameters) {
  return ((ofKeyEventArgs *)parameters)->modifiers;
}

REDGPU_F_DECLSPEC int redFEventParametersKeyGetScancode(RedFHandleEventParametersKey parameters) {
  return ((ofKeyEventArgs *)parameters)->scancode;
}

REDGPU_F_DECLSPEC RedFKeyEventType redFEventParametersKeyGetEventType(RedFHandleEventParametersKey parameters) {
  return (RedFKeyEventType)((ofKeyEventArgs *)parameters)->type;
}

REDGPU_F_DECLSPEC RedFBool32 redFEventParametersMouseHasModifier(RedFHandleEventParametersMouse parameters, int modifier) {
  return (RedFBool32)((ofMouseEventArgs *)parameters)->hasModifier(modifier);
}

REDGPU_F_DECLSPEC float redFEventParametersMouseGetX(RedFHandleEventParametersMouse parameters) {
  return ((ofMouseEventArgs *)parameters)->x;
}

REDGPU_F_DECLSPEC float redFEventParametersMouseGetY(RedFHandleEventParametersMouse parameters) {
  return ((ofMouseEventArgs *)parameters)->y;
}

REDGPU_F_DECLSPEC int redFEventParametersMouseGetButton(RedFHandleEventParametersMouse parameters) {
  return ((ofMouseEventArgs *)parameters)->button;
}

REDGPU_F_DECLSPEC int redFEventParametersMouseGetModifiers(RedFHandleEventParametersMouse parameters) {
  return ((ofMouseEventArgs *)parameters)->modifiers;
}

REDGPU_F_DECLSPEC float redFEventParametersMouseGetScrollX(RedFHandleEventParametersMouse parameters) {
  return ((ofMouseEventArgs *)parameters)->scrollX;
}

REDGPU_F_DECLSPEC float redFEventParametersMouseGetScrollY(RedFHandleEventParametersMouse parameters) {
  return ((ofMouseEventArgs *)parameters)->scrollY;
}

REDGPU_F_DECLSPEC RedFMouseEventType redFEventParametersMouseGetEventType(RedFHandleEventParametersMouse parameters) {
  return (RedFMouseEventType)((ofMouseEventArgs *)parameters)->type;
}

REDGPU_F_DECLSPEC int redFEventParametersResizeGetWidth(RedFHandleEventParametersResize parameters) {
  return ((ofResizeEventArgs *)parameters)->width;
}

REDGPU_F_DECLSPEC int redFEventParametersResizeGetHeight(RedFHandleEventParametersResize parameters) {
  return ((ofResizeEventArgs *)parameters)->height;
}

REDGPU_F_DECLSPEC uint64_t redFEventParametersDragGetFilesCount(RedFHandleEventParametersDrag parameters) {
  uint64_t out = ((ofDragInfo *)parameters)->files.size();
  return out;
}

REDGPU_F_DECLSPEC const char * redFEventParametersDragGetFile(RedFHandleEventParametersDrag parameters, uint64_t fileIndex) {
  const char * out = ((ofDragInfo *)parameters)->files[fileIndex].c_str();
  return out;
}

REDGPU_F_DECLSPEC void redFEventParametersDragGetFileW(RedFHandleEventParametersDrag parameters, uint64_t fileIndex, wchar_t ** outPath, uint64_t * outPathBytesCount) {
  std::string name = ((ofDragInfo *)parameters)->files[fileIndex];
  std::wstring ws;
  {
    if (!name.empty()) {
#ifdef _WIN32
      ws = CA2W(name.c_str(), CP_UTF8);
#else
      ws = s2ws(name);
#endif
    }
    wchar_t * p = (wchar_t *)malloc((ws.size() + 1) * sizeof(wchar_t));
    if (p != NULL) {
      for (size_t i = 0, count = ws.size(); i < count; i += 1) {
        p[i] = ws[i];
      }
      p[ws.size()] = 0;
    }
    outPath[0] = p;
  }
  if (outPathBytesCount != NULL) {
    outPathBytesCount[0] = (ws.size() + 1) * sizeof(wchar_t);
  }
}

REDGPU_F_DECLSPEC float redFEventParametersDragGetPositionX(RedFHandleEventParametersDrag parameters) {
  return ((ofDragInfo *)parameters)->position.x;
}

REDGPU_F_DECLSPEC float redFEventParametersDragGetPositionY(RedFHandleEventParametersDrag parameters) {
  return ((ofDragInfo *)parameters)->position.y;
}

REDGPU_F_DECLSPEC RedFBool32 redFGetMousePressed(int button) {
  return (RedFBool32)ofGetMousePressed(button);
}

REDGPU_F_DECLSPEC int redFGetMouseX(void) {
  return ofGetMouseX();
}

REDGPU_F_DECLSPEC int redFGetMouseY(void) {
  return ofGetMouseY();
}

REDGPU_F_DECLSPEC int redFGetPreviousMouseX(void) {
  return ofGetPreviousMouseX();
}

REDGPU_F_DECLSPEC int redFGetPreviousMouseY(void) {
  return ofGetPreviousMouseY();
}

REDGPU_F_DECLSPEC void redFPushMatrix(void) {
  ofPushMatrix();
}

REDGPU_F_DECLSPEC void redFMultMatrix(void * mat4) {
  glm::mat4 m = glm::make_mat4((float *)mat4);
  ofMultMatrix(m);
}

REDGPU_F_DECLSPEC void redFMultViewMatrix(void * mat4) {
  glm::mat4 m = glm::make_mat4((float *)mat4);
  ofMultViewMatrix(m);
}

REDGPU_F_DECLSPEC void redFScale(float x, float y, float z) {
  ofScale(x, y, z);
}

REDGPU_F_DECLSPEC void redFRotateDeg(float degrees, float axisX, float axisY, float axisZ) {
  if (axisX == 0 && axisY == 0 && axisZ == 0) {
    return;
  }
  ofRotateDeg(degrees, axisX, axisY, axisZ);
}

REDGPU_F_DECLSPEC void redFTranslate(float x, float y, float z) {
  ofTranslate(x, y, z);
}

REDGPU_F_DECLSPEC void redFPopMatrix(void) {
  ofPopMatrix();
}

REDGPU_F_DECLSPEC void redFSetMatrixMode(RedFMatrixMode matrixMode) {
  ofSetMatrixMode((ofMatrixMode)matrixMode);
}

REDGPU_F_DECLSPEC void redFLoadMatrix(void * mat4) {
  ofLoadMatrix((const float *)mat4);
}

REDGPU_F_DECLSPEC void redFLoadViewMatrix(void * mat4) {
  glm::mat4 m = glm::make_mat4((float *)mat4);
  ofLoadViewMatrix(m);
}

REDGPU_F_DECLSPEC void redFPushView(void) {
  ofPushView();
}

REDGPU_F_DECLSPEC void redFPopView(void) {
  ofPopView();
}

REDGPU_F_DECLSPEC void redFPushStyle(void) {
  ofPushStyle();
}

REDGPU_F_DECLSPEC void redFBackground(int r, int g, int b, int a) {
  ofBackground(r, g, b, a);
}

REDGPU_F_DECLSPEC void redFFill(void) {
  ofFill();
}

REDGPU_F_DECLSPEC void redFNoFill(void) {
  ofNoFill();
}

REDGPU_F_DECLSPEC void redFSetColor(int r, int g, int b, int a) {
  ofSetColor(r, g, b, a);
}

REDGPU_F_DECLSPEC void redFSetLineWidth(float width) {
  ofSetLineWidth(width);
}

REDGPU_F_DECLSPEC void redFPopStyle(void) {
  ofPopStyle();
}

REDGPU_F_DECLSPEC void redFDrawTriangle(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2) {
  glm::vec3 p0;
  p0.x = x0;
  p0.y = y0;
  p0.z = z0;
  glm::vec3 p1;
  p1.x = x1;
  p1.y = y1;
  p1.z = z1;
  glm::vec3 p2;
  p2.x = x2;
  p2.y = y2;
  p2.z = z2;
  ofDrawTriangle(p0, p1, p2);
}

REDGPU_F_DECLSPEC void redFDrawSphere(float x, float y, float z, float radius) {
  ofDrawSphere(x, y, z, radius);
}

REDGPU_F_DECLSPEC void redFDrawBox(float x, float y, float z, float width, float height, float depth) {
  ofDrawBox(x, y, z, width, height, depth);
}

REDGPU_F_DECLSPEC void redFDrawPlane(float x, float y, float z, float width, float height) {
  ofDrawPlane(x, y, z, width, height);
}

REDGPU_F_DECLSPEC void redFDrawBoxPrimitive(float x, float y, float z, float qx, float qy, float qz, float qw, float width, float height, float depth, int resWidth, int resHeight, int resDepth, RedFBool32 wireframe, int r, int g, int b, int a) {
  ofBoxPrimitive box;
  box.set(width, height, depth);
  box.setResolution(resWidth, resHeight, resDepth);
  ofColor color;
  color.r = r;
  color.g = g;
  color.b = b;
  color.a = a;
  box.setSideColor(ofBoxPrimitive::SIDE_FRONT, color);
  box.setSideColor(ofBoxPrimitive::SIDE_RIGHT, color);
  box.setSideColor(ofBoxPrimitive::SIDE_LEFT, color);
  box.setSideColor(ofBoxPrimitive::SIDE_BACK, color);
  box.setSideColor(ofBoxPrimitive::SIDE_TOP, color);
  box.setSideColor(ofBoxPrimitive::SIDE_BOTTOM, color);
  glm::quat orientation;
  orientation.x = qx;
  orientation.y = qy;
  orientation.z = qz;
  orientation.w = qw;
  box.setOrientation(orientation);
  box.setPosition(x, y, z);
  if (wireframe == 1) {
    box.drawWireframe();
  } else {
    box.draw();
  }
}

REDGPU_F_DECLSPEC void redFDrawPlanePrimitive(float x, float y, float z, float qx, float qy, float qz, float qw, float width, float height, int resWidth, int resHeight, RedFBool32 wireframe) {
  ofPlanePrimitive plane;
  plane.set(width, height);
  plane.setResolution(resWidth, resHeight);
  glm::quat orientation;
  orientation.x = qx;
  orientation.y = qy;
  orientation.z = qz;
  orientation.w = qw;
  plane.setOrientation(orientation);
  plane.setPosition(x, y, z);
  if (wireframe == 1) {
    plane.drawWireframe();
  } else {
    plane.draw();
  }
}

REDGPU_F_DECLSPEC void redFGetBoxPrimitiveMesh(float width, float height, float depth, int resWidth, int resHeight, int resDepth, RedFHandleMesh replaceMesh) {
  ofBoxPrimitive box;
  box.set(width, height, depth);
  box.setResolution(resWidth, resHeight, resDepth);
  ofMesh mesh = box.getMesh();
  ofMesh * out = (ofMesh *)replaceMesh;
  out[0] = mesh;
}

REDGPU_F_DECLSPEC void redFGetPlanePrimitiveMesh(float width, float height, int resWidth, int resHeight, RedFHandleMesh replaceMesh) {
  ofPlanePrimitive plane;
  plane.set(width, height);
  plane.setResolution(resWidth, resHeight);
  ofMesh mesh = plane.getMesh();
  ofMesh * out = (ofMesh *)replaceMesh;
  out[0] = mesh;
}

REDGPU_F_DECLSPEC void redFDrawLine(float x0, float y0, float z0, float x1, float y1, float z1) {
  ofDrawLine(x0, y0, z0, x1, y1, z1);
}

REDGPU_F_DECLSPEC void redFDrawCircle(float x, float y, float z, float radius) {
  ofDrawCircle(x, y, z, radius);
}

REDGPU_F_DECLSPEC void redFDrawRectangle(float x, float y, float z, float w, float h) {
  ofDrawRectangle(x, y, z, w, h);
}

REDGPU_F_DECLSPEC void redFDrawRectRounded(float x, float y, float z, float w, float h, float topLeftRadius, float topRightRadius, float bottomRightRadius, float bottomLeftRadius) {
  ofDrawRectRounded(x, y, z, w, h, topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius);
}

REDGPU_F_DECLSPEC void redFDrawCurve(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
  ofDrawCurve(x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3);
}

REDGPU_F_DECLSPEC void redFDrawBezier(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
  ofDrawBezier(x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3);
}

REDGPU_F_DECLSPEC void redFDrawBitmapString(const char * string, float x, float y, float z) {
  std::string s = string;
  ofDrawBitmapString(s, x, y, z);
}

REDGPU_F_DECLSPEC void redFDrawBitmapStringHighlight(const char * string, float x, float y, float z, int bgR, int bgG, int bgB, int bgA, int fgR, int fgG, int fgB, int fgA) {
  std::string s = string;
  glm::vec3 p;
  p.x = x;
  p.y = y;
  p.z = z;
  ofColor bg;
  bg.r = bgR;
  bg.g = bgG;
  bg.b = bgB;
  bg.a = bgA;
  ofColor fg;
  fg.r = fgR;
  fg.g = fgG;
  fg.b = fgB;
  fg.a = fgA;
  ofDrawBitmapStringHighlight(s, p, bg, fg);
}

REDGPU_F_DECLSPEC void redFEnableAlphaBlending(void) {
  ofEnableAlphaBlending();
}

REDGPU_F_DECLSPEC void redFDisableAlphaBlending(void) {
  ofDisableAlphaBlending();
}

REDGPU_F_DECLSPEC void redFEnableAntiAliasing(void) {
  ofEnableAntiAliasing();
}

REDGPU_F_DECLSPEC void redFDisableAntiAliasing(void) {
  ofDisableAntiAliasing();
}

REDGPU_F_DECLSPEC void redFEnableBlendMode(RedFBlendMode blendMode) {
  ofEnableBlendMode((ofBlendMode)blendMode);
}

REDGPU_F_DECLSPEC void redFDisableBlendMode(void) {
  ofDisableBlendMode();
}

REDGPU_F_DECLSPEC void redFEnableDepthTest(void) {
  ofEnableDepthTest();
}

REDGPU_F_DECLSPEC void redFDisableDepthTest(void) {
  ofDisableDepthTest();
}

REDGPU_F_DECLSPEC void redFGetClipboardStringChars(char ** outString, uint64_t * outStringBytesCount) {
  std::string clipboard = ofGetClipboardString();
  {
    std::string s = clipboard;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outString[0] = p;
  }
  if (outStringBytesCount != NULL) {
    outStringBytesCount[0] = clipboard.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFSetClipboardString(const char * string) {
  std::string clipboard = string;
  ofSetClipboardString(clipboard);
}

REDGPU_F_DECLSPEC void redFShowCursor(void) {
  ofShowCursor();
}

REDGPU_F_DECLSPEC void redFHideCursor(void) {
  ofHideCursor();
}

#ifdef _WIN32
REDGPU_F_DECLSPEC unsigned long redFGetX11Window(void) {
  return 0;
}

REDGPU_F_DECLSPEC void * redFGetX11Display(void) {
  return NULL;
}
#endif
#ifdef __linux__
REDGPU_F_DECLSPEC unsigned long redFGetX11Window(void) {
  return (unsigned long)ofGetX11Window();
}

REDGPU_F_DECLSPEC void * redFGetX11Display(void) {
  return (void *)ofGetX11Display();
}
#endif

REDGPU_F_DECLSPEC void redFSetFrameRate(int targetRate) {
  ofSetFrameRate(targetRate);
}

REDGPU_F_DECLSPEC void redFSetVerticalSync(RedFBool32 sync) {
  ofSetVerticalSync(sync);
}

REDGPU_F_DECLSPEC void redFSetFullscreen(RedFBool32 fullscreen) {
  ofSetFullscreen(fullscreen);
}

REDGPU_F_DECLSPEC int redFGetWidth(void) {
  return ofGetWidth();
}

REDGPU_F_DECLSPEC int redFGetHeight(void) {
  return ofGetHeight();
}

REDGPU_F_DECLSPEC RedFBool32 redFIsVFlipped(void) {
  return (RedFBool32)ofIsVFlipped();
}

REDGPU_F_DECLSPEC float redFGetNativeViewportX(void) {
  return ofGetNativeViewport().x;
}

REDGPU_F_DECLSPEC float redFGetNativeViewportY(void) {
  return ofGetNativeViewport().y;
}

REDGPU_F_DECLSPEC float redFGetNativeViewportWidth(void) {
  return ofGetNativeViewport().width;
}

REDGPU_F_DECLSPEC float redFGetNativeViewportHeight(void) {
  return ofGetNativeViewport().height;
}

REDGPU_F_DECLSPEC void redFViewport(float x, float y, float width, float height, RedFBool32 vFlip) {
  ofViewport(x, y, width, height, vFlip);
}

REDGPU_F_DECLSPEC void redFSetClearDepthValue(float depth) {
  glClearDepth(depth);
}

REDGPU_F_DECLSPEC void redFSetDepthFunc(unsigned func) {
  glDepthFunc(func);
}

REDGPU_F_DECLSPEC void redFColorMask(RedFBool32 r, RedFBool32 g, RedFBool32 b, RedFBool32 a) {
  glColorMask(r, g, b, a);
}

REDGPU_F_DECLSPEC void redFClear(float r, float g, float b, float a) {
  ofClear(r, g, b, a);
}

REDGPU_F_DECLSPEC void redFClearAlpha(void) {
  ofClearAlpha();
}

REDGPU_F_DECLSPEC void redFEnableLighting(void) {
  ofEnableLighting();
}

REDGPU_F_DECLSPEC void redFDisableLighting(void) {
  ofDisableLighting();
}

REDGPU_F_DECLSPEC RedFBool32 redFGetLightingEnabled(void) {
  return (RedFBool32)ofGetLightingEnabled();
}

REDGPU_F_DECLSPEC void redFEnableSeparateSpecularLight(void) {
  ofEnableSeparateSpecularLight();
}

REDGPU_F_DECLSPEC void redFDisableSeparateSpecularLight(void) {
  ofDisableSeparateSpecularLight();
}

REDGPU_F_DECLSPEC void redFSetGlobalAmbientColor(float r, float g, float b, float a) {
  ofSetGlobalAmbientColor(ofFloatColor(r, g, b, a));
}

REDGPU_F_DECLSPEC void redFGetGlobalAmbientColor(void * outVec4) {
  ofFloatColor out = ofGetGlobalAmbientColor();
  float * outFloats = (float *)outVec4;
  outFloats[0] = out.r;
  outFloats[1] = out.g;
  outFloats[2] = out.b;
  outFloats[3] = out.a;
}

REDGPU_F_DECLSPEC void redFSetSmoothLighting(RedFBool32 enable) {
  ofSetSmoothLighting(enable);
}

REDGPU_F_DECLSPEC void redFEnableCulling(unsigned mode) {
  glEnable(GL_CULL_FACE);
  glCullFace(mode);
}

REDGPU_F_DECLSPEC void redFDisableCulling(void) {
  glDisable(GL_CULL_FACE);
}

REDGPU_F_DECLSPEC void redFSaveScreenToPng(const char * fileName) {
  std::string path = fileName;
  ofSaveScreen(path);
}

REDGPU_F_DECLSPEC void redFSaveScreenRawToMemory(int x, int y, int width, int height, void * out8BitRGBAPixels) {
  // NOTE(Constantine): Won't work for OpenGL ES.
  glPixelStorei(0x0D05, 1); // GL_PACK_ALIGNMENT
  glReadPixels(x, y, width, height, 0x1908, 0x1401, out8BitRGBAPixels);
}

REDGPU_F_DECLSPEC float redFGetElapsedTimef(void) {
  return ofGetElapsedTimef();
}

REDGPU_F_DECLSPEC uint64_t redFGetElapsedTimeMillis(void) {
  return ofGetElapsedTimeMillis();
}

REDGPU_F_DECLSPEC uint64_t redFGetElapsedTimeMicros(void) {
  return ofGetElapsedTimeMicros();
}

REDGPU_F_DECLSPEC void redFResetElapsedTimeCounter(void) {
  ofResetElapsedTimeCounter();
}

REDGPU_F_DECLSPEC double redFGetLastFrameTime(void) {
  return ofGetLastFrameTime();
}

REDGPU_F_DECLSPEC uint64_t redFGetSystemTimeMillis(void) {
  return ofGetSystemTimeMillis();
}

REDGPU_F_DECLSPEC uint64_t redFGetSystemTimeMicros(void) {
  return ofGetSystemTimeMicros();
}

REDGPU_F_DECLSPEC unsigned redFGetUnixTime(void) {
  return ofGetUnixTime();
}

REDGPU_F_DECLSPEC void redFDisableArbTex(void) {
  ofDisableArbTex();
}

REDGPU_F_DECLSPEC void redFEnableArbTex(void) {
  ofEnableArbTex();
}

REDGPU_F_DECLSPEC void redFClipControl(unsigned origin, unsigned depth) {
  glClipControl(origin, depth);
}

REDGPU_F_DECLSPEC void redFEnable(unsigned glenum) {
  glEnable(glenum);
}

REDGPU_F_DECLSPEC void redFDisable(unsigned glenum) {
  glDisable(glenum);
}

REDGPU_F_DECLSPEC void redFGetIntegerv(unsigned glenum, void * outValue) {
  glGetIntegerv(glenum, (int *)outValue);
}

REDGPU_F_DECLSPEC void redFGetEnv(const char * variable, char ** outValue, uint64_t * outValueBytesCount) {
  std::string name = variable;
  std::string value = ofGetEnv(name);
  {
    std::string s = value;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outValue[0] = p;
  }
  if (outValueBytesCount != NULL) {
    outValueBytesCount[0] = value.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFSystem(const char * command, char ** outOutput, uint64_t * outOutputBytesCount) {
  std::string cmd = command;
  std::string output = ofSystem(cmd);
  if (outOutput != NULL) {
    std::string s = output;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outOutput[0] = p;
  }
  if (outOutputBytesCount != NULL) {
    outOutputBytesCount[0] = output.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFSystemAlertDialog(const char * errorMessage) {
  std::string name = errorMessage;
  ofSystemAlertDialog(name);
}

REDGPU_F_DECLSPEC RedFBool32 redFSystemLoadDialog(const char * windowTitle, RedFBool32 bFolderSelection, const char * defaultPath, char ** outFilePath, uint64_t * outFilePathBytesCount, char ** outFileName, uint64_t * outFileNameBytesCount) {
  std::string title = windowTitle;
  std::string path  = defaultPath;
  ofFileDialogResult result = ofSystemLoadDialog(title, bFolderSelection, path);
  if (outFilePath != NULL) {
    std::string s = result.filePath;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outFilePath[0] = p;
  }
  if (outFilePathBytesCount != NULL) {
    outFilePathBytesCount[0] = result.filePath.size() + 1;
  }
  if (outFileName != NULL) {
    std::string s = result.fileName;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outFileName[0] = p;
  }
  if (outFileNameBytesCount != NULL) {
    outFileNameBytesCount[0] = result.fileName.size() + 1;
  }
  return (RedFBool32)result.bSuccess;
}

REDGPU_F_DECLSPEC RedFBool32 redFSystemSaveDialog(const char * defaultName, const char * messageName, char ** outFilePath, uint64_t * outFilePathBytesCount, char ** outFileName, uint64_t * outFileNameBytesCount) {
  std::string name    = defaultName;
  std::string message = messageName;
  ofFileDialogResult result = ofSystemSaveDialog(name, message);
  if (outFilePath != NULL) {
    std::string s = result.filePath;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outFilePath[0] = p;
  }
  if (outFilePathBytesCount != NULL) {
    outFilePathBytesCount[0] = result.filePath.size() + 1;
  }
  if (outFileName != NULL) {
    std::string s = result.fileName;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outFileName[0] = p;
  }
  if (outFileNameBytesCount != NULL) {
    outFileNameBytesCount[0] = result.fileName.size() + 1;
  }
  return (RedFBool32)result.bSuccess;
}

REDGPU_F_DECLSPEC void redFSystemTextBoxDialog(const char * question, const char * text, char ** outAnswer, uint64_t * outAnswerBytesCount) {
  std::string que  = question;
  std::string show = text;
  std::string answer = ofSystemTextBoxDialog(que, show);
  {
    std::string s = answer;
    char * p = (char *)malloc(s.size() + 1);
    if (p != NULL) {
      std::copy(s.begin(), s.end(), p);
      p[s.size()] = 0;
    }
    outAnswer[0] = p;
  }
  if (outAnswerBytesCount != NULL) {
    outAnswerBytesCount[0] = answer.size() + 1;
  }
}

REDGPU_F_DECLSPEC void redFEnableFramebufferSRGBGammaCorrection(void) {
  glEnable(GL_FRAMEBUFFER_SRGB);
}

REDGPU_F_DECLSPEC void * redFGetGLFWwindowPointer(void) {
 return (void *)(static_cast<ofAppGLFWWindow *>(ofGetWindowPtr())->getGLFWWindow());
}

REDGPU_F_DECLSPEC void redFFree(void * pointer) {
  free(pointer);
}

REDGPU_F_DECLSPEC void redFDrawPoint(float x, float y, float z, float radius, RedFHandleCamera camera) {
  float cpx = redFNodeGetGlobalPositionX(redFCameraCastToNode(camera));
  float cpy = redFNodeGetGlobalPositionY(redFCameraCastToNode(camera));
  float cpz = redFNodeGetGlobalPositionZ(redFCameraCastToNode(camera));
  redFDrawSphere(x, y, z, glm::distance(glm::vec3(cpx, cpy, cpz), glm::vec3(x, y, z)) * radius);
}

REDGPU_F_DECLSPEC void redFDrawBitmapStringHighlightFromCamera(const char * string, float x, float y, float z, int bgR, int bgG, int bgB, int bgA, int fgR, int fgG, int fgB, int fgA, RedFHandleCamera camera) {
  float cpx = redFNodeGetGlobalPositionX(redFCameraCastToNode(camera));
  float cpy = redFNodeGetGlobalPositionY(redFCameraCastToNode(camera));
  float cpz = redFNodeGetGlobalPositionZ(redFCameraCastToNode(camera));
  float cdx = redFNodeGetLookAtDirX(redFCameraCastToNode(camera));
  float cdy = redFNodeGetLookAtDirY(redFCameraCastToNode(camera));
  float cdz = redFNodeGetLookAtDirZ(redFCameraCastToNode(camera));
  float p[3] = {};
  redFCameraWorldToScreen(camera, x, y, z, p);
  float dot = (x-cpx) * cdx + (y-cpy) * cdy + (z-cpz) * cdz;
  if (dot > 0) {
    redFDrawBitmapStringHighlight(string, p[0], p[1], p[2], bgR, bgG, bgB, bgA, fgR, fgG, fgB, fgA);
  }
}

REDGPU_F_DECLSPEC RedFBool32 redFFbxLoad(RedFHandleFbx handle, const char * filepath) {
  ofxFBX * fbx = (ofxFBX *)handle;
  ofxFBXSource::Scene::Settings settings = ofxFBXSource::Scene::Settings();
  settings.importBones              = true;
  settings.importMaterials          = false;
  settings.importTextures           = false;
  settings.importShapes             = false;
  settings.importGobos              = false;
  settings.importAnimations         = true;
  settings.importNurbsCurves        = false;
  settings.useKeyFrames             = true;
  settings.printInfo                = false;
  settings.filePath                 = filepath;
  settings.unloadIfUnused           = false;
  settings.cacheMeshKeyframes       = false;
  settings.blendCachedMeshKeyframes = false;
  bool loaded = fbx->load(settings);
  if (loaded) {
    if (fbx->getCurrentAnimation().isPlaying()) {
      fbx->getCurrentAnimation().stop();
    }
  }
  return (RedFBool32)loaded;
}

REDGPU_F_DECLSPEC void redFFbxCopyToFbx(RedFHandleFbx handle, RedFHandleFbx to) {
  ofxFBX * x = (ofxFBX *)to;
  ofxFBX * y = (ofxFBX *)handle;
  x[0] = y[0];
}

REDGPU_F_DECLSPEC void redFFbxClear(RedFHandleFbx handle) {
  ofxFBX * fbx = (ofxFBX *)handle;
  fbx->clear();
}

REDGPU_F_DECLSPEC void redFFbxEarlyUpdate(RedFHandleFbx handle) {
  ofxFBX * fbx = (ofxFBX *)handle;
  fbx->earlyUpdate();
}

REDGPU_F_DECLSPEC void redFFbxLateUpdate(RedFHandleFbx handle) {
  ofxFBX * fbx = (ofxFBX *)handle;
  fbx->lateUpdate();
}

REDGPU_F_DECLSPEC int redFFbxGetMeshesCount(RedFHandleFbx handle) {
  ofxFBX * fbx = (ofxFBX *)handle;
  return fbx->getNumMeshes();
}

REDGPU_F_DECLSPEC RedFHandleMesh redFFbxGetMesh(RedFHandleFbx handle, int meshIndex) {
  ofxFBX * fbx = (ofxFBX *)handle;
  std::vector< std::shared_ptr<ofxFBXMesh> > & meshes = fbx->getMeshes();
  ofMesh & mesh = meshes[meshIndex]->getMesh();
  ofMesh * meshPointer = &mesh;
  return (RedFHandleMesh)(void *)meshPointer;
}

REDGPU_F_DECLSPEC void redFFbxGetMeshName(RedFHandleFbx handle, int meshIndex, uint64_t * outCharsCountIncludingNullTerminator, char * outChars) {
  ofxFBX * fbx = (ofxFBX *)handle;
  std::string name = fbx->getMeshName(meshIndex);
  if (outCharsCountIncludingNullTerminator != NULL) {
    outCharsCountIncludingNullTerminator[0] = name.size() + 1;
  }
  if (outChars != NULL) {
    std::copy(name.begin(), name.end(), outChars);
    outChars[name.size()] = 0;
  }
}

REDGPU_F_DECLSPEC int redFFbxGetRootBonesCount(RedFHandleFbx handle) {
  ofxFBX * fbx = (ofxFBX *)handle;
  return fbx->getNumSkeletons();
}

REDGPU_F_DECLSPEC RedFHandleFbxBone redFFbxGetRootBone(RedFHandleFbx handle, int rootBoneIndex) {
  ofxFBX * fbx = (ofxFBX *)handle;
  std::vector< std::shared_ptr<ofxFBXSkeleton> > & skeletons = fbx->getSkeletons();
  ofxFBXSkeleton * skeleton = skeletons[rootBoneIndex].get();
  ofxFBXBone * root = skeleton->root.get();
  return (RedFHandleFbxBone)(void *)root;
}

REDGPU_F_DECLSPEC RedFHandleNode redFFbxBoneCastToNode(RedFHandleFbxBone handle) {
  return (RedFHandleNode)(void *)handle;
}

REDGPU_F_DECLSPEC void redFFbxBoneGetName(RedFHandleFbxBone handle, uint64_t * outCharsCountIncludingNullTerminator, char * outChars) {
  ofxFBXBone * bone = (ofxFBXBone *)handle;
  std::string name = bone->getName();
  if (outCharsCountIncludingNullTerminator != NULL) {
    outCharsCountIncludingNullTerminator[0] = name.size() + 1;
  }
  if (outChars != NULL) {
    std::copy(name.begin(), name.end(), outChars);
    outChars[name.size()] = 0;
  }
}

REDGPU_F_DECLSPEC int redFFbxBoneGetChildBonesCount(RedFHandleFbxBone handle) {
  ofxFBXBone * bone = (ofxFBXBone *)handle;
  return bone->childBones.size();
}

REDGPU_F_DECLSPEC RedFHandleFbxBone redFFbxBoneGetChildBone(RedFHandleFbxBone handle, int childBoneIndex) {
  ofxFBXBone * bone = (ofxFBXBone *)handle;
  auto b = bone->childBones.begin();
  for (uint64_t i = 0; i < childBoneIndex; i += 1) {
    ++b;
  }
  ofxFBXBone * child = b->second.get();
  return (RedFHandleFbxBone)(void *)child;
}

REDGPU_F_DECLSPEC RedFHandleFbxBone redFFbxBoneGetChildBoneByNameRecursively(RedFHandleFbxBone handle, const char * boneName) {
  ofxFBXBone * bone = (ofxFBXBone *)handle;
  std::string name = boneName;
  ofxFBXBone * b = bone->getBone(name).get();
  return (RedFHandleFbxBone)(void *)b;
}

// New in REDGPU Framework 2

REDGPU_F_DECLSPEC int redF2Main(const RedFEvents * events, int windowWidth, int windowHeight, RedFWindowMode windowMode, RedFBool32 useCustomOpenGLVersion, int customOpenGLVersionMajor, int customOpenGLVersionMinor, RedFBool32 windowIsDecorated, RedFBool32 windowIsResizable, int msaaSamples, uint64_t optionalSettingsCount, const void * optionalSettings) {
  __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events = {};
  if (events != NULL) {
    __REDGPU_F_GLOBAL_87602fd8921e493298df8495aa4fe136e9c9a7c2_events = events[0];
  }
  return ofAppMain2(windowWidth, windowHeight, windowMode, useCustomOpenGLVersion, customOpenGLVersionMajor, customOpenGLVersionMinor, windowIsDecorated, windowIsResizable, msaaSamples, optionalSettingsCount, optionalSettings);
}

REDGPU_F_DECLSPEC void redF2SetLogLevel(RedF2LogLevel logLevel) {
  ofSetLogLevel((ofLogLevel)logLevel);
}

REDGPU_F_DECLSPEC RedF2LogLevel redF2GetLogLevel(void) {
  return (RedF2LogLevel)ofGetLogLevel();
}

REDGPU_F_DECLSPEC void redF2FrontFace(unsigned mode) {
  glFrontFace(mode);
}

REDGPU_F_DECLSPEC void redF2DepthMask(unsigned char flag) {
  glDepthMask(flag);
}

REDGPU_F_DECLSPEC void redF2Finish(void) {
  glFinish();
}

REDGPU_F_DECLSPEC RedFBool32 redF2IsProgrammableRenderer(void) {
  return ofIsGLProgrammableRenderer() == true ? 1 : 0;
}

REDGPU_F_DECLSPEC RedFBool32 redF2ShadowsAreSupported(void) {
  return 1;
}

REDGPU_F_DECLSPEC void redF2MeshLoadPly(const char * fileName, RedFHandleMesh replaceMesh) {
  ofMesh * mesh = (ofMesh *)replaceMesh;
  std::string name = fileName;
#ifdef __linux__
#warning
  std::string path = name;
#else
  std::filesystem::path path = name;
#endif
  mesh->load(path);
}

REDGPU_F_DECLSPEC void redF2MeshInvertNormals(RedFHandleMesh handle) {
  ofMesh * mesh = (ofMesh *)handle;
  for (size_t i = 0; i < mesh->getNumNormals(); i += 1) {
    mesh->getNormals()[i] *= -1.f;
  }
}

REDGPU_F_DECLSPEC void redF2LightDraw(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  light->draw();
}

REDGPU_F_DECLSPEC RedFHandleShadow redF2LightGetShadow(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  ofShadow & shadow = light->getShadow();
  ofShadow * shadowPointer = &shadow;
  return (RedFHandleShadow)(void *)shadowPointer;
}

REDGPU_F_DECLSPEC RedFBool32 redF2LightShouldRenderShadowDepthPass(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  return light->shouldRenderShadowDepthPass() == true ? 1 : 0;
}

REDGPU_F_DECLSPEC int redF2LightGetNumShadowDepthPasses(RedFHandleLight handle) {
  ofLight * light = (ofLight *)handle;
  return light->getNumShadowDepthPasses();
}

REDGPU_F_DECLSPEC void redF2LightBeginShadowDepthPass(RedFHandleLight handle, int aPassIndex) {
  ofLight * light = (ofLight *)handle;
  light->beginShadowDepthPass(aPassIndex);
}

REDGPU_F_DECLSPEC void redF2LightEndShadowDepthPass(RedFHandleLight handle, int aPassIndex) {
  ofLight * light = (ofLight *)handle;
  light->endShadowDepthPass(aPassIndex);
}

REDGPU_F_DECLSPEC void redF2ShadowsEnableAllShadows(void) {
  ofShadow::enableAllShadows();
}

REDGPU_F_DECLSPEC void redF2ShadowsDisableAllShadows(void) {
  ofShadow::disableAllShadows();
}

REDGPU_F_DECLSPEC void redF2ShadowsSetAllShadowBias(float bias) {
  ofShadow::setAllShadowBias(bias);
}

REDGPU_F_DECLSPEC void redF2ShadowsSetAllShadowNormalBias(float normalBias) {
  ofShadow::setAllShadowNormalBias(normalBias);
}

REDGPU_F_DECLSPEC void redF2ShadowsSetAllShadowDepthResolutions(int width, int height) {
  ofShadow::setAllShadowDepthResolutions(width, height);
}

REDGPU_F_DECLSPEC void redF2ShadowsSetAllShadowTypes(RedF2ShadowType type) {
  ofShadow::setAllShadowTypes((ofShadowType)type);
}

REDGPU_F_DECLSPEC void redF2ShadowsSetAllShadowSampleRadius(float sampleRadius) {
  ofShadow::setAllShadowSampleRadius(sampleRadius);
}

REDGPU_F_DECLSPEC void redF2ShadowSetBias(RedFHandleShadow handle, float bias) {
  ofShadow * shadow = (ofShadow *)handle;
  shadow->setBias(bias);
}

REDGPU_F_DECLSPEC void redF2ShadowSetNormalBias(RedFHandleShadow handle, float normalBias) {
  ofShadow * shadow = (ofShadow *)handle;
  shadow->setNormalBias(normalBias);
}
REDGPU_F_DECLSPEC void redF2ShadowSetAllShadowDepthResolutions(RedFHandleShadow handle, int width, int height) {
  ofShadow * shadow = (ofShadow *)handle;
  shadow->setAllShadowDepthResolutions(width, height);
}

REDGPU_F_DECLSPEC void redF2ShadowSetShadowType(RedFHandleShadow handle, RedF2ShadowType type) {
  ofShadow * shadow = (ofShadow *)handle;
  shadow->setShadowType((ofShadowType)type);
}

REDGPU_F_DECLSPEC void redF2ShadowSetSampleRadius(RedFHandleShadow handle, float sampleRadius) {
  ofShadow * shadow = (ofShadow *)handle;
  shadow->setSampleRadius(sampleRadius);
}

REDGPU_F_DECLSPEC void redF2ShadowSetEnabled(RedFHandleShadow handle, RedFBool32 enable) {
  ofShadow * shadow = (ofShadow *)handle;
  shadow->setEnabled(enable);
}

REDGPU_F_DECLSPEC RedFBool32 redF2ShadowIsEnabled(RedFHandleShadow handle) {
  ofShadow * shadow = (ofShadow *)handle;
  return shadow->getIsEnabled() == true ? 1 : 0;
}

REDGPU_F_DECLSPEC void redF2ShadowSetNearClip(RedFHandleShadow handle, float anear) {
  ofShadow * shadow = (ofShadow *)handle;
  shadow->setNearClip(anear);
}

REDGPU_F_DECLSPEC void redF2ShadowSetFarClip(RedFHandleShadow handle, float afar) {
  ofShadow * shadow = (ofShadow *)handle;
  shadow->setFarClip(afar);
}

REDGPU_F_DECLSPEC void redF2ShadowSetStrength(RedFHandleShadow handle, float astrength) {
  ofShadow * shadow = (ofShadow *)handle;
  shadow->setStrength(astrength);
}

REDGPU_F_DECLSPEC void redF2ShadowSetCullingEnabled(RedFHandleShadow handle, RedFBool32 enable) {
  ofShadow * shadow = (ofShadow *)handle;
  shadow->setGlCullingEnabled(enable);
}

REDGPU_F_DECLSPEC void redF2ShadowSetFrontFaceWindingOrder(RedFHandleShadow handle, unsigned mode) {
  ofShadow * shadow = (ofShadow *)handle;
  shadow->setFrontFaceWindingOrder(mode);
}

REDGPU_F_DECLSPEC void redF2ShadowDrawFrustum(RedFHandleShadow handle) {
  ofShadow * shadow = (ofShadow *)handle;
  shadow->drawFrustum();
}

REDGPU_F_DECLSPEC void redF2GetSpherePrimitiveMesh(float radius, int resolution, RedFHandleMesh replaceMesh) {
  ofSpherePrimitive sphere;
  sphere.set(radius, resolution, OF_PRIMITIVE_TRIANGLES);
  ofMesh mesh = sphere.getMesh();
  ofMesh * out = (ofMesh *)replaceMesh;
  out[0] = mesh;
}

REDGPU_F_DECLSPEC void redF2GetCylinderPrimitiveMesh(float radius, float height, int radiusSegments, int heightSegments, int capSegments, RedFBool32 isCapped, RedFHandleMesh replaceMesh) {
  ofMesh::cylinder(0.4, 1.0, 48, 12, 4, true, OF_PRIMITIVE_TRIANGLES );
  ofCylinderPrimitive cylinder;
  cylinder.set(radius, height, radiusSegments, heightSegments, capSegments, isCapped, OF_PRIMITIVE_TRIANGLES);
  ofMesh mesh = cylinder.getMesh();
  ofMesh * out = (ofMesh *)replaceMesh;
  out[0] = mesh;
}

REDGPU_F_DECLSPEC void redF2ManipulatorGetCurrentState(RedFHandleManipulator handle, RedF2ManipulatorStateScale * outCurrScale, RedF2ManipulatorStateScale * outCurrScalePredict, RedF2ManipulatorStateRotation * outCurrRotation, RedF2ManipulatorStateRotation * outCurrRotationPredict, RedF2ManipulatorStateTranslation * outCurrTranslation, RedF2ManipulatorStateTranslation * outCurrTranslationPredict) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  manipulator->getManipulatorState(
    (int *)outCurrScale,
    (int *)outCurrScalePredict,
    (int *)outCurrRotation,
    (int *)outCurrRotationPredict,
    (int *)outCurrTranslation,
    (int *)outCurrTranslationPredict
  );
}

REDGPU_F_DECLSPEC RedFBool32 redF2ManipulatorIsFreezed(RedFHandleManipulator handle) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  return (RedFBool32)manipulator->isFreezed();
}

REDGPU_F_DECLSPEC void redF2ManipulatorFreeze(RedFHandleManipulator handle) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  manipulator->freeze();
}

REDGPU_F_DECLSPEC void redF2ManipulatorUnfreeze(RedFHandleManipulator handle) {
  ofxManipulator * manipulator = (ofxManipulator *)handle;
  manipulator->unfreeze();
}

REDGPU_F_DECLSPEC void redF2PostProcessingInit(RedFHandlePostProcessing handle, int width, int height) {
  ofxPostProcessing * postproc = (ofxPostProcessing *)handle;
  postproc->init(width, height, false);
}

REDGPU_F_DECLSPEC void redF2PostProcessingSetFlip(RedFHandlePostProcessing handle, RedFBool32 flip) {
  ofxPostProcessing * postproc = (ofxPostProcessing *)handle;
  postproc->setFlip(flip == 1 ? true : false);
}

REDGPU_F_DECLSPEC RedFHandlePostProcessingPass redF2PostProcessingCreatePassSSAO(RedFHandlePostProcessing handle) {
  ofxPostProcessing * postproc = (ofxPostProcessing *)handle;
  itg::SSAOPass::Ptr pass = postproc->createPass<itg::SSAOPass>(NULL, NULL);
  itg::SSAOPass * passPtr = pass.get();
  return (RedFHandlePostProcessingPass)(void *)passPtr;
}

REDGPU_F_DECLSPEC RedFHandlePostProcessingPass redF2PostProcessingCreatePassSSAOWithCustomSSAOShader(RedFHandlePostProcessing handle, const char * customSSAOShader) {
  ofxPostProcessing * postproc = (ofxPostProcessing *)handle;
  itg::SSAOPass::Ptr pass = postproc->createPass<itg::SSAOPass>(NULL, customSSAOShader);
  itg::SSAOPass * passPtr = pass.get();
  return (RedFHandlePostProcessingPass)(void *)passPtr;
}

REDGPU_F_DECLSPEC RedFHandlePostProcessingPass redF2PostProcessingCreatePassGeneric(RedFHandlePostProcessing handle, const char * vertexShader, const char * fragmentShader) {
  ofxPostProcessing * postproc = (ofxPostProcessing *)handle;
  itg::GenericPass::Ptr pass = postproc->createPass<itg::GenericPass>(vertexShader, fragmentShader);
  itg::GenericPass * passPtr = pass.get();
  return (RedFHandlePostProcessingPass)(void *)passPtr;
}

REDGPU_F_DECLSPEC void redF2PostProcessingPassSSAOSetParameters(RedFHandlePostProcessingPass handle, float cameraNearDefaultIs1, float cameraFarDefaultIs1000, float fogNearDefaultIs1, float fogFarDefaultIs1000, RedFBool32 fogEnabledDefaultIs0, RedFBool32 onlyAODefaultIs0, float aoClampDefaultIs0dot5, float lumInfluenceDefaultIs0dot9) {
  itg::SSAOPass * pass = (itg::SSAOPass *)handle;
  pass->setCameraNear(cameraNearDefaultIs1);
  pass->setCameraFar(cameraFarDefaultIs1000);
  pass->setFogNear(fogNearDefaultIs1);
  pass->setFogFar(fogFarDefaultIs1000);
  pass->setFogEnabled(fogEnabledDefaultIs0 == 1 ? true : false);
  pass->setOnlyAO(onlyAODefaultIs0 == 1 ? true : false);
  pass->setAoClamp(aoClampDefaultIs0dot5);
  pass->setLumInfluence(lumInfluenceDefaultIs0dot9);
}

REDGPU_F_DECLSPEC void redF2PostProcessingPassGenericSetParameters(RedFHandlePostProcessingPass handle, float parameter0, float parameter1, float parameter2, float parameter3, float parameter4, float parameter5, float parameter6, float parameter7, float parameter8, float parameter9, float parameter10, float parameter11, float parameter12, float parameter13, float parameter14, float parameter15) {
  itg::GenericPass * pass = (itg::GenericPass *)handle;
  pass->setParameter0(parameter0);
  pass->setParameter1(parameter1);
  pass->setParameter2(parameter2);
  pass->setParameter3(parameter3);
  pass->setParameter4(parameter4);
  pass->setParameter5(parameter5);
  pass->setParameter6(parameter6);
  pass->setParameter7(parameter7);
  pass->setParameter8(parameter8);
  pass->setParameter9(parameter9);
  pass->setParameter10(parameter10);
  pass->setParameter11(parameter11);
  pass->setParameter12(parameter12);
  pass->setParameter13(parameter13);
  pass->setParameter14(parameter14);
  pass->setParameter15(parameter15);
}

REDGPU_F_DECLSPEC void redF2PostProcessingBegin(RedFHandlePostProcessing handle, RedFHandleCamera camera) {
  ofxPostProcessing * postproc = (ofxPostProcessing *)handle;
  ofCamera *          cam      = (ofCamera *)camera;
  postproc->begin(cam[0]);
}

REDGPU_F_DECLSPEC void redF2PostProcessingEnd(RedFHandlePostProcessing handle, RedFHandleCamera camera) {
  ofxPostProcessing * postproc = (ofxPostProcessing *)handle;
  ofCamera *          cam      = (ofCamera *)camera;
  postproc->end(cam[0]);
}
