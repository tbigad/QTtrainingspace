#include <QtWidgets>
#include <QMouseEvent>
#include <QMutex>
#include "FramelessHelper.h"

class CursorPosCalculator
{
public:
  CursorPosCalculator();
  void reset();
  void recalculate( const QPoint& globalMousePos, const QRect& frameRect );

public:
  bool onEdges;
  bool onLeftEdge;
  bool onRightEdge;
  bool onTopEdge;
  bool onBottomEdge;
  bool onTopLeftEdge;
  bool onBottomLeftEdge;
  bool onTopRightEdge;
  bool onBottomRightEdge;

  static int mBorderWidth;
};

int CursorPosCalculator::mBorderWidth = 5;

class WidgetData
{
public:
  WidgetData( FramelessHelperImpl* _d, QWidget* topLevelWidget );
  ~WidgetData();

  QWidget* widget();
  void handleWidgetEvent( QEvent* event );

private:
  void updateCursorShape( const QPoint& globalMousePos );
  void resizeWidget( const QPoint& globalMousePos );
  void moveWidget( const QPoint& globalMousePos );

  void handleMousePressEvent( QMouseEvent* event );
  void handleMouseReleaseEvent( QMouseEvent* event );
  void handleMouseMoveEvent( QMouseEvent* event );
  void handleLeaveEvent( QEvent* event );
  void handleHoverMoveEvent( QHoverEvent* event );

private:
  FramelessHelperImpl* d;
  bool mLeftButtonPressed;
  QWidget* mWidget;
  QPoint mDragPos;
  CursorPosCalculator mPressedMousePos;
  CursorPosCalculator mMoveMousePos;
  bool mCursorShapeChanged;
  Qt::WindowFlags mWindowFlags;
};

class FramelessHelperImpl
{
public:
  QHash< QWidget*, WidgetData* > mHashWidgetData;
  bool mWidgetMovable;
  bool mWidgetResizable;
};

CursorPosCalculator::CursorPosCalculator()
{
  reset();
}

void CursorPosCalculator::reset()
{
  onEdges = false;
  onLeftEdge = false;
  onRightEdge = false;
  onTopEdge = false;
  onBottomEdge = false;
  onTopLeftEdge = false;
  onBottomLeftEdge = false;
  onTopRightEdge = false;
  onBottomRightEdge = false;
}

void CursorPosCalculator::recalculate( const QPoint& globalMousePos, const QRect& frameRect )
{
  int globalMouseX = globalMousePos.x();
  int globalMouseY = globalMousePos.y();

  int frameX = frameRect.x();
  int frameY = frameRect.y();

  int frameWidth = frameRect.width();
  int frameHeight = frameRect.height();

  onLeftEdge = globalMouseX >= frameX &&
      globalMouseX <= frameX + mBorderWidth;

  onRightEdge = globalMouseX >= frameX + frameWidth - mBorderWidth &&
      globalMouseX <= frameX + frameWidth;

  onTopEdge = globalMouseY >= frameY &&
      globalMouseY <= frameY + mBorderWidth;

  onBottomEdge = globalMouseY >= frameY + frameHeight - mBorderWidth &&
      globalMouseY <= frameY + frameHeight;

  onTopLeftEdge = onTopEdge && onLeftEdge;
  onBottomLeftEdge = onBottomEdge && onLeftEdge;
  onTopRightEdge = onTopEdge && onRightEdge;
  onBottomRightEdge = onBottomEdge && onRightEdge;

  //only these checks would be enough
  onEdges = onLeftEdge || onRightEdge ||
      onTopEdge || onBottomEdge;
}

WidgetData::WidgetData( FramelessHelperImpl* _d, QWidget* topLevelWidget )
{
  d = _d;
  mWidget = topLevelWidget;
  mLeftButtonPressed = false;
  mCursorShapeChanged = false;

  mWindowFlags = mWidget->windowFlags();

  //---from Qt docs of setWindowFlags()----
  //Note: This function calls setParent() when
  //changing the flags for a window, causing the
  //widget to be hidden. You must call show()
  //to make the widget visible again..

  bool visible = mWidget->isVisible();

  mWidget->setMouseTracking( true );
  mWidget->setWindowFlags( Qt::CustomizeWindowHint|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::NoDropShadowWindowHint);
  //Bug fix, mouse move events does not propagate from child widgets.
  //so need the hover events.
  mWidget->setAttribute( Qt::WA_Hover );

  mWidget->setVisible( visible );
}

