#pragma once
#include "opengl_window.h"
#include "lut/ps_lut.h"
#include "smartblur/ps_smartblur.h"
#include "defs.h"

class PSWindow : public OpenGLWindow
{
public:
	void InitializeScene() override;
	void UpdateScene() override;
	void RenderScene() override;
	void SaveScene() override;
	void ReleaseScene() override;

private:
	// ��ع���
	std::shared_ptr<PSSmartBlur> m_spPSSB;
	std::shared_ptr<PSLut> m_spPSLut;
	
	// ��������
	psOptions m_stOptions;
};

