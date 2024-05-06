## REDGPU Framework 2

```c
// REDGPU Framework 2 "Hello, World!" Program

// Compile command for Windows:
// cl main.c /link framework2/redgpu_f2dll.lib

#include "framework2/redgpu_f2.h"

RedFHandleFirstPersonCamera * gCamera;

void setup(void) {
  gCamera = redFCreateFirstPersonCamera(1);
  redFFirstPersonCameraControlEnable(gCamera[0]);
}

void draw(void) {
  redFFirstPersonCameraBegin(gCamera[0]);
  redFNoFill();
  redFSetColor(255, 0, 0, 255);
  redFDrawSphere(0, 0, 0, 25.f);
  redFFirstPersonCameraEnd(gCamera[0]);
}

int main() {
  RedFEvents events = {0};
  events.setup = setup;
  events.draw  = draw;
  redFMain(&events, 600, 600, REDF_WINDOW_MODE_WINDOW, 0, 1, 0, 1, 0, 0);
}
```

```c
// REDGPU Framework 2 "Hello, SSAO!" Program

// Compile command for Windows:
// cl main.c /link framework2/redgpu_f2dll.lib

#include "framework2/redgpu_f2.h"

RedFHandleFirstPersonCamera * gCamera;
RedFHandlePostProcessing    * gPostProc;

void setup(void) {
  gCamera = redFCreateFirstPersonCamera(1);
  redFFirstPersonCameraControlEnable(gCamera[0]);

  redFCameraSetNearClip(redFFirstPersonCameraCastToCamera(gCamera[0]), 1);
  redFCameraSetFarClip(redFFirstPersonCameraCastToCamera(gCamera[0]), 1000);
  RedFHandleNode cameraNode = redFFirstPersonCameraCastToNode(gCamera[0]);
  redFNodeSetGlobalPosition(cameraNode, 0, 5, 125);

  gPostProc = redFCreatePostProcessing(1);
  redF2PostProcessingInit(gPostProc[0], redFGetWidth(), redFGetHeight());
  redF2PostProcessingSetFlip(gPostProc[0], 1);
  RedFHandlePostProcessingPass pass = redF2PostProcessingCreatePassSSAO(gPostProc[0]);
  redF2PostProcessingPassSSAOSetParameters(pass,
    1,    // cameraNearDefaultIs1
    1000, // cameraFarDefaultIs1000
    1,    // fogNearDefaultIs1
    1000, // fogFarDefaultIs1000
    0,    // fogEnabledDefaultIs0
    0,    // onlyAODefaultIs0
    0.5,  // aoClampDefaultIs0dot5
    0.5   // lumInfluenceDefaultIs0dot9
  );
}

void draw(void) {
  redFEnableDepthTest();

  redF2PostProcessingBegin(gPostProc[0], redFFirstPersonCameraCastToCamera(gCamera[0]));

  redFSetColor(255, 0, 0, 255);
  redFDrawSphere(0, 0, 0, 25.f);

  redFSetColor(255, 0, 0, 255);
  redFPushMatrix();
  redFTranslate(0, -10, 25);
  redFDrawSphere(0, 0, 0, 15.f);
  redFPopMatrix();

  redFSetColor(255, 255, 255, 255);
  redFPushMatrix();
  redFRotateDeg(90, 1, 0, 0);
  redFTranslate(0, 0, 25);
  redFDrawPlane(0, 0, 0, 500, 500);
  redFPopMatrix();

  redF2PostProcessingEnd(gPostProc[0], redFFirstPersonCameraCastToCamera(gCamera[0]));
}

int main() {
  RedFEvents events = {0};
  events.setup = setup;
  events.draw  = draw;
  redFMain(&events, 600, 600, REDF_WINDOW_MODE_WINDOW, 0, 1, 0, 1, 0, 0);
}
```

