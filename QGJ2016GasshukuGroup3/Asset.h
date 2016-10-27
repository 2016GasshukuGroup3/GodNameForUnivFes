#pragma once

/**
* @file Asset.h
* �Q�[�����Ŏg�p����O���t�B�b�N�≹�����ꌳ�I�ɊǗ�����V�X�e����񋟂��܂��B
*/

/// <summary>�A�Z�b�g�}�l�[�W���Ɋ��ɓǂݍ��܂�Ă���摜�≹���̃n���h����o�^���܂��B</summary>
/// <param name="Key">
/// �A�Z�b�g�}�l�[�W���ւ̓o�^���B���̓o�^�����w�肷��ƁA�����œo�^�����n���h�������o�����Ƃ��ł��܂��B
/// </param>
/// <param name="GraphicHandle">���ɓǂݍ��܂�Ă���摜�≹���ւ̃n���h��</param>
/// <param name="AlwaysRegisterValue">
/// ���Ɏw�肳�ꂽ�o�^�����g���Ă������̏������w�肵�܂��B
/// true ���w�肷��ƕK���l���X�V���Afalse ���w�肷��ƁA���Ɏw�肳�ꂽ�o�^�����g���Ă������͉������܂���B
/// ����ł́Afalse ���n����܂��B
/// </param>
/// <returns>�o�^�ɐ�������� true, �����łȂ���� false</returns>
bool AddHandle(const char Key[], int Handle, bool AlwaysRegisterValue = false);

/// <summary>�V�����摜��ǂݍ��݁A�A�Z�b�g�}�l�[�W���ɂ��̃n���h����o�^���܂��B</summary>
/// <param name="Key">
/// �A�Z�b�g�}�l�[�W���ւ̓o�^���B���̓o�^�����w�肷��ƁA�����œǂݍ��񂾉摜�ւ̃n���h�������o�����Ƃ��ł��܂��B
/// </param>
/// <param name="GraphicHandle">�V�����ǂݍ��މ摜�t�@�C���̃p�X</param>
/// <returns>�o�^�ɐ�������� true, �����łȂ���� false</returns>
/// <remarks>���łɓo�^�����g���Ă����ꍇ�͉������܂���</remarks>
bool AddGraphicHandle(const char Key[], const char FileName[]);

/// <summary>�V����������ǂݍ��݁A�A�Z�b�g�}�l�[�W���ɂ��̃n���h����o�^���܂��B</summary>
/// <param name="Key">
/// �A�Z�b�g�}�l�[�W���ւ̓o�^���B���̓o�^�����w�肷��ƁA�����œǂݍ��񂾉����ւ̃n���h�������o�����Ƃ��ł��܂��B
/// </param>
/// <param name="GraphicHandle">�V�����ǂݍ��މ����t�@�C���̃p�X</param>
/// <returns>�o�^�ɐ�������� true, �����łȂ���� false</returns>
/// <summary>�V����������ǂݍ��݁A�A�Z�b�g�}�l�[�W���ɂ��̃n���h����o�^���܂��B</summary>
bool AddMusicHandle(const char Key[], const char FileName[]);

/// <summary>�A�Z�b�g�}�l�[�W������w�肵���o�^���Ɋ֘A�t�����Ă���摜�≹���̃n���h���̓o�^���������܂��B</summary>
/// <param name="Key">AddHandle �֐��œo�^�����Ƃ��̓o�^��</param>
/// <returns>�o�^�����ɐ�������� true, �����łȂ���� false</returns>
bool RemoveHandle(const char Key[]);

/// <summary>�A�Z�b�g�}�l�[�W������w�肵���o�^���Ɋ֘A�t�����Ă���摜�≹���̃n���h�������o���܂��B</summary>
/// <param name="Key">AddHandle �֐��œo�^�����Ƃ��̓o�^��</param>
/// <returns>�n���h���̎擾�ɐ�������΁A���̃n���h���B�����łȂ���΁A-1</returns>
int GetHandle(const char Key[]);