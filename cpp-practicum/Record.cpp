#include "Record.hpp"

#include <algorithm>

void Record::WriteTo(QTextStream &stream) {
  stream << books_.size() << ' ' << vendors_.size() << '\n';
  for (const auto &book : books_) {
    stream << book << '\n';
  }
  for (const auto &vendor : vendors_) {
    stream << vendor << '\n';
  }
}

void Record::ReadFrom(QTextStream &stream) {
  books_.clear();
  vendors_.clear();

  int book_count, vendor_count;
  stream >> book_count >> vendor_count;
  stream.skipWhiteSpace();
  for (int i = 0; i < book_count; i++) {
    Book book;
    stream >> book;
    books_.push_back(std::move(book));
  }
  for (int i = 0; i < vendor_count; i++) {
    Vendor vendor;
    stream >> vendor;
    vendors_.push_back(std::move(vendor));
  }
}

int BookModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return static_cast<int>(record_->GetBooks().size());
}

int BookModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return 6;
}

QVariant BookModel::headerData(int section, Qt::Orientation orientation,
                               int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();
  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return QStringLiteral("ISBN");
    case 1:
      return QStringLiteral("标题");
    case 2:
      return QStringLiteral("类型");
    case 3:
      return QStringLiteral("作者");
    case 4:
      return QStringLiteral("出版社");
    case 5:
      return QStringLiteral("库存");
    default:
      return QVariant();
    }
  }
  return QVariant();
}

QVariant BookModel::data(const QModelIndex &index, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (!index.isValid())
    return QVariant();

  if (index.row() >= static_cast<int>(record_->GetBooks().size()) ||
      index.row() < 0)
    return QVariant();

  int row = index.row();
  const Book &book = record_->GetBooks()[row];

  int col = index.column();
  switch (col) {
  case 0:
    return QString::fromStdU16String(book.GetIsbn());
  case 1:
    return QString::fromStdU16String(book.GetTitle());
  case 2:
    return QString::fromStdU16String(book.GetType());
  case 3:
    return QString::fromStdU16String(book.GetAuthor());
  case 4:
    return QString::fromStdU16String(book.GetPress());
  case 5:
    return book.GetStockCount();
  default:
    return QVariant();
  }
}

bool BookModel::setData(const QModelIndex &index, const QVariant &value,
                        int role) {
  if (index.isValid() && role == Qt::EditRole) {
    int row = index.row();
    Book &book = record_->GetBooks()[row];

    int col = index.column();
    switch (col) {
    case 0:
      if (!value.canConvert<QString>())
        return false;
      book.SetIsbn(value.toString().toStdU16String());
      return true;
    case 1:
      if (!value.canConvert<QString>())
        return false;
      book.SetTitle(value.toString().toStdU16String());
      emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
      return true;
    case 2:
      if (!value.canConvert<QString>())
        return false;
      book.SetType(value.toString().toStdU16String());
      emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
      return true;
    case 3:
      if (!value.canConvert<QString>())
        return false;
      book.SetAuthor(value.toString().toStdU16String());
      emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
      return true;
    case 4:
      if (!value.canConvert<QString>())
        return false;
      book.SetPress(value.toString().toStdU16String());
      emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
      return true;
    case 5:
      if (!value.canConvert<int>())
        return false;
      book.SetStockCount(value.toInt());
      emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
      return true;
    default:
      return false;
    }
  }
  return false;
}

Qt::ItemFlags BookModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool BookModel::insertRows(int row, int count, const QModelIndex &parent) {
  beginInsertRows(parent, row, row + count - 1);
  for (int i = 0; i < count; i++) {
    record_->GetBooks().insert(record_->GetBooks().cbegin() + row, Book());
  }
  endInsertRows();
  return true;
}

bool BookModel::removeRows(int row, int count, const QModelIndex &parent) {
  beginRemoveRows(parent, row, row + count - 1);
  record_->GetBooks().erase(record_->GetBooks().cbegin() + row,
                            record_->GetBooks().cbegin() + row + count);
  endRemoveRows();
  return true;
}

