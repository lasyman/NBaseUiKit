#ifndef NBasePageBar_H
#define NBasePageBar_H

/**
 * @author: lasyman
 * @date: 2017year
 *
 */

#include <QWidget>
#include <QMutex>
#include "nbaseuikit_global.h"

namespace Ui {
class wgtPageBar;
}

/*最大显示页码数*/
#define MAX_SHOW_SERIAL_PAGE 10

class QPushButton;
class QIntValidator;
class QButtonGroup;
class NBASEUIKITSHARED_EXPORT NBasePageBar : public QWidget
{
    Q_OBJECT
public:
    NBasePageBar(QWidget *parent = 0);
    ~NBasePageBar();

    /**
     * @brief: 设置每页个数
     */
    void setPerPageNum(int);
    /**
     * @brief: 设置当前页码
     */
    void setCurrentPage(int, bool force = false);
    /**
     * @brief: 设置最大显示页码个数
     */
    bool setMaxVisiblePages(int pages);
    /**
     * @brief: 设置左侧自定义按钮
     */
    void setCustomerButtonVisable(bool bEnable);
    /**
     * @brief: 设置左侧自定义按钮文字
     */
    void setCustomerButtonText(const QString &strText);

Q_SIGNALS:
    /**
     * @brief: 当前页码发生变化(当前页码值)
     */
    void sigCurrentPageChanged(int);
    /**
     * @brief: 自定义按钮被点击信号
     */
    void sigCustomerBtnClicked();

public slots:
    /**
     * @brief: 设置总数
     */
    void onTotalChanged(int, bool force = false);

protected slots:
    void onNextPage();
    void onPreviousPage();
    void onFirstPage();
    void onLastPage();
    void onGotoPage();
    void onChangeSkin();

protected:
    void addPage(int);
    void removePage(int);
    QPushButton* page(int);
    void resetPages(bool resetMaxMin = true);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::wgtPageBar *m_ui;
    int m_perPageNum{2};
    int m_pageNum{1};
    int m_currentPage{1};
    int m_minPage{1};
    int m_maxPage{1};
    int m_total{0};
    int m_logicalPage{1};
    QList<QPushButton*> m_pages;
    QMutex m_mutex;
    QIntValidator *m_reg;
    QButtonGroup *m_buttonGroup;

    int m_maxVisiblePages{ MAX_SHOW_SERIAL_PAGE };
};


#endif // NBasePageBar_H

