#include "_sounds.h"

_sounds::_sounds()
{
    //ctor
}

_sounds::~_sounds()
{
    //dtor
    eng->drop();
}

void _sounds::playMusic(char* filename)
{
    eng->play2D(filename,true);
}

void _sounds::playSounds(char* filename, float volume)
{
    ISound* s = eng->play2D(filename,false,true,false);
    if(s){
        s->setVolume(volume);
        s->setIsPaused(false);
    }
}
void _sounds::playRandSound(char* filename, int range, float volume){
    int snd = (rand()%range) + 1;
    std::string fileStr = filename;
    char numberStr[4];
    snprintf(numberStr, sizeof(numberStr), "%03d", snd);

    fileStr.replace(fileStr.size() - 7, 3, numberStr);
    playSounds(const_cast<char*>(fileStr.c_str()), volume);
}

void _sounds::pauseSound(char* filename)
{
    eng->play2D(filename,true,false);
}

void _sounds::initSounds()
{
   if(!eng)
    cout<< "ERROR: **** The sound could not Load\n";
}
