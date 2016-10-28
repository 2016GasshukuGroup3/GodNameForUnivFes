#pragma once

/**
* @file Asset.h
* ゲーム中で使用するグラフィックや音声を一元的に管理するシステムを提供します。
*/

/// <summary>アセットマネージャに既に読み込まれている画像や音声のハンドルを登録します。</summary>
/// <param name="Key">
/// アセットマネージャへの登録名。この登録名を指定すると、ここで登録したハンドルを取り出すことができます。
/// </param>
/// <param name="GraphicHandle">既に読み込まれている画像や音声へのハンドル</param>
/// <param name="AlwaysRegisterValue">
/// 既に指定された登録名が使われていた時の処理を指定します。
/// true を指定すると必ず値を更新し、false を指定すると、既に指定された登録名が使われていた時は何もしません。
/// 既定では、false が渡されます。
/// </param>
/// <returns>登録に成功すれば true, そうでなければ false</returns>
bool AddHandle(const char Key[], int Handle, bool AlwaysRegisterValue = false);

/// <summary>新しく画像を読み込み、アセットマネージャにそのハンドルを登録します。</summary>
/// <param name="Key">
/// アセットマネージャへの登録名。この登録名を指定すると、ここで読み込んだ画像へのハンドルを取り出すことができます。
/// </param>
/// <param name="GraphicHandle">新しく読み込む画像ファイルのパス</param>
/// <returns>登録に成功すれば true, そうでなければ false</returns>
/// <remarks>すでに登録名が使われていた場合は何もしません</remarks>
bool AddGraphicHandle(const char Key[], const char FileName[]);

/// <summary>新しく音声を読み込み、アセットマネージャにそのハンドルを登録します。</summary>
/// <param name="Key">
/// アセットマネージャへの登録名。この登録名を指定すると、ここで読み込んだ音声へのハンドルを取り出すことができます。
/// </param>
/// <param name="GraphicHandle">新しく読み込む音声ファイルのパス</param>
/// <returns>登録に成功すれば true, そうでなければ false</returns>
/// <summary>新しく音声を読み込み、アセットマネージャにそのハンドルを登録します。</summary>
bool AddMusicHandle(const char Key[], const char FileName[]);

/// <summary>アセットマネージャから指定した登録名に関連付けられている画像や音声のハンドルの登録を解除します。</summary>
/// <param name="Key">AddHandle 関数で登録したときの登録名</param>
/// <returns>登録解除に成功すれば true, そうでなければ false</returns>
bool RemoveHandle(const char Key[]);

/// <summary>アセットマネージャから指定した登録名に関連付けられている画像や音声のハンドルを取り出します。</summary>
/// <param name="Key">AddHandle 関数で登録したときの登録名</param>
/// <returns>ハンドルの取得に成功すれば、そのハンドル。そうでなければ、-1</returns>
int GetHandle(const char Key[]);