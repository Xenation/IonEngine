#include "AtomicCounterBuffer.h"

#include <gl3w.h>
using namespace IonEngine;



AtomicCounterBuffer::AtomicCounterBuffer(std::string name, unsigned int binding, unsigned int size) : name(name), binding(binding), bufferSize(size) {}

AtomicCounterBuffer::~AtomicCounterBuffer() {}


void AtomicCounterBuffer::bind() {
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
}

void AtomicCounterBuffer::clear() {
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	glClearBufferData(GL_ATOMIC_COUNTER_BUFFER, GL_R16UI, GL_RED, GL_UNSIGNED_INT, nullptr);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
}

void AtomicCounterBuffer::allocateLocal() {
	// TODO NOT IMPLEMENTED
}

void AtomicCounterBuffer::deleteLocal() {
	// TODO NOT IMPLEMENTED
}

void AtomicCounterBuffer::uploadToGL() {
	if (acbo == 0) {
		glGenBuffers(1, &acbo);
		std::string fullName = "ACBO " + name;
		glObjectLabel(GL_BUFFER, acbo, (GLsizei) fullName.size(), fullName.c_str());
	}
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, bufferSize, buffer, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

	glBindBufferRange(GL_ATOMIC_COUNTER_BUFFER, binding, acbo, 0, bufferSize);

	loadedToGL = true;
}

void AtomicCounterBuffer::deleteFromGL() {
	if (acbo != 0) {
		glDeleteBuffers(1, &acbo);
		acbo = 0;
	}

	loadedToGL = false;
}
