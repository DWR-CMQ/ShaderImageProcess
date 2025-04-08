#pragma once
#include "../render_base.h"
#include "../shader.h"
#include "../texture.h"
#include "../defs.h"

class PSSmartBlur : public RenderBase
{
public:
	PSSmartBlur();
	virtual ~PSSmartBlur();

	void Init() override;
	void Update(SmartBlur opt) override;
	void Render() override;

private:
	unsigned int m_uiVao;
	unsigned int m_uiVbo;

	unsigned int m_uiInputTex;
	unsigned int m_uiYCrCbTex;
	unsigned int m_uiBlurTex;

	Shader* m_pRgbToYCrCbProgram = nullptr;
	Shader* m_pSmartBlurProgram = nullptr;
	Shader* m_pDisplayProgram = nullptr;

	unsigned int m_ycrcb;
	unsigned int m_blur;

	int m_iWidth;
	int m_iHeight;

	SmartBlur m_stSmartBlur;
};

