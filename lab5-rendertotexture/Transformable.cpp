#pragma once
#include "Transformable.h"
#include "Engine.h"


void Transformable::update()
{
	//std::cout << "Upddate Game Entity" << std::endl;

}

void Transformable::update(GameEntity& object) {
	//std::cout << "Upddate Game Entity" << std::endl;
}

vec3 Transformable::getTranslate()
{
	return this->translateVector;
}

void Transformable::setTransLate(vec3 translate)
{
	this->translateVector = translate;
}

vec3 Transformable::getScale()
{
	return this->scaleVector;
}

void Transformable::setScale(vec3 scale)
{
	this->scaleVector = scale;
}

vec3 Transformable::getRotate()
{
	return this->rotateVector;
}

float Transformable::getAngle()
{
	return this->rotateAngle;
}

void Transformable::setRotate(vec3 rotate, float angle)
{
	this->rotateVector = rotate;
	this->rotateAngle = angle;
}

void Transformable::setRotateAngle(float angle)
{
	this->rotateAngle = angle;
}

void Transformable::setFaceMatrix(mat4 faceMatrix)
{
	this->faceMatrix = faceMatrix;
}

mat4 Transformable::getTransformationMatrix()
{
	mat4 resultingMatrix =
		translate(translateVector) *
		rotate(rotateAngle, rotateVector) *
		scale(scaleVector);

	if (faceMatrix != mat4(0)) {
		resultingMatrix = 
			translate(translateVector) * rotate(rotateAngle, rotateVector) * faceMatrix * scale(scaleVector);
	}

	return resultingMatrix;
}

Transformable::Transformable(Engine* e, GameObject* go)
{
	this->rotateAngle = 0.0f;
	this->translateVector = vec3(1, 1, 1);
	this->scaleVector = vec3(1, 1, 1);
	this->rotateAngle = 0;
	this->rotateVector = vec3(0, 1, 0);

	this->faceMatrix = mat4(0);

	ObjectComponent::create(e, go);
}

Transformable::~Transformable()
{
}

Renderable::Renderable(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
}

Renderable::~Renderable()
{
	this->model = nullptr;
}

void Renderable::update()
{

	Transformable* myTransform = (Transformable*)this
		->gameObject
		->getComponent(componentType::TRANSFORMABLE);

	if (myTransform == nullptr)
		return;

	mat4 myMatrix = myTransform->getTransformationMatrix();

	labhelper::setUniformSlow(
		engine->shaderProgram, 
		"modelViewProjectionMatrix", 
		engine->projectionMatrix * engine->viewMatrix * myMatrix
	);
	labhelper::setUniformSlow(
		engine->shaderProgram, 
		"modelViewMatrix", 
		engine->viewMatrix * myMatrix
	);

	labhelper::setUniformSlow(
		engine->shaderProgram, 
		"normalMatrix", 
		inverse(
			transpose(engine->viewMatrix * myMatrix)
		)
	);

	labhelper::render(this->model);
}

void Renderable::setModel(labhelper::Model* m)
{
	this->model = m;
}

WanderingComponent::WanderingComponent(Engine* e, GameObject* go)
{
	wanderVelosity = vec3(20.0f, 0, 0);
	ObjectComponent::create(e, go);
}

void WanderingComponent::update()
{
	int cTime = this->engine->getCurrentTime();

	RigidBodyComponent* charRigidBody = (RigidBodyComponent*)this
		->gameObject
		->getComponent(componentType::RIGID_BODY);

	Transformable* charTransform = (Transformable*)this
		->gameObject
		->getComponent(componentType::TRANSFORMABLE);

	vec3 currentPos = charTransform->getTranslate();
	vec3 unitatio = normalize(currentPos);
	//float delta = (float)this->engine->getDeltaTime();

	if (cTime % 5 == 0 && count != cTime) {
		count = cTime;
		std::cout << "on: " << cTime << std::endl;
		float newRotation = rand() % (int)maxRotation + 1;

		vec2 charOrientation(
			unitatio.x * maxSpeed,
			unitatio.z * maxSpeed
		);

		charOrientation = rotate(charOrientation, newRotation);

		wanderVelosity = vec3(
			charOrientation.x,
			0,
			charOrientation.y
		);

		charTransform->setRotateAngle(newRotation);
	}

	charRigidBody->velocity = wanderVelosity;



	//count++;

}

RigidBodyComponent::RigidBodyComponent(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
	velocity = vec3(0,0,0);
	acceleration = vec3(0, 0, 0);
}

