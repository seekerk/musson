#ifndef DATALOADER_H
#define DATALOADER_H

#include <QList>
#include <QDebug>
#include <QSet>
#include <QDateTime>
#include <QHash>

#include "utils/utils.h"
#include "datatypes/message.h"

//! Template structure that gets date from some datatype
template <typename T>
struct DataLoaderDateGetter;

//! DataLoaderDateGetter specialization for Message
template <>
struct DataLoaderDateGetter<Message>
{
    QDateTime operator()(const Message& mess) const
    {
        return QDateTime::fromString(mess.time(), DATE_TIME_FORMAT);
    }
};

//! Class that performs paged loading of arbitrary datatypes
template <typename T, typename L>
class DataLoader
{
public:
    typedef T Type;
    typedef QList<Type> TypeList;

    typedef L Loader;

    //! Constructor
    /*!
     * \param loader_ Functional object that contains one parameter - pageNumber and should return
     *                QList<T> with requested pageSize
     * \param localLoader_ paged accessor for cached data list
     * \param pageSize_ Size of page
     */
    DataLoader(Loader loader_, TypeList& existingData_, int pageSize_ = 10)
        : loader(loader_), pageSize(pageSize_),
          existingData(existingData_),
          cacheHits(0), cacheMiss(0)
    {
    }

    ~DataLoader()
    {
        qDebug() << "~DataLoader: cache efficency" << cacheHits << "/" << (cacheHits + cacheMiss);
    }

    //! Set timelimit for old data loader
    void setDateTimeLimit(const QDateTime& limit)
    {
        minDate = limit;
    }

    void loadOld()
    {
        singleCache.clear();
        pageCache.clear();

        if (!existingData.isEmpty())
            synchronize();
        loadList(existingData.size() - 1, 0, pageSize, minDate);
    }

    void loadNew()
    {
        if (existingData.isEmpty())
            loadOld();
        else
            synchronize();
    }

    int validate()
    {
        return -1;
    }

    void loadList(int after, const Type* until = 0, int count = -1, QDateTime untilDate = QDateTime())
    {
        int before = after + 1;
        int page = before / pageSize;
        int firstInPage = before % pageSize;
        int loaded = 0;

        while (!untilDate.isNull() || count == -1 || loaded < count) {
            TypeList list = loadRemotePage(page);
            if (list.isEmpty())
                return;

            for (int i = firstInPage; i < list.size(); i++) {
                Type data = list.at(i);
                if ((until != 0 && equals(*until, data))
                        || (!untilDate.isNull() && DataLoaderDateGetter<Type>()(data) < untilDate))
                    return;

                existingData.insert(before, data);
                before++;
                loaded++;
            }
            firstInPage = 0;
            page++;
        }
    }

    void removeList(int after, const Type* until) {
        typename TypeList::iterator begin = existingData.begin() + (1 + after);
        typename TypeList::iterator end = until != 0 ? qFind(begin, existingData.end(), *until)
                                                     : existingData.end();

        existingData.erase(begin, end);
    }

    void synchronize() {
        TypeList remoteFirstPage = loadRemotePage(0, 1);
        if (remoteFirstPage.isEmpty()) {
            existingData.clear();
            return;
        }

        Type remoteFirst = remoteFirstPage.first();
        Type localFirst = existingData.first();

        if (!equals(localFirst, remoteFirst)) {
            if (earlier(localFirst, remoteFirst)) {
                loadList(-1, &localFirst);
            } else {
                removeList(-1, &remoteFirst);
            }
        }

        TypeList remoteLastPage = loadRemotePage(existingData.size() - 1, 1);
        Type localLast = existingData.last();

        int lastCorrect = 0;
        while (remoteLastPage.isEmpty() || !equals(localLast, remoteLastPage.first())) {
            int first = lastCorrect;
            int last = existingData.size();

            while (first < last - 1) {
                int mid = first + (last - first) / 2;

                TypeList remoteMidPage = loadRemotePage(mid, 1);
                Type localMid = existingData.at(mid);

                if (remoteMidPage.isEmpty() || !equals(localMid, remoteMidPage.first())) {
                    last = mid;
                    remoteLastPage = remoteMidPage;
                    localLast = localMid;
                } else {
                    first = mid;
                }
            }

            // last = first error item
            if (!remoteLastPage.isEmpty() && earlier(localLast, remoteLastPage.first())) {
                loadList(last - 1, &localLast);
            } else {
                removeList(last - 1, remoteLastPage.isEmpty() ? 0 : &remoteLastPage.first());
            }

            // Check again
            remoteLastPage = loadRemotePage(existingData.size() - 1, 1);
            localLast = existingData.last();
            lastCorrect = last;
        }
    }

private:
    bool equals(const Type& data1, const Type& data2) {
        return data1 == data2;
    }

    bool earlier(const Type& data1, const Type& data2) {
        return DataLoaderDateGetter<Type>()(data1) < DataLoaderDateGetter<Type>()(data2);
    }

    TypeList loadRemotePage(int page, int pageSize = -1)
    {
        if (pageSize == -1)
            pageSize = this->pageSize;

        if (pageSize == 1) {
            if (singleCache.contains(page)) {
                cacheHits++;
                return singleCache.value(page);
            }

            int defaultPageNumber = page / this->pageSize;
            int defaultPagePosition = page % this->pageSize;

            if (pageCache.contains(defaultPageNumber)) {
                cacheHits++;
                TypeList value = pageCache.value(defaultPageNumber);
                if (defaultPagePosition < value.size())
                    return TypeList() << value.at(defaultPagePosition);
            }
        } else if (pageSize == this->pageSize) {
            if (pageCache.contains(page)) {
                cacheHits++;
                return pageCache.value(page);
            }
        }

        cacheMiss++;
        TypeList ret = loader(page, pageSize);
        if (pageSize == 1) {
            singleCache.insert(page, ret);
        } else if (pageSize == this->pageSize) {
            pageCache.insert(page, ret);
        }

        return ret;
    }

private:
    Loader loader;
    int pageSize;
    QDateTime minDate;
    TypeList& existingData;

    // Cache
    QHash<int, TypeList> singleCache;
    QHash<int, TypeList> pageCache;
    int cacheHits;
    int cacheMiss;
};

#endif // DATALOADER_H
