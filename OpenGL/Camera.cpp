#include "Camera.h"

//constructor
Camera::Camera()
{
    position = { 0,0,0 };//init cam postion
    LaMat = glm::mat4x4(1.0f, 0.0f, 0.0f, 0.0f,//init look at mat
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

void Camera::setPrespMat(float n, float f, float r, float t)
{
    if (n < f)
    {
        this->f = f;
        this->n = n;
        //matrix to transform frustrom into cube, (assuming its symetrical)
        prespMat = glm::mat4x4(n / r, 0.0f, 0.0f, 0.0f,//first column
            0.0f, n / t, 0.0f, 0.0f,//second column
            0.0f, 0.0f, -float((f + n)) / float((f - n)), (-2 * float(f * n)) / float((f - n)),//third column
            0.0f, 0.0f, -1.0f, 0.0f);//fourth column
    }

}

void Camera::setPrespMat(float n, float f, float FoV)
{
    //matrix to transform frustrom into cube (setting sides with an angle)
    float t, r;
    t = glm::tan(glm::radians(FoV));
    r = t*(aspect_ratio);
    setPrespMat(n, f, r, t);
}

void Camera::setViewMat(float width, float height)
{
    //set view matrix, using pan and scan (aspect ration same as the one set in the prespective)
    float scale = height;
    if (width > height) {
        scale = width;
    }
    viewMat = getScale({ scale * 0.5f,scale * 0.5f,1 }) * getTranslation({ width * 0.5f,height * 0.5f,0 });
}
//set position
void Camera::moveCam(float x, float y, float z)
{
    position = { x,y,z };
}

//calculate look at matrix using a relative up vector and vectors pointing to object
void Camera::lookAt(float x, float y, float z)
{
    glm::vec3 location = { x,y,z };//place to look at
    glm::vec3 f = glm::normalize(position - location);//forward axis directly at object
    glm::vec3 r = glm::normalize(glm::cross({ 0,1,0 }, f));//right with relative up
    glm::vec3 u = glm::normalize(glm::cross(f, r));//find perpendicular to right and forward vectors
    LaMat = glm::mat4x4(r.x, r.y, r.z, 0.0f,//first column
        u.x, u.y, u.z, 0.0f,//second column
        f.x, f.y, f.z, 0.0f,//third column
        0.0f, 0.0f, 0.0f, 1);//fourth column

}
//get final full camera matrix
glm::mat4x4 Camera::getPrespMat()
{
    glm::mat4x4 final = getTranslation({ -position.x,-position.y,-position.z }) * LaMat * prespMat;
    return final;
}


//getter for view mat
glm::mat4x4 Camera::getViewMat()
{
    return viewMat;
}
