#include "GlobalOpenGL.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream> 
#include "GlobalObjects.h"
#include "VAOManager.h"
#include "ShaderManager.h"
#include "LightHelper.h"
#include "BoundingBox.h"
#include "Shapes.h"
#include "PhysicsSystem.h"

static bool seeded = false;

glm::vec3 gCameraEye = glm::vec3(-295.66f, 49.8f, -97.7f); // Final view
glm::vec3 gCameraTarget = glm::vec3(0.0f, 10.0f, 0.0f);
glm::vec3 gDronePosition = glm::vec3(9.03f, 65.2f, -123.7f);

PhysicsSystem physicsSystem;
BoundingBox boundingBox;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

float RandomFloat(float a, float b) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;

    return a + r;
}


bool LoadThePLYFile(std::string filename, ModelDrawInfo& modelDrawInfo)
{
    struct sVertex_XYZ_N_RGBA_UV
    {
        float x, y, z;
        float nx, ny, nz;
        float red, green, blue, alpha;
        float texture_u, texture_v;
    };

    sVertex_XYZ_N_RGBA_UV* pTheModelArray = NULL;
    sTrianglePLY* pTheModelTriangleArray = NULL;

    std::ifstream theFile(filename);
    if (!theFile.is_open())
    {
        std::cout << "Unable to open file!" << std::endl;
        return false;
    }

    // Read an entire line
    char buffer[10000];
    theFile.getline(buffer, 10000);

    std::string theNextToken;

    // Scan for the word "vertex"
    while (theFile >> theNextToken)
    {
        if (theNextToken == "vertex")
        {
            break;
        }
    }
    // 
    theFile >> modelDrawInfo.numberOfVertices;

    // Scan for the word "face"
    while (theFile >> theNextToken)
    {
        if (theNextToken == "face")
        {
            break;
        }
    }
    // 
    theFile >> modelDrawInfo.numberOfTriangles;

    // Scan for the word "end_header"
    while (theFile >> theNextToken)
    {
        if (theNextToken == "end_header")
        {
            break;
        }
    }

    pTheModelArray = new sVertex_XYZ_N_RGBA_UV[modelDrawInfo.numberOfVertices];

    std::cout << "Loading";
    for (unsigned int count = 0; count != modelDrawInfo.numberOfVertices; count++)
    {
        theFile >> pTheModelArray[count].x;
        theFile >> pTheModelArray[count].y;
        theFile >> pTheModelArray[count].z;

        theFile >> pTheModelArray[count].nx;
        theFile >> pTheModelArray[count].ny;
        theFile >> pTheModelArray[count].nz;

        theFile >> pTheModelArray[count].red;
        theFile >> pTheModelArray[count].green;
        theFile >> pTheModelArray[count].blue;
        theFile >> pTheModelArray[count].alpha;

        theFile >> pTheModelArray[count].texture_u;
        theFile >> pTheModelArray[count].texture_v;

        if (count % 10000 == 0)
        {
            std::cout << ".";
        }
    }
    std::cout << "done" << std::endl;


    // Load the faces (or triangles)
    pTheModelTriangleArray = new sTrianglePLY[modelDrawInfo.numberOfTriangles];

    for (unsigned int count = 0; count != modelDrawInfo.numberOfTriangles; count++)
    {
        // 3 15393 15394 15395 
        unsigned int discard = 0;
        theFile >> discard;

        theFile >> pTheModelTriangleArray[count].vertexIndices[0];
        theFile >> pTheModelTriangleArray[count].vertexIndices[1];
        theFile >> pTheModelTriangleArray[count].vertexIndices[2];
    }

    theFile.close();

    modelDrawInfo.pVertices = new Vertex[modelDrawInfo.numberOfVertices];

    // Now copy the information from the PLY infomation to the model draw info structure
    for (unsigned int index = 0; index != modelDrawInfo.numberOfVertices; index++)
    {
        // To The Shader                        From the file
        modelDrawInfo.pVertices[index].x = pTheModelArray[index].x;
        modelDrawInfo.pVertices[index].y = pTheModelArray[index].y;
        modelDrawInfo.pVertices[index].z = pTheModelArray[index].z;

        modelDrawInfo.pVertices[index].r = pTheModelArray[index].red;
        modelDrawInfo.pVertices[index].g = pTheModelArray[index].green;
        modelDrawInfo.pVertices[index].b = pTheModelArray[index].blue;

        // Copy the normal information we loaded, too! :)
        modelDrawInfo.pVertices[index].nx = pTheModelArray[index].nx;
        modelDrawInfo.pVertices[index].ny = pTheModelArray[index].ny;
        modelDrawInfo.pVertices[index].nz = pTheModelArray[index].nz;

    }

    modelDrawInfo.numberOfIndices = modelDrawInfo.numberOfTriangles * 3;

    modelDrawInfo.pIndices = new unsigned int[modelDrawInfo.numberOfIndices];
    modelDrawInfo.pTriangles = new sTrianglePLY[modelDrawInfo.numberOfTriangles];

    unsigned int vertex_element_index_index = 0;

    for (unsigned int triangleIndex = 0; triangleIndex != modelDrawInfo.numberOfTriangles; triangleIndex++)
    {
        modelDrawInfo.pIndices[vertex_element_index_index + 0] = pTheModelTriangleArray[triangleIndex].vertexIndices[0];
        modelDrawInfo.pIndices[vertex_element_index_index + 1] = pTheModelTriangleArray[triangleIndex].vertexIndices[1];
        modelDrawInfo.pIndices[vertex_element_index_index + 2] = pTheModelTriangleArray[triangleIndex].vertexIndices[2];

        modelDrawInfo.pTriangles[triangleIndex].vertexIndices[0] = pTheModelTriangleArray[triangleIndex].vertexIndices[0];
        modelDrawInfo.pTriangles[triangleIndex].vertexIndices[1] = pTheModelTriangleArray[triangleIndex].vertexIndices[1];
        modelDrawInfo.pTriangles[triangleIndex].vertexIndices[2] = pTheModelTriangleArray[triangleIndex].vertexIndices[2];

        vertex_element_index_index += 3;
    }

    delete[] pTheModelArray;
    delete[] pTheModelTriangleArray;


    return true;
}