WidgetData::~WidgetData()
{
  //---from Qt docs of setWindowFlags()----
  //Note: This function calls setParent() when
  //changing the flags for a window, causing the
  //widget to be hidden. You must call show()
  //to make the widget visible again..

  bool visible = mWidget->isVisible();

  mWidget->setMouseTracking( false );
  mWidget->setWindowFlags( mWindowFlags );//^  Qt::CustomizeWindowHint ^ Qt::FramelessWindowHint );
  mWidget->setAttribute( Qt::WA_Hover, false );

  mWidget->setVisible( visible );
}

QWidget* WidgetData::widget()
{
  return mWidget;
}

void WidgetData::handleWidgetEvent( QEvent* event )
{
  switch ( event->type() )
  {
  default: //qDebug() << "Event = " << event;
    break;
  case QEvent::MouseButtonPress:
    handleMousePressEvent( static_cast<QMouseEvent*>( event ) );
    break;

  case QEvent::MouseButtonRelease:
    handleMouseReleaseEvent( static_cast<QMouseEvent*>( event ) );
    break;

  case QEvent::MouseMove:
    handleMouseMoveEvent( static_cast<QMouseEvent*>( event ) );
    break;

  case QEvent::Leave:
    handleLeaveEvent( event );
    break;

    //Bug fix, hover event is necessary coz child widget does not
    //propagate mousemove events. so the cursor remains in edge shape
    //even in middle of widget.
  case QEvent::HoverMove:
    handleHoverMoveEvent( static_cast<QHoverEvent*>( event ) );
    break;
    //case QEvent::Enter:
    //qDebug() << "Enter event";//d->handleEnterEvent( event );
    //break;
  }
}

void WidgetData::updateCursorShape( const QPoint& globalMousePos )
{
  if ( mWidget->isFullScreen() || mWidget->isMaximized())
  {
    if ( mCursorShapeChanged )
      mWidget->unsetCursor();

    return;
  }

  mMoveMousePos.recalculate( globalMousePos, mWidget->frameGeometry() );

  if( mMoveMousePos.onTopLeftEdge || mMoveMousePos.onBottomRightEdge )
  {
    mWidget->setCursor( Qt::SizeFDiagCursor );
    mCursorShapeChanged = true;
  }
  else if( mMoveMousePos.onTopRightEdge || mMoveMousePos.onBottomLeftEdge )
  {
    mWidget->setCursor( Qt::SizeBDiagCursor );
    mCursorShapeChanged = true;
  }
  else if( mMoveMousePos.onLeftEdge || mMoveMousePos.onRightEdge )
  {
    mWidget->setCursor( Qt::SizeHorCursor );
    mCursorShapeChanged = true;
  }
  else if( mMoveMousePos.onTopEdge || mMoveMousePos.onBottomEdge )
  {
    mWidget->setCursor( Qt::SizeVerCursor );
    mCursorShapeChanged = true;
  }
  else
  {
    if ( mCursorShapeChanged )
    {
      mWidget->unsetCursor();
      mCursorShapeChanged = false;
    }
  }
}

void WidgetData::resizeWidget( const QPoint& globalMousePos )
{
  if ( mWidget->isFullScreen() || mWidget->isMaximized()) {
    return;
  }

  QRect origRect;

  origRect = mWidget->frameGeometry();

  int left = origRect.left();
  int top = origRect.top();
  int right = origRect.right();
  int bottom = origRect.bottom();
  origRect.getCoords( &left, &top, &right, &bottom );

  int minWidth = mWidget->minimumWidth();
  int minHeight = mWidget->minimumHeight();

  if ( mPressedMousePos.onTopLeftEdge )
  {
    left = globalMousePos.x();
    top = globalMousePos.y();
  }
  else
  if ( mPressedMousePos.onBottomLeftEdge )
  {
    left = globalMousePos.x();
    bottom = globalMousePos.y();
  }
  else if ( mPressedMousePos.onTopRightEdge )
  {
    right = globalMousePos.x();
    top = globalMousePos.y();
  }
  else if ( mPressedMousePos.onBottomRightEdge )
  {
    right = globalMousePos.x();
    bottom = globalMousePos.y();
  }
  else if ( mPressedMousePos.onLeftEdge )
  {
    left = globalMousePos.x();
  }
  else if ( mPressedMousePos.onRightEdge )
  {
    right = globalMousePos.x();
  }
  else if ( mPressedMousePos.onTopEdge )
  {
    top = globalMousePos.y();
  }
  else if ( mPressedMousePos.onBottomEdge )
  {
    bottom = globalMousePos.y();
  }

  QRect newRect( QPoint(left, top), QPoint(right, bottom) );

  if ( newRect.isValid() )
  {
    if ( minWidth > newRect.width() )
    {
      //determine what has caused the width change.
      if( left != origRect.left() )
        newRect.setLeft( origRect.left() );
      else
        newRect.setRight( origRect.right() );
    }
    if ( minHeight > newRect.height() )
    {
      //determine what has caused the height change.
      if ( top != origRect.top() )
        newRect.setTop( origRect.top() );
      else
        newRect.setBottom( origRect.bottom() );
    }

    mWidget->setGeometry( newRect );
  }
  else
  {
    //qDebug() << "Calculated Rect is not valid" << newRect;
  }
}

