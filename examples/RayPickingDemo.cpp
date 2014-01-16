#include "RayPickingDemo.hpp"

#include <iostream>
using std::cout;
using std::endl;


//---------------------------------------------------------------------------------------
int main() {
    shared_ptr<GlfwOpenGlWindow> demo = RayPickingDemo::getInstance();
    demo->create(1024, 768, "Picking Demo");

    return 0;
}

//---------------------------------------------------------------------------------------
shared_ptr<GlfwOpenGlWindow> RayPickingDemo::getInstance() {
    static GlfwOpenGlWindow * instance = new RayPickingDemo();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

//---------------------------------------------------------------------------------------
void RayPickingDemo::init() {
    scene = {
        MeshInfo("cube", "../data/meshes/cube.obj"),
        MeshInfo("sphere", "../data/meshes/sphere_smooth.obj"),
        MeshInfo("torus", "../data/meshes/torus_smooth.obj"),
    };


    camera.lookAt(vec3(0.0f, 0.0f, 0.0f),   // position
                  vec3(0.0f, 0.0f, -1.0f), // center
                  vec3(0.0f, 1.0f, 0.0f));  // up vector

    key_f1_down = false;
    setupShaders();
    setupLight();
    setupRenderables();

    glClearColor(0.2, 0.2, 0.2, 1.0);
}

//---------------------------------------------------------------------------------------
void RayPickingDemo::setupLight() {
    LightSpec lightSpec;
    lightSpec.position = vec3(-1.5f, 4.0f, 5.0f);
    lightSpec.color = vec3(1.0f);
    lightSpec.type = LightType::Point;
    whiteLight = scene.createLight(lightSpec);
}

//---------------------------------------------------------------------------------------
void RayPickingDemo::setupShaders() {
    shader.loadFromFile("../data/shaders/Pos-Norm-Tex-Color.vert",
                        "../data/shaders/PerFragLighting_withWorldLight.frag");

    shader.setUniform("ambientIntensity", vec3(0.1f, 0.1f, 0.1f));
}

//---------------------------------------------------------------------------------------
void RayPickingDemo::setupRenderables() {
    cubeSpec.meshName = "cube";
    cubeSpec.shader = &shader;
    cubeSpec.material.Ka = vec3(1.0f);
    cubeSpec.material.Kd = vec3(0.2f, 1.0f, 0.2f);
    cubeSpec.material.Ks = 0.3f;
    cubeSpec.material.emission = vec3(0.0f);
    cubeSpec.material.shininess = 10.0f;
    cubeSpec.transform.set(vec3(0.0f, 0.0f, -5.0f), vec3(0.0f, 1.0f, 0.0f), PI/3.0f);

    cube = scene.createRenderable(cubeSpec);

    sphereSpec.meshName = "sphere";
    sphereSpec.shader = &shader;
    sphereSpec.material.Ka = vec3(1.0f);
    sphereSpec.material.Kd = vec3(0.8f, 0.1f, 0.1f);
    sphereSpec.material.Ks = 0.3f;
    sphereSpec.material.emission = vec3(0.0f);
    sphereSpec.material.shininess = 10.0f;
    sphereSpec.transform.position = vec3(-0.8f, 0.5f, -7.0f);

    sphere = scene.createRenderable(sphereSpec);

    torusSpec.meshName = "torus";
    torusSpec.shader = &shader;
    torusSpec.material.Ka = vec3(1.0f);
    torusSpec.material.Kd = vec3(0.1f, 0.1f, 0.7f);
    torusSpec.material.Ks = 0.3f;
    torusSpec.material.emission = vec3(0.0f);
    torusSpec.material.shininess = 10.0f;
    torusSpec.transform.position = vec3(0.5f, -0.5f, -8.0f);

    torus = scene.createRenderable(torusSpec);

    lightSpec.meshName = "sphere";
    lightSpec.shader = &shader;
    lightSpec.material.Ka = vec3(0);
    lightSpec.material.Kd = vec3(0);
    lightSpec.material.Kd = whiteLight->color;
    lightSpec.material.emission = whiteLight->color;
    lightSpec.transform.position = whiteLight->position;
    lightSpec.transform.scale = vec3(0.2f);

    lightMesh = scene.createRenderable(lightSpec);



}

//---------------------------------------------------------------------------------------
void RayPickingDemo::logic() {
}

//---------------------------------------------------------------------------------------
void RayPickingDemo::draw() {
    scene.render(camera);
}

//---------------------------------------------------------------------------------------
void RayPickingDemo::cursorPosition(double xPos, double yPos) {
    this->xWindowPos = xPos;
    this->yWindowPos = yPos;
}

//---------------------------------------------------------------------------------------
void RayPickingDemo::mouseButtonInput(int button , int actions, int mods) {
    if (actions == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_1) {
            createRay();
        }
    }
}

//---------------------------------------------------------------------------------------
RayCastInput RayPickingDemo::createRay() {
    // Vertical direction along front view frustum plane.
    vec3 v = camera.getUpDirection();

    // Horizontal direction along front view frustum plane.
    vec3 h = camera.getLeftDirection();
    h *= -1.0f;

    float32 angle = degreesToRadians(camera.getFieldOfViewY() / 2);
    float32 vLength = camera.getNearZDistance() * tan(angle);
    float32 hLength = vLength * ((float)windowWidth / windowHeight);

    v *= vLength;
    h *= hLength;

    float32 x = xWindowPos;
    float32 y = yWindowPos;

    // Offset mouse coordinates so center of screen is origin.
    x -= windowWidth / 2;
    y -= windowHeight / 2;

    // Scale mouse coordinates so they are in the range [-1,1].
    x /= windowWidth / 2;
    y /= windowHeight / 2;

    // Flip the y coordinate to match OpenGL coordinates.
    y *= -1.0f;

    vec3 frontPlaneOrigin = camera.getForwardDirection() * camera.getNearZDistance();

    vec3 frontPlaneIntersect = camera.getPosition() + frontPlaneOrigin + x*h + y*v;

    RayCastInput ray;
    ray.p1 = camera.getPosition();
    ray.p2 = frontPlaneIntersect;
    ray.maxLength = 100;

    return ray;

//    cout << "Click Position " << frontPlaneIntersect << endl;
//    cout << "Camera Forward Dir " << camera.getForwardDirection() << endl;
//    cout << "Camera Position " << camera.getPosition() << endl;
//    cout << endl;
}
