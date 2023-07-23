#include <indexed_iterator.h>

namespace IndexedIterator {
    std::ostream & operator<<(std::ostream & os, const iterator_base_obj & obj) {
        return obj.toStream(os);
    }
}