void RigidBodyComponent::update()
{

	Transformable* tran =
		(Transformable*)this->gameObject
		->getComponent(componentType::TRANSFORMABLE);
	
	if (!tran)
		return;
	vec3 position = tran->getTranslate();
	float dt = engine->getDeltaTime();

	if(acceleration.x == 0 && acceleration.y == 0 && acceleration.z== 0)
		tran->setTransLate(position+velocity * dt);
	else 
		tran->setTransLate(position + velocity + acceleration * dt);
	//go->position = go->position + velocity * dt;
}

BoxBound::BoxBound(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
}

void BoxBound::SetBounds(vec2 front, vec2 back)
{
	this->front = front;
	this->back = back;
}

void BoxBound::update()
{
	Transformable* transf =
		(Transformable*)this->gameObject
		->getComponent(TRANSFORMABLE);

	RigidBodyComponent* rigidBody =
		(RigidBodyComponent*)this->gameObject
		->getComponent(RIGID_BODY);

	vec3 pos = transf->getTranslate();
	vec2 twod(pos.x,pos.z);
	vec2 velosity = rigidBody->velocity;
	float delta = engine->getDeltaTime();
	if (((twod.x + velosity.x * delta) >= this->front.x) ||
		((twod.y + velosity.y * delta) >= this->front.y) ||
		((twod.x + velosity.x * delta) <= this->back.x) ||
		((twod.y + velosity.y * delta) <= this->back.y) ) {
		rigidBody->velocity *= -1.0f;
	}
}

bool BulletBehavior::isOutOfRange()
{
	Transformable* myTra =
		(Transformable*)this->gameObject->getComponent(TRANSFORMABLE);

	vec3 currentPos = myTra->getTranslate();

	double d = distance(currentPos, initialPosition);

	return (d > 500.0f);
}

BulletBehavior::BulletBehavior()
{
}

BulletBehavior::BulletBehavior(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
}

GameObject* BulletBehavior::getOwner()
{
	return this->owner;
}

void BulletBehavior::SetOwner(GameObject* o)
{
	this->owner = o;
}

void BulletBehavior::Start()
{
	RigidBodyComponent* ownerBody =
		(RigidBodyComponent*)this->owner->getComponent(RIGID_BODY);
	RigidBodyComponent* myBody =
		(RigidBodyComponent*)this->gameObject->getComponent(RIGID_BODY);
	Transformable* ownerTra =
		(Transformable*)this->owner->getComponent(TRANSFORMABLE);

	this->initialPosition = ownerTra->getTranslate();

	vec3 uniVelosity = normalize(ownerBody->velocity);

	myBody->velocity = uniVelosity;
	myBody->acceleration = uniVelosity;
}

void BulletBehavior::update()
{

	Collidable* myColli =
		(Collidable*)this->gameObject->getComponent(COLLIDABLE);
	
	bool isCollided = myColli->isCollided;

	bool outOfRange = this->isOutOfRange();

	if (isCollided && (myColli->currentColided == this->owner)) {
		return;
	}

	if (outOfRange || isCollided) {
		//SDL_Log("DESTROY BULLET");
		
		ObjectComponent* behavior = this->owner->getComponent(BEHAVIOR);
		

		PlayerBehavior* player = 
			dynamic_cast<PlayerBehavior*>(behavior);
		TankBehavior* tank = dynamic_cast<TankBehavior*>(behavior);



		if (player) {
			player->score += 10;
			SDL_Log(">>>>DESTROY");
			this->gameObject->Destroy();
			return;
		}
		
		if (tank) {
			return;
		}
		if (behavior) {
			
			this->gameObject->Destroy();
		}
	}

}

Collidable::Collidable(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
	this->isCollided = false;
}

void Collidable::update()
{
	Transformable* tr = (Transformable*)this->gameObject->getComponent(TRANSFORMABLE);
	geometry.x = tr->getTranslate().x;
	geometry.y = tr->getTranslate().z;

	this->currentColided = nullptr;
	this->isCollided = false;

	for (GameObject* object : engine->getGameObjects()) {
		if (object == this->gameObject)
			continue;

		Collidable* other =
			(Collidable*)object->getComponent(COLLIDABLE);

		if (!other)
			continue;

		CollidableGeometry g1 = this->geometry;
		CollidableGeometry g2 = other->geometry;

		float dx = g1.x - g2.x;
		float dy = g1.y - g2.y;

		float distance = std::sqrt(dx * dx + dy * dy);

		if (distance < (g1.radius + g2.radius)) {
			other->isCollided = 1;
			this->isCollided = 1;
			this->currentColided = object;
			other->currentColided = this->gameObject;

			return;
		}
	}
	
	//this->currentColided = nullptr;
}

bool Collidable::isObjectCollided()
{
	return isCollided;
}

void Collidable::setCollidableRadius(double r)
{
	this->geometry.radius = r;
}

bool Collidable::isOwner(GameObject* o)
{
	return false;
}

