#ifndef FRAMELESS_HELPER_H
#define FRAMELESS_HELPER_H

#include <QObject>

class FramelessHelperImpl;

class FramelessHelper : public QObject
{
    Q_OBJECT
public:
  explicit FramelessHelper( QObject* parent = 0 );
  ~FramelessHelper();

  void activateOn( QWidget* topLevelWidget );
  void removeFrom( QWidget* topLevelWidget );

  void setWidgetMovable( bool movable );
  bool isWidgetMovable();

  void setWidgetResizable( bool resizable );
  bool isWidgetResizable();

  //Make sure to leave the same content margins
  //around the widget as the newBorderWidth
  //this can be done by
  //yourWidget->layout()->setMargin( newBorderWidth );
  //otherwise your widget will not expose the
  //area where this class works
  void setBorderWidth( int newBorderWidth );
  int borderWidth();

protected:
  virtual bool eventFilter( QObject* obj, QEvent* event );

private:
  FramelessHelperImpl* d;
};

#endif // FRAMELESS_HELPER_H
