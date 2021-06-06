#ifndef POPUPGENERAL_H
#define POPUPGENERAL_H

#include "PopupBase.h"

class QVBoxLayout;
class PopupGeneral : public PopupBase
{
public:
    explicit PopupGeneral(QWidget *parent = nullptr);
     ~PopupGeneral();

    void setWidget(QWidget *widget);

private:
    QVBoxLayout *layout_;
};

#endif // POPUPGENERAL_H
