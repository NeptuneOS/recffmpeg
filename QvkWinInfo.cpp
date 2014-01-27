#include "QvkWinInfo.h" 
#include <QCoreApplication>
using namespace std;

QvkWinInfo::QvkWinInfo()
{
  myX = 0;
  myY = 0;
  myWidth = 0;
  myHeight = 0;
  
  setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );

  resize ( 50, 50 );
  
  QRegion Fenster( 0, 
                   0, 
                   50,
                   50,
                   QRegion::Rectangle );

  QRegion mouseHole( 21,
		     21,
		     8,
		     8,
		     QRegion::Rectangle );

  QRegion Rechteck_LO( 0,
		       0,
		       23,
		       23,
                       QRegion::Rectangle );
		       
  QRegion Rechteck_RO( 27,
		       0,
		       23,
		       23,
                       QRegion::Rectangle );
		       
  QRegion Rechteck_LU( 0,
		       27,
		       23,
		       23,
                       QRegion::Rectangle );
		       
  QRegion Rechteck_RU( 27,
		       27,
		       23,
		       23,
                       QRegion::Rectangle );

  QRegion r1 = Fenster.QRegion::subtract( mouseHole );
  QRegion r2 = r1.QRegion::subtract( Rechteck_LO );
  QRegion r3 = r2.QRegion::subtract( Rechteck_RO );
  QRegion r4 = r3.QRegion::subtract( Rechteck_LU );
  QRegion r5 = r4.QRegion::subtract( Rechteck_RU );
  this->setMask( r5 );

  lastWinID = this->winId();
  
  windowTimer = new QTimer( this );
  connect( windowTimer, SIGNAL( timeout() ), this, SLOT( selectWindow() ) );
  windowTimer->start( 500 );
  
  mouseTimer = new QTimer( this );
  connect( mouseTimer, SIGNAL( timeout() ), this, SLOT( mousePosition() ) );
  mouseTimer->start( 20 );

  show();
}


QvkWinInfo::~QvkWinInfo()
{
  
}


void QvkWinInfo::paintEvent( QPaintEvent *event ) 
{
  (void)event;
  QPainter painter( this );
  painter.setPen( QPen( Qt::blue, 4 ) );
  painter.drawLine( 50/2, 0, 50/2, 50);
  painter.drawLine( 0, 50/2, 50, 50/2);
}


void QvkWinInfo::mousePosition()
{
  QCursor cursor;
  move( cursor.pos().x() - 25 , cursor.pos().y() - 25 );
}


void QvkWinInfo::selectWindow()
{
  newWinID = QxtWindowSystem::activeWindow();

  if ( lastWinID != newWinID )
  {
    windowTimer->stop();
    mouseTimer->stop();
    myX = QxtWindowSystem::windowGeometry( newWinID ).x();
    myY = QxtWindowSystem::windowGeometry( newWinID ).y();
    myWidth = QxtWindowSystem::windowGeometry( newWinID ).width();
    myHeight = QxtWindowSystem::windowGeometry( newWinID ).height();
    
    // Cursor resize does not show in video in the first Frames
    resize( 10, 10 );
    
    emit windowChanged();
    this->close();
    delete this;
  }
}


QString QvkWinInfo::x()
{
  return QString::number( myX );
}


QString QvkWinInfo::y()
{
  return QString::number( myY );
}


QString QvkWinInfo::width()
{
  return QString::number( myWidth );
}


QString QvkWinInfo::height()
{
  return QString::number( myHeight );
}


QString QvkWinInfo::WinID()
{
  return QString().number( newWinID );
}


WId QvkWinInfo::getWinID()
{
  return newWinID;
}

