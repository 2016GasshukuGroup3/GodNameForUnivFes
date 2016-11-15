#pragma once

/**
* @file Shader.h
* �V�F�[�_�[��p���ĕ`�悷��@�\��񋟂��܂��B
*/

/// <summary>�V�F�[�_�[�Ȃǂ̃��\�[�X�����������܂��B</summary>
int InitializeShaderResourse();

/// <summary>�摜�̓��ߕ����Ɣ񓧉ߕ����̋��E��h��Ԃ����O���t�B�b�N���쐬���܂��B</summary>
/// <param name="fileName">�ǂݍ��ރt�@�C����</param>
int LoadGraphEdge(const char fileName[]);

/// <summary>�摜�̓��ߕ����Ɣ񓧉ߕ����̋��E���w�肵���F�œh��܂��B</summary>
/// <param name="x">�摜��`�悷�� x ���W</param>
/// <param name="y">�摜��`�悷�� y ���W</param>
/// <param name="grHandle">���E��h��摜�ւ̃n���h��</param>
/// <param name="edgeColor">���E�F</param>
/// <returns>�`��ɐ�������� 0, �����łȂ���� -1</returns>
int DrawGraphEdge(int x, int y, int grHandle, int edgeColor);