#include <gtest/gtest.h>

#include <util/Serde.hpp>

TEST(SerdeTests, SerializeDeserialize8) {
    const int testIterations = 100;
    typedef int8_t TEST_TYPE_SIGNED;
    typedef uint8_t TEST_TYPE_UNSIGNED;
    const TEST_TYPE_SIGNED dataSigned = -testIterations / 2;
    const TEST_TYPE_UNSIGNED dataUnsigned = 1;

    for (int i = 0; i < testIterations; ++i) {
        ASSERT_EQ(
                Serde::deserializeLEInt<TEST_TYPE_SIGNED>(&Serde::serializeLEInt<TEST_TYPE_SIGNED>(dataSigned + i)[0]),
                dataSigned + i);
        ASSERT_EQ(
                Serde::deserializeBEInt<TEST_TYPE_SIGNED>(&Serde::serializeLEInt<TEST_TYPE_SIGNED>(dataSigned + i)[0]),
                dataSigned + i);
        ASSERT_EQ(
                Serde::deserializeLEInt<TEST_TYPE_SIGNED>(&Serde::serializeBEInt<TEST_TYPE_SIGNED>(dataSigned + i)[0]),
                dataSigned + i);
        ASSERT_EQ(
                Serde::deserializeBEInt<TEST_TYPE_SIGNED>(&Serde::serializeBEInt<TEST_TYPE_SIGNED>(dataSigned + i)[0]),
                dataSigned + i);

        ASSERT_EQ(Serde::deserializeLEInt<TEST_TYPE_UNSIGNED>(
                          &Serde::serializeLEInt<TEST_TYPE_UNSIGNED>(dataUnsigned + i)[0]),
                  dataUnsigned + i);
        ASSERT_EQ(Serde::deserializeBEInt<TEST_TYPE_UNSIGNED>(
                          &Serde::serializeLEInt<TEST_TYPE_UNSIGNED>(dataUnsigned + i)[0]),
                  dataUnsigned + i);
        ASSERT_EQ(Serde::deserializeLEInt<TEST_TYPE_UNSIGNED>(
                          &Serde::serializeBEInt<TEST_TYPE_UNSIGNED>(dataUnsigned + i)[0]),
                  dataUnsigned + i);
        ASSERT_EQ(Serde::deserializeBEInt<TEST_TYPE_UNSIGNED>(
                          &Serde::serializeBEInt<TEST_TYPE_UNSIGNED>(dataUnsigned + i)[0]),
                  dataUnsigned + i);
    }
}

TEST(SerdeTests, SerializeDeserialize16) {
    const int testIterations = 100;
    typedef int16_t TEST_TYPE_SIGNED;
    typedef uint16_t TEST_TYPE_UNSIGNED;
    const TEST_TYPE_SIGNED dataSigned = -testIterations / 2;
    const TEST_TYPE_UNSIGNED dataUnsigned = 1;

    for (int i = 0; i < testIterations; ++i) {
        ASSERT_EQ(
                Serde::deserializeLEInt<TEST_TYPE_SIGNED>(&Serde::serializeLEInt<TEST_TYPE_SIGNED>(dataSigned + i)[0]),
                dataSigned + i);
        if ((dataSigned + i) != -1 && (dataSigned + i) != 0) {
            ASSERT_NE(Serde::deserializeBEInt<TEST_TYPE_SIGNED>(
                              &Serde::serializeLEInt<TEST_TYPE_SIGNED>(dataSigned + i)[0]),
                      dataSigned + i);
            ASSERT_NE(Serde::deserializeLEInt<TEST_TYPE_SIGNED>(
                              &Serde::serializeBEInt<TEST_TYPE_SIGNED>(dataSigned + i)[0]),
                      dataSigned + i);
        }
        ASSERT_EQ(
                Serde::deserializeBEInt<TEST_TYPE_SIGNED>(&Serde::serializeBEInt<TEST_TYPE_SIGNED>(dataSigned + i)[0]),
                dataSigned + i);

        ASSERT_EQ(Serde::deserializeLEInt<TEST_TYPE_UNSIGNED>(
                          &Serde::serializeLEInt<TEST_TYPE_UNSIGNED>(dataUnsigned + i)[0]),
                  dataUnsigned + i);
        ASSERT_NE(Serde::deserializeBEInt<TEST_TYPE_UNSIGNED>(
                          &Serde::serializeLEInt<TEST_TYPE_UNSIGNED>(dataUnsigned + i)[0]),
                  dataUnsigned + i);
        ASSERT_NE(Serde::deserializeLEInt<TEST_TYPE_UNSIGNED>(
                          &Serde::serializeBEInt<TEST_TYPE_UNSIGNED>(dataUnsigned + i)[0]),
                  dataUnsigned + i);
        ASSERT_EQ(Serde::deserializeBEInt<TEST_TYPE_UNSIGNED>(
                          &Serde::serializeBEInt<TEST_TYPE_UNSIGNED>(dataUnsigned + i)[0]),
                  dataUnsigned + i);
    }
}

