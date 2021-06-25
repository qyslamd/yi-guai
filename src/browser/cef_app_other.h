#ifndef CEFAPPOTHER_H
#define CEFAPPOTHER_H

#include "client_app.h"

class CefAppOther : public ClientApp
{
public:
    CefAppOther();
private:
    IMPLEMENT_REFCOUNTING(CefAppOther);
    DISALLOW_COPY_AND_ASSIGN(CefAppOther);
};

#endif // CEFAPPOTHER_H
