#include "ps_lut.h"
#include "../texture.h"
#include "../framebuffer.h"

PSLut::PSLut()
{
	m_iWidth = 0;
	m_iHeight = 0;
	Init();
}

PSLut::~PSLut()
{
	glDeleteVertexArrays(1, &m_uiVao);
	glDeleteBuffers(1, &m_uiVbo);
	glDeleteTextures(1, &m_uiInputTex);
	glDeleteTextures(1, &m_uiLutTex);
	glDeleteFramebuffers(1, &m_uiLut);
	glDeleteFramebuffers(1, &m_uiLutFBOTex);
}

void PSLut::Init()
{
	m_pLutProgram = new Shader("assets/shader/display.vs", "assets/shader/lut.fs");
	m_pDisplayProgram = new Shader("assets/shader/display.vs", "assets/shader/display.fs");

	glGenVertexArrays(1, &m_uiVao);
	glGenBuffers(1, &m_uiVbo);

	glBindVertexArray(m_uiVao);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	
	Texture inputTex;
	Texture lutTex;
	
	m_uiInputTex = inputTex.loadTexture("assets/image/1.jpg", m_iWidth, m_iHeight);
	m_uiLutTex = lutTex.loadTexture("assets/image/lut/Beagle.png");

	FrameBuffer lutFBO;
	m_uiLut = lutFBO.CreateFramebuffer(m_uiLutFBOTex, m_iWidth, m_iHeight);
}

void PSLut::Update(SmartBlur opt)
{

}

void PSLut::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiLut);
	glViewport(0, 0, m_iWidth, m_iHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glUseProgram(m_pLutProgram->ID);

	// 设置纹理单元
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uiInputTex);
	glUniform1i(glGetUniformLocation(m_pLutProgram->ID, "iChannel0"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_uiLutTex);
	glUniform1i(glGetUniformLocation(m_pLutProgram->ID, "iChannel1"), 1);

	glBindVertexArray(m_uiVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// 解除帧缓冲 切回到默认帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// 左边视口
	glViewport(0, 0, kiWidth / 2, kiHeight);
	m_pDisplayProgram->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uiInputTex);
	//m_pDisplayProgram->setInt("inputImage", 0);
	glUniform1i(glGetUniformLocation(m_pDisplayProgram->ID, "inputImage"), 0);
	glBindVertexArray(m_uiVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// 右边视口
	glViewport(kiWidth / 2, 0, kiWidth / 2, kiHeight);
	m_pDisplayProgram->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uiLutFBOTex);
	//m_pDisplayProgram->setInt("inputImage", 0);
	glUniform1i(glGetUniformLocation(m_pDisplayProgram->ID, "inputImage"), 0);
	glBindVertexArray(m_uiVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}