#include "Renderer.h"

int WINDOW_WIDTH = 1280 ,WINDOW_HEIGHT = 700;
GLFWwindow *window;

GLuint mainFramebuffer ,mainFramebufferTexture ,mainRenderbuffer;
GLuint framebufferVAO ,framebufferEBO;
Program mainFramebufferProgram,concreteFloorProgram,woodenCubeProgram;

float FOV=45.0,cameraNearClip=0.1,cameraFarClip=100.0;
Camera camera(-90.0,0.0,glm::vec3(0.0,2.0,0.0));
glm::vec2 centerVec;
glm::mat4 cameraProjectionMatrix=glm::perspective(glm::radians(FOV) ,(float)WINDOW_WIDTH/WINDOW_HEIGHT ,cameraNearClip ,cameraFarClip),cameraViewMatrix;
float movementSpeed = 2.5 ,cameraSensetivity=0.05;

float currentTime=0,deltaTime=0,prevTime=0;

Model concreteFloorModel,woodenCubeModel;

glm::vec3 lightPosition = glm::vec3(0.0,2.0,0.0);
glm::vec3 lightColor = glm::vec3(1.0),diffuseColor = glm::vec3(0.7)*lightColor,ambientColor=glm::vec3(0.05)*lightColor,specularColor=glm::vec3(0.35)*lightColor;

std::vector<glm::vec3> cubePositions = {
        glm::vec3(5.0, 1.0, 5.0),
        glm::vec3(-5.0, 1.0, 5.0),
        glm::vec3(0.0, 1.0, -5.0)
};
int cubeCount = cubePositions.size();

void framebufferResizeCallback(GLFWwindow *window ,int w, int h){
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;
    glViewport(0 ,0 ,w ,h);

    centerVec.x = WINDOW_WIDTH / 2.0;
    centerVec.y = WINDOW_HEIGHT / 2.0;
    cameraProjectionMatrix=glm::perspective(glm::radians(FOV) ,(float)WINDOW_WIDTH/WINDOW_HEIGHT ,cameraNearClip ,cameraFarClip);
}

void cursorPosCallback(GLFWwindow *window ,double x ,double y){
    glfwSetCursorPos(window ,centerVec.x ,centerVec.y);
    camera.yaw -= (centerVec.x-x) * cameraSensetivity;
    camera.pitch += (centerVec.y-y) * cameraSensetivity;
    camera.constrainPitch(-89.0 ,89.0);
    camera.updateCamera();
    cameraViewMatrix = camera.getViewMatrix();
}

void setupCallbacks(){
    glfwSetFramebufferSizeCallback(window ,framebufferResizeCallback);
    glfwSetCursorPosCallback(window ,cursorPosCallback);
}