bool LoadModelTypesIntoVAO(std::string fileTypesToLoadName,
    VAOManager* pVAOManager,
    GLuint shaderID)
{
    std::ifstream modelTypeFile(fileTypesToLoadName.c_str());
    if (!modelTypeFile.is_open())
    {
        // Can't find that file
        return false;
    }

    std::string PLYFileNameToLoad;
    std::string friendlyName;

    bool bKeepReadingFile = true;

    const unsigned int BUFFER_SIZE = 1000;
    char textBuffer[BUFFER_SIZE];
    // Clear that array to all zeros
    memset(textBuffer, 0, BUFFER_SIZE);

    while (bKeepReadingFile)
    {
        modelTypeFile.getline(textBuffer, BUFFER_SIZE);

        PLYFileNameToLoad.clear(); 
        PLYFileNameToLoad.append(textBuffer);

        if (PLYFileNameToLoad == "EOF")
        {
            // All done
            bKeepReadingFile = false;
            // Skip to the end of the while loop
            continue;
        }

        memset(textBuffer, 0, BUFFER_SIZE);
        modelTypeFile.getline(textBuffer, BUFFER_SIZE);
        friendlyName.clear();
        friendlyName.append(textBuffer);

        ModelDrawInfo motoDrawInfo;

        if (LoadThePLYFile(PLYFileNameToLoad, motoDrawInfo))
        {
            std::cout << "Loaded the file OK" << std::endl;
        }
        if (pVAOManager->LoadModelIntoVAO(friendlyName, motoDrawInfo, shaderID))
        {
            std::cout << "Loaded the MOTO model" << std::endl;
        }

    }

    return true;
}

int CalculateHashValue(const glm::vec3 v)
{
    int hashValue = 0;

    hashValue += floor(v.x + 128) / 10 * 10000;
    hashValue += floor(v.y + 128) / 10 * 100;
    hashValue += floor(v.z + 128) / 10;
    return hashValue;
}

