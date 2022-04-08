#include "file_cache.tcc"

void test()
{
    MOKV::FileCache cache;
    int fd = cache.openFile("./test_db/key_2_0.sst");
    if (-1 == fd) {
        printf("%d\n", errno);
    }
    auto& meta = cache.getMeta(fd);
    MOKV::FileIterator iter;
    if (iter.setMetaInfo(meta)) {
        printf("%d\n", errno);
    }
    while (iter.hasNext()) {
        iter.next();
        auto view = iter.getItem();
        printf("%s\n", std::string(view.data(), view.size()).c_str());
        printf("%d\n", iter.getId());
        printf("%d\n", iter.diff());

        printf("%s\n", std::string(view.data(), view.size()).c_str());
        printf("%d\n", iter.getId());
        printf("%d\n", iter.diff());
    }

    MOKV::FileIterator iter1;
    if (iter1.setMetaInfo(meta)) {
        printf("%d\n", errno);
    }
    iter1.seek("key4");
    auto view1 = iter1.getItem();
    printf("%s\n", std::string(view1.data(), view1.size()).c_str());
    printf("%d\n", iter1.getId());
    printf("%d\n", iter1.diff());

    MOKV::FileIterator iter2;
    if (iter2.setMetaInfo(meta)) {
        printf("%d\n", errno);
    }
    iter2.seekN(2);
    auto view2 = iter2.getItem();
    printf("%s\n", std::string(view2.data(), view2.size()).c_str());
    printf("%d\n", iter2.getId());
    printf("%d\n", iter2.diff());

    printf("%d\n", cache.findIndex(fd, "key3"));
}

int main()
{
    test();
    return 0;
}