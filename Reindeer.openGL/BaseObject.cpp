/*
 * BaseObject.cpp
 *
 *  Created on: May 2, 2014
 *      Author: MRP
 */

#include "BaseObject.h"

BaseObject::BaseObject() {
	position.x = position.y = position.z = 0.0f;
	euler.x = euler.y = euler.z = 0.0f;
	scale.x = scale.y = scale.z = 1.0f;
	quaternion = glm::quat(euler);
	BuildWorld();
}

BaseObject::~BaseObject() {
	// TODO Auto-generated destructor stub
}

void BaseObject::SetPosition(const glm::vec3& position, bool silent) {
	this->position = position;
	if (!silent)
		BuildWorld();
}

glm::vec3 BaseObject::GetPosition() {
	return position;
}

void BaseObject::SetEuler(const glm::vec3& euler, bool silent) {
	this->euler.x = fmod(euler.x, 360);
	this->euler.y = fmod(euler.y, 360);
	this->euler.z = fmod(euler.z, 360);
	if (!silent) {
		glm::vec3 radEuler(ToRadian(this->euler.x), ToRadian(this->euler.y), ToRadian(this->euler.z));
		quaternion = glm::quat(radEuler);
		BuildWorld();
	}
}

glm::vec3 BaseObject::GetEuler() {
	return euler;
}

void BaseObject::BuildWorld() {
	rotationMatrix = glm::toMat4(quaternion);
	world = glm::translate(glm::mat4(1.0f), position) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
}