void LoadStaticModelToAABBEnvironment(VAOManager pVAOManager, std::string name, const glm::vec3& position, float scale) {
    ModelDrawInfo cityDrawInfo;
    pVAOManager.FindDrawInfoByModelName(name, cityDrawInfo);


    std::vector<Vertex> vertices;
    for (int i = 0; i < cityDrawInfo.numberOfVertices; i++)
        vertices.push_back(cityDrawInfo.pVertices[i]);
    
    std::vector<sTrianglePLY> triangles;
    for (int i = 0; i < cityDrawInfo.numberOfTriangles; i++)
        triangles.push_back(cityDrawInfo.pTriangles[i]);

    
    glm::vec3 minPoints = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 maxPoints = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);
    glm::vec3 pos = position;
    for (int i = 0; i < vertices.size(); i++) {
        glm::vec3 vertex = glm::vec3(vertices[i].x, vertices[i].y, vertices[i].z);
        vertex *= scale;
        vertex += pos;

        if (minPoints.x > vertex.x)
            minPoints.x = vertex.x;
        if (minPoints.y > vertex.y)
            minPoints.y = vertex.y;
        if (minPoints.z > vertex.z)
            minPoints.z = vertex.z;

        if (maxPoints.x < vertex.x)
            maxPoints.x = vertex.x;
        if (maxPoints.y < vertex.y)
            maxPoints.y = vertex.y;
        if (maxPoints.z < vertex.z)
            maxPoints.z = vertex.z;
    }

    printf("MinPoints: (%.2f, %.2f, %.2f)\nMaxPoints: (%.2f, %.2f, %.2f)\n",
        minPoints.x, minPoints.y, minPoints.z,
        maxPoints.x, maxPoints.y, maxPoints.z);


    for (int i = 0; i < vertices.size(); i += 3)
    {
        glm::vec3 pointA = glm::vec3(vertices[i].x, vertices[i].y, vertices[i].z) * scale + pos;
        glm::vec3 pointB = glm::vec3(vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z) * scale + pos;
        glm::vec3 pointC = glm::vec3(vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].z) * scale + pos;

        int hashA = CalculateHashValue(pointA);
        int hashB = CalculateHashValue(pointB);
        int hashC = CalculateHashValue(pointC);

        Triangle* newTriangle = new Triangle(pointA, pointB, pointC);

        physicsSystem.AddTriangleToAABBCollisionCheck(hashA, newTriangle);

        if (hashA != hashB)
            physicsSystem.AddTriangleToAABBCollisionCheck(hashB, newTriangle);

        if (hashC != hashB && hashC != hashA)
            physicsSystem.AddTriangleToAABBCollisionCheck(hashC, newTriangle);
    }
}

float SqDistPointAABB(glm::vec3 p, AABB b)
{
    float sqDist = 0.0f;

    float v;
    v = p.x;
    if (v < b.Min[0]) sqDist += (b.Min[0] - v) * (b.Min[0] - v);
    if (v > b.Max[0]) sqDist += (v - b.Max[0]) * (v - b.Max[0]);

    v = p.y;
    if (v < b.Min[1]) sqDist += (b.Min[1] - v) * (b.Min[1] - v);
    if (v > b.Max[1]) sqDist += (v - b.Max[1]) * (v - b.Max[1]);

    v = p.z;
    if (v < b.Min[2]) sqDist += (b.Min[2] - v) * (b.Min[2] - v);
    if (v > b.Max[2]) sqDist += (v - b.Max[2]) * (v - b.Max[2]);

    return sqDist;
}

int TestSphereAABB(const glm::vec3& center, float radius, AABB b)
{
    float sqDist = SqDistPointAABB(center, b);
    return sqDist <= radius * radius;
}

