#include <util/buffer.h>

Buffer::Buffer(
    BufferBinding binding,
    BufferUsage   usage)
    : binding(binding),
      usage(usage),
      capacity(0)
{
    glGenBuffers(1, &buffer);
}

Buffer::~Buffer() {
    glDeleteBuffers(1, &buffer);
}

void Buffer::bind(int index) {
    if (index >= 0)
        glBindBufferBase(binding, index, buffer);
    else
        glBindBuffer(binding, buffer);
}

void Buffer::unbind(int index) {
    if (index >= 0)
        glBindBufferBase(binding, index, 0);
    else
        glBindBuffer(binding, 0);
}

void Buffer::setData(size_t size, void *data) {
    bind();

    // TODO: It might be faster to always call glBufferData since we don't care
    // about the old data. However, glBufferSubData doesn't force reallocation.
    if (capacity < size) {
        glBufferData(binding, size, data, usage);
        capacity = size;
    }
    else {
        glBufferSubData(binding, 0, size, data);

        //void *dest = glMapBuffer(binding, GL_WRITE_ONLY);
        //memcpy(dest, data, size);
        //glUnmapBuffer(binding);
    }

    unbind();
}
