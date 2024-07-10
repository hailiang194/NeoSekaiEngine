/**
 * @file RenderParams.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Base class of rendering paramters
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_RENDER_PARAMS_RENDER_PARAMS_H_
#define _SEKAI_ENGINE_RENDER_PARAMS_RENDER_PARAMS_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Render/RenderProperties.h"

namespace SekaiEngine
{
    namespace Render
    {
        class EXTENDAPI RenderParams
        {
        public:
            RenderParams(const RenderProperties& props);
            RenderParams(const RenderParams& params) = delete;
            RenderParams& operator=(const RenderParams& params) = delete;
            virtual ~RenderParams();
        protected:
            const RenderProperties& m_props; 
        };
    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_PARAMS_RENDER_PARAMS_H_