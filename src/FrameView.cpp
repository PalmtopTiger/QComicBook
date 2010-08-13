#include "FrameView.h"
#include "ComicBookSettings.h"
#include "Page.h"
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QPainter>
#include <QDebug>

using namespace QComicBook;

FrameView::FrameView(QWidget *parent, int physicalPages, const ViewProperties& props)
    : PageViewBase(parent, physicalPages, props)
	, m_currentPage(0) //??
	, m_currentFrame(0)
	, m_img(0)
{
    QWidget *w = new QWidget(this);
    w->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    m_layout = new QVBoxLayout(w);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->setAlignment(Qt::AlignCenter);
    setWidget(w);

	m_img = new QLabel(this);
	m_layout->addWidget(m_img);

    setWidgetResizable(true);
    
    setBackground(props.background());
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

FrameView::~FrameView()
{

}
			
void FrameView::setImage(const Page &img1)
{
    if (img1.getNumber() == m_currentPage)
	{
		m_page = img1.getImage();
	}
}

void FrameView::setImage(const Page &img1, const Page &img2)
{   
	if (img1.getNumber() == m_currentPage)
	{
		m_page = img1.getImage();
	}
}

void FrameView::setFrames(int page, const QList<ComicFrame> &frames)
{
	if (page == m_currentPage)
	{
		m_frames = frames;
		m_currentFrame = 0;
		gotoFrame(m_currentFrame);
	}
}

void FrameView::nextFrame()
{
	if (m_currentFrame < m_frames.count() - 1)
	{
		++m_currentFrame;
		gotoFrame(m_currentFrame);
	}
	else
	{
		gotoPage(m_currentPage + 1);
	}
}

void FrameView::prevFrame()
{
	if (m_currentFrame > 0)
	{
		--m_currentFrame;
		gotoFrame(m_currentFrame);
	}
	else
	{
		gotoPage(m_currentPage - 1);
	}
}

void FrameView::gotoFrame(int n)
{
	if (n < m_frames.count())
	{
		m_currentFrame = n;
		const ComicFrame f(m_frames.at(n));
		QPixmap pix(f.width(), f.height());
		QPainter paint;
		paint.begin(&pix);
		paint.drawImage(0, 0, m_page, f.xPos(), f.yPos(), f.width(), f.height());
		paint.end();
		m_img->setPixmap(pix);
	}
}

void FrameView::clear()
{
}

void FrameView::gotoPage(int n)
{
	if (n>= 0 && n < numOfPages())
    {
        m_currentPage = n = roundPageNumber(n);

        addRequest(m_currentPage, false);
        emit currentPageChanged(n);

        ComicBookSettings &cfg(ComicBookSettings::instance());
        if (cfg.preloadPages() && n < numOfPages())
        {   
            ++n;
            qDebug() << "preloading" << n;            
            addRequest(n, false);
        }
    }
}

void FrameView::scrollToTop()
{

}

void FrameView::scrollToBottom()
{

}
            
void FrameView::propsChanged()
{

}

int FrameView::visiblePages() const
{
	return 1;
}

int FrameView::viewWidth() const
{

    return width(); //TODO?
}

int FrameView::currentPage() const
{
	return m_currentPage;
}

