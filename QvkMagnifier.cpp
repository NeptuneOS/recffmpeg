#include "QvkMagnifier.h"

using namespace std;


// Lupe Rund, Qadratisch, Oval

QvkMagnifier::QvkMagnifier()
{
  faktor = 2;
  label = new QLabel( this );

  Magnifier400x200();

  resize( 2 * distanceX * faktor, 2 * distanceY * faktor );
  setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip ); //With tooltip, no entry in Taskbar
  setWindowTitle(tr("Magnifier"));
  border = 3;

  label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
  label->setAlignment( Qt::AlignCenter );
  label->setScaledContents( true );
  label->show();

  QTimer *timer = new QTimer( this );
  connect( timer, SIGNAL( timeout() ), this, SLOT( mytimer() ) );
  timer->start( 40 );
}


void QvkMagnifier::closeEvent( QCloseEvent * event )
{
  (void)event;
  emit closeMagnifier();
}


void QvkMagnifier::getDialogMagnifier( QWidget *parent )
{
  QDialog *dialog = new QDialog( parent );

  QFont qfont = dialog->font();
  qfont.setPixelSize( 12 );
  dialog->setFont( qfont );
  dialog->setWindowTitle(tr("Magnifier Settings"));

  QLabel* label = new QLabel( dialog );
  label->setText("");
  label->setGeometry( QRect( 20, 30, 120, 150) );
  label->setAlignment( Qt::AlignCenter );
  label->show();
  QImage* qImage = new QImage( ":/pictures/magnifier.png" );
  label->setPixmap( QPixmap::fromImage( *qImage, Qt::AutoColor ) );
//  label->setScaledContents( true );

  radioButton1 = new QRadioButton( dialog );
  radioButton1->setGeometry( 170, 50, 200, 21 );
  radioButton1->setText( "200 x 200" );
  radioButton1->show();
  connect( radioButton1, SIGNAL( clicked() ), SLOT( Magnifier200x200() ) );
  if ( formValue == 1 )
    radioButton1->setChecked( true );

  radioButton2 = new QRadioButton( dialog );
  radioButton2->setGeometry( 170, 80, 200, 21 );
  radioButton2->setText( "400 x 200" );
  radioButton2->show();
  connect( radioButton2, SIGNAL( clicked() ), SLOT( Magnifier400x200() ) );
  if ( formValue == 2 )
    radioButton2->setChecked( true );

  radioButton3 = new QRadioButton( dialog );
  radioButton3->setGeometry( 170, 110, 200, 21 );
  radioButton3->setText( "600 x 200" );
  radioButton3->show();
  connect( radioButton3, SIGNAL( clicked() ), SLOT( Magnifier600x200() ) );
  if ( formValue == 3 )
    radioButton3->setChecked( true );

  QPropertyAnimation *animation = new QPropertyAnimation( dialog, "geometry");
  animation->setDuration( 1000 );
  animation->setStartValue( QRect( parent->x() + parent->width()/2,
                                   parent->y() + parent->height()/2,
                                   0,
                                   0 ) );

  animation->setEndValue( QRect( ( parent->x() ) + ( ( parent->width()/2 ) - ( 300/2 ) ), 
                                 ( parent->y() ) + ( ( parent->height()/2) - ( 200/2 ) ),
                                   300,
                                   200 ) );

  animation->setEasingCurve( QEasingCurve::OutBounce );
  animation->start();  
  
  //dialog->setFixedSize( 300, 200 );

  dialog->exec();
}


void QvkMagnifier::Magnifier200x200()
{
  distanceX = 50;
  distanceY = 50;
  resize( 2 * distanceX * faktor, 2 * distanceY * faktor );
  label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
  formValue = 1;
}


void QvkMagnifier::Magnifier400x200()
{
  distanceX = 100;
  distanceY = 50;
  resize( 2 * distanceX * faktor, 2 * distanceY * faktor );
  label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
  formValue = 2;
}


void QvkMagnifier::Magnifier600x200()
{
  distanceX = 150;
  distanceY = 50;
  resize( 2 * distanceX * faktor, 2 * distanceY * faktor );
  label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
  formValue = 3;
}