int main(void)
{
    std::cout << "starting up..." << std::endl;

    GLFWwindow* window;
    GLuint vertex_buffer = 0;
    GLuint shaderID = 0;
    boundingBox = BoundingBox();
    physicsSystem = PhysicsSystem();
    
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Collision Detection", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    std::cout << "Window created." << std::endl;

    glfwSetKeyCallback(window, KeyCallback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    ShaderManager* shaderManager = new ShaderManager();

    Shader vertexShader01;
    Shader fragmentShader01;

    vertexShader01.fileName = "assets/shaders/vertexShader01.glsl";
    fragmentShader01.fileName = "assets/shaders/fragmentShader01.glsl";

    if (!shaderManager->CreateProgramFromFile("Shader_1", vertexShader01, fragmentShader01))
    {
        std::cout << "Didn't compile shaders" << std::endl;
        std::string theLastError = shaderManager->GetLastError();
        std::cout << "Because: " << theLastError << std::endl;
        return -1;
    }
    else
    {
        std::cout << "Compiled shader OK." << std::endl;
    }

    shaderManager->UseShaderProgram("Shader_1");

    shaderID = shaderManager->GetIDFromFriendlyName("Shader_1");

    glUseProgram(shaderID);

    ::gLightManager = new LightManager();
    LightHelper* pLightHelper = new LightHelper();
    ::gLightManager->LoadLightUniformLocations(shaderID);
    
    ::gLightManager->vLights[0].position = glm::vec4(-64.0f, 95.0f, -46.0f, 1.0f);
    ::gLightManager->vLights[0].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::gLightManager->vLights[0].atten = glm::vec4(0.01f, 0.1f, 0.001f, 1.0f);
    ::gLightManager->vLights[0].TurnOn();

    // Load the models
    VAOManager* pVAOManager = new VAOManager();
    if (!LoadModelTypesIntoVAO("assets/PLYFilesToLoadIntoVAO.txt", pVAOManager, shaderID))
    {
        std::cout << "Error: Unable to load list of models to load into VAO file" << std::endl;
    }

    std::vector< MeshObject* > vec_pMeshObjects;

    MeshObject* pCity = new MeshObject();
    pCity->meshName = "City";
    pCity->friendlyName = "Sirus City";
    pCity->scale = 10.0f;
    vec_pMeshObjects.push_back(pCity);

    MeshObject* pBoat = new MeshObject();
    pBoat->meshName = "Boat";
    pBoat->friendlyName = "Boat";
    pBoat->scale = 2.0f;
    pBoat->position = gDronePosition;
    vec_pMeshObjects.push_back(pBoat);

    MeshObject* pDebugSphere_1 = new MeshObject();
    pDebugSphere_1->meshName = "ISOSphere";
    pDebugSphere_1->friendlyName = "Debug Sphere 1";
    pDebugSphere_1->bUseRGBAColour = true;
    pDebugSphere_1->RGBAColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pDebugSphere_1->isWireframe = true;
    pDebugSphere_1->scale = 1.0f;
    pDebugSphere_1->bDoNotLight = true;
    vec_pMeshObjects.push_back(pDebugSphere_1);

    MeshObject* pSphere = new MeshObject();
    pSphere->meshName = "ISOSphere";
    pSphere->friendlyName = "Debug Sphere 1";
    pSphere->bUseRGBAColour = true;
    pSphere->position = glm::vec3(7.94f, 80.2f, -93.8f);
    pSphere->RGBAColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pSphere->isWireframe = true;
    pSphere->scale = 1.0f;
    pSphere->bDoNotLight = true;
    vec_pMeshObjects.push_back(pSphere);

    // Get the vertices of the city
    glm::vec3 minPoints = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 maxPoints = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);

    ModelDrawInfo droneDrawInfo;
    pVAOManager->FindDrawInfoByModelName("Drone", droneDrawInfo);

    for (int i = 0; i < droneDrawInfo.numberOfVertices; i++) {
        Vertex& vertex = droneDrawInfo.pVertices[i];

        if (minPoints.x > vertex.x)
            minPoints.x = vertex.x;
        if (minPoints.y > vertex.y)
            minPoints.y = vertex.y;
        if (minPoints.z > vertex.z)
            minPoints.z = vertex.z;

        if (maxPoints.x < vertex.x)
            maxPoints.x = vertex.x;
        if (maxPoints.y < vertex.y)
            maxPoints.y = vertex.y;
        if (maxPoints.z < vertex.z)
            maxPoints.z = vertex.z;
    }

    glm::vec3 halfExtents = (maxPoints - minPoints) / 2.f;
    glm::vec3 centerPoint = minPoints + halfExtents;

    printf("Minimum Points: (%.2f, %.2f, %.2f)\n",
        minPoints.x,
        minPoints.y,
        minPoints.z);
    printf("Maximum Points: (%.2f, %.2f, %.2f)\n",
        maxPoints.x,
        maxPoints.y,
        maxPoints.z);
    printf("Half Extents: (%.2f, %.2f, %.2f)\n",
        halfExtents.x,
        halfExtents.y,
        halfExtents.z);
    printf("Center Point: (%.2f, %.2f, %.2f)\n",
        centerPoint.x,
        centerPoint.y,
        centerPoint.z);

    boundingBox.centerPoint = centerPoint;
    boundingBox.halfExtents = halfExtents;
    boundingBox.maxPoints = maxPoints;
    boundingBox.minPoints = minPoints;

    LoadStaticModelToAABBEnvironment(*pVAOManager, "City", pCity->position, 0.2f);

    GLint mvp_location = glGetUniformLocation(shaderID, "MVP");
    GLint mModel_location = glGetUniformLocation(shaderID, "mModel");
    GLint mView_location = glGetUniformLocation(shaderID, "mView");
    GLint mProjection_location = glGetUniformLocation(shaderID, "mProjection");
    GLint mModelInverseTransform_location = glGetUniformLocation(shaderID, "mModelInverseTranspose");

    while (!glfwWindowShouldClose(window))
    {
        ::gDronePosition.z += 0.05f;
        ::gDronePosition.y -= 0.03f;

        pSphere->position.z += 0.05f;
        pSphere->position.y -= 0.03f;

        pDebugSphere_1->position = glm::vec3(::gLightManager->vLights[0].position); 

        pBoat->position = gDronePosition;

        // Detect Collusion
        Sphere* sphere = new Sphere(glm::vec3(0), 0.5f);
        int spherePositionHash = CalculateHashValue(pSphere->position);
        

        std::vector<Triangle*> aabbTriangles = physicsSystem.GetAABBStructure()[spherePositionHash];
        if (aabbTriangles.size() > 0) {
            for (int i = 0; i < aabbTriangles.size(); i++) {
                //TestSphereAABB(aabbTriangles[i]., sphere->Radius, *dynamic_cast<AABB*>(sphere)); 
            }
        }

        float distance10percent = pLightHelper->CalcApproxDistFromAtten(
            0.1f,
            0.001f,
            100000.0f,
            ::gLightManager->vLights[0].atten.x,
            ::gLightManager->vLights[0].atten.y,
            ::gLightManager->vLights[0].atten.z);

        ::gLightManager->CopyLightInformationToShader(shaderID);

        float ratio;
        int width, height;

        glm::mat4x4 matModel;
        glm::mat4x4 matProjection;
        glm::mat4x4 matView;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        matView = glm::lookAt(::gCameraEye,
            ::gCameraTarget,
            upVector);

        GLint eyeLocation_UniLoc = glGetUniformLocation(shaderID, "eyeLocation");

        glUniform4f(eyeLocation_UniLoc,
            ::gCameraEye.x, ::gCameraEye.y, ::gCameraEye.z, 1.0f);

        matProjection = glm::perspective(
            0.6f,
            ratio,
            0.1f,
            10000.0f);

        for (std::vector< MeshObject* >::iterator itCurrentMesh = vec_pMeshObjects.begin();
            itCurrentMesh != vec_pMeshObjects.end();
            itCurrentMesh++)
        {
            MeshObject* pCurrentMeshObject = *itCurrentMesh;

            glCullFace(GL_BACK);
            glEnable(GL_DEPTH_TEST);

            matModel = glm::mat4x4(1.0f);
            
            glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f),
                pCurrentMeshObject->position);

            glm::mat4 matRoationZ = glm::rotate(glm::mat4(1.0f),
                pCurrentMeshObject->rotation.z,
                glm::vec3(0.0f, 0.0f, 1.0f));

            glm::mat4 matRoationY = glm::rotate(glm::mat4(1.0f),
                pCurrentMeshObject->rotation.y,
                glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 matRoationX = glm::rotate(glm::mat4(1.0f),
                pCurrentMeshObject->rotation.x,
                glm::vec3(1.0f, 0.0f, 0.0f));

            if (pCurrentMeshObject->friendlyName == "Good") {
                pCurrentMeshObject->position.x += 0.005f;
            }
            else if (pCurrentMeshObject->friendlyName == "Evil") {
                pCurrentMeshObject->position.x -= 0.006f;
            }

            // Scale the object
            float uniformScale = pCurrentMeshObject->scale;
            glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                glm::vec3(uniformScale, uniformScale, uniformScale));

            matModel = matModel * matTranslation;

            matModel = matModel * matRoationX;
            matModel = matModel * matRoationY;
            matModel = matModel * matRoationZ;

            matModel = matModel * matScale;

            glUniformMatrix4fv(mModel_location, 1, GL_FALSE, glm::value_ptr(matModel));
            glUniformMatrix4fv(mView_location, 1, GL_FALSE, glm::value_ptr(matView));
            glUniformMatrix4fv(mProjection_location, 1, GL_FALSE, glm::value_ptr(matProjection));

            glm::mat4 mModelInverseTransform = glm::inverse(glm::transpose(matModel));
            glUniformMatrix4fv(mModelInverseTransform_location, 1, GL_FALSE, glm::value_ptr(mModelInverseTransform));

            if (pCurrentMeshObject->isWireframe)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            GLint RGBA_Colour_ULocID = glGetUniformLocation(shaderID, "RGBA_Colour");

            glUniform4f(RGBA_Colour_ULocID,
                pCurrentMeshObject->RGBAColour.r,
                pCurrentMeshObject->RGBAColour.g,
                pCurrentMeshObject->RGBAColour.b,
                pCurrentMeshObject->RGBAColour.w);


            GLint bUseRGBA_Colour_ULocID = glGetUniformLocation(shaderID, "bUseRGBA_Colour");

            if (pCurrentMeshObject->bUseRGBAColour)
            {
                glUniform1f(bUseRGBA_Colour_ULocID, (GLfloat)GL_TRUE);
            }
            else
            {
                glUniform1f(bUseRGBA_Colour_ULocID, (GLfloat)GL_FALSE);
            }

            //uniform bool bDoNotLight;	
            GLint bDoNotLight_Colour_ULocID = glGetUniformLocation(shaderID, "bDoNotLight");

            if (pCurrentMeshObject->bDoNotLight)
            {
                glUniform1f(bDoNotLight_Colour_ULocID, (GLfloat)GL_TRUE);
            }
            else
            {
                glUniform1f(bDoNotLight_Colour_ULocID, (GLfloat)GL_FALSE);
            }


            // Choose the VAO that has the model we want to draw...
            ModelDrawInfo drawingInformation;
            if (pVAOManager->FindDrawInfoByModelName(pCurrentMeshObject->meshName, drawingInformation))
            {
                glBindVertexArray(drawingInformation.VAOId);

                glDrawElements(GL_TRIANGLES,
                    drawingInformation.numberOfIndices,
                    GL_UNSIGNED_INT,
                    (void*)0);

                glBindVertexArray(0);

            }
            else
            {
                std::cout << "Error: didn't find model to draw." << std::endl;

            }

        }

        glfwSwapBuffers(window);
        glfwPollEvents();;

        std::stringstream ssTitle;
        ssTitle << "Camera (x,y,z): "
            << ::gCameraEye.x << ", "
            << ::gCameraEye.y << ", "
            << ::gCameraEye.z
            << "  Light #0 (xyz): "
            << ::gLightManager->vLights[0].position.x << ", "
            << ::gLightManager->vLights[0].position.y << ", "
            << ::gLightManager->vLights[0].position.z
            << " linear: " << ::gLightManager->vLights[0].atten.y
            << " quad: " << ::gLightManager->vLights[0].atten.z;

        std::string theText = ssTitle.str();

        glfwSetWindowTitle(window, ssTitle.str().c_str());
    }

    delete shaderManager;
    delete ::gLightManager;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}