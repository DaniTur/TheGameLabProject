#include "pch.h"
#include "Scene.h"
#include "glm.hpp"
// Temporary
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

NLOHMANN_JSON_SERIALIZE_ENUM(GameObjectType, {
	{GameObjectType::Entity, "Entity"},
	{GameObjectType::LightObject, "LightObject"}
	})

Scene::Scene() :
	m_SceneID("000DEFAULT"), 
	m_SceneFilePath("defaultScene.json"), 
	m_Shader("src/Graphics/vertex_shader.vert", "src/Graphics/fragment_shader.frag")
{
}

Scene::Scene(const std::string_view &id) : m_SceneID(id)
{
	// Map the SceneId to an actual scene filepath here, or do it at the Load function
}

void Scene::Load()
{
	// 1. Open the Scene file path and check for errors
	try {
		std::string relPathFromProjDir = "src/Scene/" + m_SceneFilePath;
		std::ifstream fileStream(relPathFromProjDir);
		if (!fileStream.is_open()) {
			LOG_ERROR("[Scene] Cannot open the input file stream of the Scene::Load() : {}", m_SceneFilePath);
			return;
		}
		json Jdata = json::parse(fileStream);
		if (std::string SceneId = Jdata.at("scene_id"); SceneId.compare(m_SceneID) != 0) {
			LOG_ERROR("[Scene] Error from Json {}: 'scene_id' inside the json does not match the 'scene_id' of the Scene instance", m_SceneFilePath);
			return;
		}
		// Get the assets objects
		json JAssets = Jdata.at("assets");
		if (JAssets.empty()) {
			LOG_ERROR("[Scene] Error reading Json {}: assets is missing or empty", m_SceneFilePath);
			return;
		}
		// 2. Obtain every asset data and load in to the assets container
		for (auto it = JAssets.begin(); it != JAssets.end(); ++it)
		{
			json JAssetObject = (*it);
			GameObjectType type = JAssetObject.at("type");
			if (type == GameObjectType::Entity)
			{
				GameObjectData gameObjectData;
				Transform transform;
				gameObjectData.type = type;
				gameObjectData.name = JAssetObject.at("desc");
				gameObjectData.filePath = JAssetObject.at("model_file_path");
				json position = JAssetObject.at("position");
				transform.position = glm::vec3(position[0], position[1], position[2]);
				json rotation = JAssetObject.at("rotation");
				transform.rotation = glm::vec3(rotation[0], rotation[1], rotation[2]);
				json scale = JAssetObject.at("scale");
				transform.scale = glm::vec3(scale[0], scale[1], scale[2]);
				transform.uniformScale = JAssetObject.at("uniform_scale");
				gameObjectData.colissions = JAssetObject.at("colissions");

				m_GameObjectContainer.push_back(new GameObject(gameObjectData, transform, m_SceneAssetManager));
			}
		}
	}
	catch (const nlohmann::json::parse_error& e) {
		LOG_ERROR("[Scene] Json Parse Error[{}]: {}", e.id, e.what());
		throw;
	}
}

void Scene::AddGameObjectWithModel(const std::string& modelPath)
{
	LOG_INFO("[Scene] AddGameObjectWithModel {}", modelPath.c_str());
	GameObjectData data;
	data.type = GameObjectType::Entity;
	data.filePath = modelPath;
	data.name = modelPath;
	data.colissions = true;

	Transform defaultTransform;
	defaultTransform.position = glm::vec3(0.0f);
	defaultTransform.rotation = glm::vec3(0.0f);
	defaultTransform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	defaultTransform.uniformScale = 1.0f;

	m_GameObjectContainer.push_back(new GameObject(data, defaultTransform, m_SceneAssetManager));
}

void Scene::Render(Camera& camera)
{
	m_Shader.use();

	// Light source
	// Point of light
	glm::vec3 lightSourcePosition(0.0f, 10.0f, 0.0f);

	glm::mat4 lightTransform(1.0f);
	glm::mat4 translate = glm::translate(lightTransform, glm::vec3(0.0f, 0.0f, 5.0f));
	glm::mat4 rotate = glm::rotate(lightTransform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

	lightTransform = rotate * translate;

	glm::vec4 lightPosV4(lightSourcePosition, 1.0f);	// tranform to vec4
	lightPosV4 = lightTransform * lightPosV4; // apply transformation
	lightSourcePosition = lightPosV4;	//transform back to vec3

	m_Shader.setVec3("light.position", lightSourcePosition);

	// Directional light
	glm::vec3 lightDirection(-0.2f, -1.0f, -0.3f);
	m_Shader.setVec3("light.direction", lightDirection);

	// light color and intensity/strenght
	glm::vec3 lightAmbient(0.2f);
	glm::vec3 lightDiffuse(0.8f);
	glm::vec3 lightSpecular(1.0f);
	m_Shader.setVec3("light.ambient", lightAmbient);
	m_Shader.setVec3("light.diffuse", lightDiffuse);
	m_Shader.setVec3("light.specular", lightSpecular);
	m_Shader.setVec3("viewPos", camera.getPosition());

	// Only renders assets of type GameObject here
	// Allocate memory here one time for performance improvement
	glm::mat4 worldTransformMatrix;
	glm::mat4 cameraView;
	glm::mat4 projectionTransformMatrix;

	for (GameObject* gameObject : m_GameObjectContainer)
	{
		GameObjectData& gameObjectData = gameObject->GetData();
		Transform& transform = gameObject->GetTransform();

		if (gameObjectData.type == GameObjectType::Entity) {
			WorldTransform worldTransform;

			worldTransform.setTranslation(transform.position);
			worldTransform.setScale(transform.scale);
			if (transform.rotation != glm::vec3{ 0.0f, 0.0f, 0.0f }) {	// rotation exists
				float degrees = 0.0f;
				glm::vec3 rotationAxis{ 0.0f };
				if (transform.rotation[0] != 0.0f) {
					degrees = transform.rotation[0];
					rotationAxis[0] = 1.0f;
				} else if (transform.rotation[1] != 0.0f) {
					degrees = transform.rotation[1];
					rotationAxis[1] = 1.0f;
				} else if (transform.rotation[2] != 0.0f) {
					degrees = transform.rotation[2];
					rotationAxis[2] = 1.0f;
				}
				worldTransform.setRotation(glm::radians(degrees), rotationAxis);
			}

			worldTransformMatrix = worldTransform.getMatrix();
			cameraView = camera.getView();
			projectionTransformMatrix = m_projectionTransform.getMatrix();

			m_Shader.setMatrix4("model", worldTransformMatrix);
			m_Shader.setMatrix4("view", cameraView);
			m_Shader.setMatrix4("projection", projectionTransformMatrix);
		}
		gameObject->Draw(m_Shader);
	}
}

std::vector<GameObject*>::const_iterator Scene::begin()
{
	return m_GameObjectContainer.begin();
}

std::vector<GameObject*>::const_iterator Scene::end()
{
	return m_GameObjectContainer.end();
}
