#include "SeanceManagerImpl.h"

#include <QDebug>

namespace NSApplication {
namespace NSKernel {

CSeanceManagerImpl::CSeanceManagerImpl()
    : KeyPressingInput_(
          [this](const CKeyPressing& KeyPressing) { handle(KeyPressing); }),
      KeyReleasingInput_(
          [this](const CKeyReleasing& KeyReleasing) { handle(KeyReleasing); }),
      CurrentSeanceOutput_(
          [this]() -> CSeanceGetType { return CurrentSeance_; }) {
}

NSLibrary::CObserver<CSeanceManagerImpl::CKeyPressing>*
CSeanceManagerImpl::pressingInput() {
  return &KeyPressingInput_;
}

NSLibrary::CObserver<CSeanceManagerImpl::CKeyReleasing>*
CSeanceManagerImpl::releasingInput() {
  return &KeyReleasingInput_;
}

void CSeanceManagerImpl::subscribeToCurrentSeance(CSeanceObserver* observer) {
  CurrentSeanceOutput_.subscribe(observer);
}

void CSeanceManagerImpl::makeSessions() {
  SeanceMaker_.transferTo(&CurrentSeance_);
  qDebug() << "CurrentSeance_.size() = " << CurrentSeance_.size();
  CurrentSeanceOutput_.notify();
}

void CSeanceManagerImpl::handle(const CKeyPressing& KeyPressing) {
  SeanceMaker_.add(KeyPressing);
}

void CSeanceManagerImpl::handle(const CKeyReleasing& KeyReleasing) {
  SeanceMaker_.add(KeyReleasing);
}

} // namespace NSKernel
} // namespace NSApplication