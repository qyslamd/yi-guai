#ifndef HISTORYPOPUP_H
#define HISTORYPOPUP_H

#include <QWidget>

#include "PopupBase.h"

namespace Ui {
class HistoryPopup;
}

class HistoryPopup : public PopupBase
{
    Q_OBJECT
public:
    explicit HistoryPopup(QWidget *parent = nullptr);
    ~HistoryPopup();

private:
    Ui::HistoryPopup *ui;
};

#endif // HISTORYPOPUP_H