int QvkMagnifier::getDistanceX()
{
  return distanceX;
}


int QvkMagnifier::getDistanceY()
{
  return distanceY;
}


int QvkMagnifier::NewDistanceXLeft()
{
  QCursor cursor;
  QDesktopWidget *desk = QApplication::desktop();

  int newDistanceX = ( ( desk->screenGeometry().width() / 2 ) - cursor.pos().x() ) *
                 ( distanceX + ( width() / 2 ) ) /
                 ( desk->screenGeometry().width() / 2 - distanceX ) -
                 ( width() / 2 );
  return  newDistanceX;
}


int QvkMagnifier::NewDistanceXRight()
{
  QCursor cursor;
  QDesktopWidget *desk = QApplication::desktop();

  int newDistanX = ( ( desk->screenGeometry().width() / 2 ) - cursor.pos().x() ) *
                 ( -distanceX - ( width() / 2 ) ) /
                 ( desk->screenGeometry().width() / 2 - distanceX ) -
                 ( width() / 2 );
  return  newDistanX;
}


QvkMagnifier::~QvkMagnifier()
{
}

void QvkMagnifier::setMagnifier()
{
  QCursor cursor;
  QDesktopWidget *desk = QApplication::desktop();


  // Lupe an oberen linke Ecke setzen
  if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() <  distanceY ) )
  {
    move( 2 * distanceX,  2 * distanceY );
    return;
  }

  // Lupe obere rechte Ecke setzen
  if ( ( cursor.pos().x() > ( desk->screenGeometry().width() - distanceX ) ) and ( cursor.pos().y() < distanceY ) )
  {
    move( desk->screenGeometry().width() - 2 * distanceX - width(), 2 * distanceY);
    return;
  }

  // Lupe am oberen Rand setzen
  // Linke Hälfte am oberen Rand
  if ( ( cursor.pos().y() < distanceY ) and ( cursor.pos().x() < desk->screenGeometry().width() / 2 ) )
  {
    move( cursor.pos().x() + NewDistanceXLeft(), 2 * distanceY );
    return;
  }
  // Rechte Hälfte am oberen Rand
  if ( ( cursor.pos().y() < distanceY ) and ( cursor.pos().x() > desk->screenGeometry().width() / 2 ) )
  {
    move( cursor.pos().x() - NewDistanceXRight() - width(), 2 * distanceY );
    return;
  }

  // Lupe an untere rechte Ecke setzen
  if ( ( cursor.pos().x() > desk->screenGeometry().width() - distanceX ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanceY ) )
  {
      move( desk->screenGeometry().width() - ( 2 * distanceX + width() ), desk->screenGeometry().height() - ( 2 * distanceY + height() ) );
      return;
  }

  // Lupe am rechten Rand setzen
  // Obere Hälfte am rechten Rand
  if ( ( cursor.pos().x() > desk->screenGeometry().width() - distanceX ) and ( cursor.pos().y() < desk->screenGeometry().height() / 10 * 8 ) )// div 2
  {
    move( desk->screenGeometry().width() - ( 2 * distanceX + width() ), cursor.pos().y() + 1 * distanceY );
    return;
  }
  // untere Hälfte am rechten Rand
  if ( ( cursor.pos().x() > desk->screenGeometry().width() - distanceX ) and ( cursor.pos().y() > desk->screenGeometry().height() / 10 * 8 ) )
  {
    move( desk->screenGeometry().width() - ( 2 * distanceX + width() ), cursor.pos().y() - distanceY - height() );
    return;
  }

  // Lupe an linken unteren Ecke setzen
  if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanceY ) )
  {
    move( 2 * distanceX, desk->screenGeometry().height() - 2 * distanceY - height() );
    return;
  }

  // Lupe am unteren Rand setzen
  // Linke Hälfte unterer Rand
  if ( ( cursor.pos().x() < desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanceY ) )
  {
    move( cursor.pos().x() + NewDistanceXLeft(), desk->screenGeometry().height() - ( 2 * distanceY + height() ) );
    return;
  }
  // Rechte Hälfte unterer Rand
  if ( ( cursor.pos().x() > desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanceY ) )
  {
    move( cursor.pos().x() - NewDistanceXRight() - width(), desk->screenGeometry().height() - 2 * distanceY - height() );
    return;
  }

  // Lupe am linken Rand setzen
  // Obere Hälfte am linken Rand
  if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() < desk->screenGeometry().height() / 10 * 8 ) ) // div 2

  {
    move( 2 * distanceX, cursor.pos().y() + distanceY );
    return;
  }
  // Untere Hälfte am linken Rand
  if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() > desk->screenGeometry().height() / 10 * 8 ) )
  {
    move( 2 * distanceX, cursor.pos().y() - distanceY - height() );
    return;
  }

  // Linke obere Hälfte
  if ( ( cursor.pos().x() < desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() < desk->screenGeometry().height() / 10 * 8 ) ) // div 2
    move( cursor.pos().x() + NewDistanceXLeft(), cursor.pos().y() + distanceY );

  // Rechte obere Hälfte
  if ( ( cursor.pos().x() > desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() < desk->screenGeometry().height() / 10 * 8 ) )
    move( cursor.pos().x() - NewDistanceXRight() - width(), cursor.pos().y() + distanceY );

  // Linke untere Hälfte
  if ( ( cursor.pos().x() < desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() > desk->screenGeometry().height() / 10 * 8 ) )
    move( cursor.pos().x() + NewDistanceXLeft(), cursor.pos().y() - distanceY - height() );

  // Rechte untere Hälfte
  if ( ( cursor.pos().x() > desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() > desk->screenGeometry().height() / 10 * 8 ) )
    move( cursor.pos().x() - NewDistanceXRight() -width(), cursor.pos().y() - distanceY - width() );
}


