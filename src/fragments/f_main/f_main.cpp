#include <QMenuBar>
#include <QMessageBox>
#include <QApplication>
#include "fragments/f_main/f_main.hpp"
#include "fragments/f_main/f_main_spec.hpp"
#include "utils.hpp"
#include "exception.hpp"
#include "effects.hpp"
#include "event_system.hpp"


using std::string;


//===========================================
// FMain::FMain
//===========================================
FMain::FMain(const CommonFragData& commonData)
  : QMainWindow(nullptr),
    Fragment("FMain", m_data, commonData) {

  DBG_PRINT("FMain::FMain\n");

  setWindowTitle("Pro Office Calculator");

  m_data.wgtCentral = makeQtObjPtr<QWidget>();
  setCentralWidget(m_data.wgtCentral.get());

  m_data.wgtCentral->setLayout(m_data.box.get());

  menuBar()->setNativeMenuBar(false);
  m_data.mnuFile = makeQtObjPtrFromRawPtr(menuBar()->addMenu("File"));
  m_data.actQuit = makeQtObjPtr<QAction>("Quit", this);
  m_data.mnuFile->addAction(m_data.actQuit.get());

  m_data.mnuHelp = makeQtObjPtrFromRawPtr(menuBar()->addMenu("Help"));
  m_data.actAbout = makeQtObjPtr<QAction>("About", this);
  m_data.mnuHelp->addAction(m_data.actAbout.get());

  connect(m_data.actAbout.get(), SIGNAL(triggered()), this, SLOT(showAbout()));
  connect(m_data.actQuit.get(), SIGNAL(triggered()), this, SLOT(close()));
}

//===========================================
// FMain::reload
//===========================================
void FMain::reload(const FragmentSpec& spec_) {
  DBG_PRINT("FMain::reload\n");

  auto& spec = dynamic_cast<const FMainSpec&>(spec_);

  setFixedSize(spec.width, spec.height);

  m_data.mnuFile->setTitle(spec.fileLabel);
  m_data.actQuit->setText(spec.quitLabel);

  setColour(*this, spec.bgColour, QPalette::Window);
  setWindowTitle(spec.windowTitle);

  if (spec.backgroundImage.length() > 0) {
    setStyleSheet(QString("") +
      "QMainWindow {"
      "  background-image: url(\"" + spec.backgroundImage + "\");"
      "}");
  }
  else {
    setStyleSheet("");
  }

  m_aboutDialogTitle = spec.aboutDialogTitle;
  m_aboutDialogText = spec.aboutDialogText;

  m_data.mnuHelp->setTitle(spec.helpLabel);
  m_data.actAbout->setText(spec.aboutLabel);
}

//===========================================
// FMain::showAbout
//===========================================
void FMain::showAbout() {
  QMessageBox msgBox(this);
  msgBox.setTextFormat(Qt::RichText);
  msgBox.setWindowTitle(m_aboutDialogTitle);
  msgBox.setText(m_aboutDialogText);
  msgBox.exec();
}

//===========================================
// FMain::closeEvent
//===========================================
void FMain::closeEvent(QCloseEvent*) {
  m_data.fnOnQuit();
  DBG_PRINT("Quitting\n");

  commonData.eventSystem.fire(pEvent_t(new Event("quit")));
}

//===========================================
// FMain::cleanUp
//===========================================
void FMain::cleanUp() {
  DBG_PRINT("FMain::cleanUp\n");
}

//===========================================
// FMain::~FMain
//===========================================
FMain::~FMain() {
  DBG_PRINT("FMain::~FMain()\n");
}
