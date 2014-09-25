#include "World.h"
#include "BoxObject.h"

using namespace std;

int main()
{
	int nBox;
	printf("nBox: ");
	scanf("%d", &nBox);
	BoxObject* box = new BoxObject[nBox];

	printf("scale: ");
	scanf("%f", &PHYSICS_WORLD_SCALE);

	World world;
    world.Init();

    glm::vec3 boxmin(-0.1f, -0.1f, -0.1f);
	glm::vec3 boxmax( 0.1f,  0.1f,  0.1f);

	for (int i = 0; i < nBox; i++)
	{
		box[i].SetProgramId(world.programId);
		box[i].SetMinMax(boxmin, boxmax);
		box[i].SetPosition(glm::vec3(-1.0f + i*0.1f, 7.0f + i*1.0f, 0.0f), true);
		box[i].SetEuler(glm::vec3(44.0f, 10.0f, 30.0f), true);
		box[i].BuildWorld();
		world.DrawableObjects.push_back(&box[i]);
	}

	Camera& camera = world.camera;
	camera.SetPosition(glm::vec3(0.0f, 1.0f, -3.0f), true);
	//camera.SetEuler(glm::vec3(0, 0, 0), true);
	camera.BuildWorld();

	world.Render();

	delete[] box;

	return 0;
}
