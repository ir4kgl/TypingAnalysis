#include "SeanceMaker.h"

#include <QDebug>


namespace NSApplication {
namespace NSKernel {

void CSeanceMaker::add(const CKeyPressing& KeyPressing) {
  assert(LastEvent_ <= KeyPressing.Time);
  if (needNewSession(KeyPressing))
    openNewSession();
  CurrentSession().emplace_back(KeyPressing);
  auto It = findKey(KeyPressing.KeyPosition);
  if (It == PressedKeys_.end())
    PressedKeys_.push_back(&CurrentSession().back());
  LastEvent_ = KeyPressing.Time;
  qDebug() << QString("seances = %1 current size = %2 pressed = %3")
           .arg(RawSeance_.size(), 4, 10)
           .arg(CurrentSession().size(), 4, 10)
           .arg(PressedKeys_.size(), 4, 10);
}

void CSeanceMaker::add(const CKeyReleasing& KeyReleasing) {
  assert(LastEvent_ <= KeyReleasing.Time);
  auto It = findKey(KeyReleasing.KeyPosition);
  if (It != PressedKeys_.end()) {
    (*It)->setReleasingTime(KeyReleasing.Time);
    PressedKeys_.erase(It);
    LastEvent_ = KeyReleasing.Time;
  }
  qDebug() << QString("seances = %1 current size = %2 pressed = %3")
           .arg(RawSeance_.size(), 4, 10)
           .arg(CurrentSession().size(), 4, 10)
           .arg(PressedKeys_.size(), 4, 10);
}

bool CSeanceMaker::needNewSession(const CKeyPressing& KeyPressing) const {
  if (!PressedKeys_.empty())
    return false;
  if (CurrentSession().empty())
    return false;
  if (!TimeLimit_.has_value() || KeyPressing.Time - LastEvent_ < *TimeLimit_)
    return false;
  return true;
}

void CSeanceMaker::openNewSession() {
  assert(PressedKeys_.empty());
  assert(!CurrentSession().empty());
  RawSeance_.emplace_back();
}

CSeanceMaker::CPressedKeys::iterator
CSeanceMaker::findKey(CKeyPosition KeyPosition) {
  return std::find_if(PressedKeys_.begin(),
                      PressedKeys_.end(),
  [KeyPosition](CKeyEvent* KeyEvent) {
    return KeyEvent->getPosition() == KeyPosition;
  });
}

CSeanceMaker::CRawSession& CSeanceMaker::CurrentSession() {
  assert(!RawSeance_.empty());
  return RawSeance_.back();
}

const CSeanceMaker::CRawSession& CSeanceMaker::CurrentSession() const {
  assert(!RawSeance_.empty());
  return RawSeance_.back();
}

} // NSKernel
} // NSApplication