TEST(SerdeTests, SerializeDeserialize32) {
    const int testIterations = 100;
    typedef int32_t TEST_TYPE_SIGNED;
    typedef uint32_t TEST_TYPE_UNSIGNED;
    const TEST_TYPE_SIGNED dataSigned = -testIterations / 2;
    const TEST_TYPE_UNSIGNED dataUnsigned = 1;

    for (int i = 0; i < testIterations; ++i) {
        ASSERT_EQ(
                Serde::deserializeLEInt<TEST_TYPE_SIGNED>(&Serde::serializeLEInt<TEST_TYPE_SIGNED>(dataSigned + i)[0]),
                dataSigned + i);
        if ((dataSigned + i) != -1 && (dataSigned + i) != 0) {
            ASSERT_NE(Serde::deserializeBEInt<TEST_TYPE_SIGNED>(
                              &Serde::serializeLEInt<TEST_TYPE_SIGNED>(dataSigned + i)[0]),
                      dataSigned + i);
            ASSERT_NE(Serde::deserializeLEInt<TEST_TYPE_SIGNED>(
                              &Serde::serializeBEInt<TEST_TYPE_SIGNED>(dataSigned + i)[0]),
                      dataSigned + i);
        }
        ASSERT_EQ(
                Serde::deserializeBEInt<TEST_TYPE_SIGNED>(&Serde::serializeBEInt<TEST_TYPE_SIGNED>(dataSigned + i)[0]),
                dataSigned + i);

        ASSERT_EQ(Serde::deserializeLEInt<TEST_TYPE_UNSIGNED>(
                          &Serde::serializeLEInt<TEST_TYPE_UNSIGNED>(dataUnsigned + i)[0]),
                  dataUnsigned + i);
        ASSERT_NE(Serde::deserializeBEInt<TEST_TYPE_UNSIGNED>(
                          &Serde::serializeLEInt<TEST_TYPE_UNSIGNED>(dataUnsigned + i)[0]),
                  dataUnsigned + i);
        ASSERT_NE(Serde::deserializeLEInt<TEST_TYPE_UNSIGNED>(
                          &Serde::serializeBEInt<TEST_TYPE_UNSIGNED>(dataUnsigned + i)[0]),
                  dataUnsigned + i);
        ASSERT_EQ(Serde::deserializeBEInt<TEST_TYPE_UNSIGNED>(
                          &Serde::serializeBEInt<TEST_TYPE_UNSIGNED>(dataUnsigned + i)[0]),
                  dataUnsigned + i);
    }
}

TEST(SerdeTests, SerializeDeserialize64) {
    const int testIterations = 100;
    typedef int64_t TEST_TYPE_SIGNED;
    typedef uint64_t TEST_TYPE_UNSIGNED;
    const TEST_TYPE_SIGNED dataSigned = -testIterations / 2;
    const TEST_TYPE_UNSIGNED dataUnsigned = 1;

    for (int i = 0; i < testIterations; ++i) {
        ASSERT_EQ(
                Serde::deserializeLEInt<TEST_TYPE_SIGNED>(&Serde::serializeLEInt<TEST_TYPE_SIGNED>(dataSigned + i)[0]),
                dataSigned + i);
        if ((dataSigned + i) != -1 && (dataSigned + i) != 0) {
            ASSERT_NE(Serde::deserializeBEInt<TEST_TYPE_SIGNED>(
                              &Serde::serializeLEInt<TEST_TYPE_SIGNED>(dataSigned + i)[0]),
                      dataSigned + i);
            ASSERT_NE(Serde::deserializeLEInt<TEST_TYPE_SIGNED>(
                              &Serde::serializeBEInt<TEST_TYPE_SIGNED>(dataSigned + i)[0]),
                      dataSigned + i);
        }
        ASSERT_EQ(
                Serde::deserializeBEInt<TEST_TYPE_SIGNED>(&Serde::serializeBEInt<TEST_TYPE_SIGNED>(dataSigned + i)[0]),
                dataSigned + i);

        ASSERT_EQ(Serde::deserializeLEInt<TEST_TYPE_UNSIGNED>(
                          &Serde::serializeLEInt<TEST_TYPE_UNSIGNED>(dataUnsigned + i)[0]),
                  dataUnsigned + i);
        ASSERT_NE(Serde::deserializeBEInt<TEST_TYPE_UNSIGNED>(
                          &Serde::serializeLEInt<TEST_TYPE_UNSIGNED>(dataUnsigned + i)[0]),
                  dataUnsigned + i);
        ASSERT_NE(Serde::deserializeLEInt<TEST_TYPE_UNSIGNED>(
                          &Serde::serializeBEInt<TEST_TYPE_UNSIGNED>(dataUnsigned + i)[0]),
                  dataUnsigned + i);
        ASSERT_EQ(Serde::deserializeBEInt<TEST_TYPE_UNSIGNED>(
                          &Serde::serializeBEInt<TEST_TYPE_UNSIGNED>(dataUnsigned + i)[0]),
                  dataUnsigned + i);
    }
}

