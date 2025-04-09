#pragma once
#include "../render_base.h"
#include "../shader.h"
#include "../texture.h"
#include "../defs.h"

class PSLut : public RenderBase
{
public:
	PSLut();
	virtual ~PSLut();

	void Init() override;
	void Update(psOptions opt) override;
	void Render() override;

private:
	unsigned int m_uiVao;
	unsigned int m_uiVbo;

	unsigned int m_uiInputTex;
	unsigned int m_uiLutTex;

	unsigned int m_uiLut;
	unsigned int m_uiLutFBOTex;

	int m_iWidth;
	int m_iHeight;

	Shader* m_pDisplayProgram = nullptr;
	Shader* m_pLutProgram = nullptr;
};

