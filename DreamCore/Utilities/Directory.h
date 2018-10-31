#pragma once

#include "../Common/DreamObject.h"

namespace Dream
{

    class Directory : public DreamObject
    {
    public:
        Directory(string dir);
        ~Directory();

        vector<string> list(string regex = "");

        string getPath() const;
        void setPath(string path);

        bool exists();
        bool create();
        bool deleteDirectory();

    private:
        string mPath;
    };

}
