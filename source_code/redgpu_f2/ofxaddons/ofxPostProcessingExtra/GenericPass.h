/*
 *  Generic.h
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
#pragma once

#include "../ofxPostProcessing/src/RenderPass.h"
#include "ofShader.h"

namespace itg
{
    class GenericPass : public RenderPass
    {
    public:
        
        typedef shared_ptr<GenericPass> Ptr;
        
        GenericPass(const ofVec2f& aspect, bool arb, const char * customVertexShader, const char * customFragmentShader);
        
        void render(ofFbo& readFbo, ofFbo& writeFbo, ofTexture& depth);
        
        void setParameter0(float v){ parameter0 = v; }
        void setParameter1(float v){ parameter1 = v; }
        void setParameter2(float v){ parameter2 = v; }
        void setParameter3(float v){ parameter3 = v; }
        void setParameter4(float v){ parameter4 = v; }
        void setParameter5(float v){ parameter5 = v; }
        void setParameter6(float v){ parameter6 = v; }
        void setParameter7(float v){ parameter7 = v; }
        void setParameter8(float v){ parameter8 = v; }
        void setParameter9(float v){ parameter9 = v; }
        void setParameter10(float v){ parameter10 = v; }
        void setParameter11(float v){ parameter11 = v; }
        void setParameter12(float v){ parameter12 = v; }
        void setParameter13(float v){ parameter13 = v; }
        void setParameter14(float v){ parameter14 = v; }
        void setParameter15(float v){ parameter15 = v; }
    private:
        
        ofShader shader;
        
        float parameter0 = 0;
        float parameter1 = 0;
        float parameter2 = 0;
        float parameter3 = 0;
        float parameter4 = 0;
        float parameter5 = 0;
        float parameter6 = 0;
        float parameter7 = 0;
        float parameter8 = 0;
        float parameter9 = 0;
        float parameter10 = 0;
        float parameter11 = 0;
        float parameter12 = 0;
        float parameter13 = 0;
        float parameter14 = 0;
        float parameter15 = 0;
    };
}