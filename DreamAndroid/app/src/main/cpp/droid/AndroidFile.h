//
// Created by Ashley Thompson on 04/01/21.
//

#pragma once

#include <DreamCore.h>

using octonic::dream::File;

namespace octronic::dream::android
{
    class AndroidFileManager;
    class AndroidFile : public File
    {
    public:
        AndroidFile(AndroidFileManager* fm, string path);

        bool readBinary() override;
        bool exists() const override;

    protected:
        AndroidFileManager* mFileManager;
    };
}