void QvkMagnifier::mytimer()
{
  QCursor cursor;
  QDesktopWidget *desk = QApplication::desktop();
//    Qt::Window | Qt::WindowStaysOnTopHint

  setMagnifier();

  // Obere linke Ecke
  if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() <  distanceY ) )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(),
					  0,
					  0,
					  2 * distanceX ,
					  2 * distanceY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Obere rechte Ecke
  if ( ( cursor.pos().x() > ( desk->screenGeometry().width() - distanceX ) ) and ( cursor.pos().y() < distanceY ) )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(),
					  desk->screenGeometry().width() - 2 * distanceX,
					  0,
					  2 * distanceX ,
					  2 * distanceY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Linke untere Ecke
  if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanceY ) )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(),
					  0,
					  desk->screenGeometry().height() - 2 * distanceY,
					  2 * distanceX ,
					  2 * distanceY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Rechte untere Ecke
  if ( ( cursor.pos().x() > desk->screenGeometry().width() - distanceX ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanceY ) )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(),
					  desk->screenGeometry().width() - 2 * distanceX,
					  desk->screenGeometry().height() - 2 * distanceY,
					  2 * distanceX ,
					  2 * distanceY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Unterer Rand
  if ( cursor.pos().y() > desk->screenGeometry().height() - distanceY )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(),
					  cursor.pos().x() - distanceX,
					  desk->screenGeometry().height() - 2 * distanceY,
					  2 * distanceX ,
					  2 * distanceY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Oberen Rand
  if ( cursor.pos().y() < distanceY )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(),
					  cursor.pos().x() - distanceX,
					  0,
					  2 * distanceX,
					  2 * distanceY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Rechter Rand
  if ( cursor.pos().x() > desk->screenGeometry().width() - distanceX )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(),
					  desk->screenGeometry().width() - 2 * distanceX,
					  cursor.pos().y() - distanceY,
					  2 * distanceX ,
					  2 * distanceY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Linker Rand
  if ( cursor.pos().x() < distanceX )
  {
    QPixmap originalPixmap = QPixmap();
    originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(),
					  0,
					  cursor.pos().y() - distanceY,
					  2 * distanceX ,
					  2 * distanceY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Fläche
  QPixmap originalPixmap = QPixmap();
  originalPixmap = QPixmap::grabWindow( QApplication::desktop()->winId(),
					cursor.pos().x() - distanceX,
					cursor.pos().y() - distanceY,
					2 * distanceX ,
					2 * distanceY );
  label->setPixmap( originalPixmap );
}