void setupData(){
    float framebufferData[] = {
            -1.0,-1.0,0.0,0.0,
            -1.0, 1.0,0.0,1.0,
             1.0, 1.0,1.0,1.0,
             1.0,-1.0,1.0,0.0
    };

    unsigned int framebufferIndices[] = {
            0,1,2,
            2,3,0
    };

    GLuint framebufferVBO;
    glGenBuffers(1 ,&framebufferVBO);
    glBindBuffer(GL_ARRAY_BUFFER ,framebufferVBO);
    glBufferData(GL_ARRAY_BUFFER ,sizeof(framebufferData) ,framebufferData ,GL_STATIC_DRAW);

    glGenVertexArrays(1 ,&framebufferVAO);
    glBindVertexArray(framebufferVAO);

    glVertexAttribPointer(0 ,2 ,GL_FLOAT ,GL_FALSE ,sizeof(float)*4 ,(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1 ,2 ,GL_FLOAT ,GL_FALSE ,sizeof(float)*4 ,(void*)(sizeof(float)*2));
    glEnableVertexAttribArray(1);

    glGenBuffers(1 ,&framebufferEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER ,framebufferEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER ,sizeof(framebufferIndices) ,framebufferIndices ,GL_STATIC_DRAW);

}

void setupFramebuffers() {
    glGenFramebuffers(1, &mainFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);

    glGenTextures(1, &mainFramebufferTexture);
    glBindTexture(GL_TEXTURE_2D, mainFramebufferTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mainFramebufferTexture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &mainRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mainRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mainRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error Setting Main Framebuffer\n";
        exit(-1);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void setupPrograms(){
    mainFramebufferProgram.initialize();
    mainFramebufferProgram.attachShader(Shader("Shaders/FramebufferShader/VertexShader.glsl" ,GL_VERTEX_SHADER));
    mainFramebufferProgram.attachShader(Shader("Shaders/FramebufferShader/FragmentShader.glsl" ,GL_FRAGMENT_SHADER));
    mainFramebufferProgram.link();

    mainFramebufferProgram.use();
    mainFramebufferProgram.setInt("inputTexture" ,0);

    concreteFloorProgram.initialize();
    concreteFloorProgram.attachShader(Shader("Shaders/ConcreteFloorShader/VertexShader.glsl" ,GL_VERTEX_SHADER));
    concreteFloorProgram.attachShader(Shader("Shaders/ConcreteFloorShader/FragmentShader.glsl" ,GL_FRAGMENT_SHADER));
    concreteFloorProgram.link();

    concreteFloorProgram.use();
    concreteFloorProgram.setInt("depthMap" ,4);

    woodenCubeProgram.initialize();
    woodenCubeProgram.attachShader(Shader("Shaders/WoodenCubeShader/VertexShader.glsl" ,GL_VERTEX_SHADER));
    woodenCubeProgram.attachShader(Shader("Shaders/WoodenCubeShader/FragmentShader.glsl" ,GL_FRAGMENT_SHADER));
    woodenCubeProgram.link();

    woodenCubeProgram.use();
    woodenCubeProgram.setInt("depthMap" ,4);
}

std::vector<glm::mat4> translatePositions(std::vector<glm::vec3> positions){
    std::vector<glm::mat4> translations;
    for(int i = 0; i < positions.size(); ++i){
        translations.push_back(glm::translate(glm::mat4(1.0) ,positions[i]));
    }
    return translations;
}

void loadModels(){
    concreteFloorModel.initialize("Models/ConcreteFloor/concreteFloor.obj" ,[]() {
        glTexParameteri(GL_TEXTURE_2D ,GL_TEXTURE_WRAP_S ,GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D ,GL_TEXTURE_WRAP_T ,GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D ,GL_TEXTURE_MIN_FILTER ,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D ,GL_TEXTURE_MAG_FILTER ,GL_LINEAR);
    },true);
    concreteFloorModel.setTextureUniforms(concreteFloorProgram);

    woodenCubeModel.initialize("Models/WoodenCube/woodenCube.obj" ,true);
    woodenCubeModel.setTextureUniforms(woodenCubeProgram);

    std::vector<glm::mat4> cubeTranslations = translatePositions(cubePositions);

    GLuint cubeTranslationsBuffer;
    glGenBuffers(1 ,&cubeTranslationsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER ,cubeTranslationsBuffer);
    glBufferData(GL_ARRAY_BUFFER ,cubeTranslations.size() * sizeof(glm::mat4) ,&cubeTranslations[0] ,GL_STATIC_DRAW);

    std::vector<GLuint> meshVAO = woodenCubeModel.getAllMeshVAO();

    size_t vec4Size = sizeof(glm::vec4);
    size_t mat4Size = sizeof(glm::mat4);
    for(int i = 0; i < meshVAO.size(); ++i){
        glBindVertexArray(meshVAO[i]);

        glVertexAttribPointer(3 ,4 ,GL_FLOAT ,GL_FALSE ,mat4Size ,(void*)0);
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(4 ,4 ,GL_FLOAT ,GL_FALSE ,mat4Size ,(void*)(vec4Size));
        glEnableVertexAttribArray(4);

        glVertexAttribPointer(5 ,4 ,GL_FLOAT ,GL_FALSE ,mat4Size ,(void*)(vec4Size*2));
        glEnableVertexAttribArray(5);

        glVertexAttribPointer(6 ,4 ,GL_FLOAT ,GL_FALSE ,mat4Size ,(void*)(vec4Size*3));
        glEnableVertexAttribArray(6);

        glVertexAttribDivisor(3,1);
        glVertexAttribDivisor(4,1);
        glVertexAttribDivisor(5,1);
        glVertexAttribDivisor(6,1);
    }
}

void setLightUniforms(Program program){
    program.use();
    program.setVec3("pointLight.ambient" ,ambientColor);
    program.setVec3("pointLight.diffuse" ,diffuseColor);
    program.setVec3("pointLight.specular" ,specularColor);
    program.setVec3("pointLight.position" ,lightPosition);
    program.setFloat("pointLight.constant" ,1.0);
    program.setFloat("pointLight.linear" ,0.02);
    program.setFloat("pointLight.quadratic" ,0.05);
}

void initializeGraphics(){
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR ,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR ,6);
    glfwWindowHint(GLFW_OPENGL_PROFILE ,GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH ,WINDOW_HEIGHT ,"Point Shadows" ,nullptr ,nullptr);
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window ,GLFW_CURSOR ,GLFW_CURSOR_HIDDEN);

    setupCallbacks();

    glewExperimental = GL_TRUE;
    glewInit();

    setupData();
    setupPrograms();
    setupFramebuffers();
    loadModels();

    setLightUniforms(concreteFloorProgram);
    setLightUniforms(woodenCubeProgram);
}

void drawFramebuffer(){
    mainFramebufferProgram.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D ,mainFramebufferTexture);
    glBindVertexArray(framebufferVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER ,framebufferEBO);
    glDrawElements(GL_TRIANGLES ,6 ,GL_UNSIGNED_INT ,nullptr);
}

