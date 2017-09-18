#include "nbasepagebar.h"
#include <QButtonGroup>
#include <QPushButton>
#include <QMouseEvent>
#include <QIntValidator>
#include <QGroupBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>

#include "ui_page_bar.h"

NBasePageBar::NBasePageBar(QWidget *parent /*= 0*/)
    : QWidget(parent),m_ui(new Ui::wgtPageBar())
{
    m_ui->setupUi(this);
    m_buttonGroup = new QButtonGroup(this);
    m_reg = new QIntValidator(1,1);
    m_ui->lineEdit->setValidator(m_reg);
    m_ui->btn_customer->setVisible(false);
    addPage(1);
    m_pages.first()->setChecked(true);
    onChangeSkin();
    connect(m_ui->btn_first, SIGNAL(clicked()), this, SLOT(onFirstPage()));
    connect(m_ui->btn_last, SIGNAL(clicked()), this, SLOT(onLastPage()));
    connect(m_ui->btn_previous, SIGNAL(clicked()), this, SLOT(onPreviousPage()));
    connect(m_ui->btn_next, SIGNAL(clicked()), this, SLOT(onNextPage()));
    connect(m_ui->btn_go, SIGNAL(clicked()), this, SLOT(onGotoPage()));
    connect(m_ui->lineEdit, SIGNAL(returnPressed()), m_ui->btn_go, SIGNAL(clicked()));
    connect(m_ui->btn_customer, SIGNAL(clicked()), this, SIGNAL(sigCustomerBtnClicked()));
}

NBasePageBar::~NBasePageBar()
{
    delete m_reg;
    delete m_buttonGroup;
    delete m_ui;
}

void NBasePageBar::setPerPageNum(int num)
{
    m_perPageNum = num;
    onTotalChanged(m_total, true);
}

void NBasePageBar::setCurrentPage(int page, bool force)
{
    if (page != m_currentPage || force)
        emit sigCurrentPageChanged(page);
    m_currentPage = page;
    resetPages(false);
}

bool NBasePageBar::setMaxVisiblePages(int pages)
{
    if (pages < 1 || pages > MAX_SHOW_SERIAL_PAGE)
    {
        return false;
    }
    m_maxVisiblePages = pages;
    onTotalChanged(m_total, true);
    return true;
}

void NBasePageBar::setCustomerButtonVisable(bool bEnable)
{
    m_ui->btn_customer->setVisible(bEnable);
}

void NBasePageBar::setCustomerButtonText(const QString &strText)
{
    m_ui->btn_customer->setText(strText);
}

void NBasePageBar::onTotalChanged(int total, bool force)
{
    m_total = total;
    int pageNum = total / m_perPageNum;
    if (total % m_perPageNum)
        pageNum += 1;

    if (pageNum == 0)
        pageNum = 1;

    m_reg->setRange(1, pageNum);

    if (m_currentPage > pageNum)
    {
        m_currentPage = pageNum;
        emit sigCurrentPageChanged(m_currentPage);
    }

    int pageButtonCount = m_pages.size();
    int min = qMin(m_maxVisiblePages, pageNum);
    if (min != pageButtonCount)
    {
        if (pageButtonCount > min)
        {
            for (int i = pageButtonCount; i > min; i--)
                removePage(i - 1);
        }
        else
        {
            for (int i = pageButtonCount; i < min; i++)
                addPage(i + 1);
        }
    }
    m_pageNum = pageNum;

    if (m_maxPage > m_pageNum)
    {
        m_maxPage = m_pageNum;
    }

    resetPages(force);
}

void NBasePageBar::onNextPage()
{
    if ((m_pages.size() > m_logicalPage) && (m_logicalPage >= 1))
    {
        m_logicalPage++;
        m_currentPage++;

        if ((m_pages.size() == m_logicalPage) || (m_logicalPage == 1))
            resetPages(false);
        else
            page(m_logicalPage - 1)->setChecked(true);

        emit sigCurrentPageChanged(m_currentPage);
    }
}

void NBasePageBar::onPreviousPage()
{
    if ((m_pages.size() >= m_logicalPage) && (m_logicalPage > 1))
    {
        m_logicalPage--;
        m_currentPage--;

        if ((m_pages.size() == m_logicalPage) || (m_logicalPage == 1))
            resetPages(false);
        else
            page(m_logicalPage - 1)->setChecked(true);

        if (m_currentPage == 0)
            m_currentPage = 1;

        emit sigCurrentPageChanged(m_currentPage);
    }
}

