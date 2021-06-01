#ifndef ZOOMPOPUP_H
#define ZOOMPOPUP_H

#include "PopupBase.h"

namespace Ui {
class ZoomPopup;
}

class ZoomPopup : public PopupBase
{
    Q_OBJECT

public:
    explicit ZoomPopup(QWidget *parent = nullptr);
    ~ZoomPopup();
    void setZoomLevelStr(const QString &str);
signals:
    void zoomIn();
    void zoomOut();
    void zoomReset();
private:
    Ui::ZoomPopup *ui;
};

#endif // ZOOMPOPUP_H
