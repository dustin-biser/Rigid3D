#include "PickingDemo.hpp"

#include <iostream>
using std::cout;
using std::endl;


//---------------------------------------------------------------------------------------
int main() {
    shared_ptr<GlfwOpenGlWindow> demo = PickingDemo::getInstance();
    demo->create(1024, 768, "Picking Demo");

    return 0;
}

//---------------------------------------------------------------------------------------
shared_ptr<GlfwOpenGlWindow> PickingDemo::getInstance() {
    static GlfwOpenGlWindow * instance = new PickingDemo();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

//---------------------------------------------------------------------------------------
void PickingDemo::init() {
    scene = {
        MeshInfo("cube", "../data/meshes/cube.obj"),
        MeshInfo("sphere", "../data/meshes/sphere_smooth.obj"),
        MeshInfo("torus", "../data/meshes/torus_smooth.obj"),
    };


    camera.lookAt(vec3(0.0f, 1.0f, 0.0f),   // position
                  vec3(0.0f, 0.0f, -10.0f), // center
                  vec3(0.0f, 1.0f, 0.0f));  // up vector

    setupShaders();
    setupLight();
    setupRenderables();

    glClearColor(0.2, 0.2, 0.2, 1.0);
}

//---------------------------------------------------------------------------------------
void PickingDemo::setupLight() {
    LightSpec lightSpec;
    lightSpec.position = vec3(1.5f, 4.0f, -9.0f);
    lightSpec.color = vec3(1.0f, 0.2f, 0.2f);
    lightSpec.type = LightType::Point;
    redLight = scene.createLight(lightSpec);

    lightSpec.position = vec3(-2.0f, 2.0f, -4.0f);
    lightSpec.color = vec3(0.2f, 0.2f, 1.0f);
    lightSpec.type = LightType::Point;
    blueLight = scene.createLight(lightSpec);

    lightSpec.position = vec3(0.0f, 3.0f, -12.0f);
    lightSpec.color = vec3(0.2f, 1.0f, 0.2f);
    lightSpec.type = LightType::Point;
    greenLight = scene.createLight(lightSpec);
}

//---------------------------------------------------------------------------------------
void PickingDemo::setupShaders() {
    shader.loadFromFile("../data/shaders/Pos-Norm-Tex-Color.vert",
                        "../data/shaders/PerFragLighting_withWorldLight.frag");

    shader.setUniform("ambientIntensity", vec3(0.1f, 0.1f, 0.1f));
}

//---------------------------------------------------------------------------------------
void PickingDemo::setupRenderables() {
    RenderableSpec cubeSpec;
    cubeSpec.meshName = "cube";
    cubeSpec.shader = &shader;
    cubeSpec.material.Ka = vec3(1.0f);
    cubeSpec.material.Kd = vec3(1.0f, 1.0f, 1.0f);
    cubeSpec.material.Ks = 0.3f;
    cubeSpec.material.emission = vec3(0.0f);
    cubeSpec.material.shininess = 10.0f;
    cubeSpec.transform.set(vec3(0.0f, 0.0f, -5.0f), vec3(0.0f, 1.0f, 0.0f), PI/3.0f);

    cube = scene.createRenderable(cubeSpec);

    RenderableSpec sphereSpec;
    sphereSpec.meshName = "sphere";
    sphereSpec.shader = &shader;
    sphereSpec.material.Ka = vec3(1.0f);
    sphereSpec.material.Kd = vec3(1.0f, 1.0f, 1.0f);
    sphereSpec.material.Ks = 0.3f;
    sphereSpec.material.emission = vec3(0.0f);
    sphereSpec.material.shininess = 10.0f;
    sphereSpec.transform.position = vec3(-0.8f, 0.5f, -7.0f);

    sphere = scene.createRenderable(sphereSpec);

    RenderableSpec torusSpec;
    torusSpec.meshName = "torus";
    torusSpec.shader = &shader;
    torusSpec.material.Ka = vec3(1.0f);
    torusSpec.material.Kd = vec3(1.0f, 1.0f, 1.0f);
    torusSpec.material.Ks = 0.3f;
    torusSpec.material.emission = vec3(0.0f);
    torusSpec.material.shininess = 10.0f;
    torusSpec.transform.position = vec3(0.5f, -0.5f, -8.0f);

    torus = scene.createRenderable(torusSpec);

    RenderableSpec redLightSpec;
    redLightSpec.meshName = "sphere";
    redLightSpec.shader = &shader;
    redLightSpec.material.Ka = vec3(0);
    redLightSpec.material.Kd = vec3(0);
    redLightSpec.material.Kd = redLight->color;
    redLightSpec.material.emission = redLight->color;
    redLightSpec.transform.position = redLight->position;
    redLightSpec.transform.scale = vec3(0.2f);

    redLightMesh = scene.createRenderable(redLightSpec);

    RenderableSpec blueLightSpec;
    blueLightSpec.meshName = "sphere";
    blueLightSpec.shader = &shader;
    blueLightSpec.material.Ka = vec3(0);
    blueLightSpec.material.Kd = vec3(0);
    blueLightSpec.material.emission = blueLight->color;
    blueLightSpec.transform.position = blueLight->position;
    blueLightSpec.transform.scale = vec3(0.2f);

    blueLightMesh = scene.createRenderable(blueLightSpec);

    RenderableSpec greenLightSpec;
    greenLightSpec.meshName = "sphere";
    greenLightSpec.shader = &shader;
    greenLightSpec.material.Ka = vec3(0);
    greenLightSpec.material.Kd = vec3(0);
    greenLightSpec.material.emission = greenLight->color;
    greenLightSpec.transform.position = greenLight->position;
    greenLightSpec.transform.scale = vec3(0.2f);

    greenLightMesh = scene.createRenderable(greenLightSpec);
}

//---------------------------------------------------------------------------------------
void PickingDemo::logic() {
}

//---------------------------------------------------------------------------------------
void PickingDemo::draw() {
    scene.render(camera);
}
