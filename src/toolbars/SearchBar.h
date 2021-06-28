#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QFrame>

class QLineEdit;
class QToolButton;
class QHBoxLayout;
class SearchBar : public QFrame
{
    Q_OBJECT
public:
    SearchBar(bool inprivate = false, QWidget *parent = nullptr);
    bool eventFilter(QObject *obj, QEvent *ev) override;
private:
    bool inprivate_ = false;

    QHBoxLayout *layout_ = nullptr;
    QLineEdit *line_edit_ = nullptr;
    QToolButton *btn_search_engine_ = nullptr;
    QToolButton *btn_search_ = nullptr;


    void initUi();
    void setIcons();
};

#endif // SEARCHBAR_H
