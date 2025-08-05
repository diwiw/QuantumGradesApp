#include "doctest.h"
#include "Grades.h"

TEST_CASE("Grades add and mean"){
	Grades g;
	g.add(5);
	g.add(3);
	g.add(4);
	CHECK(g.getMean() == doctest::Approx(4.0));
}

TEST_CASE("Grades max and min") {
	Grades g;
	g.add(2);
	g.add(5);
	g.add(1);
	CHECK(g.getMax() == 5);
	CHECK(g.getMin() == 1);
}

TEST_CASE("Grades print (just runs)") {
	Grades g;
	g.add(1);
	g.add(2);
	g.printGrades(); // Not the output value is tested, but no errors
}
