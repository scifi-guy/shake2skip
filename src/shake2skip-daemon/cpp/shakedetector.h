/*
@version: 0.1
@author: Sudheer K. <scifi1947 at gmail.com>
@license: GNU General Public License
*/

#ifndef SHAKEDETECTOR_H
#define SHAKEDETECTOR_H

#include "mafwrendererproxy.h"
#include "accelerometerfilter.h"
QTM_USE_NAMESPACE

#ifdef Q_WS_MAEMO_5

#define MAFW_RENDERER_DBUS_PATH           "/com/nokia/mafw/renderer/gstrenderer"
#define MAFW_RENDERER_DBUS_INTERFACE      "com.nokia.mafw.renderer"
#define MAFW_RENDERER_DBUS_SERVICE        "com.nokia.mafw.renderer.Mafw-Gst-Renderer-Plugin.gstrenderer"

#else

#define MAFW_RENDERER_DBUS_PATH           "/com/nokia/mafw/renderer/mafw_gst_renderer"
#define MAFW_RENDERER_DBUS_INTERFACE      "com.nokia.mafw.renderer"
#define MAFW_RENDERER_DBUS_SERVICE        "com.nokia.mafw.renderer.MafwGstRendererPlugin.mafw_gst_renderer"

#endif

class ShakeDetector: public QObject
{
  Q_OBJECT
public:
    explicit ShakeDetector(QObject *parent = 0);
    virtual ~ShakeDetector();

public slots:
    void mediaStateChanged(int currentState);
    void skipSong();

private:
    enum MediaState {
        Stopped = 0,
        Playing,
        Paused,
        Transitioning
    };

    bool debugMode;

    MafwRendererProxy *mafwProxy;
    AccelerometerFilter *accelFilter;
};

#endif // SHAKEDETECTOR_H
