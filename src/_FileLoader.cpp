#include "_FileLoader.h"

_FileLoader::_FileLoader()
{
    //ctor
}

_FileLoader::~_FileLoader()
{
    //dtor
}

std::unordered_map<std::string,_item> _FileLoader::loadItems(std::string fileName) {
    std::string fileDir = "gamedata/" + fileName + ".txt";

    std::unordered_map<std::string,_item> items;
    std::ifstream file(fileDir);

    if(!file.is_open()){
        std::cout << "Error: Can't read file" << fileDir << std::endl;
    }

    std::string line;

    while(std::getline(file, line)){
        std::stringstream ss(line);

        std::string type;
        ss >> type;

        _item i;
        ss >> i.name;

        std::string statName;
        float statValue;

        while(ss >> statName >> statValue){
            i.stats[statName] = statValue;
        }
        std::cout << "Loaded \"" << i.name <<"\"";
        for(const auto& s : i.stats) std::cout << s.first << " " << s.second << std::endl;
        items[i.name] = i;
    }
    return items;
}
