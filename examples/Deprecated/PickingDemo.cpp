#include "PickingDemo.hpp"

using glm::angleAxis;


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
    meshConsolidator =  {
            {"cube", "../data/meshes/cube.obj"},
            {"sphere", "../data/meshes/sphere_smooth.obj"},
            {"torus", "../data/meshes/torus_smooth.obj"},
            {"bunny", "../data/meshes/bunny_smooth.obj"},
    };

    meshConsolidator.getBatchInfo(batchInfoMap);

    glGenVertexArrays(1, &vao);

    camera.lookAt(vec3(0.0f, 1.0f, 0.0f),   // position
                  vec3(0.0f, 0.0f, -10.0f), // center
                  vec3(0.0f, 1.0f, 0.0f));  // up vector

    setupShaders();
    setupVertexBuffers();
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

    glBindVertexArray(vao);

    GLint position_Location = shader.getAttribLocation("vertexPosition");
    glEnableVertexAttribArray(position_Location);
    GLint normal_Location = shader.getAttribLocation("vertexNormal");
    glEnableVertexAttribArray(normal_Location);

    glBindVertexArray(0);

    checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void PickingDemo::setupVertexBuffers() {
    glBindVertexArray(vao);

    // Copy position data to OpenGL buffer.
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexPositionBytes(),
            meshConsolidator.getVertexPositionDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(shader.getAttribLocation("vertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Copy normal data to OpenGL buffer.
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexNormalBytes(),
            meshConsolidator.getVertexNormalDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(shader.getAttribLocation("vertexNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void PickingDemo::setupRenderables() {
    cube = Renderable(&vao, &shader, &batchInfoMap.at("cube"));
    sphere = Renderable(&vao, &shader, &batchInfoMap.at("sphere"));
    torus = Renderable(&vao, &shader, &batchInfoMap.at("torus"));

    cube.setPosition(vec3(0.0f, 0.0f, -5.0f));
    cube.setPose(angleAxis(PI/3.0f, vec3(0.0f, 1.0f, 0.0f)));
    cube.setEmissionLevels(vec3(0.0f));
    cube.setAmbientLevels(vec3(1.0f, 1.0f, 1.0f));
    cube.setDiffuseLevels(vec3(0.2f, 0.7f, 0.2f));
    cube.setSpecularIntensity(0.3f);
    cube.setShininessFactor(10.0f);

    sphere.setPosition(vec3(-0.8f, 0.5f, -7.0f));
    sphere.setEmissionLevels(vec3(0.0f));
    sphere.setAmbientLevels(vec3(1.0f, 1.0f, 1.0f));
    sphere.setDiffuseLevels(vec3(0.9f, 0.2f, 0.2f));
    sphere.setSpecularIntensity(0.3f);
    sphere.setShininessFactor(10.0f);

    torus.setPosition(vec3(0.5f, -0.5f, -8.0f));
    torus.setEmissionLevels(vec3(0.0f));
    torus.setAmbientLevels(vec3(1.0f, 1.0f, 1.0f));
    torus.setDiffuseLevels(vec3(0.2f, 0.2f, 0.9f));
    torus.setSpecularIntensity(0.3f);
    torus.setShininessFactor(10.0f);

}

//---------------------------------------------------------------------------------------
void PickingDemo::logic() {
   renderContext.projectionMatrix = camera.getProjectionMatrix();
   renderContext.viewMatrix= camera.getViewMatrix();
}

//---------------------------------------------------------------------------------------
void PickingDemo::draw() {
    cube.render(renderContext);
    sphere.render(renderContext);
    torus.render(renderContext);
}

//---------------------------------------------------------------------------------------
void PickingDemo::cleanup() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteVertexArrays(1, &vao);
    checkGLErrors(__FILE__, __LINE__);
}
