#ifndef LOCALUSERLOCALIMAGE_H
#define LOCALUSERLOCALIMAGE_H

#include "AbstractUserLocalImage.h"

class LocalUserLocalImage : public AbstractUserLocalImage
{
public:
    LocalUserLocalImage();
    virtual ~LocalUserLocalImage(){}

    virtual bool isInvisible()=0;
};

#endif // LOCALUSERLOCALIMAGE_H