TEST(SerdeTests, SerializeDeserializeFloats) {
    const int testIterations = 100;
    typedef float TEST_TYPE_FLOAT;
    typedef double TEST_TYPE_DOUBLE;
    const TEST_TYPE_FLOAT dataFloat = -static_cast<TEST_TYPE_FLOAT>(testIterations) / 2;
    const TEST_TYPE_DOUBLE dataDouble = -static_cast<TEST_TYPE_DOUBLE>(testIterations) / 2;

    for (int i = 0; i < testIterations; ++i) {
        ASSERT_EQ(
                Serde::deserializeLEFloat<TEST_TYPE_FLOAT>(&Serde::serializeLEFloat<TEST_TYPE_FLOAT>(dataFloat + i)[0]),
                dataFloat + i);
        if ((dataFloat + static_cast<TEST_TYPE_FLOAT>(i)) != 0) {
            ASSERT_NE(Serde::deserializeBEFloat<TEST_TYPE_FLOAT>(
                              &Serde::serializeLEFloat<TEST_TYPE_FLOAT>(dataFloat + i)[0]),
                      dataFloat + i);
            ASSERT_NE(Serde::deserializeLEFloat<TEST_TYPE_FLOAT>(
                              &Serde::serializeBEFloat<TEST_TYPE_FLOAT>(dataFloat + i)[0]),
                      dataFloat + i);
        }
        ASSERT_EQ(
                Serde::deserializeBEFloat<TEST_TYPE_FLOAT>(&Serde::serializeBEFloat<TEST_TYPE_FLOAT>(dataFloat + i)[0]),
                dataFloat + i);

        ASSERT_EQ(Serde::deserializeLEFloat<TEST_TYPE_DOUBLE>(
                          &Serde::serializeLEFloat<TEST_TYPE_DOUBLE>(dataDouble + i)[0]),
                  dataDouble + i);
        if ((dataDouble + static_cast<TEST_TYPE_DOUBLE>(i)) != 0) {
            ASSERT_NE(Serde::deserializeBEFloat<TEST_TYPE_DOUBLE>(
                              &Serde::serializeLEFloat<TEST_TYPE_DOUBLE>(dataDouble + i)[0]),
                      dataDouble + i);
            ASSERT_NE(Serde::deserializeLEFloat<TEST_TYPE_DOUBLE>(
                              &Serde::serializeBEFloat<TEST_TYPE_DOUBLE>(dataDouble + i)[0]),
                      dataDouble + i);
        }
        ASSERT_EQ(Serde::deserializeBEFloat<TEST_TYPE_DOUBLE>(
                          &Serde::serializeBEFloat<TEST_TYPE_DOUBLE>(dataDouble + i)[0]),
                  dataDouble + i);
    }
}

TEST(SerdeTests, DeserializeTYPE) {
    //TODO(bit-width): find a way to explicitly verify that the result (r) from deserialize always has the correct bit width!
    //  i.e. static casts into type must be performed during int type construction in deserialize**Int(),
    //  otherwise doubles do not get deserialized correctly
    //    typedef double TEST_TYPE;
    //    const TEST_TYPE data = 15;
    //    const auto a = Serde::deserializeBEFloat<TEST_TYPE>(&Serde::serializeBEFloat<TEST_TYPE>(data)[0]);
    //    const auto b = Serde::deserializeLEFloat<TEST_TYPE>(&Serde::serializeLEFloat<TEST_TYPE>(data)[0]);
    //    std::cout << "sizeof a: " << sizeof(a) << ", a: " << a << std::endl;
    //    std::cout << "sizeof b: " << sizeof(b) << ", b: " << b << std::endl;
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}