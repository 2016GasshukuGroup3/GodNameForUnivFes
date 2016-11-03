#include "Asset.h"
#include "DxLib.h"
#
#include <unordered_map>
#include <string>

static std::unordered_map<std::string, int> Handles;

bool AddHandle(const char Key[], int Handle, bool AlwaysRegisterValue) {
	// 常に登録するか、キーが登録されていない、かつ、ハンドルが有効ならば追加する。
	if ((AlwaysRegisterValue || Handles.find(Key) == Handles.end()) && Handle != -1) {
		Handles[Key] = Handle;
		return true;
	} else {
		return false;
	}
}

bool AddGraphicHandle(const char Key[], const char FileName[]) {
	if (Handles.find(Key) == Handles.end()) {
		return AddHandle(Key, LoadGraph(FileName));
	} else {
		return false;
	}
}

bool AddMusicHandle(const char Key[], const char FileName[]) {
	if (Handles.find(Key) == Handles.end()) {
		return AddHandle(Key, LoadSoundMem(FileName));
	} else {
		return false;
	}
}

bool RemoveHandle(const char Key[]) {
	return Handles.erase(Key) == 1;
}

int GetHandle(const char Key[]) {
	if (Handles.find(Key) == Handles.end()) {
		return -1;
	}

	return Handles[Key];
}