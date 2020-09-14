#include "CSamplesListModel.h"

CSamplesListModel::CSamplesListModel(const PurrFX::CDpcmDataFileReader& i_rDpcmProvider):
	QAbstractItemModel(nullptr),
	m_rDpcmProvider(i_rDpcmProvider)
{
}

void CSamplesListModel::onAppended()
{
	Q_ASSERT(m_nSize == m_rDpcmProvider.size()-1);
	beginInsertRows(QModelIndex(), m_nSize+1, m_nSize+1);
	m_nSize++;
	endInsertRows();
}

QModelIndex CSamplesListModel::index(int i_nRow, int i_nColumn, const QModelIndex& i_rParent) const
{
	if (i_rParent.isValid())
		return QModelIndex(); // No 2nd level items

	return createIndex(i_nRow, i_nColumn);
}

QModelIndex CSamplesListModel::parent(const QModelIndex& i_rIndex) const
{
	Q_UNUSED(i_rIndex);
	return QModelIndex(); // No parents
}

int CSamplesListModel::rowCount(const QModelIndex& i_rParent) const
{
	if (i_rParent.isValid())
		return 0; // No 2nd level items
	else
		return int(m_rDpcmProvider.size());
}

int CSamplesListModel::columnCount(const QModelIndex& i_rParent) const
{
	Q_UNUSED(i_rParent);
	return 1;
}

QVariant CSamplesListModel::data(const QModelIndex& i_rIndex, int i_nRole) const
{
	if (i_nRole != Qt::DisplayRole)
		return QVariant();
	Q_ASSERT(i_rIndex.isValid());
	Q_ASSERT(i_rIndex.row() >= 0);
	size_t nSampleIndex = size_t( i_rIndex.row() );
	Q_ASSERT(nSampleIndex < m_rDpcmProvider.size());
	Q_ASSERT(i_rIndex.column() == 0);

	return QString::number(nSampleIndex+1);
}