```c
// REDGPU Framework 2 "Hello, Shadows!" Program

// Compile command for Windows:
// cl main.c /link framework2/redgpu_f2dll.lib

#ifdef _WIN32
#pragma comment(lib, "framework2/glfw3dll.lib")
#endif

#include "framework2/redgpu_f2.h"
#include "framework2/glfw3.h"
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "framework2/cimgui.h"
#include <math.h>

const RedFBool32              doDrawFrustums = 0;
const int                     lightsCount    = 2;
RedFHandleImgui *             imgui;
RedFHandleLight *             lights;
RedFHandleFirstPersonCamera * camera;
RedFHandleMesh *              sphereMesh;
RedFHandleMesh *              boxMesh;
RedFHandleMesh *              cylinderMesh;
RedFHandleMesh *              logoMesh;
RedFHandleMaterial *          logoMaterial;
RedFHandleMaterial *          boxMaterial;
RedFHandleMaterial *          bgMaterial;
RedFHandleMaterial *          sphereMaterial;

void setup(void) {
  glfwSwapInterval(1);
  redFSetVerticalSync(0);
  
  redF2SetLogLevel(REDF2_LOG_LEVEL_VERBOSE);

  imgui = redFCreateImgui(1);
  redFImguiSetup(imgui[0]);

  sphereMesh     = redFCreateMesh(1);
  boxMesh        = redFCreateMesh(1);
  cylinderMesh   = redFCreateMesh(1);
  logoMesh       = redFCreateMesh(1);
  logoMaterial   = redFCreateMaterial(1);
  boxMaterial    = redFCreateMaterial(1);
  bgMaterial     = redFCreateMaterial(1);
  sphereMaterial = redFCreateMaterial(1);

  lights = redFCreateLight(lightsCount);
  for (int i = 0; i < lightsCount; i += 1) {
    redFLightSetup(lights[i]);
    redFLightEnable(lights[i]);
    if (i == 0) {
      redFLightSetPointLight(lights[i]);
    } else {
      // Set the light to be a spot light with a cutoff (cone/fov) of 30 degrees and a concentration (softness) of 50 out of a range 0 - 128
      redFLightSetSpotlight(lights[i], 60 /* spotCutOffDefaultIs45 */, 20 /* exponentDefaultIs0 */);
      redF2ShadowSetNearClip(redF2LightGetShadow(lights[i]), 200);
      redF2ShadowSetFarClip(redF2LightGetShadow(lights[i]), 2000);
      redFNodeSetGlobalPosition(redFLightCastToNode(lights[i]), 210, 330, 750);
      redFLightSetAmbientColor(lights[i], 0.4, 0.4, 0.4, 1.0);
    }
    // Set the strength of the shadow, how visible it is.
    // Shadow strength default is 0.5.
    redF2ShadowSetStrength(redF2LightGetShadow(lights[i]), 0.6);

    if (redFLightGetType(lights[i]) != REDF_LIGHT_TYPE_POINT) {
      redFNodeSetGlobalOrientationQuaternion(redFLightCastToNode(lights[i]), -0.254887, 0.167731, 0.044943, 0.951251);
    }
    
    if (redF2IsProgrammableRenderer() == 0) {
      redFLightEnable(lights[i]);
    }
  }
  // Shadows are disabled by default.
  // Call this function to enable all of them that are attached to lights
  redF2ShadowsEnableAllShadows();
  // Shadow bias is the margin of error the shadow depth.
  // Increasing the bias helps reduce shadow acne, but can cause light leaking.
  // Try to find a good balance that fits your needs.
  // Bias default is 0.005.
  redF2ShadowsSetAllShadowBias(0.007);
  // Moves the bias along the normal of the mesh, helps reduce shadow acne.
  // Normal bias default is 0.
  redF2ShadowsSetAllShadowNormalBias(-4.0);
  redF2ShadowsSetAllShadowDepthResolutions(2048, 2048);
  redF2ShadowsSetAllShadowTypes(REDF2_SHADOW_TYPE_PCF_HIGH);
  redF2ShadowsSetAllShadowSampleRadius(4);
  redF2GetSpherePrimitiveMesh(1.0, 48, sphereMesh[0]);
  redFGetBoxPrimitiveMesh(1, 1, 1, 24, 24, 24, boxMesh[0]);
  redF2GetCylinderPrimitiveMesh(0.4, 1.0, 48, 12, 4, 1, cylinderMesh[0]);

  // Shadows and lights work with materials.
  // Create some different materials so the lighting and shadows can be applied.
  redFMaterialSetDiffuseColor(boxMaterial[0], 0.25, 0.25, 0.25, 1.0);
  redFMaterialSetShininess(boxMaterial[0], 60);
  redFMaterialSetSpecularColor(boxMaterial[0], 1.0, 1.0, 1.0, 1.0);
  
  redFMaterialSetDiffuseColor(bgMaterial[0], 0.15, 0.15, 0.15, 1.0);
  redFMaterialSetShininess(bgMaterial[0], 0.0);
  redFMaterialSetSpecularColor(bgMaterial[0], 0.25, 0.25, 0.25, 1.0);
  
  redFMaterialSetAmbientColor(sphereMaterial[0], 0.25, 0.25, 0.25, 1.0);
  redFMaterialSetDiffuseColor(sphereMaterial[0], 0.85, 0.85, 0.85, 1.0);
  redFMaterialSetSpecularColor(sphereMaterial[0], 1.0, 1.0, 1.0, 1.0);
  redFMaterialSetShininess(sphereMaterial[0], 100);
  
  redF2MeshLoadPly("logo.ply", logoMesh[0]);
  redFMeshMergeDuplicateVertices(logoMesh[0]);
  redF2MeshInvertNormals(logoMesh[0]);
  redFMaterialSetAmbientColor(logoMaterial[0], 0.85 * 0.8, 0.16 * 0.8, 0.43 * 0.8, 1.0);
  redFMaterialSetDiffuseColor(logoMaterial[0], 0.85, 0.16, 0.43, 1.0);
  redFMaterialSetSpecularColor(logoMaterial[0], 1.0, 1.0, 1.0, 1.0);
  redFMaterialSetShininess(logoMaterial[0], 50);

  camera = redFCreateFirstPersonCamera(1);
  redFFirstPersonCameraControlEnable(camera[0]);
}

void update(void) {
  glfwPollEvents();
  
  if (redFGetMousePressed(2) == 1) {
    redFFirstPersonCameraControlToggle(camera[0]);
  }
  
  float etimef = redFGetElapsedTimef();

  if (lightsCount > 0) {
    if (redFLightGetType(lights[0]) == REDF_LIGHT_TYPE_POINT) {
      float tangle = etimef * 1.05;
      redFNodeSetGlobalPosition(redFLightCastToNode(lights[0]), -120, sinf(tangle) * 180.0 + 150, cosf(tangle) * 100.0);
    }
  }
}

void RenderScene(void) {
  float etimef = redFGetElapsedTimef();
  redFSetColor(200, 50, 120, 255);
  redFMaterialBegin(sphereMaterial[0]);
  redFPushMatrix();
  redFTranslate(-120 + cosf(etimef * 1.4) * 20.0, 0, sinf(etimef * 1.7) * 250);
  redFScale(30, 30, 30);
  redFMeshDraw(sphereMesh[0]);
  redFMaterialEnd(sphereMaterial[0]);
  redFSetColor(255, 255, 255, 255);
  redFPopMatrix();
  
  redFMaterialBegin(boxMaterial[0]);
  
  redFPushMatrix();
  {
    redFTranslate(250, cosf(etimef * 0.6) * 50 - 80, 200 );
    redFRotateDeg((etimef * 0.04) * 360, 0, 0, 1);
    redFRotateDeg((etimef * 0.06) * 360, 1, 0, 0);
    redFScale(100, 100, 100.0);
    redFMeshDraw(boxMesh[0]);
  }
  redFPopMatrix();
  
  redFPushMatrix();
  {
    redFTranslate(-250, cosf(etimef * 0.4) * 90 - 30, 200 );
    redFRotateDeg((etimef * 0.034) * 360, 0, 0, 1);
    redFRotateDeg((etimef * 0.067) * 360, 1, 0, 0);
    redFScale(100, 100, 100.0);
    redFMeshDraw(boxMesh[0]);
  }
  redFPopMatrix();
  
  redFMaterialEnd(boxMaterial[0]);
  
  redFSetColor(200, 200, 200, 255);
  
  redFMaterialBegin(bgMaterial[0]);
  
  redFPushMatrix();
  {
    redFTranslate(0, -250, 0);
    redFScale(1000, 50, 1000);
    redFMeshDraw(boxMesh[0]);
  }
  redFPopMatrix();
  
  redFPushMatrix();
  {
    redFTranslate(500, 0, 0);
    redFScale(50, 1000, 1000);
    redFMeshDraw(boxMesh[0]);
  }
  redFPopMatrix();
  
  redFPushMatrix();
  {
    redFTranslate(-500, 0, 0);
    redFScale(50, 1000, 1000);
    redFMeshDraw(boxMesh[0]);
  }
  redFPopMatrix();
  
  redFPushMatrix();
  {
    redFTranslate(0, 0, -500);
    redFScale(1000, 1000, 50);
    redFMeshDraw(boxMesh[0]);
  }
  redFPopMatrix();
  
  redFPushMatrix();
  {
    redFTranslate(0, 500, 0);
    redFScale(1000, 50, 1000);
    redFMeshDraw(boxMesh[0]);
  }
  redFPopMatrix();
  
  redFMaterialEnd(bgMaterial[0]);
  
  redFMaterialBegin(logoMaterial[0]);
  redFPushMatrix();
  redFTranslate(-50, -170, 50);
  redFRotateDeg(-90, 1, 0, 0);
  redFScale(60, 60, 60);
  redFMeshDraw(logoMesh[0]);
  redFPopMatrix();
  redFMaterialEnd(logoMaterial[0]);
}

void draw(void) {
  redFEnableDepthTest();
  
  for (int i = 0; i < lightsCount; i += 1) {
    RedFHandleLight light = lights[i];
    // Query the light to see if it has a depth pass.
    if (redF2LightShouldRenderShadowDepthPass(light) == 1) {
      // Get the number of passes required.
      // By default the number of passes is 1.
      // It will be more than one pass if it is a platform that does not support geometry shaders.
      // Most likely it will be a single pass, but we get the number of passes to be safe.
      int numShadowPasses = redF2LightGetNumShadowDepthPasses(light);
      for (int j = 0; j < numShadowPasses; j += 1) {
        redF2LightBeginShadowDepthPass(light, j);
        // Shadows have the following OpenGL culling enabled by default.
        // This helps reduce z fighting by only rendering the rear facing triangles to the depth map.
        // Enables face culling:
        //   glEnable(GL_CULL_FACE);
        // Sets the OpenGL triangle winding order, default for RedFHandleShadow is GL_CW:
        //   glFrontFace(OpenGLFrontFaceWindingOrder);
        // Tells OpenGL to cull front faces:
        //   glCullFace(GL_FRONT);
        // The culling can be disabled by calling:
        //   redF2ShadowSetCullingEnabled(redF2LightGetShadow(light), 0);
        // or the culling winding order can be changed by calling:
        //   redF2ShadowSetFrontFaceWindingOrder(redF2LightGetShadow(light), GL_CCW); // Default is GL_CW
        if (redF2IsProgrammableRenderer() == 1 && redF2ShadowsAreSupported() == 1) {
          RenderScene();
        }
        redF2LightEndShadowDepthPass(light, j);
      }
    }
  }
  
  redFFirstPersonCameraBegin(camera[0]);
  
  if (redF2IsProgrammableRenderer() == 0) {
    redFEnableLighting();
  }
  
  // Cull the back faces of the geometry for rendering.
  redF2FrontFace(2304); // GL_CW
  redFEnableCulling(1029); // GL_BACK
  RenderScene();
  redFDisableCulling();
  
  if (redF2IsProgrammableRenderer() == 0) {
    redFDisableLighting();
  }

  for (int i = 0; i < lightsCount; i += 1) {
    RedFHandleLight light = lights[i];
    
    redFSetColor(255, 255, 255, 255);
    if (redFLightGetType(light) == REDF_LIGHT_TYPE_POINT) {
      float x = redFNodeGetGlobalPositionX(redFLightCastToNode(light));
      float y = redFNodeGetGlobalPositionY(redFLightCastToNode(light));
      float z = redFNodeGetGlobalPositionZ(redFLightCastToNode(light));
      redFDrawSphere(x, y, z, 12);
    } else {
      redF2LightDraw(light);
    }
    if (redFLightGetIsEnabled(light) == 1 &&
        redF2ShadowIsEnabled(redF2LightGetShadow(light)) == 1 &&
        doDrawFrustums == 1)
    {
      redF2ShadowDrawFrustum(redF2LightGetShadow(light));
    }
  }

  redFFirstPersonCameraEnd(camera[0]);
  
  redFDisableDepthTest();
  
  redFSetColor(255, 255, 255, 255);
  redFEnableAlphaBlending();
  
  redFImguiBegin(imgui[0]);
  igShowDemoWindow(0);
  redFImguiEnd(imgui[0]);
}

int main() {
  RedFEvents events = {0};
  events.setup  = setup;
  events.update = update;
  events.draw   = draw;
  redF2Main(&events, 600, 600, REDF_WINDOW_MODE_WINDOW, 1, 4, 1, 1, 0, 1, 0, 0);
}
```

