#version 330
#include "foundation/foundation.h"
#include "foundation/vert_header.h"

// VERTEX
#include "model2d.vert"
#include "model3d.vert"
#include "text.vert"
#include "cubemap.vert"
#include "shadow.vert"
#include "particle.vert"

#include "foundation/invoke.h"

void main(void)
{
    init_layouts();

    GM_InvokeTechniqueEntrance();
}