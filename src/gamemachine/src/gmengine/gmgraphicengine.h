﻿#ifndef __GMGRAPHICENGINE_H__
#define __GMGRAPHICENGINE_H__
#include <gmcommon.h>
#include <gmtools.h>
#include <gmmodel.h>
#include <gmcamera.h>
BEGIN_NS

struct GMShaderVariablesTextureDesc
{
	const char* OffsetX;
	const char* OffsetY;
	const char* ScaleX;
	const char* ScaleY;
	const char* Enabled;
	const char* Texture;
};

struct GMShaderVariablesLightDesc
{
	const char* Name;
	const char* Count;
};

struct GMShaderVariablesMaterialDesc
{
	const char* Ka;
	const char* Kd;
	const char* Ks;
	const char* Shininess;
	const char* Refreactivity;
};

constexpr int GMFilterCount = 6;
struct GMShaderVariablesFilterDesc
{
	const char* Filter;
	const char* KernelDeltaX;
	const char* KernelDeltaY;
	const char* Types[GMFilterCount];
};

struct GMShaderVariablesScreenInfoDesc
{
	const char* ScreenInfo;
	const char* ScreenWidth;
	const char* ScreenHeight;
	const char* Multisampling;
};

struct GMShaderVariableShadowInfo
{
	const char* ShadowInfo;
	const char* HasShadow;
	const char* ShadowProjectionMatrix;
	const char* ShadowViewMatrix;
	const char* Position;
	const char* ShadowMap;
	const char* ShadowMapMSAA;
	const char* ShadowMapWidth;
	const char* ShadowMapHeight;
	const char* Bias;
};

struct GMShaderVariablesDesc
{
	// 矩阵
	const char* ModelMatrix;
	const char* ViewMatrix;
	const char* ProjectionMatrix;
	const char* InverseTransposeModelMatrix;
	const char* InverseViewMatrix;

	// 位置
	const char* ViewPosition;

	// 材质
	GMShaderVariablesTextureDesc TextureAttributes;
	const char* AmbientTextureName;
	const char* DiffuseTextureName;
	const char* NormalMapTextureName;
	const char* LightMapTextureName;
	const char* CubeMapTextureName;

	// 光照
	const char* LightCount;

	// 材质
	GMShaderVariablesMaterialDesc MaterialAttributes;
	const char* MaterialName;

	// 滤镜
	GMShaderVariablesFilterDesc FilterAttributes;

	// 状态
	GMShaderVariablesScreenInfoDesc ScreenInfoAttributes;
	const char* RasterizerState;
	const char* BlendState;
	const char* DepthStencilState;

	// 阴影
	GMShaderVariableShadowInfo ShadowInfo;
};

struct GMShadowSourceDesc
{
	enum Type
	{
		NoShadow,
		DirectShadow,
	};

	Type type;
	GMCamera camera;
	GMVec4 position;
	float bias = 0.005f;
};

GM_PRIVATE_OBJECT(GMFramebuffersStack)
{
	Stack<IFramebuffers*> framebuffers;
};

class GMFramebuffersStack : public GMObject
{
	DECLARE_PRIVATE(GMFramebuffersStack);

public:
	void push(IFramebuffers* framebuffers);
	IFramebuffers* pop();
	IFramebuffers* peek();
};

GM_PRIVATE_OBJECT(GMGraphicEngine)
{
	IFramebuffers* filterFramebuffers = nullptr;
	GMGameObject* filterQuad = nullptr;
	GMScopePtr<GMModel> filterQuadModel;
	GMFramebuffersStack framebufferStack;
	IGBuffer* gBuffer = nullptr;
	GMRenderConfig renderConfig;
	GMDebugConfig debugConfig;
	GMStencilOptions stencilOptions;
	Vector<ILight*> lights;
	IShaderLoadCallback* shaderLoadCallback = nullptr;

	// Shadow
	GMShadowSourceDesc shadow;
	IFramebuffers* shadowDepthFramebuffers = nullptr;
	bool isDrawingShadow = false;
};

class GMGraphicEngine : public GMObject, public IGraphicEngine
{
	DECLARE_PRIVATE(GMGraphicEngine)

public:
	GMGraphicEngine();
	~GMGraphicEngine();

public:
	virtual void init() override;
	virtual IGBuffer* getGBuffer() override;
	virtual IFramebuffers* getFilterFramebuffers() override;
	virtual void draw(const List<GMGameObject*>& forwardRenderingObjects, const List<GMGameObject*>& deferredRenderingObjects) override;
	virtual void addLight(AUTORELEASE ILight* light) override;
	virtual void removeLights() override;
	virtual void setStencilOptions(const GMStencilOptions& options) override;
	virtual const GMStencilOptions& getStencilOptions() override;
	virtual void setShaderLoadCallback(IShaderLoadCallback* cb) override;
	virtual void setShadowSource(const GMShadowSourceDesc& desc) override;

public:
	const GMFilterMode::Mode getCurrentFilterMode();
	void draw(const List<GMGameObject*>& objects);
	IFramebuffers* getShadowMapFramebuffers();

protected:
	void createFilterFramebuffer();

public:
	inline GMFramebuffersStack& getFramebuffersStack()
	{
		D(d);
		return d->framebufferStack;
	}

protected:
	inline GMGameObject* getFilterQuad()
	{
		D(d);
		return d->filterQuad;
	}

	inline IShaderLoadCallback* getShaderLoadCallback()
	{
		D(d);
		return d->shaderLoadCallback;
	}

	inline const Vector<ILight*>& getLights()
	{
		D(d);
		return d->lights;
	}

private:
	IGBuffer* createGBuffer();

private:
	virtual void createShadowFramebuffers(OUT IFramebuffers** framebuffers) = 0;

public:
	static constexpr const GMuint getMaxLightCount()
	{
		return 50;
	}

	inline const GMShadowSourceDesc& getShadowSourceDesc()
	{
		D(d);
		return d->shadow;
	}

	inline bool isDrawingShadow()
	{
		D(d);
		return d->isDrawingShadow;
	}
};

END_NS
#endif