#include "qt/osm_auth_dialog.hpp"

#include "editor/osm_auth.hpp"

#include "platform/settings.hpp"

#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

using namespace osm;

namespace qt
{

char const * kTokenKeySetting = "OsmTokenKey";
char const * kTokenSecretSetting = "OsmTokenSecret";
char const * kLoginDialogTitle = "OpenStreetMap Login Dialog";
char const * kLogoutDialogTitle = "Logout Dialog";

OsmAuthDialog::OsmAuthDialog(QWidget * parent)
{
  string key, secret;
  Settings::Get(kTokenKeySetting, key);
  Settings::Get(kTokenSecretSetting, secret);

  bool const isLoginDialog = key.empty() || secret.empty();

  QVBoxLayout * vLayout = new QVBoxLayout(parent);

  QHBoxLayout * loginRow = new QHBoxLayout();
  loginRow->addWidget(new QLabel(tr("UserName or EMail:")));
  QLineEdit * loginLineEdit = new QLineEdit();
  loginLineEdit->setObjectName("login");
  if (!isLoginDialog)
    loginLineEdit->setEnabled(false);
  loginRow->addWidget(loginLineEdit);
  vLayout->addLayout(loginRow);

  QHBoxLayout * passwordRow = new QHBoxLayout();
  passwordRow->addWidget(new QLabel(tr("Password:")));
  QLineEdit * passwordLineEdit = new QLineEdit();
  passwordLineEdit->setEchoMode(QLineEdit::Password);
  passwordLineEdit->setObjectName("password");
  if (!isLoginDialog)
    passwordLineEdit->setEnabled(false);
  passwordRow->addWidget(passwordLineEdit);
  vLayout->addLayout(passwordRow);

  QPushButton * actionButton = new QPushButton(isLoginDialog ? tr("Login") : tr("Logout"));
  actionButton->setObjectName("button");
  connect(actionButton, SIGNAL(clicked()), this, SLOT(OnAction()));

  QPushButton * closeButton = new QPushButton(tr("Close"));
  connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

  QHBoxLayout * buttonsLayout = new QHBoxLayout();
  buttonsLayout->addWidget(closeButton);
  buttonsLayout->addWidget(actionButton);
  vLayout->addLayout(buttonsLayout);

  setLayout(vLayout);
  setWindowTitle(isLoginDialog ? tr(kLoginDialogTitle) : tr(kLogoutDialogTitle));
}

void SwitchToLogin(QDialog * dlg)
{
  dlg->findChild<QLineEdit *>("login")->setEnabled(true);
  dlg->findChild<QLineEdit *>("password")->setEnabled(true);
  dlg->findChild<QPushButton *>("button")->setText(dlg->tr("Login"));
  dlg->setWindowTitle(dlg->tr(kLoginDialogTitle));
}

void SwitchToLogout(QDialog * dlg)
{
  dlg->findChild<QLineEdit *>("login")->setEnabled(false);
  dlg->findChild<QLineEdit *>("password")->setEnabled(false);
  dlg->findChild<QPushButton *>("button")->setText(dlg->tr("Logout"));
  dlg->setWindowTitle(dlg->tr(kLoginDialogTitle));
}

void OsmAuthDialog::OnAction()
{
  bool const isLoginDialog = findChild<QPushButton *>("button")->text() == tr("Login");
  if (isLoginDialog)
  {
    string const login = findChild<QLineEdit *>("login")->text().toStdString();
    string const password = findChild<QLineEdit *>("password")->text().toStdString();

    if (login.empty())
    {
      setWindowTitle("Please enter Login");
      return;
    }

    if (password.empty())
    {
      setWindowTitle("Please enter Password");
      return;
    }

    // TODO(AlexZ): Change to production server.
    OsmOAuth auth = osm::OsmOAuth::DevServerAuth();
    OsmOAuth::AuthResult const res = auth.AuthorizePassword(login, password);
    if (res != OsmOAuth::AuthResult::OK)
    {
      setWindowTitle(("Auth failed: " + DebugPrint(res)).c_str());
      return;
    }

    auto const token = auth.GetToken();
    Settings::Set(kTokenKeySetting, token.m_key);
    Settings::Set(kTokenSecretSetting, token.m_secret);

    SwitchToLogout(this);
  }
  else
  {
    Settings::Set(kTokenKeySetting, string());
    Settings::Set(kTokenSecretSetting, string());

    SwitchToLogin(this);
  }
}

} // namespace qt