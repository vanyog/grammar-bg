#ifndef BGENDIC_H
#define BGENDIC_H

#include <QObject>

class BgEnDictionary : public QObject
{
  Q_OBJECT

public:
  BgEnDictionary(const QString &dn);

  QString translation(const QString &w);

private:
   QString dirName;
};

#endif
