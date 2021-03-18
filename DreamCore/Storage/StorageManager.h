#pragma once

#include "File.h"
#include "Directory.h"


#include <string>
#include <vector>
#include <memory>

using std::string;
using std::vector;
using std::shared_ptr;
using std::weak_ptr;
using std::enable_shared_from_this;

namespace octronic::dream
{
	class File;
	class Directory;

    class StorageManager: public enable_shared_from_this<StorageManager>
	{
	public:
		StorageManager();
		~StorageManager();

		virtual weak_ptr<File> openFile(const string& path);
		void closeFile(const weak_ptr<File>& f);

		virtual weak_ptr<Directory> openDirectory(const string& path);
		void closeDirectory(const weak_ptr<Directory>& d);

	protected:
		vector<shared_ptr<File>> mOpenFiles;
		vector<shared_ptr<Directory>> mOpenDirectories;
	};
}
