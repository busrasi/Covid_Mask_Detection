#ifndef RGB_FRAME_HELPER_H
#define RGB_FRAME_HELPER_H

#include <QImage>
#include <QVideoFrame>
#include <QAbstractVideoBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>

class RGBHelper
{
    public:
        RGBHelper() {}

        /*
          Returns a QImage that wraps the given video frame.

          This is suitable only for QAbstractVideoBuffer::NoHandle frames with RGB (or BGR)
          data. YUV is not supported here.

          The QVideoFrame must be mapped and kept mapped as long as the wrapping QImage
          exists.

          As a convenience the function also supports frames with a handle type of
          QAbstractVideoBuffer::GLTextureHandle. This allows creating a system memory backed
          QVideoFrame containing the image data from an OpenGL texture. However, readback is a
          slow operation and may stall the GPU pipeline and should be avoided in production code.
        */
        static QImage imageWrapper(const QVideoFrame &frame);

        /*
          Creates and returns a new video frame wrapping the OpenGL texture textureId. The size
          must be passed in size, together with the format of the underlying image data in
          format. When the texture originates from a QImage, use
          QVideoFrame::imageFormatFromPixelFormat() to get a suitable format. Ownership is not
          altered, the new QVideoFrame will not destroy the texture.
        */
        static QVideoFrame frameFromTexture(uint textureId, const QSize &size, QVideoFrame::PixelFormat format);
};


#ifndef QT_NO_OPENGL
class TextureBuffer : public QAbstractVideoBuffer
{
    public:
        TextureBuffer(uint id) : QAbstractVideoBuffer(GLTextureHandle), m_id(id) { }
        MapMode mapMode() const { return NotMapped; }
        uchar *map(MapMode, int *, int *) { return 0; }
        void unmap() { }
        QVariant handle() const { return QVariant::fromValue<uint>(m_id); }

    private:
        GLuint m_id;
};
#endif // QT_NO_OPENGL

#endif // RGB_FRAME_HELPER_H