## Tips:

* Prefer to call `redFDisableArbTex` at setup event to enable normalized texture coordinates and disable the default use of rectangle textures that don't support different texture wrap modes.
* Prefer to call `redFEnableFramebufferSRGBGammaCorrection` at setup event to enable gamma-correct rendering. Images loaded from disk will likely be in the wrong gamma, so convert their 0 to 255 8-bit RGB pixel values to 0.0 to 1.0 float values, apply `r *= r; g *= g; b *= b;` and set the modified float values back to image pixels as 0 to 255 8-bit values.
* REDGPU Framework doesn't call `glfwPollEvents` or `glfwWaitEvents` anywhere internally, so prefer to call `glfwPollEvents` each frame at the beginning of update event to get correct input events.
* Many draw procedures are affected by the currently set `redFSetColor` color.
* You may need to call `glfwSwapInterval(1); redFSetVerticalSync(0);` at setup event for G-Sync monitors.
* Compile command for Linux (tested on Ubuntu 16.04.6 with Clang 8, Elementary OS 7.1): clang++ main.cpp -std=c++17 -Wl,--start-group libredgpu_f2.so libs_linux/libopenFrameworks.a -lm -lGL -lX11 -lglfw libs_linux/libfbxsdk.a -lxml2 -licuuc -lz libs_linux/libtess2.a -lGLEW -lpugixml -lcurl libs_linux/libassimp.so -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0 -lgstapp-1.0 -lgstbase-1.0 -lgstvideo-1.0 -luriparser -lgtk-3 -lfontconfig -lfreetype -lopenal -lsndfile libs_linux/libkiss.a -lfreeimage -lboost_system -lboost_filesystem -lmpg123 -ldl -lpthread -Wl,--end-group
