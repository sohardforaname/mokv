//
// Created by ucchi_mchxyz on 2022/1/9.
//

#ifndef MOKV_MERGE_ITERATOR_TCC
#define MOKV_MERGE_ITERATOR_TCC

namespace DB {

    class MergeIterator {
    private:
        FILE *sst_file;

    public:
        explicit MergeIterator(FILE *sst_file);

        const char *next();

        bool valid() const;

        size_t size() const;
    };

}


#endif //MOKV_MERGE_ITERATOR_TCC
