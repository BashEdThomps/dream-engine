//
// Created by Ashley Thompson on 04/01/21.
//

#pragma once

#include <DreamCore.h>

using octronic::dream::File;

namespace octronic::dream::android
{
    class AndroidStorageManager;

    class AndroidFile : public File
    {
    public:
        AndroidFile(AndroidStorageManager* fm, const string& path);
        bool readBinary() override;
        bool exists() const override;

    protected:
        AndroidStorageManager* mStorageManager;
    };
}
