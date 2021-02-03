#pragma once

#include "File.h"
#include "Directory.h"
#include "Common/LockableObject.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace octronic::dream
{
	class File;
	class Directory;

	class StorageManager : public LockableObject
	{
	public:
		StorageManager();
		~StorageManager();

		virtual File* openFile(const string& path);
		void closeFile(File* f);

		virtual Directory* openDirectory(const string& path);
		void closeDirectory(Directory* d);

	protected:
		vector<File*> mOpenFiles;
		vector<Directory*> mOpenDirectories;
	};
}
