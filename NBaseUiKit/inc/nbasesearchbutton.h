#ifndef NBASESEARCHBUTTON_H
#define NBASESEARCHBUTTON_H

/**
 * @author: lasyman
 * @date: 2017year
 *
 */

#include <QWidget>
#include "nbaseuikit_global.h"

namespace Ui {
class NBaseSearchButton;
}

class NBASEUIKITSHARED_EXPORT NBaseSearchButton : public QWidget
{
    Q_OBJECT

public:
    explicit NBaseSearchButton(QWidget *parent = 0);
    ~NBaseSearchButton();

signals:
    /**
     * @brief: 返回输入的字符串
     */
    void sigSearchKey(const QString &);
    /**
     * @brief: 清除搜索
     */
    void sigSearchClear();

protected slots:
    void onSearchBtnClicked();
    void onSearchEditFinished();

private:
    Ui::NBaseSearchButton *ui;
};

#endif // NBASESEARCHBUTTON_H
