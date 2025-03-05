#include "list.h"

#include <gtest.h>

TEST(Iterator, oper_adress) {          
    List<int> l(2);
    l.insert_front(2);
    l.insert_front(1);

    List<int>::Iterator it = l.begin();
    EXPECT_EQ(*it, 1);
}

TEST(Iterator, pref_plus) {

    List<int> l(2);
    l.insert_front(2);
    l.insert_front(1);

    List<int>::Iterator it = l.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);

}

TEST(Iterator, post_plus) {

    List<int> l(2);
    l.insert_front(2);
    l.insert_front(1);

    List<int>::Iterator it = l.begin();
    EXPECT_EQ(*it, 1);
    List<int>::Iterator it2 = it++;
    EXPECT_EQ(*it2, 1);
    EXPECT_EQ(*it, 2);

}

TEST(Iterator, oper_not_equal) {

    List<int> l(3);

    List<int>::Iterator it1 = l.begin();
    List<int>::Iterator it2 = l.end();
 
    EXPECT_EQ(it1 != it2, 1);
}