void NBasePageBar::onFirstPage()
{
    if (!m_pages.isEmpty())
    {
        m_currentPage = 1;
        resetPages();

        emit sigCurrentPageChanged(m_currentPage);
    }
}

void NBasePageBar::onLastPage()
{
    if (!m_pages.isEmpty())
    {
        m_currentPage = m_pageNum;
        resetPages();

        emit sigCurrentPageChanged(m_currentPage);
    }
}

void NBasePageBar::onGotoPage()
{
    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    m_currentPage = btn->text().toInt();

    if (btn == m_ui->btn_go)
    {
        if (m_ui->lineEdit->text().isEmpty())
            return;

        if (m_ui->lineEdit->text().toInt() == 0)
            return;

        int page = m_ui->lineEdit->text().toInt();
        if ((page < 1) || (page > m_pageNum))
            return;

        m_currentPage = m_ui->lineEdit->text().toInt();

        emit sigCurrentPageChanged(m_currentPage);
    }
    else
    {
        emit sigCurrentPageChanged(m_currentPage);

        m_logicalPage = m_pages.indexOf(btn) + 1;
        if ((m_pages.size() > m_logicalPage) && (m_logicalPage > 1))
        {
            page(m_logicalPage - 1)->setChecked(true);
            return;
        }
    }

    resetPages();
}

void NBasePageBar::onChangeSkin()
{
    QFile styleFile(":/qss/pageBar.qss");
    if (styleFile.open(QIODevice::ReadOnly))
    {
        QString strStyleSheet = styleFile.readAll();
        this->setStyleSheet(strStyleSheet);
    }
}

void NBasePageBar::addPage(int index)
{
    QMutexLocker l(&m_mutex);

    QPushButton *page = new QPushButton(QString::number(index));
    connect(page, SIGNAL(clicked()), this, SLOT(onGotoPage()));

    page->setObjectName(page->text());
    page->setCheckable(true);
    page->installEventFilter(this);

    m_buttonGroup->addButton(page);
    m_ui->gbx_pages->layout()->addWidget(page);
    m_pages.append(page);
}

void NBasePageBar::removePage(int index)
{
    QMutexLocker l(&m_mutex);
    if ((m_pages.size() > index) && (index >= 0))
        delete m_pages.takeAt(index);
}

QPushButton* NBasePageBar::page(int index)
{
    QMutexLocker l(&m_mutex);
    if ((m_pages.size() > index) && (index >= 0))
        return m_pages[index];
    return NULL;
}


void NBasePageBar::resetPages(bool resetMaxMin)
{
    if (m_pageNum <= m_maxVisiblePages)
    {
        m_minPage = 1;
        m_maxPage = m_pageNum;
    }
    else
    {
        int half = m_maxVisiblePages / 2;

        if (m_maxPage == m_currentPage || m_minPage == m_currentPage || resetMaxMin)
        {
            m_maxPage = qMin(m_currentPage + half - ((m_maxVisiblePages & 1) ? 0 : 1), m_pageNum);
            m_minPage = qMax(m_currentPage - half, 1);
        }

        int offset = m_maxVisiblePages - (m_maxPage - m_minPage + 1);
        if (offset > 0)
        {
            if (m_minPage > offset)
            {
                m_minPage -= offset;
            }
            else
            {
                m_maxPage += offset;
            }
        }
    }

    for (int i = m_minPage; i <= m_maxPage; i++)
    {
        QPushButton *btn = page(i - m_minPage);
        if (btn == NULL)
            continue;

        btn->setText(QString::number(i));
        if (i == m_currentPage)
        {
            m_logicalPage = i - m_minPage + 1;
            btn->setChecked(true);
        }
    }
}

bool NBasePageBar::eventFilter(QObject *watched, QEvent *event)
{
    QPushButton *page = dynamic_cast<QPushButton *>(watched);
    if ((page != NULL) && m_pages.contains(page))
    {
        QMouseEvent *mEvent = dynamic_cast<QMouseEvent *>(event);
        if ((mEvent != NULL) && (mEvent->button() == Qt::LeftButton))
            if (page->isChecked())
                return true;
    }

    return QWidget::eventFilter(watched, event);
}
