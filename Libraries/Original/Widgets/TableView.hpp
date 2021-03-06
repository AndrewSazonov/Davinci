/*
    Davinci, a software for the single-crystal diffraction data reduction.
    Copyright (C) 2015-2017 Andrew Sazonov

    This file is part of Davinci.

    Davinci is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Davinci is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Davinci.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AS_WIDGETS_TABLEVIEW_HPP
#define AS_WIDGETS_TABLEVIEW_HPP

#include <QTableView>

class QStandardItemModel;

namespace As { //AS_BEGIN_NAMESPACE

class SortFilterProxyModel;

class TableView : public QTableView {

  public:
    TableView(QWidget* parent = Q_NULLPTR);

    virtual ~TableView();

  public slots:
    void setModel(QAbstractItemModel* model) Q_DECL_OVERRIDE;

  protected:
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

  private:
    As::SortFilterProxyModel* m_proxyModel;

    const int m_minRowHeight = 26;
    const int m_minRowHeadersWidth = 50;
    const int m_minColumnWidth = 100;

    int m_tableWidth;
    int m_tableHeight;

    void adjustRowColumnCount();
    void setTableHeight();
    void setTableWidth();

};

} //AS_END_NAMESPACE

#endif // AS_WIDGETS_TABLEVIEW_HPP
