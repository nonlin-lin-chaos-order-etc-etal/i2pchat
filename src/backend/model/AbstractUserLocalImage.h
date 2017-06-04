#ifndef ABSTRACTUSERLOCALIMAGE_H
#define ABSTRACTUSERLOCALIMAGE_H


class AbstractUserLocalImage : public std::enable_shared_from_this<AbstractUserLocalImage>
{
public:
    AbstractUserLocalImage();
    virtual ~AbstractUserLocalImage(){}
};

#endif // ABSTRACTUSERLOCALIMAGE_H
