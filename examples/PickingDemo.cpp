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
    if(GLEW_ARB_explicit_uniform_location) {
        cout << "Uniform locations enabled" << endl;
    } else {
        cout << "Uniform locations disabled" << endl;
    }


    scene = {
        MeshInfo("cube", "../data/meshes/cube.obj"),
        MeshInfo("sphere", "../data/meshes/sphere_smooth.obj"),
        MeshInfo("torus", "../data/meshes/torus_smooth.obj"),
    };


    camera.lookAt(vec3(0.0f, 1.0f, 0.0f),   // position
                  vec3(0.0f, 0.0f, -10.0f), // center
                  vec3(0.0f, 1.0f, 0.0f));  // up vector

    setupShaders();
    setupRenderables();

    glClearColor(0.2, 0.2, 0.2, 1.0);
}

//---------------------------------------------------------------------------------------
void PickingDemo::setupShaders() {
    shader.loadFromFile("../data/shaders/Pos-Norm-Tex-Color.vert",
                        "../data/shaders/PerFragLighting_withWorldLight.frag");

    light.position = vec3(0.0f, 5.0f, 10.0f);
    light.rgbIntensity = vec3(1.0f);

    shader.setUniform("ambientIntensity", vec3(0.1f, 0.1f, 0.1f));
    shader.setUniform("lightSource.position", light.position);
    shader.setUniform("lightSource.rgbIntensity", light.rgbIntensity);
}

//---------------------------------------------------------------------------------------
void PickingDemo::setupRenderables() {
    RenderableSpec cubeSpec;
    cubeSpec.meshName = "cube";
    cubeSpec.shader = &shader;
    cubeSpec.material.Ka = vec3(1.0f);
    cubeSpec.material.Kd = vec3(0.2f, 0.7f, 0.2f);
    cubeSpec.material.Ks = 0.3f;
    cubeSpec.material.emission = vec3(0.0f);
    cubeSpec.material.shininess = 10.0f;
    cubeSpec.transform.set(vec3(0.0f, 0.0f, -5.0f), vec3(0.0f, 1.0f, 0.0f), PI/3.0f);

    cube = scene.createRenderable(cubeSpec);

    RenderableSpec sphereSpec;
    sphereSpec.meshName = "sphere";
    sphereSpec.shader = &shader;
    sphereSpec.material.Ka = vec3(1.0f);
    sphereSpec.material.Kd = vec3(0.9f, 0.2f, 0.2f);
    sphereSpec.material.Ks = 0.3f;
    sphereSpec.material.emission = vec3(0.0f);
    sphereSpec.material.shininess = 10.0f;
    sphereSpec.transform.position = vec3(-0.8f, 0.5f, -7.0f);

    sphere = scene.createRenderable(sphereSpec);

    RenderableSpec torusSpec;
    torusSpec.meshName = "torus";
    torusSpec.shader = &shader;
    torusSpec.material.Ka = vec3(1.0f);
    torusSpec.material.Kd = vec3(0.2f, 0.2f, 0.9f);
    torusSpec.material.Ks = 0.3f;
    torusSpec.material.emission = vec3(0.0f);
    torusSpec.material.shininess = 10.0f;
    torusSpec.transform.position = vec3(0.5f, -0.5f, -8.0f);

    torus = scene.createRenderable(torusSpec);
}

//---------------------------------------------------------------------------------------
void PickingDemo::logic() {
}

//---------------------------------------------------------------------------------------
void PickingDemo::draw() {
    scene.render(camera);
}
