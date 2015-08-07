
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/gl/Light.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class QuatApp : public AppNative {
private:
  Vec3f mouse_pos;

  Vec3f pos;
  Vec3f dir;


  CameraPersp camera;
  Vec3f eye;
  Vec3f target;

  unique_ptr<gl::Light> light;

public:
  void setup();

  void mouseDown(MouseEvent event);
  void mouseMove(MouseEvent event);

  void update();
  void draw();
};

void QuatApp::setup() {
  pos = Vec3f(0, 150, 0);
  dir = Vec3f::yAxis();

  eye = Vec3f(0, 0, 790);
  target = Vec3f(0, 0, 0);
  camera = CameraPersp(getWindowWidth(), getWindowHeight(),
                       35.f, 0.5f, 790.f);
  camera.lookAt(eye, target);

  light = make_unique<gl::Light>(gl::Light::DIRECTIONAL, 0);
  light->setAmbient(Color(0.8f, 0.8f, 0.8f));
  light->setDiffuse(Color(0.6f, 0.6f, 0.6f));
  light->setDirection(Vec3f(0, 0, 1));

  gl::enable(GL_LIGHTING);
  gl::enable(GL_CULL_FACE);

  //hideCursor();
}

void QuatApp::mouseDown(MouseEvent event) {}

void QuatApp::mouseMove(MouseEvent event) {
  mouse_pos = Vec3f(event.getPos().x - getWindowWidth() / 2,
                    -(event.getPos().y - getWindowHeight() / 2),
                    0);


  Vec3f vTarget = (mouse_pos - pos).normalized();
  Vec3f cross = dir.cross(vTarget);

  Quatf quat = Quatf(cross.normalized(), cross.length());
  dir = quat * dir;
}

void QuatApp::update() {
}

void QuatApp::draw() {
  gl::clear(Color(0, 0, 0));
  gl::setMatrices(camera);
  light->enable();

  gl::pushModelView();
  gl::translate(pos);

  Quatf quat = Quatf(Vec3f(0, 1, 0), dir);
  gl::rotate(quat);
  gl::drawCube(Vec3f::zero(), Vec3f(10, 50, 10));
  gl::drawCube(Vec3f(0, 20, 0), Vec3f(20, 5, 5));
  gl::popModelView();
}

CINDER_APP_NATIVE(QuatApp, RendererGl)
