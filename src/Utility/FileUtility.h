#pragma once

#include "DefineFiles/DefineFile.h"

namespace EricCore {
    class FileUtility
    {
    public:
        FileUtility(void);
        ~FileUtility(void);

        //for file
        void toFile(const estring& filePath, const estring& msg, bool isAppend);
        void toFile(const estring& filePath, eu8* data, int length, bool isAppend = false);

        eu8* getFileData(const estring& filePath, eu8* data);
        eu8* getFileData(const estring& filePath, eu32 fileSize, eu8* data);
        eu8* getFileData(const estring& filePath, int startAddr, int length, eu8* data);

        eu32 getFileSize(const estring& filePath);
        FILE* getFilePtr(const estring& filePath, const estring& condition);
        bool isFileExist(const estring& filePath);
        int deleteFile(const estring& filePath);

        // for folder
        void createFolder(const estring& folderName);

    private:
        eu8* getFileDataNew(const estring& filePath, int length, eu8* data);

    };
}