void BookModel::sort(int column, Qt::SortOrder order) {
  if (column == 0) {
    beginResetModel();
    if (order == Qt::AscendingOrder) {
      std::sort(record_->GetBooks().begin(), record_->GetBooks().end(),
                [](const Book &left, const Book &right) {
                  return left.GetIsbn() < right.GetIsbn();
                });
    } else {
      std::sort(record_->GetBooks().begin(), record_->GetBooks().end(),
                [](const Book &left, const Book &right) {
                  return left.GetIsbn() > right.GetIsbn();
                });
    }
    endResetModel();
  } else if (column == 1) {
    beginResetModel();
    if (order == Qt::AscendingOrder) {
      std::sort(record_->GetBooks().begin(), record_->GetBooks().end(),
                [](const Book &left, const Book &right) {
                  return left.GetTitle() < right.GetTitle();
                });
    } else {
      std::sort(record_->GetBooks().begin(), record_->GetBooks().end(),
                [](const Book &left, const Book &right) {
                  return left.GetTitle() > right.GetTitle();
                });
    }
    endResetModel();
  }
}

int VendorModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return static_cast<int>(record_->GetVendors().size());
}

int VendorModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return 5;
}

QVariant VendorModel::headerData(int section, Qt::Orientation orientation,
                                 int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();
  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return QStringLiteral("编号");
    case 1:
      return QStringLiteral("名称");
    case 2:
      return QStringLiteral("类型");
    case 3:
      return QStringLiteral("地址");
    case 4:
      return QStringLiteral("电话");
    default:
      return QVariant();
    }
  }
  return QVariant();
}

QVariant VendorModel::data(const QModelIndex &index, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (!index.isValid())
    return QVariant();

  if (index.row() >= static_cast<int>(record_->GetVendors().size()) ||
      index.row() < 0)
    return QVariant();

  int row = index.row();
  const Vendor &vendor = record_->GetVendors()[row];

  int col = index.column();
  switch (col) {
  case 0:
    return vendor.GetId();
  case 1:
    return QString::fromStdU16String(vendor.GetName());
  case 2:
    return QString::fromStdU16String(vendor.GetType());
  case 3:
    return QString::fromStdU16String(vendor.GetAddress());
  case 4:
    return QString::fromStdU16String(vendor.GetPhone());
  default:
    return QVariant();
  }
}

bool VendorModel::setData(const QModelIndex &index, const QVariant &value,
                          int role) {
  if (index.isValid() && role == Qt::EditRole) {
    int row = index.row();
    Vendor &vendor = record_->GetVendors()[row];

    int col = index.column();
    switch (col) {
    case 0:
      if (!value.canConvert<int>())
        return false;
      vendor.SetId(value.toInt());
      return true;
    case 1:
      if (!value.canConvert<QString>())
        return false;
      vendor.SetName(value.toString().toStdU16String());
      emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
      return true;
    case 2:
      if (!value.canConvert<QString>())
        return false;
      vendor.SetType(value.toString().toStdU16String());
      emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
      return true;
    case 3:
      if (!value.canConvert<QString>())
        return false;
      vendor.SetAddress(value.toString().toStdU16String());
      emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
      return true;
    case 4:
      if (!value.canConvert<QString>())
        return false;
      vendor.SetPhone(value.toString().toStdU16String());
      emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
      return true;
    default:
      return false;
    }
  }
  return false;
}

Qt::ItemFlags VendorModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool VendorModel::insertRows(int row, int count, const QModelIndex &parent) {
  beginInsertRows(parent, row, row + count - 1);
  for (int i = 0; i < count; i++) {
    record_->GetVendors().insert(record_->GetVendors().cbegin() + row,
                                 Vendor());
  }
  endInsertRows();
  return true;
}

bool VendorModel::removeRows(int row, int count, const QModelIndex &parent) {
  beginRemoveRows(parent, row, row + count - 1);
  record_->GetVendors().erase(record_->GetVendors().cbegin() + row,
                              record_->GetVendors().cbegin() + row + count);
  endRemoveRows();
  return true;
}
