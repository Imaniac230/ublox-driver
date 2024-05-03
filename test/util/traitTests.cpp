#include <gtest/gtest.h>

#include <array>
#include <list>
#include <map>
#include <vector>

#include <util/Traits.h>

TEST(TraitsTests, isIterable) {
    ASSERT_TRUE(Traits::is_iterable<std::vector<uint8_t>>);
    ASSERT_TRUE(Traits::is_iterable<std::list<uint8_t>>);
    ASSERT_TRUE((Traits::is_iterable<std::array<uint8_t, 3>>) );
    ASSERT_TRUE((Traits::is_iterable<std::map<uint8_t, uint8_t>>) );

    ASSERT_FALSE(Traits::is_iterable<uint8_t>);
    ASSERT_FALSE(Traits::is_iterable<uint8_t[3]>);
}

TEST(TraitsTests, hasValueType) {
    ASSERT_TRUE((Traits::has_value_type<std::vector<uint8_t>, uint8_t>) );
    ASSERT_TRUE((Traits::has_value_type<std::list<uint8_t>, uint8_t>) );
    ASSERT_TRUE((Traits::has_value_type<std::array<uint8_t, 3>, uint8_t>) );

    ASSERT_FALSE((Traits::has_value_type<std::vector<uint8_t>, uint16_t>) );
    ASSERT_FALSE((Traits::has_value_type<std::list<uint8_t>, uint16_t>) );
    ASSERT_FALSE((Traits::has_value_type<std::array<uint8_t, 3>, uint16_t>) );
    ASSERT_FALSE((Traits::has_value_type<std::map<uint8_t, uint8_t>, uint8_t>) );
    ASSERT_FALSE((Traits::has_value_type<std::map<uint8_t, uint8_t>, uint16_t>) );
}

TEST(TraitsTests, iterableWith) {
    ASSERT_TRUE((Traits::IterableWith<std::vector<uint8_t>, uint8_t>) );
    ASSERT_TRUE((Traits::IterableWith<std::vector<uint16_t>, uint16_t>) );
    ASSERT_TRUE((Traits::IterableWith<std::list<uint8_t>, uint8_t>) );
    ASSERT_TRUE((Traits::IterableWith<std::list<uint16_t>, uint16_t>) );
    ASSERT_TRUE((Traits::IterableWith<std::array<uint8_t, 3>, uint8_t>) );
    ASSERT_TRUE((Traits::IterableWith<std::array<uint16_t, 3>, uint16_t>) );

    ASSERT_FALSE((Traits::IterableWith<std::vector<uint16_t>, uint8_t>) );
    ASSERT_FALSE((Traits::IterableWith<std::vector<uint8_t>, uint16_t>) );
    ASSERT_FALSE((Traits::IterableWith<std::list<uint16_t>, uint8_t>) );
    ASSERT_FALSE((Traits::IterableWith<std::list<uint8_t>, uint16_t>) );
    ASSERT_FALSE((Traits::IterableWith<std::array<uint16_t, 3>, uint8_t>) );
    ASSERT_FALSE((Traits::IterableWith<std::array<uint8_t, 3>, uint16_t>) );
    ASSERT_FALSE((Traits::IterableWith<std::map<uint8_t, uint8_t>, uint8_t>) );
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