void WidgetData::moveWidget( const QPoint& globalMousePos )
{
    mWidget->move( globalMousePos - mDragPos );
}

void WidgetData::handleMousePressEvent( QMouseEvent* event )
{
  if ( event->button() == Qt::LeftButton )
  {
    mLeftButtonPressed = true;

    QRect frameRect = mWidget->frameGeometry();
    mPressedMousePos.recalculate( event->globalPos(), frameRect );

    mDragPos = event->globalPos() - frameRect.topLeft();
  }
}

void WidgetData::handleMouseReleaseEvent( QMouseEvent* event )
{
  if ( event->button() == Qt::LeftButton )
  {
    mLeftButtonPressed = false;
    mPressedMousePos.reset();
  }
}

void WidgetData::handleMouseMoveEvent( QMouseEvent* event )
{
  if ( mLeftButtonPressed )
  {
    if ( d->mWidgetResizable && mPressedMousePos.onEdges )
    {
      resizeWidget( event->globalPos() );
    }
    else if ( d->mWidgetMovable )
    {
      moveWidget( event->globalPos() );
    }
  }
  else if ( d->mWidgetResizable )
  {
    updateCursorShape( event->globalPos() );
  }
}

void WidgetData::handleLeaveEvent( QEvent* /*event*/ )
{
  if ( !mLeftButtonPressed )
    mWidget->unsetCursor();
}

void WidgetData::handleHoverMoveEvent( QHoverEvent* event )
{
  if ( d->mWidgetResizable )
    updateCursorShape( mWidget->mapToGlobal( event->pos() ) );
}

FramelessHelper::FramelessHelper( QObject* parent )
  : QObject( parent ),
    d( new FramelessHelperImpl )
{
  d->mWidgetMovable = true;
  d->mWidgetResizable = true;
}

FramelessHelper::~FramelessHelper()
{
  QList<QWidget*> keys = d->mHashWidgetData.keys();
  int size = keys.size();
  for ( int i = 0; i < size; ++i )
  {
    delete d->mHashWidgetData.take( keys[i] );
  }

  delete d;
}

bool FramelessHelper::eventFilter( QObject *obj, QEvent *event )
{
  QEvent::Type type = event->type();

  if ( type == QEvent::MouseMove ||
       type == QEvent::HoverMove ||
       type == QEvent::MouseButtonPress ||
       type == QEvent::MouseButtonRelease ||
       type == QEvent::Leave
     )
 {
   WidgetData* data = d->mHashWidgetData.value( static_cast<QWidget*>(obj) );
   if ( data )
   {
     data->handleWidgetEvent( event );
   }
 }
  return false;
}

void FramelessHelper::activateOn( QWidget* topLevelWidget )
{
  if ( d->mHashWidgetData.contains( topLevelWidget ) )
    return;

  WidgetData* data = new WidgetData( d, topLevelWidget );
  d->mHashWidgetData.insert( topLevelWidget, data );

  topLevelWidget->installEventFilter( this );
}

void FramelessHelper::removeFrom( QWidget* topLevelWidget )
{
  WidgetData* data = d->mHashWidgetData.take( topLevelWidget );
  if ( data )
  {
    topLevelWidget->removeEventFilter( this );
    delete data;
  }
}

void FramelessHelper::setWidgetMovable( bool movable )
{
  d->mWidgetMovable = movable;
}

bool FramelessHelper::isWidgetMovable()
{
  return d->mWidgetMovable;
}

void FramelessHelper::setWidgetResizable( bool resizable )
{
  d->mWidgetResizable = resizable;
}

bool FramelessHelper::isWidgetResizable()
{
  return d->mWidgetResizable;
}

void FramelessHelper::setBorderWidth( int newBorderWidth )
{
  if ( newBorderWidth >= 0 )
    CursorPosCalculator::mBorderWidth = newBorderWidth;
}

int FramelessHelper::borderWidth()
{
  return CursorPosCalculator::mBorderWidth;
}
