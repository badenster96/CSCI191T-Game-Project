#ifndef _FILELOADER_H
#define _FILELOADER_H

#include<string>
#include<iostream>
#include <_item.h>
#include<sstream>
#include<unordered_map>

class _FileLoader
{
    public:
        _FileLoader();
        virtual ~_FileLoader();
        std::unordered_map<std::string,_item> loadItems(std::string fileName);

    protected:

    private:
};

#endif // _FILELOADER_H