RockBehavior::RockBehavior(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
}

void RockBehavior::update()
{
	Collidable* myColli =
		(Collidable*)this->gameObject->getComponent(COLLIDABLE);

	bool isCollided = myColli->isCollided;
	
	if (!isCollided || !myColli->currentColided)
		return;

	ObjectComponent* behavior =
		myColli->currentColided->getComponent(BEHAVIOR);

	BulletBehavior* t = dynamic_cast<BulletBehavior*>(behavior);
	TargetedBullet* tb = dynamic_cast<TargetedBullet*>(behavior);


	if ( t != nullptr && tb == nullptr) {
		this->gameObject->Destroy();
	}
}

TankBehavior::TankBehavior(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
	this->fireFunction = nullptr;
}

void TankBehavior::addFireFunction(void(*function)(
	Engine* engine, 
	GameObject* owner, 
	GameObject* target))
{
	fireFunction = function;
}

void TankBehavior::update()
{
	

	int cTime = this->engine->getCurrentTime();
	if (cTime % rate == 0 && count!=cTime) {
		SDL_Log("CREATE");
		createTankBullet();

		this->count = cTime;
	}

	


	Collidable* myColli =
		(Collidable*)this->gameObject->getComponent(COLLIDABLE);

	bool isCollided = myColli->isCollided;

	if (!isCollided || !myColli->currentColided)
		return;

	ObjectComponent* behavior =
		myColli->currentColided->getComponent(BEHAVIOR);

	BulletBehavior* t = dynamic_cast<BulletBehavior*>(behavior);

	RockBehavior* rock = dynamic_cast<RockBehavior*>(behavior);


	if (t != nullptr) {
		ObjectComponent* ownerBehav = t->getOwner()->getComponent(BEHAVIOR);
		TankBehavior* ownerTank = dynamic_cast<TankBehavior*>(ownerBehav);

		if (ownerTank)
			return;
		if(ownerBehav)
			this->gameObject->Destroy();

	}

	if (rock != nullptr) {
		RigidBodyComponent* rigidBody =
			(RigidBodyComponent*)this->gameObject->getComponent(RIGID_BODY);
		
		rigidBody->velocity *= -1.0f;
	}
}

void TankBehavior::createTankBullet()
{
	GameObject* bullet = new GameObject();

	TargetedBullet* behavior = new TargetedBullet(this->engine, bullet);
	behavior->SetOwner(this->gameObject);

	Renderable* renderBullet = new Renderable(this->engine, bullet);
	renderBullet->setModel(this->bulletModel);

	Transformable* transf = new Transformable(this->engine, bullet);
	Transformable* ownTransf = (Transformable*)this->gameObject->getComponent(TRANSFORMABLE);
	transf->setTransLate(ownTransf->getTranslate());

	RigidBodyComponent* rigidBody = new RigidBodyComponent(this->engine, bullet);

	Collidable* colli = new Collidable(this->engine, bullet);
	colli->setCollidableRadius(10.0f);

	bullet->addComponent(
		behavior, BEHAVIOR
	);
	bullet->addComponent(
		renderBullet, RENDERABLE
	);
	bullet->addComponent(
		transf, TRANSFORMABLE
	);
	bullet->addComponent(
		rigidBody, RIGID_BODY
	);
	bullet->addComponent(
		colli, COLLIDABLE
	);
	behavior->Start();

	this->engine->addGameObject(bullet);
}

WandeSeekComponent::WandeSeekComponent(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
}

void WandeSeekComponent::update()
{
	RigidBodyComponent* charRigidBody = (RigidBodyComponent*)this
		->gameObject
		->getComponent(componentType::RIGID_BODY);

	Transformable* charTransform = (Transformable*)this
		->gameObject
		->getComponent(componentType::TRANSFORMABLE);

	/*vec3 currentPos = charTransform->getTranslate();
	vec3 unitatio = normalize(currentPos);*/
	maxVelosity = 5.0f;

	/*charRigidBody->velocity = 
		vec3(maxVelosity *unitatio.x,0, 
			maxVelosity *unitatio.z);*/

	vec3 charPos = charTransform->getTranslate();
	vec3 targetPos = vec3(target->x, 0.0f, target->z);
	double dist = distance(targetPos,charPos);

	if (dist < 100) {
		
		faceToTarget(charTransform, targetPos);
		charRigidBody->velocity =
			this->getSeekSteating(charTransform, charRigidBody);

		return;
	}

	
	charRigidBody->velocity = 
		this->getWanderingStearing(charTransform, charRigidBody);
}

void WandeSeekComponent::setTarget(vec3* t)
{
	this->target = t;
}

