#include <iostream>
#include "RandAccessIterConcept.h"
#include <iterator>
#include <forward_list>
#include <list>
#include <vector>

int main() {
    //типа тесты. Ничего лучше не придумал...
    BOOST_CONCEPT_ASSERT((MyInputIterator<std::istreambuf_iterator<char>>));
    BOOST_CONCEPT_ASSERT((MyOutputIterator<std::ostreambuf_iterator<char>, char>));

    BOOST_CONCEPT_ASSERT((MyForwardIterator<std::forward_list<int>::const_iterator>));
    BOOST_CONCEPT_ASSERT((MyMutableForwardIterator<std::forward_list<int>::iterator>));

    BOOST_CONCEPT_ASSERT((MyBidirectionalIterator<std::list<int>::const_iterator>));
    BOOST_CONCEPT_ASSERT((MyMutableBidirectionalIterator<std::list<int>::iterator>));

    BOOST_CONCEPT_ASSERT((MyRandomAccessIterator<std::vector<int>::const_iterator>));
    BOOST_CONCEPT_ASSERT((MyMutableRandomAccessIterator<std::vector<int>::iterator>));

    //негативные тесты
    //для проверки раскомментировать соответствующую строчку
    //BOOST_CONCEPT_ASSERT((MyOutputIterator<std::istreambuf_iterator<char>, char>));
    //BOOST_CONCEPT_ASSERT((MyInputIterator<std::ostreambuf_iterator<char>>));

    //BOOST_CONCEPT_ASSERT((MyBidirectionalIterator<std::forward_list<int>::const_iterator>));
    //BOOST_CONCEPT_ASSERT((MyMutableBidirectionalIterator<std::forward_list<int>::iterator>));

    //BOOST_CONCEPT_ASSERT((MyRandomAccessIterator<std::list<int>::const_iterator>));
    //BOOST_CONCEPT_ASSERT((MyMutableRandomAccessIterator<std::list<int>::iterator>));

    return 0;
}