//------------------------------------------------------------------------------
//  simple.fx
//  (C) 2012 Gustav Sterbrant
//------------------------------------------------------------------------------

#include "lib/std.fxh"
#include "lib/shared.fxh"
#include "lib/util.fxh"
#include "lib/techniques.fxh"

group(SYSTEM_GROUP) push varblock Simple
{
	mat4 ShapeModel;
	vec4 MatDiffuse;
};

state WireframeState
{
	CullMode = None;	
	BlendEnabled[0] = true;	
	SrcBlend[0] = SrcAlpha;
	DstBlend[0] = OneMinusSrcAlpha;
	FillMode = Line;
	//MultisampleEnabled = true;
};

state DepthEnabledState
{
	CullMode = None;	
	BlendEnabled[0] = true;
	SrcBlend[0] = SrcAlpha;	
	DstBlend[0] = OneMinusSrcAlpha;
	DepthEnabled = true;
	DepthWrite = true;
	MultisampleEnabled = true;
};

state DepthDisabledState
{
	CullMode = None;	
	BlendEnabled[0] = true;
	SrcBlend[0] = SrcAlpha;
	DstBlend[0] = OneMinusSrcAlpha;
	DepthEnabled = false;
	DepthWrite = false;
	MultisampleEnabled = true;
};

//------------------------------------------------------------------------------
/**
*/
shader
void
vsMainPrimitives(
	[slot=0] in vec4 position, 
	[slot=5] in vec4 color, 
	out vec4 Color)  
{
	gl_Position = ViewProjection * Simple.ShapeModel * position;
	Color = color;
}
	
//------------------------------------------------------------------------------
/**
*/
shader
void
psMainPrimitives(in vec4 color, [color0] out vec4 Color) 
{
	Color = color;
}

//------------------------------------------------------------------------------
/**
*/
shader
void
vsMainShape(
	[slot=0] in vec4 position)  
{
	gl_Position = ViewProjection * Simple.ShapeModel * position;
}
	
//------------------------------------------------------------------------------
/**
*/
shader
void
psMainShape([color0] out vec4 Color) 
{
	Color = Simple.MatDiffuse;
}

//------------------------------------------------------------------------------
/**
*/
SimpleTechnique(DefaultPrim, "Colored", vsMainPrimitives(), psMainPrimitives(), DepthDisabledState);
SimpleTechnique(DepthPrim, "Colored|Alt0", vsMainPrimitives(), psMainPrimitives(), DepthEnabledState);
SimpleTechnique(WireframePrim, "Colored|Alt1", vsMainPrimitives(), psMainPrimitives(), WireframeState);

SimpleTechnique(DefaultShape, "Static", vsMainShape(), psMainShape(), DepthDisabledState);
SimpleTechnique(DepthShape, "Static|Alt0", vsMainShape(), psMainShape(), DepthEnabledState);
SimpleTechnique(WireframeShape, "Static|Alt1", vsMainShape(), psMainShape(), WireframeState);