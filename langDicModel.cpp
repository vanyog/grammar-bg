/*
grammar-bg - open source Bulgarian grammar
Copyright (C) 2007  Vanyo Georgiev <info@vanyog.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "langDicModel.h"
#include "showMessage.h"

LangDicModel::LangDicModel(WordRootFormPairList *rfpl, QObject *parent)
   :QAbstractListModel(parent)
{
   rfpList = rfpl;
};

int LangDicModel::rowCount(const QModelIndex &parent) const{
   return rfpList->size();
};

QVariant LangDicModel::data(const QModelIndex &index, int role) const{
   if (!index.isValid())
      return QVariant();
   if (index.row() >= rfpList->size())
      return QVariant();
   if (role == Qt::DisplayRole)
      return rfpList->word(index.row());
   else
      return QVariant();
};

