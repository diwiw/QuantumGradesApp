#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Grades.h"

TEST_CASE("Testowanie sredniej z ocen"){
	Grades g;
	g.add(4);
	g.add(5);
	g.add(3);

	CHECK(g.getMean() == doctest::Approx(4.0));
}

TEST_CASE("Testowanie minimum i maksimum"){
	Grades g;
	g.add(2);
	g.add(5);
	g.add(1);

	CHECK(g.getMin() == 1);
	CHECK(g.getMax() == 5);

}