vec3 WandeSeekComponent::getSeekSteating(Transformable* tr, RigidBodyComponent* rb)
{
	vec3 pos = tr->getTranslate();
	vec3 targ = vec3(target->x, target->y, target->z);
	vec3 newVelosity = targ - pos;
	

	newVelosity = normalize(newVelosity) * maxVelosity;
	
	return newVelosity;
}

vec3 WandeSeekComponent::getWanderingStearing(Transformable* tr, RigidBodyComponent* rb)
{
	int cTime = this->engine->getCurrentTime();
	if (cTime % 5 == 0 && count != cTime) {
		count = cTime;
		int wanderRadius = 100;
		int wanderOffset = 200;
		int wanderRate = 350;
		float wanderOrientation =
			rand() % (int)wanderRate + 1;
		vec3 charPos = tr->getTranslate();

		float charOrientation =
			atan(charPos.z, charPos.x);

		float targetOrientation =
			wanderOrientation + charOrientation;

		vec3 targetn =
			charPos +
			vec3(
				wanderOffset * cos(charOrientation),
				0,
				wanderOffset * sin(charOrientation)
			);


		targetn +=

			vec3(
				wanderRadius * cos(targetOrientation),
				0,
				wanderRadius * sin(targetOrientation)
			);

		faceToTarget(tr, targetn);

		vec3 newVelosity = targetn - charPos;
		newVelosity = normalize(newVelosity) * maxVelosity;

		//SDL_Log("NEW VEL: %d %d", newVelosity.x, newVelosity.z);
		return newVelosity;

	}
	//SDL_Log("SAME VEL: %d %d", rb->velocity.x, rb->velocity.z);
	return rb->velocity;
}

void WandeSeekComponent::faceToTarget(Transformable* tr, vec3 targetPos)
{	
	vec3 charPos = tr->getTranslate();
	//vec3 targetPos = vec3(target->x, target->y, target->z);
	vec3 direction = normalize(targetPos - charPos);

	glm::vec3 up(0, 1, 0);
	
	GLfloat theta = angle(
		vec2(targetPos.x, targetPos.z), 
		vec2(direction.x, direction.z));
	
	mat4 lookAtTargetMatrix = 
		transpose(lookAt(charPos, targetPos, vec3(0,1,0)));

	tr->setFaceMatrix(toMat4(toQuat(lookAtTargetMatrix)));


}

PlayerBehavior::PlayerBehavior(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
	this->score = 0;
	this->life = 100000;
}

void PlayerBehavior::setLife(int l)
{
	life = l;
}

void PlayerBehavior::update()
{
	Collidable* col = (Collidable*)this->gameObject->getComponent(COLLIDABLE);
	if (col->currentColided)
		SDL_Log("HOLAAA");
}



TargetedBullet::TargetedBullet(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
}

void TargetedBullet::update()
{
	Collidable* myColli =
		(Collidable*)this->gameObject->getComponent(COLLIDABLE);

	bool isCollided = myColli->isCollided;

	
	bool outOfRange = this->isOutOfRange();

	if (outOfRange) {
		this->gameObject->Destroy();
		return;
	}

	if (isCollided) {
		GameObject* other = myColli->currentColided;
		if (other == this->getOwner())
			return;
		ObjectComponent* behavior =
			other->getComponent(BEHAVIOR);
		PlayerBehavior* player =
			dynamic_cast<PlayerBehavior*>(behavior);

		if (player) {
			player->life -= 1;
			this->gameObject->Destroy();
			return;
		}
	}
}

UfoBehavior::UfoBehavior(Engine* e, GameObject* go)
{
	this->engine = e;
	this->gameObject = go;
	this->currentPoint = 0;
}

void UfoBehavior::setPoints(vec3(&p)[9])
{
	for (int i = 0; i < 9; i++) {
		this->points[i] = p[i];
	}
}

void UfoBehavior::setTransform(Transformable* t)
{
	this->transform = t;
}

void UfoBehavior::setRigidBody(RigidBodyComponent* rb)
{
	this->rigidBody = rb;
}

void UfoBehavior::update()
{

	vec3 currentPos = this->transform->getTranslate();
	vec3 nextPos = this->points[currentPoint];
	float dist = distance(currentPos, nextPos);
	if (dist<=1) {
		if (currentPoint + 1 > 8)
			currentPoint = 0;
		else
			currentPoint += 1;
	}

	this->rigidBody->velocity = 
		this->seekPoint(this->points[currentPoint]);

	
}

vec3 UfoBehavior::seekPoint(vec3 target)
{
	vec3 pos = transform->getTranslate();
	//vec3 targ = vec3(target.x, target.y, target->z);
	vec3 newVelosity = target - pos;


	newVelosity = normalize(newVelosity) * maxVelosity;
	return newVelosity;
}
