//Copyright © 2006- EricWang(wwssllabcd@gmail.com). All rights reserved

#include "stdafx.h"
#include "FileUtility.h"
#include <fstream> //ofs
#include "EricException.h"
#include <Windows.h>//Sleep

using namespace EricCore;
using namespace std;

FileUtility::FileUtility(void) {
}

FileUtility::~FileUtility(void) {
}

eu32 FileUtility::getFileSize(const estring& filePath) {
    FILE* fpSource = getFilePtr(filePath, _ET("rb"));

    if (fpSource == NULL) {
        estring msg = _ET("GetFileSize: Can`t open file ") + filePath;
        THROW_MYEXCEPTION(UTI_GET_FILE_SIZE_FAIL, msg.c_str());
    }

    fseek(fpSource, 0, SEEK_END);
    eu32 fileLength = 0;
    fileLength = ftell(fpSource);
    fclose(fpSource);
    return fileLength;
}

eu8* FileUtility::getFileDataNew(const estring& filePath, int length, eu8* data) {
    fstream file;
    file.open(filePath, ios::in | ios::binary);
    file.read((char*)data, length);
    file.close();
    return data;
}

eu8* FileUtility::getFileData(const estring& filePath, int startAddr, int length, eu8* data) {
    if (startAddr == 0) {
        return getFileDataNew(filePath, length, data);
    }

    int i;
    FILE* fpSource = getFilePtr(filePath, _ET("rb"));

    if (fpSource == NULL) {
        estring msg = _ET("GetFileData: Can`t open file ") + filePath;
        THROW_MYEXCEPTION(UTI_GET_FILE_DATA_FAIL, msg.c_str());
    }

    long fileSize = getFileSize(filePath);

    if ((startAddr + length) > fileSize) {
        THROW_MYEXCEPTION(UTI_GET_FILE_DATA_FAIL, _ET("GetFileData: get file size out of boundary"));
    }

    fseek(fpSource, startAddr, SEEK_SET);
    for (i = 0; i < length; i++) {
        data[i] = fgetc(fpSource);
    }
    fclose(fpSource);
    return data;
}

eu8* FileUtility::getFileData(const estring& filePath, eu32 fileSize, eu8* data) {
    getFileData(filePath, 0, fileSize, data);
    return data;
}

eu8* FileUtility::getFileData(const estring& filePath, eu8* data) {
    long fileSize = getFileSize(filePath);
    getFileData(filePath, fileSize, data);
    return data;
}

// output to Text file
void FileUtility::toFile(const estring& filePath, const estring& msg, bool isAppend) {
    toFile(filePath, (eu8*)msg.c_str(), (int)msg.length(), isAppend);
}

// output Array to Binary file
void FileUtility::toFile(const estring& filePath, eu8* data, int length, bool isAppend) {
    //if it have not the file , ofstream will make a new file
    ofstream ofs;
    int i;
    for (i = 0; i < 10; i++) {
        if (isAppend == true) {
            // ios::binary is mean," Do not replace CRLF "
            ofs.open(filePath.c_str(), ios::out | ios::binary | ios::app);
        } else {
            ofs.open(filePath.c_str(), ios::out | ios::binary | ios::trunc);
        }

        if (ofs.fail() == false) {
            break;
        }
        Sleep(500);
    }

    if (ofs.fail() == true) {
        estring msg = _ET("toFile: Can`t open file ") + filePath;
        THROW_MYEXCEPTION(UTI_TO_FILE_FAIL, msg.c_str());
    }

    // output data by ANSI stream, because it's "byte array"
    ofs.write((char*)data, length);

    // even you didn't close, ofs will be close in destructor
    // fstream destruct call close for you. When an exception is thrown, the file is closed automatically.(RAII)
    ofs.close();
}

// output Array to Binary file
FILE* FileUtility::getFilePtr(const estring& filePath, const estring& condition) {
    FILE* fpSource = 0;
#ifdef _VC_COMPILER_
    tfopen_s(&fpSource, filePath.c_str(), condition.c_str());
#else
    fpSource = fopen(filePath.c_str(), condition.c_str());
#endif
    return fpSource;
}

bool FileUtility::isFileExist(const estring& filePath) {
    struct _stat buffer;
    return (_tstat(filePath.c_str(), &buffer) == 0);
}

int FileUtility::deleteFile(const estring& filePath) {
    return _tremove(filePath.c_str());
}

void FileUtility::createFolder(const estring& folderName) {
    if (CreateDirectory(folderName.c_str(), NULL) == 0) {
        if (ERROR_ALREADY_EXISTS != GetLastError()) {
            estring msg = _ET("createFolder: Can`t create folder file ") + folderName;
            THROW_MYEXCEPTION(0, msg.c_str());
        }
    }
}