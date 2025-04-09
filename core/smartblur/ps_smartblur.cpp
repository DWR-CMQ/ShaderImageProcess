#include "ps_smartblur.h"
#include "../texture.h"
#include "../framebuffer.h"

PSSmartBlur::PSSmartBlur()
{
	m_iWidth = 0;
	m_iHeight = 0;
	Init();
}

PSSmartBlur::~PSSmartBlur()
{
	glDeleteVertexArrays(1, &m_uiVao);
	glDeleteBuffers(1, &m_uiVbo);
	glDeleteTextures(1, &m_uiInputTex);
	glDeleteTextures(1, &m_uiYCrCbTex);
	glDeleteTextures(1, &m_uiBlurTex);
	glDeleteFramebuffers(1, &m_ycrcb);
	glDeleteFramebuffers(1, &m_blur);
}

void PSSmartBlur::Init()
{
	// ������������
	Texture input;
	m_uiInputTex = input.loadTexture("assets/image/1.jpg", m_iWidth, m_iHeight);

	m_pRgbToYCrCbProgram = new Shader("assets/shader/display.vs", "assets/shader/smartblurbuffer.fs");
	m_pSmartBlurProgram = new Shader("assets/shader/display.vs", "assets/shader/smartblur.fs");
	m_pDisplayProgram = new Shader("assets/shader/display.vs", "assets/shader/display.fs");

	glGenVertexArrays(1, &m_uiVao);
	glGenBuffers(1, &m_uiVbo);

	glBindVertexArray(m_uiVao);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	// λ������
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	// ������������
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// �����м������֡����
	FrameBuffer ycrcbFBO;
	FrameBuffer blurredFBO;
	m_ycrcb = ycrcbFBO.CreateFramebuffer(m_uiYCrCbTex, m_iWidth, m_iHeight);
	m_blur = blurredFBO.CreateFramebuffer(m_uiBlurTex, m_iWidth, m_iHeight);
}

void PSSmartBlur::Update(psOptions opt)
{
	m_stSmartBlur.fThreshold = opt.fThreshold;
	m_stSmartBlur.iRadius = opt.iRadius;
}

void PSSmartBlur::Render()
{
	// ��һ�׶Σ�RGBתYCrCb����Ⱦ��m_ycrcb֡���壩
	glBindFramebuffer(GL_FRAMEBUFFER, m_ycrcb);
	glViewport(0, 0, m_iWidth, m_iHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glUseProgram(m_pRgbToYCrCbProgram->ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uiInputTex);
	glUniform1i(glGetUniformLocation(m_pRgbToYCrCbProgram->ID, "inputImage"), 0);

	glBindVertexArray(m_uiVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// �ڶ��׶Σ�����ģ������Ⱦ��m_blur֡���壩
	glBindFramebuffer(GL_FRAMEBUFFER, m_blur);
	glViewport(0, 0, m_iWidth, m_iHeight);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_pSmartBlurProgram->ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uiYCrCbTex);
	glUniform1i(glGetUniformLocation(m_pSmartBlurProgram->ID, "inputImage"), 0);
	glUniform1i(glGetUniformLocation(m_pSmartBlurProgram->ID, "radius"), m_stSmartBlur.iRadius);
	glUniform1f(glGetUniformLocation(m_pSmartBlurProgram->ID, "threshold"), m_stSmartBlur.fThreshold);
	glUniform2f(glGetUniformLocation(m_pSmartBlurProgram->ID, "resolution"), (float)m_iWidth, (float)m_iHeight);

	glBindVertexArray(m_uiVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// ����������ʾ������������ӿڣ�
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// ����ӿڣ�ԭʼͼ��
	glViewport(0, 0, kiWidth / 2, kiHeight);
	glUseProgram(m_pDisplayProgram->ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uiInputTex);
	glUniform1i(glGetUniformLocation(m_pDisplayProgram->ID, "inputImage"), 0);
	glBindVertexArray(m_uiVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// �Ҳ��ӿڣ�������ͼ��
	glViewport(kiWidth / 2, 0, kiWidth / 2, kiHeight);
	glUseProgram(m_pDisplayProgram->ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uiBlurTex);
	glUniform1i(glGetUniformLocation(m_pDisplayProgram->ID, "inputImage"), 0);
	glBindVertexArray(m_uiVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}