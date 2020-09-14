#ifndef CSAMPLESLISTMODEL_H
#define CSAMPLESLISTMODEL_H

#include <QAbstractItemModel>
#include "purrfx/CDpcmDataFileReader.h"

class CSamplesListModel: public QAbstractItemModel
{
	Q_OBJECT
public:
	CSamplesListModel(const PurrFX::CDpcmDataFileReader& i_rDpcmProvider);

	void onAppended();

	QModelIndex index(int i_nRow, int i_nColumn, const QModelIndex& i_rParent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex& i_rIndex) const;
	int         rowCount(const QModelIndex& i_rParent = QModelIndex()) const;
	int         columnCount(const QModelIndex& i_rParent = QModelIndex()) const;
	QVariant    data(const QModelIndex& i_rIndex, int i_nRole = Qt::DisplayRole) const;

private:
	const PurrFX::CDpcmDataFileReader& m_rDpcmProvider;
	size_t m_nSize = 0;
};

#endif // CSAMPLESLISTMODEL_H
