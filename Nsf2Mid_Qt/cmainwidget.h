#ifndef CMAINWIDGET_H
#define CMAINWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include "DString.h"
#include "purrfx/PurrFX.h"
#include "Nsf2Mid/CMidiFile.h"

namespace Ui { class CMainWidget; }

class CMainWidget : public QWidget
{
	Q_OBJECT

public:
	CMainWidget(QWidget* i_pParent = nullptr);
	~CMainWidget();

private slots:
	void onLoad();
	void onExport() const;

private:
	Ui::CMainWidget* m_pUI;
	PurrFX::CNesPtr m_oNes;
};
#endif // CMAINWIDGET_H
