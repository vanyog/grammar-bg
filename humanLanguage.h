#ifndef HUMANLANGUAGE_H
#define HUMANLANGUAGE_H

#include <QObject>
#include <QSet>
#include <QChar>

enum HumanLanguageID { bulgarian };

class HumanLanguage : public QObject
{
  Q_OBJECT
public:
  HumanLanguage(HumanLanguageID id);
  
  bool isLetter(QChar c);
  bool isPMark(QChar c);
  QChar nextPMark(const QString &s, int i=0);

private:
  QSet<QChar> letters;
  QSet<QChar> pMarks;
};

#endif
