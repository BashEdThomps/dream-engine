#pragma once

#include <cstring>

namespace octronic::dream
{
	class KeyboardState
	{
	public:
		KeyboardState();
		bool isKeyPressed(unsigned int key);
		void setKeysPressed(bool* b, size_t sz);

	private:
		const static unsigned int KeysMax = 512;
		bool mKeysPressed[KeysMax] = {false};
	};
}
