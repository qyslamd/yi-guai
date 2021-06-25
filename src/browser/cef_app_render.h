#ifndef CEFAPPRENDER_H
#define CEFAPPRENDER_H

#include "client_app.h"

class CefAppRender : public ClientApp
{
public:
    CefAppRender();
private:
    IMPLEMENT_REFCOUNTING(CefAppRender);
    DISALLOW_COPY_AND_ASSIGN(CefAppRender);
};

#endif // CEFAPPRENDER_H
