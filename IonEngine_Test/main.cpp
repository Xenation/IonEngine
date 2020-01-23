#include <iostream>
#include <Core/EngineCore.h>
#include <Core/Collections/BulkSparseSet.h>

using namespace IonEngine;


struct TestSt {
	u8 a;
	u8 b;
	u16 c;
	u32 d;
};

int main(int argc, char** args) {

	BulkSparseSet<TestSt> testCol;
	for (u32 i = 0; i < 3000; i++) {
		TestSt* st = testCol.allocate();
		st->a = static_cast<u8>(i);
	}

	for (TestSt& st : testCol) {
		st.b++;
	}

	for (u32 i = 0; i < 3000; i += 2) {
		testCol.removeAt(i);
	}

	for (u32 i = 0; i < 1500; i++) {
		TestSt* st = testCol.allocate();
		st->b = 2;
	}

	return 0;
}