void handleEvents(){
    if(glfwGetKey(window ,GLFW_KEY_ESCAPE)){
        glfwSetWindowShouldClose(window ,true);
    }else{
        if(glfwGetKey(window ,GLFW_KEY_LEFT_SHIFT)){
            movementSpeed = 5.0;
        }else{
            movementSpeed = 2.5;
        }
        if(glfwGetKey(window ,GLFW_KEY_W)){
            camera.position += glm::normalize(camera.front) * deltaTime * movementSpeed;
        }
        if(glfwGetKey(window ,GLFW_KEY_S)){
            camera.position -= glm::normalize(camera.front) * deltaTime * movementSpeed;
        }
        if(glfwGetKey(window ,GLFW_KEY_A)){
            camera.position -= glm::normalize(camera.right) * deltaTime * movementSpeed;
        }
        if(glfwGetKey(window ,GLFW_KEY_D)){
            camera.position += glm::normalize(camera.right) * deltaTime * movementSpeed;
        }
        if(glfwGetKey(window ,GLFW_KEY_SPACE)){
            camera.position += camera.up * deltaTime * movementSpeed;
        }
        if(glfwGetKey(window ,GLFW_KEY_LEFT_ALT)){
            camera.position -= camera.up * deltaTime * movementSpeed;
        }
        camera.updateCamera();
        cameraViewMatrix = camera.getViewMatrix();
    }
}

void drawConcreteFloor(Program program ,void (*loadingFunction)()){
    program.use();
    loadingFunction();
    concreteFloorModel.draw(program);
}

void drawWoodenCubes(Program program ,void (*loadingFunction)()){
    program.use();
    loadingFunction();
    woodenCubeModel.drawInstanced(program ,cubeCount);
}

void renderGraphics(){
    while(!glfwWindowShouldClose(window)){
        currentTime = glfwGetTime();
        deltaTime = currentTime - prevTime;

        handleEvents();
        glBindFramebuffer(GL_FRAMEBUFFER ,mainFramebuffer);
        glClearColor(0.0,0.0,0.0,1.0);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawConcreteFloor(concreteFloorProgram ,[](){
            concreteFloorProgram.setMat4("projection" ,cameraProjectionMatrix);
            concreteFloorProgram.setMat4("view" ,cameraViewMatrix);
            concreteFloorProgram.setMat4("model" ,glm::mat4(1.0));
            concreteFloorProgram.setVec3("cameraPosition" ,camera.position);
        });

        drawWoodenCubes(woodenCubeProgram ,[](){
            woodenCubeProgram.setMat4("projection" ,cameraProjectionMatrix);
            woodenCubeProgram.setMat4("view" ,cameraViewMatrix);
            woodenCubeProgram.setMat4("model" ,glm::mat4(1.0));
            woodenCubeProgram.setVec3("cameraPosition" ,camera.position);
        });

        glBindFramebuffer(GL_FRAMEBUFFER ,0);
        glClearColor(0.0,0.0,0.0,1.0);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        drawFramebuffer();

        glfwSwapBuffers(window);
        glfwPollEvents();

        prevTime = currentTime;
    }
}

void terminateGraphics(){
    glfwTerminate();
}

int main() {
    initializeGraphics();
    renderGraphics();
    terminateGraphics();

    return 0;
}
