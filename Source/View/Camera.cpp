#include "Camera.h"
#include <iostream>

Camera::Camera(const glm::vec3 &eye, const glm::vec3 &forward, const glm::vec3 &world_up, double vertical_fov, double aspect_ratio, double near, double far)
	: InputEnabled(), _vertical_fov(vertical_fov), _aspect_ratio(aspect_ratio), _near(near), _far(far) {
	_perspective = glm::perspective(_vertical_fov, _aspect_ratio, _near, _far);
	_translation = glm::translate(_translation, eye);
}

void Camera::update(double delta_time) {
	//The scale on third parameter to SLERP determines speed of interpolation
	_rotation_quat = glm::slerp(_rotation_quat, _next_rotation_quat, (float)delta_time * 10.0f);
	_rotation_mat = glm::mat4_cast(_rotation_quat);
	//Multiply rotation and translation matrices to get view matrix, keeping to quaternions to avoid Gimbal locking
	_view = _rotation_mat * _translation;

	//DEBUG
	/*_time += delta_time;
	if (_time > .50) {
		_time = 0;
		std::cout << _view[0].x << " " << _view[0].y << " " << _view[0].z << std::endl;
		std::cout << _view[1].x << " " << _view[1].y << " " << _view[1].z << std::endl;
		std::cout << _view[2].x << " " << _view[2].y << " " << _view[2].z << std::endl;
		std::cout << _view[3].x << " " << _view[3].y << " " << _view[3].z << std::endl;
		std::cout << std::endl;
	}*/
}

bool Camera::intersects_point(const glm::vec3 &point) {
	return false;
}

bool Camera::intersects_sphere(const glm::vec3 &center, double radius) {
	return false;
}

bool Camera::intersects_box(const glm::vec3 &center, const glm::vec3 &extents) {
	return false;
}

void Camera::handle_mouse_movement(double x, double y, double delta_x, double delta_y, double acc_x, double acc_y, double delta_time) {
	float rx = glm::radians((float)acc_x) * 0.01f;
	float ry = glm::radians((float)acc_y) * 0.01f;
	//Create an identity quaternion and rotate the original orthonormal basis
	_next_rotation_quat = glm::quat(1, 0, 0, 0);
	_next_rotation_quat = glm::rotate(_next_rotation_quat, -ry, glm::vec3(1,0,0));
	_next_rotation_quat = glm::rotate(_next_rotation_quat, -rx, glm::vec3(0,1,0));
}

void Camera::handle_multiple_keystrokes(GLFWwindow *window, double delta_time) {
	//Create a translation in the original orthonormal basis
	glm::vec4 translation;
	if (glfwGetKey(window, GLFW_KEY_W)) {
		translation.z += (float)delta_time;
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		translation.z -= (float)delta_time;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		translation.x += (float)delta_time;
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		translation.x -= (float)delta_time;
	}

	//For rolling
	if (glfwGetKey(window, GLFW_KEY_Q)) {
		_accumulated_roll += delta_time;
	} 
	if (glfwGetKey(window, GLFW_KEY_E)) {
		_accumulated_roll -= delta_time;
	}
	_next_rotation_quat = glm::rotate(_next_rotation_quat, (float)_accumulated_roll, glm::vec3(0, 0, 1));

	//Undo rotation that will be applied later
	translation = glm::inverse(_rotation_mat) * translation;
	_translation = glm::translate(_translation, glm::vec3(translation));
}




