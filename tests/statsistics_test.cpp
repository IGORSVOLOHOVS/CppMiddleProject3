#include <gtest/gtest.h>
#include "statistics.hpp"
#include "book_database.hpp"
#include "comparators.hpp"

using namespace bookdb;

class StatisticsTest : public ::testing::Test {
protected:
    BookDatabase<> db;
    BookDatabase<> empty_db;

    void SetUp() override {
        db.EmplaceBack("George Orwell", "1984", 1949, Genre::SciFi, 4.8, 200);
        db.EmplaceBack("George Orwell", "Animal Farm", 1945, Genre::Fiction, 4.4, 150);
        db.EmplaceBack("J.R.R. Tolkien", "The Hobbit", 1937, Genre::Fiction, 4.9, 300);
        db.EmplaceBack("Aldous Huxley", "Brave New World", 1932, Genre::SciFi, 4.5, 180);
    }
};

TEST_F(StatisticsTest, AuthorHistogram) {
    auto histogram = buildAuthorHistogramFlat(db);
    EXPECT_EQ(histogram.size(), 3);
    EXPECT_EQ(histogram["George Orwell"], 2);
    EXPECT_EQ(histogram["J.R.R. Tolkien"], 1);
    EXPECT_EQ(histogram["Aldous Huxley"], 1);

    auto empty_hist = buildAuthorHistogramFlat(empty_db);
    EXPECT_TRUE(empty_hist.empty());
}

TEST_F(StatisticsTest, GenreRatings) {
    auto ratings = calculateGenreRatings(db);
    EXPECT_EQ(ratings.size(), 2);
    EXPECT_NEAR(ratings[Genre::SciFi], (4.8 + 4.5) / 2.0, 1e-9);
    EXPECT_NEAR(ratings[Genre::Fiction], (4.4 + 4.9) / 2.0, 1e-9);
    
    auto empty_ratings = calculateGenreRatings(empty_db);
    EXPECT_TRUE(empty_ratings.empty());
}

TEST_F(StatisticsTest, AverageRating) {
    double avg = calculateAverageRating(db);
    double expected = (4.8 + 4.4 + 4.9 + 4.5) / 4.0;
    EXPECT_NEAR(avg, expected, 1e-9);

    EXPECT_EQ(calculateAverageRating(empty_db), 0.0);
}

TEST_F(StatisticsTest, SampleRandomBooks) {
    auto sample1 = sampleRandomBooks(db, 2);
    EXPECT_EQ(sample1.size(), 2);

    EXPECT_THROW(sampleRandomBooks(db, 10), std::logic_error);
    
    auto sample0 = sampleRandomBooks(db, 0);
    EXPECT_TRUE(sample0.empty());
}

TEST_F(StatisticsTest, GetTopNByRating) {
    auto top2 = getTopNBy(db, 2, comp::GreaterByRating{});
    EXPECT_EQ(top2.size(), 2);
    EXPECT_EQ(top2[0].get().title, "The Hobbit"); // 4.9
    EXPECT_EQ(top2[1].get().title, "1984"); // 4.8
    
    EXPECT_EQ(db.GetBooks()[0].title, "The Hobbit");
    EXPECT_EQ(db.GetBooks()[1].title, "1984");
}

TEST_F(StatisticsTest, GetTopNByPopularity) {
    auto top3 = getTopNBy(db, 3, comp::GreaterByPopularity{});
    EXPECT_EQ(top3.size(), 3);
    EXPECT_EQ(top3[0].get().title, "The Hobbit"); // 300
    EXPECT_EQ(top3[1].get().title, "1984"); // 200
    EXPECT_EQ(top3[2].get().title, "Brave New World"); // 180
}

