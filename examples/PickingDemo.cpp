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

    key_f1_down = false;
    setupShaders();
    setupLight();
    setupRenderables();

    glClearColor(0.2, 0.2, 0.2, 1.0);
}

//---------------------------------------------------------------------------------------
void PickingDemo::setupLight() {
    LightSpec lightSpec;
    lightSpec.position = vec3(-1.5f, 4.0f, 5.0f);
    lightSpec.color = vec3(1.0f);
    lightSpec.type = LightType::Point;
    whiteLight = scene.createLight(lightSpec);
}

//---------------------------------------------------------------------------------------
void PickingDemo::setupShaders() {
    shader.loadFromFile("../data/shaders/Pos-Norm-Tex-Color.vert",
                        "../data/shaders/PerFragLighting_withWorldLight.frag");

    shader.setUniform("ambientIntensity", vec3(0.1f, 0.1f, 0.1f));
}

//---------------------------------------------------------------------------------------
void PickingDemo::setupRenderables() {
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
void PickingDemo::logic() {
}

//---------------------------------------------------------------------------------------
void PickingDemo::draw() {
    scene.render(camera);
}

//---------------------------------------------------------------------------------------
void PickingDemo::keyInput(int key, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_F1) {
            if (key_f1_down == false) {
                key_f1_down = true;

                // Create 15*6*3 = 270 more meshes for the scene to render.
                const int numShapes = 15;
                for(int i = 0; i < numShapes; i++) {
                    cubeSpec.transform.position = cube->getTransform().position + vec3(0, 0, 3*i);
                    scene.createRenderable(cubeSpec);

                    cubeSpec.transform.position = cube->getTransform().position + vec3(0, 0, -3*i);
                    scene.createRenderable(cubeSpec);

                    cubeSpec.transform.position = cube->getTransform().position + vec3(0, 2, 3*i);
                    scene.createRenderable(cubeSpec);

                    cubeSpec.transform.position = cube->getTransform().position + vec3(0, 2, -3*i);
                    scene.createRenderable(cubeSpec);

                    cubeSpec.transform.position = cube->getTransform().position + vec3(0, -2, 3*i);
                    scene.createRenderable(cubeSpec);

                    cubeSpec.transform.position = cube->getTransform().position + vec3(0, -2, -3*i);
                    scene.createRenderable(cubeSpec);
                }

                for(int i = 0; i < numShapes; i++) {
                    sphereSpec.transform.position = sphere->getTransform().position + vec3(0, 0, 3*i);
                    scene.createRenderable(sphereSpec);

                    sphereSpec.transform.position = sphere->getTransform().position + vec3(0, 0, -3*i);
                    scene.createRenderable(sphereSpec);

                    sphereSpec.transform.position = sphere->getTransform().position + vec3(0, 4, 3*i);
                    scene.createRenderable(sphereSpec);

                    sphereSpec.transform.position = sphere->getTransform().position + vec3(0, 4, -3*i);
                    scene.createRenderable(sphereSpec);

                    sphereSpec.transform.position = sphere->getTransform().position + vec3(0, -4, 3*i);
                    scene.createRenderable(sphereSpec);

                    sphereSpec.transform.position = sphere->getTransform().position + vec3(0, -4, -3*i);
                    scene.createRenderable(sphereSpec);
                }

                for(int i = 0; i < numShapes; i++) {
                    torusSpec.transform.position = torus->getTransform().position + vec3(0, 0, 3*i);
                    scene.createRenderable(torusSpec);

                    torusSpec.transform.position = torus->getTransform().position + vec3(0, 0, -3*i);
                    scene.createRenderable(torusSpec);

                    torusSpec.transform.position = torus->getTransform().position + vec3(0, -5, 3*i);
                    scene.createRenderable(torusSpec);

                    torusSpec.transform.position = torus->getTransform().position + vec3(0, -5, -3*i);
                    scene.createRenderable(torusSpec);

                    torusSpec.transform.position = torus->getTransform().position + vec3(0, 5, 3*i);
                    scene.createRenderable(torusSpec);

                    torusSpec.transform.position = torus->getTransform().position + vec3(0, 5, -3*i);
                    scene.createRenderable(torusSpec);
                }
            }

        }
    }

}
