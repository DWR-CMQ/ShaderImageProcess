#include "ps_window.h"

void PSWindow::InitializeScene()
{
	m_spPSSB = std::make_shared<PSSmartBlur>();
	m_spPSLut = std::make_shared<PSLut>();
}

void PSWindow::UpdateScene()
{

}

void PSWindow::RenderScene()
{
	UI::RenderUI(m_stOptions);
	m_spPSLut->Update(m_stOptions);
	m_spPSLut->Render();
}

void PSWindow::SaveScene()
{

}

void PSWindow::ReleaseScene()
{

}