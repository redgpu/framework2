/*
 *  Generic.cpp
 *
 *  Copyright (c) 2013, satcy, http://satcy.net, 2023, Constantine Tarasenkov
 *  All rights reserved. 
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met: 
 *  
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer. 
 *  * Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *  * Neither the name of Neil Mendoza nor the names of its contributors may be used 
 *    to endorse or promote products derived from this software without 
 *    specific prior written permission. 
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 *  POSSIBILITY OF SUCH DAMAGE. 
 *
 */
#include "GenericPass.h"
#include "ofMain.h"

namespace itg
{
    GenericPass::GenericPass(const ofVec2f& aspect, bool arb, const char * customVertexShader, const char * customFragmentShader) :
        RenderPass(aspect, arb, "GenericPass")
    {
        string vertShaderSrc = "";
        string fragShaderSrc = "";
        
        if (customVertexShader != NULL) {
          vertShaderSrc = customVertexShader;
        }
        if (customFragmentShader != NULL) {
          fragShaderSrc = customFragmentShader;
        }
    
        if (customVertexShader != NULL) { shader.setupShaderFromSource(GL_VERTEX_SHADER, vertShaderSrc); }
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
        shader.linkProgram();
    }

    void GenericPass::render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depthTex)
    {
        writeFbo.begin();
        shader.begin();
        
        shader.setUniformTexture("tColor", readFbo.getTexture(), 0);
        shader.setUniformTexture("tDepth", depthTex, 1);
        shader.setUniform2f("colorDimensions", readFbo.getWidth(), readFbo.getHeight());
        shader.setUniform2f("writeDimensions", writeFbo.getWidth(), writeFbo.getHeight());
        shader.setUniform1f("time", ofGetElapsedTimef());
        shader.setUniform1f("parameter0", parameter0);
        shader.setUniform1f("parameter1", parameter1);
        shader.setUniform1f("parameter2", parameter2);
        shader.setUniform1f("parameter3", parameter3);
        shader.setUniform1f("parameter4", parameter4);
        shader.setUniform1f("parameter5", parameter5);
        shader.setUniform1f("parameter6", parameter6);
        shader.setUniform1f("parameter7", parameter7);
        shader.setUniform1f("parameter8", parameter8);
        shader.setUniform1f("parameter9", parameter9);
        shader.setUniform1f("parameter10", parameter10);
        shader.setUniform1f("parameter11", parameter11);
        shader.setUniform1f("parameter12", parameter12);
        shader.setUniform1f("parameter13", parameter13);
        shader.setUniform1f("parameter14", parameter14);
        shader.setUniform1f("parameter15", parameter15);
        
        texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
        
        shader.end();
        writeFbo.end();
    }
}