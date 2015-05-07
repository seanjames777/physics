#ifndef __BUFFER_H
#define __BUFFER_H

#include <util/gldefs.h>

enum BufferBinding {
    BufferBindingVertexBuffer  = GL_ARRAY_BUFFER,
    BufferBindingIndexBuffer   = GL_ELEMENT_ARRAY_BUFFER,
    BufferBindingUniformBuffer = GL_UNIFORM_BUFFER
};

enum BufferUsage {
    BufferUsageStatic  = GL_STATIC_DRAW,
    BufferUsageDynamic = GL_DYNAMIC_DRAW,
    BufferUsageStream  = GL_STREAM_DRAW
};

class Buffer {
private:

    GLuint        buffer;
    BufferBinding binding;
    BufferUsage   usage;
    size_t        capacity;

public:

    Buffer(
        BufferBinding binding,
        BufferUsage   usage);

    ~Buffer();

    void bind(int index = -1);

    void unbind(int index = -1);

    void setData(size_t size, void *data);

};

#endif
